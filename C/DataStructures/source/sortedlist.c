#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include "sortedlist.h"

#define FOUND (0)
#define TRUE (1)

static sorted_list_iter_t DoublyToSortedIter(dlist_iter_t doubly_iter,
													const sorted_list_t *list);

static void ListAssertDebug(sorted_list_iter_t iter1, sorted_list_iter_t iter2);

struct sortedlist
{
	dlist_t *dlist;
	sorted_list_cmp_t func;
};


sorted_list_t *SortedListCreate(sorted_list_cmp_t func)
{
	sorted_list_t *list = NULL;
	assert (NULL != func);

	list = (sorted_list_t *)malloc(sizeof(sorted_list_t));
	
	if (NULL != list)
	{
		list->func = func;
		list->dlist = DoublyListCreate();
		if (NULL == list->dlist)
		{
			free(list);
			list = NULL;
		}
	}

	return (list);
}

void SortedListDestroy(sorted_list_t *list)
{
	assert (NULL != list);
	DoublyListDestroy(list->dlist);
	free(list);
}

int SortedListIsEmpty(const sorted_list_t *list)
{
	assert (NULL != list);
	return (DoublyListIsEmpty(list->dlist));	
}

sorted_list_iter_t SortedListInsert(sorted_list_t *list, void *data)
{
	sorted_list_iter_t runner;
	sorted_list_iter_t list_end;

	assert (NULL != list);
	assert (data != list);

	list_end = SortedListEnd(list);
	runner = SortedListBegin(list);

	if (!(SortedListIsEmpty(list)))
	{
		while (!(SortedListIsSameIter(runner, list_end)) &&
				(0 > list->func((SortedListGetData(runner)), data)))
		{
			runner = SortedListIterNext(runner);
		}	
	}
	runner.internal_iter = DoublyListInsertBefore(list->dlist, 
													runner.internal_iter, 
													(const void *)data);

	return (runner);
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t where)
{

	assert (NULL != where.internal_iter);

	where.internal_iter = DoublyListRemove(where.internal_iter);

	return (where);

}

void *SortedListPopBack(sorted_list_t *list)
{
	assert (NULL != list);

	return (DoublyListPopBack(list->dlist));	
}

void *SortedListPopFront(sorted_list_t *list)
{
	assert (NULL != list);

	return (DoublyListPopFront(list->dlist));
}

sorted_list_iter_t SortedListFind(const sorted_list_iter_t from, 
								  const sorted_list_iter_t to, 
								  const sorted_list_t *list, 
								  const void *param)
{
	sorted_list_iter_t from_runner = from;

	assert (NULL != from.internal_iter);
	assert (NULL != to.internal_iter);
	assert (NULL != param);
	assert (NULL != list);

	ListAssertDebug(from, to);

	while (!SortedListIsSameIter(from_runner, to))
	{
		if (FOUND == list->func(SortedListGetData(from_runner), param))
		{
			break;
		}
		from_runner = SortedListIterNext(from_runner);
	}

	return (from_runner);	
}


sorted_list_iter_t SortedListFindIf(const sorted_list_iter_t from, 
									const sorted_list_iter_t to, 
									sorted_list_is_match_t is_match, 
									const void *param)
{
	sorted_list_iter_t ret;
	assert (NULL != is_match);
	assert (NULL != param);
	assert (NULL != from.internal_iter);
	assert (NULL != to.internal_iter);

	#ifndef NDEBUG
	ListAssertDebug(from, to);
	ret.list = from.list;
	#endif /* NDEBUG */

	ret.internal_iter = DoublyListFind(from.internal_iter, to.internal_iter, 
												is_match, (const void *)param);
	return (ret);
}

size_t SortedListSize(const sorted_list_t *list)
{
	assert (NULL != list);
	
	return (DoublyListSize(list->dlist));	
}

void *SortedListGetData(const sorted_list_iter_t where)
{
	assert (NULL != where.internal_iter);

	return (DoublyListGetData(where.internal_iter));
}

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to, 
						sorted_list_action_t action_func, void *param)
{
	assert (NULL != action_func);
	assert (NULL != param);
	assert (NULL != from.internal_iter);
	assert (NULL != to.internal_iter);

	ListAssertDebug(from, to);

	return (DoublyListForEach(from.internal_iter, to.internal_iter, action_func, param));
}

sorted_list_iter_t SortedListIterNext(sorted_list_iter_t curr)
{
	assert (NULL != curr.internal_iter);

	curr.internal_iter = DoublyListIterNext(curr.internal_iter);

	return (curr);
}

sorted_list_iter_t SortedListIterPrev(sorted_list_iter_t curr)
{
	assert (NULL != curr.internal_iter);

	curr.internal_iter = DoublyListIterPrev(curr.internal_iter); 
	return (curr);
}

sorted_list_iter_t SortedListBegin(const sorted_list_t *list)
{
	sorted_list_iter_t list_begin;
	
	assert (NULL != list);

	list_begin = DoublyToSortedIter(DoublyListBegin(list->dlist), list);
	
	return (list_begin);	
}

sorted_list_iter_t SortedListEnd(const sorted_list_t *list)
{
	sorted_list_iter_t list_end;

	assert (NULL != list);

	list_end = DoublyToSortedIter(DoublyListEnd(list->dlist), list);
	
	return (list_end);
}

int SortedListIsSameIter(sorted_list_iter_t iter_one, sorted_list_iter_t iter_two)
{
	assert (NULL != iter_one.internal_iter);
	assert (NULL != iter_two.internal_iter);

	return(DoublyListIsSameIter(iter_one.internal_iter, iter_two.internal_iter));
}

void SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list)
{
	sorted_list_iter_t dest_runner;
	sorted_list_iter_t src_from_runner;
	sorted_list_iter_t src_to_runner;

	assert (NULL != dest_list);
	assert (NULL != src_list);

	dest_runner = SortedListBegin(dest_list);
	src_from_runner = SortedListBegin(src_list);
	src_to_runner = src_from_runner;

	while (TRUE)
	{
		if(SortedListIsSameIter(dest_runner, SortedListEnd(dest_list)) ||
			SortedListIsSameIter(src_to_runner, SortedListEnd(src_list)))
		{
			DoublyListSplice(src_from_runner.internal_iter, 
							 SortedListEnd(src_list).internal_iter, 
							 dest_runner.internal_iter);
			break;
		}
		
		if(0 <= dest_list->func(SortedListGetData(dest_runner),
							SortedListGetData(src_to_runner)))
		{
			src_to_runner = SortedListIterNext(src_to_runner);
			continue;
		}

		if(0 > dest_list->func(SortedListGetData(dest_runner),
								SortedListGetData(src_from_runner)))
		{
			dest_runner = SortedListIterNext(dest_runner);
			continue;
		}
		
		DoublyListSplice(src_from_runner.internal_iter,
						 src_to_runner.internal_iter,
						 dest_runner.internal_iter);
		
		src_from_runner = SortedListBegin(src_list);
		src_to_runner = src_from_runner;
	}
}

static sorted_list_iter_t DoublyToSortedIter(
dlist_iter_t doubly_iter, const sorted_list_t *list)
{
	sorted_list_iter_t iter;
	iter.internal_iter = doubly_iter;
	#ifndef NDEBUG
	iter.list = (sorted_list_t*) list;	
	#endif
	(void)list;
	return (iter);
}

static void ListAssertDebug(sorted_list_iter_t iter1, sorted_list_iter_t iter2)
{
	(void)iter1;
	(void)iter2;
	#ifndef NDEBUG
	assert(iter1.list == iter2.list);
	#endif
}
