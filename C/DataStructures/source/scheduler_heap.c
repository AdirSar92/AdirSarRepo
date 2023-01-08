#include <stdlib.h>			/* malloc, free */
#include <assert.h>			/* assert */
#include "scheduler_heap.h" /* Function decleration */

/* Checked by Tzach */
#define RUN (1)
#define STOP (0)
#define CYCLIC (0)
#define FALSE (0)
#define TRUE (1)

struct scheduler
{
	priority_q_t *queue;
	int run_flag;
	int is_task_running;
};

static int SortByTime(const void *task1, const void *task2);
static int UIDCompare(const void *task, const void *uid);

scheduler_t *HSchedulerCreate(void)
{
	scheduler_t *new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL != new_sched)
	{
		new_sched->run_flag = STOP;
		new_sched->is_task_running = FALSE;
		new_sched->queue = PQCreate(SortByTime);
		if (NULL == new_sched->queue)
		{
			free(new_sched);
			new_sched = NULL;
		}
	}
	return (new_sched);
}

void HSchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	HSchedulerClear(scheduler);
	PQDestroy(scheduler->queue);
	scheduler->queue = NULL;
	free(scheduler);
}

UID_t HSchedulerAddTask(scheduler_t *scheduler, action_func *func, void *param, size_t interval)
{
	task_t *new_task;
	return_type_t is_added = success;
	UID_t return_uid = badUID;

	assert(NULL != scheduler);
	assert(NULL != func);
	assert(NULL != param);

	new_task = TaskCreate(func, interval, param);
	if (NULL != new_task)
	{
		is_added = PQEnqueue(scheduler->queue, (void *)new_task);
		if (success != is_added)
		{
			TaskDestroy(new_task);
		}
		else
		{
			return_uid = TaskGetUID(new_task);
		}
	}
	return (return_uid);
}

int HSchedulerRemoveTask(scheduler_t *scheduler, UID_t uid)
{
	void *has_removed = NULL;
	return_type_t return_status = fail;

	assert(NULL != scheduler);

	has_removed = (task_t *)PQErase(scheduler->queue, UIDCompare, (void *)&uid);
	if (NULL != has_removed)
	{
		return_status = success;
		TaskDestroy(has_removed);
	}
	return (return_status);
}

void HSchedulerClear(scheduler_t *scheduler)
{
	task_t *curr_task;
	assert(NULL != scheduler);

	while (!PQIsEmpty(scheduler->queue))
	{
		curr_task = (task_t *)PQDequeue(scheduler->queue);
		TaskDestroy(curr_task);
	}
}

int HSchedulerRun(scheduler_t *scheduler)
{
	task_t *curr_task;
	return_type_t run_status = success;
	int cyclic_flag = CYCLIC;
	time_t time_delta = 0;

	assert(NULL != scheduler);

	scheduler->run_flag = RUN;

	while (STOP != scheduler->run_flag && !HSchedulerIsEmpty(scheduler))
	{

		curr_task = (task_t *)PQDequeue(scheduler->queue);
		time_delta = TaskGetNextRunTime(curr_task) - time(NULL);
		cyclic_flag = CYCLIC;

		if (0 < time_delta)
		{
			sleep(time_delta);
		}

		scheduler->is_task_running = TRUE;
		cyclic_flag = TaskRun(curr_task);
		scheduler->is_task_running = FALSE;

		if (CYCLIC == cyclic_flag)
		{
			TaskUpdateNextRunTime(curr_task);
			run_status = PQEnqueue(scheduler->queue, (void *)curr_task);
			if (success != run_status)
			{
				TaskDestroy(curr_task);
				break;
			}
		}
		else
		{
			TaskDestroy(curr_task);
		}
	}

	if (STOP == scheduler->run_flag)
	{
		run_status = stop_run;
	}

	return (run_status);
}

void HSchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	scheduler->run_flag = STOP;
}

size_t HSchedulerSize(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return (PQSize(scheduler->queue) + scheduler->is_task_running);
}

int HSchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return (PQIsEmpty(scheduler->queue) ||
			TRUE == scheduler->is_task_running);
}

static int SortByTime(const void *task1, const void *task2)
{
	return (TaskGetNextRunTime((task_t *)task2) -
			TaskGetNextRunTime((task_t *)task1));
}

static int UIDCompare(const void *task, const void *uid)
{
	return (TaskCompare((task_t *)task, *(UID_t *)uid));
}