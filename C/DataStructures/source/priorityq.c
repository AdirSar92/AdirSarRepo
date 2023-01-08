#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "sortedlist.h" /* SortedListCreate, Destroy etc */
#include "priorityq.h"

#define FAIL (1)
#define SUCCESS (0)

struct priority_q
{
	sorted_list_t *list;
};

priority_q_t *PriorityQCreate(priority_q_cmp_t func)
{
	priority_q_t *queue = NULL;

	assert (NULL != func);
	
	queue = (priority_q_t *)malloc(sizeof(priority_q_t));
	if (NULL != queue)
	{
		queue->list = SortedListCreate(func);

		if (NULL == queue->list)
		{
			free (queue);
			return (NULL);
		}
	}
	return (queue);
}

void PriorityQDestroy(priority_q_t *queue)
{
	assert (NULL != queue);
	SortedListDestroy(queue->list);
	queue->list = NULL;
	free(queue);
}

int PriorityQIsEmpty(const priority_q_t *queue)
{
	assert (NULL != queue);
	return (SortedListIsEmpty(queue->list));
}

int PriorityQEnqueue(priority_q_t *queue, void *data)
{
	sorted_list_iter_t status_iter;

	assert (NULL != queue);
	assert (NULL != data);

	status_iter = SortedListInsert(queue->list, data);

	return (SortedListIsSameIter(SortedListEnd(queue->list), status_iter));
}

void *PriorityQDequeue(priority_q_t *queue)
{
	assert (NULL != queue);

	return (SortedListPopBack(queue->list));	
}

void *PriorityQErase(priority_q_t *queue, priority_q_is_match_t is_match, const void *param)
{
	sorted_list_iter_t to_find;
	void *data_to_return = NULL;

	assert (NULL != queue);
	assert (NULL != is_match);
	assert (NULL != param);

	to_find = SortedListFindIf(SortedListBegin(queue->list), 
					 SortedListEnd(queue->list), is_match, param);
	data_to_return = SortedListGetData(to_find);
	if (!SortedListIsSameIter(to_find, SortedListEnd(queue->list)))
	{
		SortedListRemove(to_find);
	}

	return (data_to_return);

}

size_t PriorityQSize(const priority_q_t *queue)
{
	assert (NULL != queue);
	return (SortedListSize(queue->list));
}
void *PriorityQPeek(const priority_q_t *queue)
{
	return (SortedListGetData(SortedListIterPrev(SortedListEnd(queue->list))));
}

void PriorityQClear(priority_q_t *queue)
{
	sorted_list_iter_t runner;

	assert (NULL != queue);
	
	runner = SortedListBegin(queue->list);
	while (!SortedListIsSameIter(runner, SortedListEnd(queue->list)))
	{
		runner = SortedListRemove(SortedListBegin(queue->list));	
	}
	
}

