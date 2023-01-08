#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "heap.h"	/* Heap Create, Destroy etc */
#include "pq_heap.h"

#define FAIL (1)
#define SUCCESS (0)

struct priority_q
{
	bin_heap_t *list;
};

priority_q_t *PQCreate(priority_q_cmp_t func)
{
	priority_q_t *queue = NULL;

	assert(NULL != func);

	queue = (priority_q_t *)malloc(sizeof(priority_q_t));
	if (NULL != queue)
	{
		queue->list = BinHeapCreate(func);

		if (NULL == queue->list)
		{
			free(queue);
			return (NULL);
		}
	}
	return (queue);
}

void PQDestroy(priority_q_t *queue)
{
	assert(NULL != queue);
	BinHeapDestroy(queue->list);
	queue->list = NULL;
	free(queue);
}

int PQIsEmpty(const priority_q_t *queue)
{
	assert(NULL != queue);
	return (BinHeapIsEmpty(queue->list));
}

int PQEnqueue(priority_q_t *queue, void *data)
{
	assert(NULL != queue);
	assert(NULL != data);

	return (BinHeapPush(queue->list, data));
}

void *PQDequeue(priority_q_t *queue)
{
	void *to_remove = NULL;
	assert(NULL != queue);
	to_remove = PQPeek(queue);
	BinHeapPop(queue->list);
	return (to_remove);
}

void *PQErase(priority_q_t *queue, priority_q_is_match_t is_match, const void *param)
{
	assert(NULL != queue);
	assert(NULL != is_match);
	assert(NULL != param);

	return (BinHeapRemove(queue->list, is_match, (void *)param));
}

size_t PQSize(const priority_q_t *queue)
{
	assert(NULL != queue);
	return (BinHeapSize(queue->list));
}
void *PQPeek(const priority_q_t *queue)
{
	return (BinHeapPeek(queue->list));
}

void PQClear(priority_q_t *queue)
{
	assert(NULL != queue);

	while (!BinHeapIsEmpty(queue->list))
	{
		BinHeapPop(queue->list);
	}
}
