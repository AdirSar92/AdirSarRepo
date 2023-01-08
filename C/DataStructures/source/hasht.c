/* ask everyone time complexity of find, insert, remove (Average and worst).space complexity.
	destroy function
	lazy initialization - option for creating list only when needed
 */
/*========================== LIBRARIES && MACROS =============================*/
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <math.h>	/* pow, sqrt */

#include "dlist.h" /* dlist */
#include "hasht.h" /* declerations */

#define TRUE (1)
#define FAIL (1)
#define SUCCESS (0)
#define FALSE (0)

/*================================= STRUCTS ==================================*/
struct hasht
{
	dlist_t **table;
	hash_func_t hash_func;
	hash_is_match_t is_match;
	size_t expected_capacity;
};

/*============================ STATIC DECLERATIONS ===========================*/

static size_t GetIndex(hasht_t *hash_table, void *key);
static void DestroyList(hasht_t *hash_table, size_t size);

/*============================ FUNCTION DEFINITIONS ==========================*/
hasht_t *HashtCreate(size_t expected_capacity, hash_is_match_t is_match, hash_func_t hash_func)
{
	long i = 0;
	hasht_t *hash_table = (hasht_t *)malloc(sizeof(hasht_t));
	if (NULL == hash_table)
	{
		return (NULL);
	}
	hash_table->table = (dlist_t **)malloc(sizeof(dlist_t *) * expected_capacity);
	if (NULL == hash_table->table)
	{
		free(hash_table);
		return (NULL);
	}

	hash_table->hash_func = hash_func;
	hash_table->is_match = is_match;
	hash_table->expected_capacity = expected_capacity;

	for (; i < (long)expected_capacity; ++i)
	{
		hash_table->table[i] = DoublyListCreate();
		if (NULL == hash_table->table[i])
		{
			break;
		}
	}
	if (i != (long)expected_capacity)
	{
		while (i-- > 0)
		{
			DestroyList(hash_table, i);
		}
		free(hash_table);
	}

	return (hash_table);
}

void HashtDestroy(hasht_t *hash_table)
{
	DestroyList(hash_table, hash_table->expected_capacity);
	free(hash_table->table);
	free(hash_table);
}

int HashtIsEmpty(const hasht_t *hash_table)
{
	size_t i = 0;
	assert(NULL != hash_table);
	for (; i < hash_table->expected_capacity; ++i)
	{
		if (!DoublyListIsEmpty(hash_table->table[i]))
		{
			return (FALSE);
		}
	}
	return (TRUE);
}

int HashtInsert(hasht_t *hash_table, void *data)
{
	size_t index = 0;

	assert(NULL != hash_table);

	index = GetIndex(hash_table, data);

	if (DoublyListEnd(hash_table->table[index]) ==
		DoublyListPushBack(hash_table->table[index], data))
	{
		return (FAIL);
	}
	return (SUCCESS);
}

static size_t GetIndex(hasht_t *hash_table, void *key)
{
	return (hash_table->hash_func(key) % hash_table->expected_capacity);
}

void HashtRemove(hasht_t *hash_table, void *key)
{
	size_t index = 0;
	dlist_iter_t to_remove = NULL;
	dlist_t *curr_list = NULL;

	assert(NULL != hash_table);

	index = GetIndex(hash_table, key);
	curr_list = hash_table->table[index];
	to_remove = DoublyListFind(DoublyListBegin(curr_list),
							   DoublyListEnd(curr_list),
							   hash_table->is_match, key);

	if (!DoublyListIsSameIter(to_remove, DoublyListEnd(curr_list)))
	{
		DoublyListRemove(to_remove);
	}
}

void *HashtFind(const hasht_t *hash_table, const void *key)
{
	size_t index = 0;
	dlist_iter_t found = NULL;
	dlist_t *curr_list = NULL;
	void *ret = NULL;

	assert(NULL != hash_table);

	index = GetIndex((hasht_t *)hash_table, (void *)key);
	curr_list = hash_table->table[index];
	found = DoublyListFind(DoublyListBegin(curr_list),
						   DoublyListEnd(curr_list),
						   hash_table->is_match, key);
	ret = DoublyListGetData(found);
	if (!DoublyListIsSameIter(found, DoublyListEnd(curr_list)))
	{
		DoublyListRemove(found);
		DoublyListPushFront(curr_list, ret);
	}

	return (ret);
}

size_t HashtSize(const hasht_t *hash_table)
{
	size_t i = 0, count = 0;
	assert(NULL != hash_table);

	for (; i < hash_table->expected_capacity; ++i)
	{
		count += DoublyListSize(hash_table->table[i]);
	}
	return (count);
}

int HashtForEach(hasht_t *hash_table, action_func_t action_func, void *param)
{
	size_t i = 0;
	int status = SUCCESS;

	dlist_t *curr_list = NULL;

	assert(NULL != hash_table);
	assert(NULL != action_func);

	for (; i < hash_table->expected_capacity && FAIL != status; ++i)
	{
		curr_list = hash_table->table[i];
		status = DoublyListForEach(DoublyListBegin(curr_list), DoublyListEnd(curr_list), action_func, param);
	}
	return (status);
}

double HashtLoad(const hasht_t *hash_table)
{
	assert(NULL != hash_table);
	return ((double)HashtSize(hash_table) / hash_table->expected_capacity);
}

double HashtStandartDeviation(const hasht_t *hash_table)
{
	double sum = 0;
	double average = 0;
	size_t i = 0;

	assert(NULL != hash_table);

	average = HashtLoad(hash_table);

	for (; i < hash_table->expected_capacity; ++i)
	{
		sum = sum + pow((DoublyListSize(hash_table->table[i]) - average), 2);
	}
	sum /= hash_table->expected_capacity;

	return (sqrt(sum));
}

static void DestroyList(hasht_t *hash_table, size_t size)
{
	size_t i = 0;
	for (; i < size; ++i)
	{
		DoublyListDestroy(hash_table->table[i]);
	}
}