#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "slist.h" /* slist funcs */
#include "queue.h"


/* Approved By Keren */
struct queue
{
	slist_t *list_ptr;
 	size_t size;
};

queue_t * QCreate(void)
{
	queue_t * q = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == q)
	{
		return (NULL);
	}
	
	q -> list_ptr = SListCreate();
	if (NULL == q)
	{	
		free(q);
		return (NULL);
	}

	q -> size = 0;
	return (q);
}

void QDestroy(queue_t * queue)
{
	assert (NULL != queue);

	SListDestroy(queue -> list_ptr);
	free(queue);
}

int QIsEmpty(const queue_t * queue)
{
	assert (NULL != queue);

	return (0 == (queue -> size));	
}

q_status_t QEnQueue(queue_t * queue, const void *element)
{
	slist_iter_t rear = NULL;

	assert (NULL != queue);

	rear = SListInsertBefore(SListEnd(queue -> list_ptr), element);
	if (NULL == rear)
	{
		return (Q_FAIL);
	}
	++(queue -> size);

	return (Q_SUCCESS);

}

void QDeQueue(queue_t * queue)
{

	assert (NULL != queue);
	
	SListRemove(SListBegin(queue -> list_ptr));

	--(queue -> size);
}


void *QPeek(const queue_t * queue)
{

	assert (NULL != queue);
	
	return ((void *)SListGetData(SListBegin(queue -> list_ptr)));

}


size_t QGetSize(const queue_t * queue)
{
	assert (NULL != queue);

	return (queue -> size);
}

void QAppend(queue_t * qdest, queue_t * qsrc)
{
	assert (NULL != qdest);
	assert (NULL != qsrc);

	qdest -> size += (qsrc -> size);
	SListAppend(qdest -> list_ptr, qsrc -> list_ptr);
	qsrc -> size = 0;
}


void *DeQueue(queue_t * queue)
{
	slist_iter_t front = NULL;
	void *to_return = NULL;

	assert (NULL != queue);
	
	front = SListBegin(queue -> list_ptr);
	to_return = front;
	SListRemove(front);

	--(queue -> size);

	return (to_return);
}