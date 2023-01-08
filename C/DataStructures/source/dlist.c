#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "dlist.h"

#define FOUND (1)
#define FAIL (1)
#define SUCCESS (0)

struct dlist_node
{
	void *data;
	struct dlist_node *next;
	struct dlist_node *previous;
};

struct dlist
{
	struct dlist_node head;
	struct dlist_node tail;
};

/* Checked by Tzach */

static dlist_node_t *CreateNode()
{
	dlist_node_t *head_node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if (NULL == head_node)
	{
		return (NULL);
	}
	return (head_node);
}

dlist_t *DoublyListCreate(void)
{
	dlist_t *list = (dlist_t *)malloc(sizeof(dlist_t));
	
	if(NULL != list)
	{
		list->head.data = NULL;
		list->head.next = &list->tail;
		list->head.previous = NULL;
			
		list->tail.data = NULL;
		list->tail.next = NULL;
		list->tail.previous = &list->head;
	}
	
	return (list);
}

void DoublyListDestroy(dlist_t *list)
{
	dlist_node_t *current_node = NULL;
	char clean = '\0';
	size_t list_size = DoublyListSize(list);

	assert (NULL != list);

	current_node = DoublyListBegin(list);

	while (0 < list_size)
	{
		DoublyListSetData(current_node, &clean);
		current_node = DoublyListRemove(current_node);
		--list_size;
	}

	free (list);

}

int DoublyListIsEmpty(const dlist_t *list)
{
	return (DoublyListBegin(list) == &list->tail);
}

dlist_iter_t DoublyListInsertBefore(dlist_t *list, dlist_iter_t where, const void *data)
{
	dlist_node_t *new_node = NULL;

	assert (NULL != list);
	assert (NULL != where);

	new_node = CreateNode();
	if (NULL != new_node)
	{
		new_node -> data = (void *)data;
		new_node -> next = where;
		new_node -> previous = where -> previous;
		where -> previous -> next = new_node;
		where -> previous = new_node;
	}
	else
	{
		return (DoublyListEnd(list));
	}
	
	return (new_node);
}

dlist_iter_t DoublyListRemove(dlist_iter_t where)
{
	dlist_iter_t next_element = NULL;
	dlist_iter_t previous_element = NULL;
	
	assert (NULL != where);

	if ((NULL != where -> next) && (NULL != where->previous))
	{
		previous_element = DoublyListIterPrev(where);
		next_element = DoublyListIterNext(where);

		previous_element->next = next_element;
		next_element->previous = previous_element;

		free(where);
	}
	
	return (next_element);
}

void *DoublyListPopBack(dlist_t *list)
{
	dlist_iter_t node_to_pop = NULL;
	void *popped_data = NULL;

	assert (NULL != list);

	node_to_pop = DoublyListIterPrev(DoublyListEnd(list));
	popped_data = DoublyListGetData(node_to_pop);
	DoublyListRemove(node_to_pop);

	return (popped_data);	
}

void *DoublyListPopFront(dlist_t *list)
{
	dlist_iter_t node_to_pop = NULL;
	void *popped_data = NULL;

	assert (NULL != list);

	node_to_pop = DoublyListBegin(list);
	popped_data = DoublyListGetData(node_to_pop);
	DoublyListRemove(node_to_pop);

	return (popped_data);
}

dlist_iter_t DoublyListPushBack(dlist_t *list, void *data)
{

	assert (NULL != list);

	return (DoublyListInsertBefore(list, DoublyListEnd(list), 
									(const void *)data));

}

dlist_iter_t DoublyListPushFront(dlist_t *list, void *element_to_push)
{
	assert (NULL != list);
	assert (NULL != element_to_push);
	
	return (DoublyListInsertBefore(list, DoublyListBegin(list), 
									(const void *)element_to_push));
}

dlist_iter_t DoublyListFind(const dlist_iter_t from, const dlist_iter_t to, is_match_t func, const void *param)
{
	dlist_iter_t from_runner = from;

	assert (NULL != from);
	assert (NULL != to);
	assert (NULL != param);

	while (from_runner != to)
	{
		if (FOUND == func(from_runner->data, param))
		{
			break;
		}
		from_runner = DoublyListIterNext(from_runner);
	}

	return (from_runner);	
}

int DoublyListMultiFind(const dlist_iter_t from, const dlist_iter_t to, 
		         is_match_t func, const void *param, dlist_t *dest_list)
{
	int is_found = 0;
	dlist_iter_t from_runner = from;
	
	assert (NULL != from);
	assert (NULL != to);
	assert (NULL != param);
	assert (NULL != dest_list);

	while (from_runner != to)
	{
		if (FOUND == func(from->data, param))
		{
			DoublyListInsertBefore(dest_list, from_runner, param);
			is_found = 1;
		}
		from_runner = DoublyListIterNext(from_runner);
	}

	return (is_found);
}

size_t DoublyListSize(const dlist_t *list)
{
	size_t count = 0;
	dlist_iter_t runner = NULL;
	
	assert(NULL != list);
	
	runner = DoublyListBegin(list);
	
	while(runner->next != NULL)
	{
		++count;
		runner = DoublyListIterNext(runner);
	}
    return (count);

}

void *DoublyListGetData(const dlist_iter_t where)
{
	assert (NULL != where);

	return ((void *)(where->data));	
}

void DoublyListSetData(dlist_iter_t where, void *data)
{
	assert (NULL != where);
	
	where->data = data;
}

int DoublyListForEach(dlist_iter_t from, dlist_iter_t to, dlist_action_t action_func, void *param)
{
	int status = 0;
	
	assert (NULL != from);
	assert (NULL != to);
	assert (NULL != param);

	while (from != to && FAIL != status)
	{
		status = action_func(from -> data, param);
		from = DoublyListIterNext(from);
	}

	return (status);
}

dlist_iter_t DoublyListIterNext(const dlist_iter_t where)
{
	assert (NULL != where);

	return (where->next);
}

dlist_iter_t DoublyListIterPrev(const dlist_iter_t where)
{
	assert (NULL != where);

	return (where->previous);
}

dlist_iter_t DoublyListBegin(const dlist_t *list)
{
	assert (NULL != list);

	return (DoublyListIterNext((dlist_iter_t)&list->head));
}

dlist_iter_t DoublyListEnd(const dlist_t *list)
{
	assert (NULL != list);

	return ((dlist_iter_t)&list->tail);
}

int DoublyListIsSameIter(const dlist_iter_t iter_one, const dlist_iter_t iter_two)
{
	assert (NULL != iter_one);
	assert (NULL != iter_two);

	return (iter_one == iter_two);	
}

dlist_iter_t DoublyListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t where)
{
	dlist_iter_t before_where = DoublyListIterPrev(where);
	dlist_iter_t before_from = DoublyListIterPrev(from);
    dlist_iter_t before_to = DoublyListIterPrev(to);

	assert (NULL != from);
	assert (NULL != to);
	assert (NULL != where);	

    before_where->next = from;
    from->previous = before_where;

    before_to->next = where;
    where->previous = before_to;

    before_from->next = to;
    to->previous = before_from;

	return (where->previous);
}
