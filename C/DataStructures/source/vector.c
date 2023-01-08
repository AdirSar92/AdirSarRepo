#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */

#include "vector.h"

#define MIN(a, b) (a > b ? a : b)
#define MIN_CAPACITY (1)
#define GROWTH_RATE (2)
#define MEM_ERR (1)
#define SUCCESS (0)

struct vector
{
	char *base;
	size_t size_of_element;
	size_t capacity;
	size_t size;
};

/* Approved by Tzach Halfon */
vector_t *VectorCreate(size_t initial_capacity, size_t element_size)
{
	vector_t *vector = NULL;

	assert(0 != initial_capacity);

	vector = (vector_t *)malloc(sizeof(vector_t));
	if (NULL == vector)
	{
		return (NULL);
	}

	vector->base = (char *)malloc(element_size * initial_capacity);
	if (NULL == vector->base)
	{
		return (NULL);
	}
	vector->size = 0;
	vector->capacity = MIN(initial_capacity, MIN_CAPACITY);
	vector->size_of_element = element_size;

	return (vector);
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);

	free(vector->base);
	vector->base = NULL;

	free(vector);
	vector = NULL;
}

int VectorPushBack(vector_t *vector, const void *element_to_push)
{
	size_t current_place = 0;
	int status = SUCCESS;

	assert(NULL != vector);
	assert(NULL != element_to_push);

	if (VectorGetSize(vector) == VectorGetCapacity(vector))
	{
		status = VectorReserve(vector, (GROWTH_RATE * vector->capacity));
	}

	if (SUCCESS == status)
	{
		current_place = ((vector->size) * (vector->size_of_element));

		memcpy(vector->base + current_place, element_to_push,
			   vector->size_of_element);

		++vector->size;
	}

	return (status);
}

void *VectorPopBack(vector_t *vector)
{
	size_t shrink_rate = 0;

	assert(NULL != vector);
	assert(0 < vector->size);

	--(vector->size);
	shrink_rate = vector->capacity / (GROWTH_RATE * 2);

	if (vector->size < shrink_rate)
	{
		VectorReserve(vector, (vector->capacity / GROWTH_RATE));
	}

	return ((vector->base + (vector->size * vector->size_of_element)));
}

int VectorReserve(vector_t *vector, size_t new_capacity)
{
	char *temp_base = NULL;
	assert(NULL != vector);

	temp_base = (char *)realloc(vector->base,
								(new_capacity * vector->size_of_element));
	if (NULL == temp_base)
	{
		return (MEM_ERR);
	}
	vector->base = temp_base;

	vector->capacity = new_capacity;

	return (SUCCESS);
}

int VectorShrinkToFit(vector_t *vector)
{
	int status = SUCCESS;
	assert(NULL != vector);

	status = VectorReserve(vector, (vector->size));
	return (status);
}

void *VectorAccessAt(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	assert(index < vector->capacity);

	return ((void *)(vector->base + (index * vector->size_of_element)));
}

size_t VectorGetSize(const vector_t *vector)
{
	assert(NULL != vector);

	return (vector->size);
}

size_t VectorGetCapacity(const vector_t *vector)
{
	assert(NULL != vector);

	return (vector->capacity);
}
int VectorIsEmpty(const vector_t *vector)
{
	assert(vector);

	return ((0 == (vector->size)));
}
