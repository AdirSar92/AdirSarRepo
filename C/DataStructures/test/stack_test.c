#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include "../include/stack.h"

static int RandomNumber(unsigned int rand_num)
{
	static const int p = 2749;
	static const int b = 2767;
	static const int c = 3167;
	
	rand_num = (rand_num * p + b) % c;

	return rand_num - (c / 2);
}

int main (void)
{
	unsigned int i = 0;
	int temp;
	int temp2 = 5;

	stack_ptr_t stack = StackCreate(32, 4);

	printf ("************ Test Stack ***********\n");
	printf ("************ Create Stack with 32 X 4 elements ***********\n");
	printf ("************ Capacity of stack : ***********\n");
	printf ("\t\t\t%lu\n", StackGetCapacity(stack));

	printf ("************ push 20 elements to stack ***********\n");

	for (i = 0; i < 20; ++i)
	{
		temp = RandomNumber(i);
		printf("Is empty? %d\n", StackIsEmpty(stack));
		StackPush(stack, &temp);
		printf("Size: %ld, Capacity: %ld, empty? %d, peeking: %d\n", StackGetSize(stack), StackGetCapacity(stack),
			StackIsEmpty(stack), *(int *)StackPeek(stack));

		printf("Push test %d passed successfully\n\n", 20 - i);
	}

	printf ("************ Size of stack after pushing 20 : ***********\n");
	printf ("\t\t\t%lu\n", StackGetSize(stack));

	printf ("************ Empty stack : ***********\n");

	for (i = 19; i > 0; --i)
	{
		printf("Size: %ld, Capacity: %ld, empty? %d, peeking: %d\n", StackGetSize(stack), StackGetCapacity(stack),
			StackIsEmpty(stack), *(int *)StackPeek(stack));

		
		printf("Pop test %d passed successfully\n\n", 20 - i);
	}

	printf("Peek to last element: %d\n", *(int *)StackPeek(stack));
	printf("Pop the last element: Popping %d\n", *(int *)StackPop(stack));
	printf("Is empty? %d\n", StackIsEmpty(stack));

	printf("Peek to last element, no elements: %d\n", *(int *)StackPeek(stack));
	printf ("************ Push 5 more elements : ***********\n");
	for (i = 0; i < 5; ++i)
	{
		temp = RandomNumber(3*i);
		StackPush(stack, &temp);
		printf("Size: %ld, Capacity: %ld, empty? %d, peeking: %d\n", StackGetSize(stack), StackGetCapacity(stack),
			StackIsEmpty(stack), *(int *)StackPeek(stack));
	}
	StackPush(stack, &temp2);
		printf("Size: %ld, Capacity: %ld, empty? %d, peeking: %d\n", StackGetSize(stack), StackGetCapacity(stack),
			StackIsEmpty(stack), *(int *)StackPeek(stack));

	temp2 = 12;
	StackPop(stack);
	printf("Size: %ld, Capacity: %ld, empty? %d, peeking: %d\n", StackGetSize(stack), StackGetCapacity(stack),
			StackIsEmpty(stack), *(int *)StackPeek(stack));
	StackDestroy(stack);
	printf ("************ Stack destroyed ***********\n");

	return (0);
}
