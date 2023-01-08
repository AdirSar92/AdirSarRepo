#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "task.h"

struct task
{
	UID_t uid;
	action_func *func;
	void *param;
	size_t interval_in_seconds;
	time_t next_run_time;
};

task_t* TaskCreate(action_func *func, size_t interval_in_seconds, void *param)
{
	task_t *new_task = NULL;
	assert (NULL != func);

	new_task = (task_t *)malloc(sizeof(task_t));
	if (NULL != new_task)
	{
		new_task->uid = UIDCreate();
		new_task->func = func;
		new_task->param = param;
		new_task->interval_in_seconds = interval_in_seconds;
		TaskUpdateNextRunTime(new_task);	
	}
	return (new_task);
}

void TaskDestroy(task_t *task)
{
	assert (NULL != task);

	free(task);

}

int TaskRun(task_t *task)
{
	assert (NULL != task);
	return (task->func(task->param));	
}

UID_t TaskGetUID(const task_t *task)
{
	assert (NULL != task);
	return (task->uid);
}

int TaskCompare(const task_t *task, UID_t uid)
{
	assert (NULL != task);
	return (UIDIsSame(task->uid, uid));
}

time_t TaskGetNextRunTime(const task_t *task)
{
	assert (NULL != task);
	return (task->next_run_time);
}

void TaskUpdateNextRunTime(task_t *task)
{
	assert (NULL != task);
	task->next_run_time = time(NULL) + task->interval_in_seconds;	
}
