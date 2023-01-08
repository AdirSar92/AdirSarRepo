#include "../include/circbuff.h"
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* strlen */

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define FLEXIBLE_ARRAY (4)
#define ARRAY_STARTING_POINT (FLEXIBLE_ARRAY * sizeof(size_t))

#define FAIL (-1)
#define EMPTY_ARRAY (0)

enum indexes
{
	ELEMENT_COUNTER,
	TOTAL_CAPACITY,
	READ_INDEX,
	WRITE_INDEX
};

enum size_change
{
	DECREASE = -1,
	INCREASE = 1
};

static void IncreaseRequestedIndex(circbuff_ptr_t buffer, int index);
static void ChangeCurrentSize(circbuff_ptr_t buffer, int size);

/*
 * buff[0] = Buffer current size
 * buff[1] = Capacity
 * buff[2] = Read Index
 * buff[3] = Write Index
 */

/* checked by Noa */
typedef struct circbuff
{
	size_t buff[FLEXIBLE_ARRAY];
} circbuff_t;

circbuff_ptr_t CircBuffCreate(size_t capacity)
{
	circbuff_ptr_t buffer = (circbuff_ptr_t)malloc(sizeof(circbuff_t) +
												   capacity);

	buffer->buff[ELEMENT_COUNTER] = EMPTY_ARRAY;
	buffer->buff[TOTAL_CAPACITY] = capacity;
	buffer->buff[READ_INDEX] = ARRAY_STARTING_POINT;
	buffer->buff[WRITE_INDEX] = ARRAY_STARTING_POINT;

	return (buffer);
}

void CircBuffDestroy(circbuff_ptr_t buffer)
{
	assert(NULL != buffer);

	free(buffer);
	buffer = NULL;
}

ssize_t CircBuffWrite(circbuff_ptr_t buffer, const void *to_read_from, size_t num_of_bytes)
{
	char *read_byte_runner = (char *)to_read_from;
	char *write_byte_runner;
	ssize_t byte_counter = 0;

	assert(NULL != buffer);
	assert(NULL != to_read_from);

	if (NULL == to_read_from || NULL == buffer)
	{
		return (FAIL);
	}

	write_byte_runner = (char *)buffer->buff;

	while (0 < num_of_bytes--)
	{
		write_byte_runner[buffer->buff[WRITE_INDEX]] = *read_byte_runner++;
		IncreaseRequestedIndex(buffer, WRITE_INDEX);
		ChangeCurrentSize(buffer, INCREASE);
		++byte_counter;
	}

	return (byte_counter);
}

ssize_t CircBuffRead(circbuff_ptr_t buffer, void *to_write_to, size_t num_of_bytes)
{
	char *write_byte_runner = (char *)to_write_to;
	char *read_byte_runner = NULL;
	ssize_t byte_counter = 0;

	if (NULL == to_write_to || NULL == buffer)
	{
		return (FAIL);
	}

	assert(NULL != buffer);
	assert(NULL != to_write_to);

	byte_counter = MIN(num_of_bytes, CircBuffSize(buffer));
	read_byte_runner = (char *)buffer->buff;

	while ((0 < num_of_bytes--) && !(CircBuffIsEmpty(buffer)))
	{
		*write_byte_runner++ = read_byte_runner[buffer->buff[READ_INDEX]];
		IncreaseRequestedIndex(buffer, READ_INDEX);
		ChangeCurrentSize(buffer, DECREASE);
		++byte_counter;
	}

	return (byte_counter);
}

int CircBuffIsEmpty(const circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	return (0 == buffer->buff[ELEMENT_COUNTER]);
}

size_t CircBuffSize(const circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	return (buffer->buff[ELEMENT_COUNTER]);
}

size_t CircBuffFreeSpace(const circbuff_ptr_t buffer)
{
	assert(NULL != buffer);
	return (buffer->buff[TOTAL_CAPACITY] - buffer->buff[ELEMENT_COUNTER]);
}

static void IncreaseRequestedIndex(circbuff_ptr_t buffer, int index)
{
	assert(NULL != buffer);

	buffer->buff[index] += 1;

	if (buffer->buff[index] >
		(buffer->buff[TOTAL_CAPACITY] + ARRAY_STARTING_POINT - 1))
	{
		buffer->buff[index] = ARRAY_STARTING_POINT;
	}
}

static void ChangeCurrentSize(circbuff_ptr_t buffer, int size)
{
	assert(NULL != buffer);

	buffer->buff[ELEMENT_COUNTER] += size;

	if (buffer->buff[ELEMENT_COUNTER] > buffer->buff[TOTAL_CAPACITY])
	{
		--buffer->buff[ELEMENT_COUNTER];
		buffer->buff[READ_INDEX] = buffer->buff[WRITE_INDEX];
	}
}
