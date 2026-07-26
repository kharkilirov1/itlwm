//
//  _task.cpp
//  itlwm
//
//  Created by qcwap on 2020/3/1.
//  Copyright © 2020 钟先耀. All rights reserved.
//

/*
* Copyright (C) 2020  钟先耀
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#include <sys/_task.h>
#include <sys/proc.h>

#include <IOKit/IOLib.h>
#include <IOKit/IOCommandGate.h>
#include <libkern/OSAtomic.h>

enum ETQ_STATE {
    TQ_S_CREATED,
    TQ_S_RUNNING,
    TQ_S_DESTROYED
};

struct taskq {
    enum ETQ_STATE       tq_state;
    unsigned int         tq_running;
    unsigned int         tq_waiting; /* tasks currently executing */
    unsigned int         tq_nthreads;
    unsigned int         tq_flags;
    const char        *tq_name;

    IORecursiveLock *tq_mtx;
    struct task_list     tq_worklist;
};

static const char taskq_sys_name[] = "systq";

struct taskq taskq_sys = {
    TQ_S_CREATED,
    0,
    0,
    1,
    0,
    taskq_sys_name,
};

struct taskq *const systq = &taskq_sys;
static unsigned int taskq_sys_users;
static unsigned int taskq_sys_barriers;
static bool taskq_sys_destroying;
static IOLock *taskq_sys_lifecycle_lock;

static IOLock *
taskq_sys_get_lifecycle_lock(void)
{
    IOLock *lock = taskq_sys_lifecycle_lock;
    if (lock != NULL)
        return lock;

    IOLock *newLock = IOLockAlloc();
    if (newLock == NULL)
        return NULL;
    if (!OSCompareAndSwapPtr(NULL, newLock, &taskq_sys_lifecycle_lock))
        IOLockFree(newLock);
    return taskq_sys_lifecycle_lock;
}

int
taskq_next_work(struct taskq *tq, struct task *work)
{
    struct task *next;
    
    //    IOLog("itlwm: taskq %s lock\n", __FUNCTION__);
    IORecursiveLockLock(tq->tq_mtx);
    
    while ((next = TAILQ_FIRST(&tq->tq_worklist)) == NULL) {
        if (tq->tq_state != TQ_S_RUNNING) {
            IORecursiveLockUnlock(tq->tq_mtx);
            return (0);
        }
        IORecursiveLockSleep(tq->tq_mtx, tq, THREAD_INTERRUPTIBLE);
    }

    TAILQ_REMOVE(&tq->tq_worklist, next, t_entry);
    CLR(next->t_flags, TASK_ONQUEUE);
    tq->tq_waiting++;

    *work = *next; /* copy to caller to avoid races */

    next = TAILQ_FIRST(&tq->tq_worklist);
    IORecursiveLockUnlock(tq->tq_mtx);
//    IOLog("itlwm: taskq %s unlock\n", __FUNCTION__);

    if (next != NULL && tq->tq_nthreads > 1)
        IORecursiveLockWakeup(tq->tq_mtx, tq, true);

    return (1);
}

void
taskq_thread(void *xtq)
{
    struct taskq *tq = (struct taskq *)xtq;
    struct task work;
    int last;

//    if (ISSET(tq->tq_flags, TASKQ_MPSAFE))
//        KERNEL_UNLOCK();

//    WITNESS_CHECKORDER(&tq->tq_lock_object, LOP_NEWORDER, NULL);
    
    IOLog("itlwm: taskq %s schedule task\n", __FUNCTION__);

    while (taskq_next_work(tq, &work)) {
//        WITNESS_LOCK(&tq->tq_lock_object, 0);
//        IOLog("itlwm: taskq worker thread=%lld work=%s\n", thread_tid(current_thread()), work.name);
        (*work.t_func)(work.t_arg);
        IORecursiveLockLock(tq->tq_mtx);
        if (tq->tq_waiting > 0)
            tq->tq_waiting--;
        if (tq->tq_waiting == 0 && TAILQ_EMPTY(&tq->tq_worklist))
            IORecursiveLockWakeup(tq->tq_mtx, tq, false);
        IORecursiveLockUnlock(tq->tq_mtx);
//        IOLog("itlwm: taskq worker thread=%lld work=%s done", thread_tid(current_thread()), work.name);
//        WITNESS_UNLOCK(&tq->tq_lock_object, 0);
//        sched_pause(yield);
        IOSleep(1);
    }
    
    IOLog("itlwm: taskq %s schedule task done\n", __FUNCTION__);

    IORecursiveLockLock(tq->tq_mtx);
    last = (--tq->tq_running == 0);
    if (last) {
        IOLog("itlwm: taskq %s schedule task wakeup\n", __FUNCTION__);
        IORecursiveLockWakeup(tq->tq_mtx, tq, false);
    }
    IORecursiveLockUnlock(tq->tq_mtx);

//    if (ISSET(tq->tq_flags, TASKQ_MPSAFE))
//        KERNEL_LOCK();

//    kthread_exit(0);
    thread_terminate(current_thread());
}

