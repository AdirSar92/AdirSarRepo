#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <stdio.h> /* printf */

#include "stack.h"

#define MAX_CAPACITY_IN_BYTES 1048576
#define MIN_2(a, b) (((a) > (b)) ? (b) : (a))

struct stack
{
 	char *top;
 	size_t size_of_element;
 	size_t capacity;
	size_t size;
};

/* Time complexity: O(1) */
/* Space complexity: O(capacity) */
stack_ptr_t StackCreate(size_t capacity, size_t element_size)
{
	stack_ptr_t stack = NULL;

	capacity = MIN_2(capacity, MAX_CAPACITY_IN_BYTES/element_size);
	
	stack = malloc(sizeof(stack_t) + (capacity * element_size));
	if (NULL == stack)
	{
		return (NULL);
	}
	
	stack->top = (char *)stack + sizeof(stack_t);
	stack->capacity = capacity;
	stack->size_of_element = element_size;
	stack->size = 0;

	return (stack);
}

/* Time complexity: O(1) */
/* Space complexity: O(1) */
void StackDestroy(stack_ptr_t stack)
{
	assert (NULL != stack);	
	
	free(stack);
	stack = NULL;
}


/* Time complexity: O(size_of_element) */
/* Space complexity: O(size_of_element) */
void StackPush(stack_ptr_t stack, const void *element_to_push)
{
	assert (NULL != stack);	
	assert (NULL != element_to_push);
	assert((stack -> size) < (stack -> capacity));

	memcpy(stack -> top, element_to_push, stack -> size_of_element);
	stack -> size += 1;
	stack -> top += stack -> size_of_element;
}

/* Time complexity: O(1) */
/* Space complexity: O(1) */
void *StackPop(stack_ptr_t stack)
{
	
	assert (NULL != stack);
	assert(!StackIsEmpty(stack));

	stack -> top -= stack -> size_of_element;
	stack -> size -= 1;
	return ((void *)(stack -> top));

}

/* Time complexity: O(1) */
/* Space complexity: O(1) */
int StackIsEmpty(const stack_ptr_t stack)
{	
	assert (NULL != stack);
	
	return (0 == (stack->size));

}

/* Time complexity: O(1) */
/* Space complexity: O(1) */
size_t StackGetSize(const stack_ptr_t stack)
{
	assert (NULL != stack);

	return (stack->size);	
}

/* Time complexity: O(1) */
/* Space complexity: O(1) */
size_t StackGetCapacity(const stack_ptr_t stack)
{	
	assert (NULL != stack);

	return (stack->capacity);	
}

/* Time complexity: O(1) */
/* Space complexity: O(1) */
void *StackPeek(const stack_ptr_t stack)
{
	assert (NULL != stack);
	assert(!StackIsEmpty(stack));
	
	return ((void *)(stack->top - stack->size_of_element));
}
