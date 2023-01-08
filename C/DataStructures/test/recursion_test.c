#include <stdio.h> /* printf */
#include <stdlib.h> /* rand */
#include <time.h> /* time */

#include "recursion.h" /* declerations */

#define STACK_SIZE (10)

static void FlipTest();
static void FibTest();
static void StrLenTest();
static void StrCmpTest();
static void StrCpyTest();
static void StrCatTest();
static void StrStrTest();
static void SortStackTest();
static void AssignRand(int *array, size_t size);

int main(void)
{
	
	FibTest();
	FlipTest();
	StrLenTest();
	StrCmpTest();
	StrCpyTest();
	StrCatTest();
	StrStrTest();
	SortStackTest();
	return (0);
}

static void FlipTest()
{
    size_t i = 0;
    int data_array[5] = {1, 2, 3, 4, 5};
    node_t node_array[5];
    node_t *head = NULL;
    for (i = 0; i < 5; ++i)
    {
        node_array[i].data = data_array[i];
        node_array[i].next = &node_array[i+1];
    }
    node_array[4].next = NULL;

    head = FlipList(node_array);
    if(node_array[4].next == head->next && 
    (node_array[3].next == head->next->next))
    {
        printf("Flip Working!                                        V\n");
    }
    else
    {
        printf("Flip NOT Working!                                    X\n");
    }
}

static void FibTest()
{	
	size_t i = 0;
	int is_same = 1;
	for (i = 0; i < 10; ++i)
	{
		if (Fibonacci(i) != FibonacciVer2(i))
		{
			is_same = 0;
			break;
		}
	}

	if (is_same)
	{
		printf("Fibonacci Working!                                   V\n");
	}
	else
	{
		printf("Fibonacci NOT Working!                               X\n");
	}
}

static void StrLenTest()
{
	char *test1 = "Hello World";
	char *test2 = "   ";
	char *test3 = "\0";
	char *test4 = "   Hi   ";

	if (11 == StrLen(test1) && 3 == StrLen(test2) &&
		0 == StrLen(test3) && 8 == StrLen(test4))
	{
		printf("StrLen Working!                                      V\n");
    }
    else
    {
        printf("StrLen NOT Working!                                  X\n");
    }
}

static void StrCmpTest()
{
	char *test1 = "Hello";
	char *test2 = "hello";
	char *test3 = "World";
	char *test4 = "World";
	char *test5 = "WorldD";

	if (('H'-'h') == StrCmp(test1, test2) && 0 == StrCmp(test3, test4) &&
		('h' - 'W') == StrCmp(test2, test3) && ('D') == StrCmp(test5, test4))
	{
		printf("StrCmp Working!                                      V\n");
    }
    else
    {
        printf("StrCmp NOT Working!                                  X\n");
    }
}

static void StrCpyTest()
{
	char *test1 = "Hello";
	char *test2 = "00heo";
	char *test3 = "Hajkwj ad";
	char *test4 = "orld@@@!!!";
	char buffer1[10];
	char buffer2[10];
	char buffer3[10];
	char buffer4[10];

	StrCpy(buffer1, test1);
	StrCpy(buffer2, test2);
	StrCpy(buffer3, test3);
	StrCpy(buffer4, test4);

	if (0 == StrCmp(buffer1, test1) && 0 == StrCmp(buffer2, test2) &&
		0 == StrCmp(buffer3, test3) && 0 == StrCmp(buffer4, test4))
	{
		printf("StrCpy Working!                                      V\n");
    }
    else
    {
        printf("StrCpy NOT Working!                                  X\n");
    }
}

static void StrCatTest()
{
	char str[10] = " world";
	char str2[20] = "hello";
	StrCat(str2,str);
	
	if(0 == StrCmp(str2, "hello world"))
	{
		printf("StrCat working!                                      V\n");
	}
	else
	{
		printf("StrCat NOT working!                                  X\n");
	}
}

static void StrStrTest()
{
	char needle1[10] = "ll";
	char needle2[10] = "f";
	char str2[20] = "hello world";
	char *ret1 = StrStr(str2, needle1);
	char *ret2 = StrStr(str2, needle2);
	
	if(NULL != ret1 && NULL == ret2)
	{
		printf("StrStr working!                                      V\n");
	}
	else
	{
		printf("StrStr NOT working!                                  X\n");
	}
	
}

static void SortStackTest()
{
	int is_sorted = 1;
	size_t i = 0;
	int arr[STACK_SIZE] = {0};
	stack_t *stack;

	srand(time(NULL));
	AssignRand(arr, STACK_SIZE);
	
	stack = StackCreate(STACK_SIZE, sizeof(int));
	
	for ( ; i < STACK_SIZE; ++i)
	{
		StackPush(stack, &arr[i]);
	}

	SortStack(stack);

	for ( i = 0; i < 9; ++i)
	{
		if (*(int *)StackPop(stack) < *(int *)StackPeek(stack))
		{
			is_sorted = 0;
			break;
		}
	}

	if(is_sorted)
	{
		printf("StackSort working!                                   V\n");
	}
	else
	{
		printf("StackSort NOT working!                               X\n");
	}


	StackDestroy(stack);
}

static void AssignRand(int *array, size_t size)
{
	size_t i;

	for (i = 0; i < size; ++i)
	{
		array[i] = rand() % 100; 
	}
}