int
taskq_init(void)
{
    IOLock *lifecycle = taskq_sys_get_lifecycle_lock();
    if (lifecycle == NULL)
        return (0);
    IOLockLock(lifecycle);

    while (taskq_sys_destroying)
        IOLockSleep(lifecycle, systq, THREAD_INTERRUPTIBLE);

    if (systq->tq_mtx != NULL) {
        IORecursiveLockLock(systq->tq_mtx);
        if (systq->tq_state == TQ_S_RUNNING) {
            taskq_sys_users++;
            IORecursiveLockUnlock(systq->tq_mtx);
            IOLockUnlock(lifecycle);
            return (1);
        }
        IORecursiveLockUnlock(systq->tq_mtx);
    }

    systq->tq_state = TQ_S_RUNNING;
    systq->tq_running = 1;
    systq->tq_waiting = 0;
    systq->tq_nthreads = 1;
    systq->tq_flags = 0;
    systq->tq_name = taskq_sys_name;
    systq->tq_mtx = IORecursiveLockAlloc();
    if (systq->tq_mtx == NULL) {
        systq->tq_state = TQ_S_DESTROYED;
        systq->tq_running = 0;
        IOLockUnlock(lifecycle);
        return (0);
    }
    TAILQ_INIT(&systq->tq_worklist);
    taskq_sys_users = 1;
    taskq_sys_barriers = 0;

    thread_t new_thread = THREAD_NULL;
    int rv = kernel_thread_start((thread_continue_t)taskq_thread, systq, &new_thread);
    if (rv != KERN_SUCCESS) {
        IORecursiveLockFree(systq->tq_mtx);
        systq->tq_mtx = NULL;
        systq->tq_state = TQ_S_DESTROYED;
        systq->tq_running = 0;
        taskq_sys_users = 0;
        IOLockUnlock(lifecycle);
        return (0);
    }
    thread_deallocate(new_thread);
    IOLockUnlock(lifecycle);
    return (1);
}

struct taskq *
taskq_create(const char *name, unsigned int nthreads, int ipl,
             unsigned int flags)
{
    struct taskq *tq;

    tq = (struct taskq *)IOMalloc(sizeof(*tq));
    if (tq == NULL)
        return (NULL);

    tq->tq_state = TQ_S_RUNNING;
    tq->tq_running = 0;
    tq->tq_waiting = 0;
    tq->tq_nthreads = nthreads;
    tq->tq_name = name;
    tq->tq_flags = flags;
    tq->tq_mtx = IORecursiveLockAlloc();
    if (tq->tq_mtx == NULL) {
        IOFree(tq, sizeof(*tq));
        return (NULL);
    }

    TAILQ_INIT(&tq->tq_worklist);
    for (unsigned int i = 0; i < nthreads; i++) {
        thread_t new_thread = THREAD_NULL;
        tq->tq_running++;
        int rv = kernel_thread_start((thread_continue_t)taskq_thread, tq,
                                     &new_thread);
        if (rv != KERN_SUCCESS) {
            tq->tq_running--;
            break;
        }
        thread_deallocate(new_thread);
    }
    if (tq->tq_running == 0) {
        IORecursiveLockFree(tq->tq_mtx);
        IOFree(tq, sizeof(*tq));
        return (NULL);
    }
    tq->tq_nthreads = tq->tq_running;
    return (tq);
}

