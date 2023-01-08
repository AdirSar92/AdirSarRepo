#include <stdlib.h>	   /* malloc, free */
#include <assert.h>	   /* assert */
#include "priorityq.h" /* priority queue */

#include "scheduler.h" /* Function decleration */

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
static void SleepTimeDelta(time_t time_to_sleep);

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *new_sched = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL != new_sched)
	{
		new_sched->run_flag = STOP;
		new_sched->is_task_running = FALSE;
		new_sched->queue = PriorityQCreate(SortByTime);
		if (NULL == new_sched->queue)
		{
			free(new_sched);
			new_sched = NULL;
		}
	}
	return (new_sched);
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	SchedulerClear(scheduler);
	PriorityQDestroy(scheduler->queue);
	scheduler->queue = NULL;
	free(scheduler);
}

UID_t SchedulerAddTask(scheduler_t *scheduler, action_func *func, void *param, size_t interval)
{
	task_t *new_task;
	return_type_t is_added = success;
	UID_t return_uid = badUID;

	assert(NULL != scheduler);
	assert(NULL != func);

	new_task = TaskCreate(func, interval, param);
	if (NULL != new_task)
	{
		is_added = PriorityQEnqueue(scheduler->queue, (void *)new_task);
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

int SchedulerRemoveTask(scheduler_t *scheduler, UID_t uid)
{
	void *has_removed = NULL;
	return_type_t return_status = fail;

	assert(NULL != scheduler);

	has_removed = (task_t *)PriorityQErase(scheduler->queue, UIDCompare, (void *)&uid);
	if (NULL != has_removed)
	{
		return_status = success;
		TaskDestroy(has_removed);
	}
	return (return_status);
}

void SchedulerClear(scheduler_t *scheduler)
{
	task_t *curr_task;
	assert(NULL != scheduler);

	while (!PriorityQIsEmpty(scheduler->queue))
	{
		curr_task = (task_t *)PriorityQDequeue(scheduler->queue);
		TaskDestroy(curr_task);
	}
}

int SchedulerRun(scheduler_t *scheduler)
{
	task_t *curr_task;
	return_type_t run_status = success;
	int cyclic_flag = CYCLIC;
	time_t time_delta = 0;

	assert(NULL != scheduler);

	scheduler->run_flag = RUN;

	while (STOP != scheduler->run_flag && !SchedulerIsEmpty(scheduler))
	{

		curr_task = (task_t *)PriorityQDequeue(scheduler->queue);
		time_delta = TaskGetNextRunTime(curr_task) - time(NULL);
		cyclic_flag = CYCLIC;

		SleepTimeDelta(time_delta);

		scheduler->is_task_running = TRUE;
		cyclic_flag = TaskRun(curr_task);
		scheduler->is_task_running = FALSE;

		if (CYCLIC == cyclic_flag)
		{
			TaskUpdateNextRunTime(curr_task);
			run_status = PriorityQEnqueue(scheduler->queue, (void *)curr_task);
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

void SchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	scheduler->run_flag = STOP;
}

size_t SchedulerSize(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return (PriorityQSize(scheduler->queue) + scheduler->is_task_running);
}

int SchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return (PriorityQIsEmpty(scheduler->queue) ||
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

static void SleepTimeDelta(time_t time_to_sleep)
{
	while (0 < time_to_sleep)
	{
		sleep(1);
		--time_to_sleep;
	}
}