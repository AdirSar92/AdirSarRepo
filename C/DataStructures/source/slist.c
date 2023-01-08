#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "slist.h"

#define SUCCESS (0)
#define FAIL (-1)

struct slist_node
{
	void *data;
	slist_node_t *next;
} list_node_t;

struct slist
{
	slist_node_t *head;
	slist_node_t *tail;
} list_t;

/* Approved By Keren */
static slist_node_t *CreateNode();
static int Increase(void *ptr, void *param);

slist_t *SListCreate(void)
{
	slist_node_t *dummy_node = NULL;
	slist_t *list = (slist_t *)malloc(sizeof(list_t));

	if (NULL == list)
	{
		return (NULL);
	}

	dummy_node = CreateNode();
	if (NULL == dummy_node)
	{
		free(list);
		return (NULL);
	}

	list->head = dummy_node;
	list->tail = dummy_node;

	dummy_node->data = list;
	dummy_node->next = NULL;

	return (list);
}

void SListDestroy(slist_t *list)
{
	slist_node_t *current_node = NULL;
	slist_node_t *next_node = NULL;

	assert(NULL != list);

	current_node = list->head;

	while (NULL != current_node)
	{
		next_node = current_node->next;
		current_node->data = '\0';
		free(current_node);
		current_node = next_node;
	}

	free(list);
}

slist_iter_t SListFind(slist_iter_t from, slist_iter_t to, is_match_t func, void *data)
{
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != func);

	while (!(SListIsIterEqual(from, to)))
	{
		if (func((from->data), data))
		{
			return (from);
		}
		from = SListIterNext(from);
	}
	return (from);
}

void SListRemove(slist_iter_t iter)
{
	slist_node_t *temp_next = NULL;

	assert(NULL != iter);

	temp_next = SListIterNext(iter);

	iter->data = temp_next->data;
	iter->next = temp_next->next;

	if (NULL == iter->next)
	{
		((slist_t *)(iter->data))->tail = iter;
	}

	free(temp_next);
}

slist_iter_t SListInsertBefore(slist_iter_t place_to_insert, const void *data)
{
	slist_node_t *new_node = CreateNode();
	if (NULL == new_node)
	{
		return (place_to_insert);
	}

	assert(NULL != place_to_insert);

	new_node->data = place_to_insert->data;
	new_node->next = place_to_insert->next;

	if (NULL == new_node->next)
	{
		((slist_t *)new_node->data)->tail = new_node;
	}

	place_to_insert->data = (void *)data;
	place_to_insert->next = new_node;

	return (place_to_insert);
}

size_t SListCount(const slist_t *list)
{
	size_t count = 0;
	slist_iter_t start_iter = NULL;
	slist_iter_t end_iter = NULL;

	assert(NULL != list);

	start_iter = SListBegin(list);
	end_iter = SListEnd(list);

	SListForEach(start_iter, end_iter, Increase, (void *)&count);

	return (count);
}

const void *SListGetData(const slist_iter_t iter)
{
	assert(NULL != iter);

	return (iter->data);
}

void SListSetData(slist_iter_t iter, void *data)
{
	assert(NULL != iter);
	iter->data = data;
}

int SListForEach(slist_iter_t from, slist_iter_t to,
				 slist_action_t action_func, void *param)
{
	int status = SUCCESS;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action_func);

	while (!(SListIsIterEqual(from, to)) && FAIL != status)
	{
		status = action_func(from->data, param);
		from = SListIterNext(from);
	}
	return (status);
}

slist_iter_t SListIterNext(slist_iter_t iter)
{
	assert(NULL != iter);

	return (iter->next);
}

slist_iter_t SListBegin(const slist_t *list)
{
	assert(NULL != list);

	return (list->head);
}

slist_iter_t SListEnd(const slist_t *list)
{

	assert(NULL != list);

	return (list->tail);
}

int SListIsIterEqual(slist_iter_t iter_one, slist_iter_t iter_two)
{
	assert(NULL != iter_one);
	assert(NULL != iter_two);

	return (iter_one == iter_two);
}

static slist_node_t *CreateNode()
{
	slist_node_t *new_node = NULL;
	new_node = (slist_node_t *)malloc(sizeof(slist_node_t));
	if (NULL == new_node)
	{
		return (NULL);
	}
	return (new_node);
}

static int Increase(void *ptr, void *param)
{
	(void)ptr;

	++*(size_t *)param;

	return (SUCCESS);
}

void SListAppend(slist_t *slist_dest, slist_t *slist_src)
{
	slist_iter_t dest_dummy;
	slist_iter_t src_head;
	slist_iter_t src_dummy;
	slist_node_t *new_dummy = CreateNode();

	assert(NULL != slist_dest);
	assert(NULL != slist_src);

	dest_dummy = SListEnd(slist_dest);
	src_head = SListBegin(slist_src);
	src_dummy = SListEnd(slist_src);

	new_dummy->data = slist_src;
	new_dummy->next = NULL;

	dest_dummy->next = src_head;
	slist_dest->tail = src_dummy;
	src_dummy->data = slist_dest;

	SListRemove(dest_dummy);

	slist_src->head = new_dummy;
	slist_src->tail = new_dummy;
}