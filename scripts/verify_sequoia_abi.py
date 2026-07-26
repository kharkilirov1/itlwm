#!/usr/bin/env python3
"""CI gate for the AirportSequoia ABI skeleton.

This checks two independent witnesses:

* Clang's ``-fdump-vtable-layouts`` output, including the Sequoia controller
  and infrastructure-protocol slot boundaries.
* The linked kext's Mach-O symbol table, including the IONetworkController
  slot-6/7 imports and the absence of project-local unresolved HAL symbols.

The gate deliberately makes no functional Wi-Fi claim. In particular, it does
not establish WCL scan, association, key-installation, or device behaviour.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import plistlib
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, Sequence


ANSI_ESCAPE_RE = re.compile(r"\x1b\[[0-?]*[ -/]*[@-~]")
VTABLE_HEADER_RE = re.compile(r"^Vtable for '([^']+)' \((\d+) entries\)\.$")
VTABLE_ENTRY_RE = re.compile(r"^\s*(\d+) \| (.*)$")

CLAIM = (
    "ABI skeleton only: this gate does not establish functional WCL scan, "
    "association, key installation, or device runtime."
)

CONTROLLER_PURE_METHODS: Mapping[int, str] = {
    394: "isCommandProhibited",
    403: "handleCardSpecific",
    408: "getDRIVER_VERSION",
    409: "getHARDWARE_VERSION",
    410: "getCARD_CAPABILITIES",
    411: "getPOWER",
    412: "setPOWER",
    413: "getCOUNTRY_CODE",
    414: "setCOUNTRY_CODE",
    415: "setGET_DEBUG_INFO",
    424: "getLogger",
    432: "getFaultReporterFromDriver",
}

CONTROLLER_DERIVED_METHODS: Mapping[int, str] = {
    **CONTROLLER_PURE_METHODS,
    395: "createWorkQueue",
    397: "getWorkQueue",
    462: "setMulticastList",
}

WCL_ROUTE_METHODS: Mapping[int, str] = {
    536: "setCIPHER_KEY",
    592: "setWCL_SCAN_REQ",
    593: "setWCL_ASSOCIATE",
    595: "setWCL_LINK_UP_DONE",
}

EXPECTED_NETWORK_IMPORTS: Sequence[str] = (
    "ZN19IONetworkController20allocatePacketNoWaitEj",
    "ZN19IONetworkController18setHardwareAssistsEjj",
)

EXPECTED_SKYWALK_IMPORTS: Sequence[str] = (
    "ZN26IOSkywalkEthernetInterface20initRegistrationInfo"
    "EPNS_16RegistrationInfoEjm",
)

FORBIDDEN_IMPORTS: Sequence[str] = (
    "IOSkywalkNetworkInterface20initRegistrationInfo",
)


class GateFailure(RuntimeError):
    """A failed ABI assertion."""


@dataclass(frozen=True)
class VtableSection:
    class_name: str
    declared_entries: int
    raw_entries: Mapping[int, str]

    def logical_slots(self, max_slot: int) -> Dict[int, str]:
        """Return Itanium address-point slots (two header words excluded)."""
        if self.raw_entries.get(0, "").startswith("offset_to_top") is False:
            raise GateFailure(
                f"{self.class_name}: raw entry 0 is not offset_to_top"
            )
        if "RTTI" not in self.raw_entries.get(1, ""):
            raise GateFailure(f"{self.class_name}: raw entry 1 is not RTTI")

        missing = [
            slot for slot in range(max_slot + 1) if slot + 2 not in self.raw_entries
        ]
        if missing:
            preview = ", ".join(str(slot) for slot in missing[:8])
            raise GateFailure(
                f"{self.class_name}: missing logical slot(s): {preview}"
            )

        trailing = self.raw_entries.get(max_slot + 3, "")
        if not trailing.startswith("vcall_offset"):
            raise GateFailure(
                f"{self.class_name}: expected vcall_offset immediately after "
                f"logical slot {max_slot}, got {trailing!r}"
            )
        return {
            slot: self.raw_entries[slot + 2] for slot in range(max_slot + 1)
        }


def _read_text(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except OSError as exc:
        raise GateFailure(f"cannot read {path}: {exc}") from exc


def _write_text(path: Path, value: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(value, encoding="utf-8", newline="\n")


def _write_json(path: Path, value: object) -> None:
    _write_text(path, json.dumps(value, indent=2, sort_keys=True) + "\n")


def _write_claim_marker(report_dir: Path) -> None:
    _write_text(
        report_dir / "ABI-SKELETON-NOT-WCL-FUNCTIONAL.txt",
        (
            "AirportSequoia ABI SKELETON ONLY\n"
            "\n"
            "This CI artifact validates compile-time vtable layout and linked "
            "Mach-O symbol shape only.\n"
            "It does not establish functional WCL scan or association. Do not "
            "present it as a functional Wi-Fi driver.\n"
        ),
    )


def parse_vtable_sections(text: str) -> Mapping[str, List[VtableSection]]:
    """Parse every vtable section emitted by Clang."""
    lines = ANSI_ESCAPE_RE.sub("", text).replace("\r\n", "\n").splitlines()
    sections: Dict[str, List[VtableSection]] = {}
    current_name: str | None = None
    current_count = 0
    current_entries: Dict[int, str] = {}

    def finish() -> None:
        nonlocal current_name, current_count, current_entries
        if current_name is not None:
            section = VtableSection(
                current_name, current_count, dict(current_entries)
            )
            sections.setdefault(current_name, []).append(section)
        current_name = None
        current_count = 0
        current_entries = {}

    for line in lines:
        header = VTABLE_HEADER_RE.match(line)
        if header:
            finish()
            current_name = header.group(1)
            current_count = int(header.group(2))
            continue

        if current_name is None:
            continue
        if line.startswith("VTable indices for "):
            finish()
            continue

        entry = VTABLE_ENTRY_RE.match(line)
        if entry:
            current_entries[int(entry.group(1))] = entry.group(2).strip()

    finish()
    return sections


def _require_owner(
    slots: Mapping[int, str],
    slot: int,
    owner: str,
    method: str | None = None,
    pure: bool | None = None,
) -> None:
    value = slots.get(slot)
    if value is None:
        raise GateFailure(f"slot {slot}: missing")
    expected = f"{owner}::"
    if expected not in value:
        raise GateFailure(
            f"slot {slot}: expected owner {owner}, got {value!r}"
        )
    if method is not None and f"::{method}(" not in value:
        raise GateFailure(
            f"slot {slot}: expected method {method}, got {value!r}"
        )
    if pure is True and "[pure]" not in value:
        raise GateFailure(f"slot {slot}: expected pure method, got {value!r}")
    if pure is False and "[pure]" in value:
        raise GateFailure(f"slot {slot}: unexpectedly pure: {value!r}")


def _require_no_pure(class_name: str, slots: Mapping[int, str]) -> None:
    pure = [(slot, value) for slot, value in slots.items() if "[pure]" in value]
    if pure:
        preview = "; ".join(f"{slot}: {value}" for slot, value in pure[:4])
        raise GateFailure(f"{class_name}: unresolved pure virtual(s): {preview}")


def _validate_controller_base(section: VtableSection) -> None:
    if section.declared_entries != 466:
        raise GateFailure(
            "IO80211Controller: expected 466 raw entries "
            f"(logical slots 0..462), got {section.declared_entries}"
        )
    slots = section.logical_slots(462)
    _require_owner(
        slots, 329, "IONetworkController", method="allocatePacketNoWait"
    )
    _require_owner(
        slots, 330, "IONetworkController", method="setHardwareAssists"
    )
    _require_owner(slots, 393, "IOEthernetController")
    for slot, method in CONTROLLER_PURE_METHODS.items():
        _require_owner(
            slots, slot, "IO80211Controller", method=method, pure=True
        )
    _require_owner(
        slots, 446, "IO80211Controller", method="_RESERVEDIO80211Controller0"
    )
    _require_owner(
        slots, 461, "IO80211Controller", method="_RESERVEDIO80211Controller15"
    )
    _require_owner(
        slots, 462, "IO80211Controller", method="setMulticastList", pure=False
    )


def _validate_controller_derived(section: VtableSection) -> None:
    if section.declared_entries != 466:
        raise GateFailure(
            "AirportSequoia: expected 466 raw entries "
            f"(logical slots 0..462), got {section.declared_entries}"
        )
    slots = section.logical_slots(462)
    _require_no_pure("AirportSequoia", slots)
    _require_owner(
        slots, 329, "IONetworkController", method="allocatePacketNoWait"
    )
    _require_owner(
        slots, 330, "IONetworkController", method="setHardwareAssists"
    )
    _require_owner(slots, 393, "IOEthernetController")
    for slot, method in CONTROLLER_DERIVED_METHODS.items():
        _require_owner(
            slots, slot, "AirportSequoia", method=method, pure=False
        )
    _require_owner(
        slots, 446, "IO80211Controller", method="_RESERVEDIO80211Controller0"
    )
    _require_owner(
        slots, 461, "IO80211Controller", method="_RESERVEDIO80211Controller15"
    )


def _validate_protocol_base(section: VtableSection) -> None:
    if section.declared_entries != 623:
        raise GateFailure(
            "IO80211InfraProtocol: expected 623 raw entries "
            f"(logical slots 0..619), got {section.declared_entries}"
        )
    slots = section.logical_slots(619)
    _require_owner(
        slots, 415, "IO80211SkywalkInterface", method="setMacAddress", pure=True
    )
    _require_owner(
        slots, 459, "IO80211SkywalkInterface", method="getLastRxTimeStamp"
    )
    _require_owner(
        slots, 460, "IO80211InfraInterface", method="setLinkStateInternal"
    )
    _require_owner(
        slots, 464, "IO80211InfraInterface", method="createLQMData"
    )
    for slot in range(465, 620):
        _require_owner(slots, slot, "IO80211InfraProtocol", pure=True)
    for slot, method in WCL_ROUTE_METHODS.items():
        _require_owner(
            slots, slot, "IO80211InfraProtocol", method=method, pure=True
        )
    if "::getCHANNEL(" not in slots[465]:
        raise GateFailure(
            f"IO80211InfraProtocol slot 465 is not getCHANNEL: {slots[465]!r}"
        )
    if "::setWCL_WNM_OFFLOAD(" not in slots[619]:
        raise GateFailure(
            "IO80211InfraProtocol slot 619 is not setWCL_WNM_OFFLOAD: "
            f"{slots[619]!r}"
        )


def _validate_protocol_derived(section: VtableSection) -> None:
    if section.declared_entries != 623:
        raise GateFailure(
            "AirportSequoiaInfra: expected 623 raw entries "
            f"(logical slots 0..619), got {section.declared_entries}"
        )
    slots = section.logical_slots(619)
    _require_no_pure("AirportSequoiaInfra", slots)
    _require_owner(
        slots, 415, "AirportSequoiaInfra", method="setMacAddress", pure=False
    )
    _require_owner(
        slots, 459, "IO80211SkywalkInterface", method="getLastRxTimeStamp"
    )
    _require_owner(
        slots, 460, "IO80211InfraInterface", method="setLinkStateInternal"
    )
    _require_owner(
        slots, 464, "IO80211InfraInterface", method="createLQMData"
    )
    for slot in range(465, 620):
        _require_owner(slots, slot, "AirportSequoiaInfra", pure=False)
    for slot, method in WCL_ROUTE_METHODS.items():
        _require_owner(
            slots, slot, "AirportSequoiaInfra", method=method, pure=False
        )
    if "::getCHANNEL(" not in slots[465]:
        raise GateFailure(
            f"AirportSequoiaInfra slot 465 is not getCHANNEL: {slots[465]!r}"
        )
    if "::setWCL_WNM_OFFLOAD(" not in slots[619]:
        raise GateFailure(
            "AirportSequoiaInfra slot 619 is not setWCL_WNM_OFFLOAD: "
            f"{slots[619]!r}"
        )


def verify_vtables(log_path: Path, report_dir: Path) -> None:
    text = _read_text(log_path)
    parsed = parse_vtable_sections(text)
    validators = {
        "IO80211Controller": _validate_controller_base,
        "AirportSequoia": _validate_controller_derived,
        "IO80211InfraProtocol": _validate_protocol_base,
        "AirportSequoiaInfra": _validate_protocol_derived,
    }

    counts: Dict[str, int] = {}
    for class_name, validator in validators.items():
        sections = parsed.get(class_name, [])
        if not sections:
            raise GateFailure(
                f"{class_name}: no Clang vtable dump found in {log_path}"
            )
        for section in sections:
            validator(section)
        counts[class_name] = len(sections)

    _write_claim_marker(report_dir)
    _write_json(
        report_dir / "vtable-gate.json",
        {
            "claim": CLAIM,
            "log": str(log_path),
            "log_sha256": hashlib.sha256(text.encode("utf-8")).hexdigest(),
            "status": "pass",
            "validated_sections": counts,
            "controller_logical_slots": "0..462",
            "infra_protocol_logical_slots": "0..619",
        },
    )
    print(
        "Sequoia vtable gate: PASS "
        "(AirportSequoia 0..462; AirportSequoiaInfra 0..619)"
    )


def _find_kext_executable(kext_path: Path) -> Path:
    info_path = kext_path / "Contents" / "Info.plist"
    try:
        with info_path.open("rb") as stream:
            executable_name = plistlib.load(stream).get("CFBundleExecutable")
    except (OSError, plistlib.InvalidFileException) as exc:
        raise GateFailure(f"cannot read kext Info.plist {info_path}: {exc}") from exc
    if not executable_name or not isinstance(executable_name, str):
        raise GateFailure(f"{info_path}: CFBundleExecutable is missing")
    executable = kext_path / "Contents" / "MacOS" / executable_name
    if not executable.is_file():
        raise GateFailure(f"kext executable is missing: {executable}")
    return executable


def _run(command: Sequence[str]) -> subprocess.CompletedProcess[str]:
    try:
        return subprocess.run(
            list(command),
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            errors="replace",
        )
    except OSError as exc:
        raise GateFailure(f"cannot run {command[0]}: {exc}") from exc


def _nm_command() -> List[str]:
    xcrun = shutil.which("xcrun")
    if xcrun:
        return [xcrun, "nm"]
    nm = shutil.which("nm")
    if nm:
        return [nm]
    raise GateFailure("neither xcrun nm nor nm is available")


def _matching_lines(text: str, token: str) -> List[str]:
    return [line for line in text.splitlines() if token in line]


def _verify_symbol_text(all_symbols: str, undefined_symbols: str) -> None:
    unresolved_hal = _matching_lines(undefined_symbols, "ItlHalService")
    if unresolved_hal:
        raise GateFailure(
            "ItlHalService remains undefined: " + "; ".join(unresolved_hal[:4])
        )
    if "ItlHalService" not in all_symbols:
        raise GateFailure(
            "linked binary contains no ItlHalService definition; "
            "the HAL implementation path was not witnessed"
        )

    reserved = [
        line
        for line in undefined_symbols.splitlines()
        if re.search(r"_RESERVEDIONetworkController(?:6|7)(?:\D|$)", line)
    ]
    if reserved:
        raise GateFailure(
            "obsolete IONetworkController reserved-slot import(s): "
            + "; ".join(reserved[:4])
        )

    missing_imports = [
        symbol
        for symbol in EXPECTED_NETWORK_IMPORTS
        if symbol not in undefined_symbols
    ]
    if missing_imports:
        raise GateFailure(
            "expected IONetworkController import(s) missing: "
            + ", ".join(missing_imports)
        )

    missing_skywalk_imports = [
        symbol
        for symbol in EXPECTED_SKYWALK_IMPORTS
        if symbol not in undefined_symbols
    ]
    if missing_skywalk_imports:
        raise GateFailure(
            "expected IOSkywalk import(s) missing: "
            + ", ".join(missing_skywalk_imports)
        )

    forbidden_imports = [
        symbol for symbol in FORBIDDEN_IMPORTS if symbol in undefined_symbols
    ]
    if forbidden_imports:
        raise GateFailure(
            "forbidden unresolved import(s): " + ", ".join(forbidden_imports)
        )


def verify_binary(kext_path: Path, report_dir: Path) -> None:
    executable = _find_kext_executable(kext_path)
    nm = _nm_command()

    all_result = _run([*nm, str(executable)])
    _write_text(report_dir / "nm-all.txt", all_result.stdout)
    if all_result.returncode != 0:
        raise GateFailure(f"nm failed with exit code {all_result.returncode}")

    undefined_result = _run([*nm, "-u", str(executable)])
    _write_text(report_dir / "nm-undefined.txt", undefined_result.stdout)
    if undefined_result.returncode != 0:
        raise GateFailure(
            f"nm -u failed with exit code {undefined_result.returncode}"
        )

    kextlibs_path = shutil.which("kextlibs")
    kextlibs_status = "unavailable"
    kextlibs_result: subprocess.CompletedProcess[str] | None = None
    if kextlibs_path:
        kextlibs_result = _run(
            [kextlibs_path, "-undef-symbols", str(kext_path)]
        )
        _write_text(
            report_dir / "kextlibs-undef-symbols.txt",
            kextlibs_result.stdout,
        )
    else:
        _write_text(
            report_dir / "kextlibs-undef-symbols.txt",
            "kextlibs is unavailable on this runner; check skipped.\n",
        )

    _verify_symbol_text(all_result.stdout, undefined_result.stdout)
    if kextlibs_result is not None:
        if kextlibs_result.returncode != 0:
            raise GateFailure(
                "kextlibs -undef-symbols failed with exit code "
                f"{kextlibs_result.returncode}; see "
                f"{report_dir / 'kextlibs-undef-symbols.txt'}"
            )
        kextlibs_status = "pass"

    _write_claim_marker(report_dir)
    _write_json(
        report_dir / "binary-gate.json",
        {
            "claim": CLAIM,
            "executable": str(executable),
            "executable_sha256": hashlib.sha256(executable.read_bytes()).hexdigest(),
            "expected_undefined_imports": list(EXPECTED_NETWORK_IMPORTS),
            "expected_skywalk_imports": list(EXPECTED_SKYWALK_IMPORTS),
            "forbidden_imports": list(FORBIDDEN_IMPORTS),
            "kextlibs": kextlibs_status,
            "status": "pass",
        },
    )
    print(
        "Sequoia binary ABI gate: PASS "
        "(HAL resolved; IONetworkController slots 6/7 imports verified; "
        f"kextlibs={kextlibs_status})"
    )


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    vtables = subparsers.add_parser(
        "vtables", help="verify Clang -fdump-vtable-layouts output"
    )
    vtables.add_argument("--log", type=Path, required=True)
    vtables.add_argument(
        "--report-dir",
        type=Path,
        default=Path("build/sequoia-abi-witness"),
    )

    binary = subparsers.add_parser(
        "binary", help="verify linked kext symbols and run kextlibs when present"
    )
    binary.add_argument("--kext", type=Path, required=True)
    binary.add_argument(
        "--report-dir",
        type=Path,
        default=Path("build/sequoia-abi-witness"),
    )
    return parser


def main(argv: Sequence[str] | None = None) -> int:
    args = build_parser().parse_args(argv)
    try:
        report_dir = args.report_dir.resolve()
        report_dir.mkdir(parents=True, exist_ok=True)
        _write_claim_marker(report_dir)
        if args.command == "vtables":
            verify_vtables(args.log.resolve(), report_dir)
        elif args.command == "binary":
            verify_binary(args.kext.resolve(), report_dir)
        else:  # pragma: no cover - argparse enforces the command set
            raise GateFailure(f"unknown command: {args.command}")
    except GateFailure as exc:
        print(f"Sequoia ABI gate: FAIL: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