void
taskq_destroy(struct taskq *tq)
{
    if (!tq)
        return;

    if (tq == systq) {
        IOLock *lifecycle = taskq_sys_get_lifecycle_lock();
        if (lifecycle == NULL)
            return;
        IOLockLock(lifecycle);

        IORecursiveLock *mutex = tq->tq_mtx;
        if (taskq_sys_destroying || mutex == NULL || taskq_sys_users == 0) {
            IOLockUnlock(lifecycle);
            return;
        }

        taskq_sys_users--;
        if (taskq_sys_users > 0) {
            IOLockUnlock(lifecycle);
            return;
        }

        taskq_sys_destroying = true;
        while (taskq_sys_barriers > 0)
            IOLockSleep(lifecycle, systq, THREAD_INTERRUPTIBLE);
        IOLockUnlock(lifecycle);

        IORecursiveLockLock(mutex);
        while (!TAILQ_EMPTY(&tq->tq_worklist) || tq->tq_waiting > 0) {
            IORecursiveLockWakeup(mutex, tq, false);
            IORecursiveLockSleep(mutex, tq, THREAD_INTERRUPTIBLE);
        }

        tq->tq_state = TQ_S_DESTROYED;
        while (tq->tq_running > 0) {
            IORecursiveLockWakeup(mutex, tq, false);
            IORecursiveLockSleep(mutex, tq, THREAD_INTERRUPTIBLE);
        }
        IORecursiveLockUnlock(mutex);

        IOLockLock(lifecycle);
        tq->tq_mtx = NULL;
        IORecursiveLockFree(mutex);
        taskq_sys_destroying = false;
        IOLockWakeup(lifecycle, systq, false);
        IOLockUnlock(lifecycle);
        return;
    }

    IORecursiveLock *mutex = tq->tq_mtx;
    if (mutex == NULL)
        return;

    IORecursiveLockLock(mutex);
    switch (tq->tq_state) {
        case TQ_S_RUNNING:
            tq->tq_state = TQ_S_DESTROYED;
            break;

        default:
            IOLog("itlwm: unexpected %s tq state %u", tq->tq_name, tq->tq_state);
            tq->tq_state = TQ_S_DESTROYED;
            IORecursiveLockUnlock(mutex);
            return;
    }

    while (tq->tq_running > 0) {
        IORecursiveLockWakeup(mutex, tq, false);
        IORecursiveLockSleep(mutex, tq, THREAD_INTERRUPTIBLE);
    }

    IORecursiveLockUnlock(mutex);
    IORecursiveLockFree(mutex);
    IOFree(tq, sizeof(*tq));
}

void
taskq_barrier(struct taskq *tq)
{
    if (!tq)
        return;

    if (tq == systq) {
        IOLock *lifecycle = taskq_sys_get_lifecycle_lock();
        if (lifecycle == NULL)
            return;
        IOLockLock(lifecycle);

        if (taskq_sys_destroying || tq->tq_mtx == NULL ||
            taskq_sys_users == 0) {
            IOLockUnlock(lifecycle);
            return;
        }

        IORecursiveLock *mutex = tq->tq_mtx;
        taskq_sys_barriers++;
        IOLockUnlock(lifecycle);

        IORecursiveLockLock(mutex);
        while (!TAILQ_EMPTY(&tq->tq_worklist) || tq->tq_waiting > 0) {
            IORecursiveLockWakeup(mutex, tq, false);
            IORecursiveLockSleep(mutex, tq, THREAD_INTERRUPTIBLE);
        }
        IORecursiveLockUnlock(mutex);

        IOLockLock(lifecycle);
        if (taskq_sys_barriers > 0)
            taskq_sys_barriers--;
        if (taskq_sys_barriers == 0)
            IOLockWakeup(lifecycle, systq, false);
        IOLockUnlock(lifecycle);
        return;
    }

    IORecursiveLock *mutex = tq->tq_mtx;
    if (mutex == NULL)
        return;

    IORecursiveLockLock(mutex);
    while (!TAILQ_EMPTY(&tq->tq_worklist) || tq->tq_waiting > 0) {
        IORecursiveLockWakeup(mutex, tq, false);
        IORecursiveLockSleep(mutex, tq, THREAD_INTERRUPTIBLE);
    }
    IORecursiveLockUnlock(mutex);
}

