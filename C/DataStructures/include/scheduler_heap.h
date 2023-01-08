#ifndef __SCHEDULER_HEAP_H__
#define __SCHEDULER_HEAP_H__

#include "task.h"	 /* create, add etc */
#include "pq_heap.h" /* priority queue */

typedef enum return_type
{
	success = 0,
	fail,
	stop_run
} return_type_t;

typedef struct scheduler scheduler_t;

scheduler_t *HSchedulerCreate(void);

void HSchedulerDestroy(scheduler_t *scheduler);

UID_t HSchedulerAddTask(scheduler_t *scheduler, action_func *func, void *param, size_t interval);

int HSchedulerRemoveTask(scheduler_t *scheduler, UID_t uid);

void HSchedulerClear(scheduler_t *scheduler);

int HSchedulerRun(scheduler_t *scheduler);

void HSchedulerStop(scheduler_t *scheduler);

size_t HSchedulerSize(scheduler_t *scheduler);

int HSchedulerIsEmpty(scheduler_t *scheduler);

#endif /*__SCHEDULER_HEAP_H__*/