void
task_set(struct task *t, void (*fn)(void *), void *arg, const char *name)
{
    t->t_func = fn;
    t->t_arg = arg;
    t->t_flags = 0;
    memcpy(t->name, name, sizeof(t->name));
}

int
task_add(struct taskq *tq, struct task *w)
{
    int rv = 0;
//    IOLog("itlwm: taskq task_add %s thread: %lld\n", w->name, thread_tid(current_thread()));

    if (!tq)
        return (0);

    IOLock *lifecycle = NULL;
    if (tq == systq) {
        lifecycle = taskq_sys_get_lifecycle_lock();
        if (lifecycle == NULL)
            return (0);
        IOLockLock(lifecycle);
        if (taskq_sys_destroying) {
            IOLockUnlock(lifecycle);
            return (0);
        }
    }

    IORecursiveLock *mutex = tq->tq_mtx;
    if (mutex == NULL) {
        if (lifecycle)
            IOLockUnlock(lifecycle);
        return (0);
    }

    IORecursiveLockLock(mutex);
    if (tq->tq_state == TQ_S_DESTROYED) {
        IORecursiveLockUnlock(mutex);
        if (lifecycle)
            IOLockUnlock(lifecycle);
        return (0);
    }
    if (ISSET(w->t_flags, TASK_ONQUEUE)) {
//        IOLog("itlwm: taskq task_add %s is already on queue thread: %lld\n", w->name, thread_tid(current_thread()));
        IORecursiveLockUnlock(mutex);
        if (lifecycle)
            IOLockUnlock(lifecycle);
        return (0);
    }
    if (!ISSET(w->t_flags, TASK_ONQUEUE)) {
//        IOLog("itlwm: taskq task_add %s add to queue thread: %lld\n", w->name, thread_tid(current_thread()));
        rv = 1;
        SET(w->t_flags, TASK_ONQUEUE);
        TAILQ_INSERT_TAIL(&tq->tq_worklist, w, t_entry);
    }
    if (rv)
        IORecursiveLockWakeup(mutex, tq, true);
    IORecursiveLockUnlock(mutex);
    if (lifecycle)
        IOLockUnlock(lifecycle);

    return (rv);
}

int
task_del(struct taskq *tq, struct task *w)
{
    int rv = 0;
//    IOLog("itlwm: taskq task_del %s thread: %lld\n", w->name, thread_tid(current_thread()));

    if (!tq)
        return (0);

    IOLock *lifecycle = NULL;
    if (tq == systq) {
        lifecycle = taskq_sys_get_lifecycle_lock();
        if (lifecycle == NULL)
            return (0);
        IOLockLock(lifecycle);
        if (taskq_sys_destroying) {
            IOLockUnlock(lifecycle);
            return (0);
        }
    }

    IORecursiveLock *mutex = tq->tq_mtx;
    if (mutex == NULL) {
        if (lifecycle)
            IOLockUnlock(lifecycle);
        return (0);
    }

    IORecursiveLockLock(mutex);
    if (tq->tq_state == TQ_S_DESTROYED) {
        IORecursiveLockUnlock(mutex);
        if (lifecycle)
            IOLockUnlock(lifecycle);
        return (0);
    }
    if (ISSET(w->t_flags, TASK_ONQUEUE)) {
        rv = 1;
        CLR(w->t_flags, TASK_ONQUEUE);
        TAILQ_REMOVE(&tq->tq_worklist, w, t_entry);
    }
    if (TAILQ_EMPTY(&tq->tq_worklist) && tq->tq_waiting == 0)
        IORecursiveLockWakeup(mutex, tq, false);
    IORecursiveLockUnlock(mutex);
    if (lifecycle)
        IOLockUnlock(lifecycle);

    return (rv);
}
