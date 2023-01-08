#include <stdio.h> /* printf */

#include "pq_heap.h"

static void TestAllFuncs();
static void TestCreate();
static void TestDestroy();
static void TestClear();
static void TestEnqueueNDequeue();
static void TestSizeAndEmpty();
static void TestErase();
static void TestPeek();

static int IntCmp(const void *data, const void *data2);
static int ModCmp(const void *data, const void *param);

int main()
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestEnqueueNDequeue();
	TestSizeAndEmpty();
	TestErase();
	TestPeek();
	TestClear();
	TestDestroy();
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestCreate()
{
	priority_q_t *queue = PQCreate(IntCmp);

	if (NULL != queue)
	{
		printf("PQCreate working!                             V\n");
	}
	else
	{
		printf("PQCreate NOT working!                         X\n");
	}

	PQDestroy(queue);
}

static void TestDestroy()
{
	printf("*Run vlg to test SortedListDestroy*\n");
}

static void TestEnqueueNDequeue()
{
	int x = 3;
	int between = 4;
	int y = 5;
	int first = -1;
	int second = -1;
	int third = -1;

	priority_q_t *queue = PQCreate(IntCmp);

	PQEnqueue(queue, (void *)&x);
	PQEnqueue(queue, (void *)&y);
	PQEnqueue(queue, (void *)&between);

	third = *(int *)PQDequeue(queue);
	second = *(int *)PQDequeue(queue);
	first = *(int *)PQDequeue(queue);

	if ((5 == third) && (4 == second) && (3 == first))
	{
		printf("PQEnqueue & PQDequeue working!                V\n");
	}
	else
	{
		printf("PQEnqueue & PQDequeue NOT working!            X\n");
	}

	PQDestroy(queue);
}

static void TestSizeAndEmpty()
{
	int x = 3;
	int y = 5;
	int before = -1;
	int after = -1;
	int empty = -1;
	int not_empty = -1;

	priority_q_t *queue = PQCreate(IntCmp);

	before = PQSize(queue);
	empty = PQIsEmpty(queue);

	PQEnqueue(queue, (void *)&x);
	PQEnqueue(queue, (void *)&y);

	after = PQSize(queue);
	not_empty = PQIsEmpty(queue);

	if (0 == before && 2 == after)
	{
		printf("PQSize working!                               V\n");
	}
	else
	{
		printf("PQSize NOT working!                           X\n");
	}

	if (1 == empty && 0 == not_empty)
	{
		printf("PQIsEmpty working!                            V\n");
	}
	else
	{
		printf("PQIsEmpty NOT working!                        X\n");
	}

	PQDestroy(queue);
}

static void TestErase()
{
	int x = 3;
	int y = 4;
	int z = 5;
	int to_div = 2;

	priority_q_t *queue = PQCreate(IntCmp);

	PQEnqueue(queue, (void *)&x);
	PQEnqueue(queue, (void *)&y);
	PQEnqueue(queue, (void *)&z);

	y = *(int *)PQErase(queue, ModCmp, (void *)&to_div);
	z = *(int *)PQPeek(queue);
	PQDequeue(queue);
	x = *(int *)PQPeek(queue);
	PQDequeue(queue);

	if ((3 == x) && (4 == y) && (5 == z))
	{
		printf("PQErase working!                              V\n");
	}
	else
	{
		printf("PQErase NOT working!                          X\n");
	}

	PQDestroy(queue);
}

static void TestPeek()
{
	int x = 3;
	int y = 4;
	int z = 5;
	int peeked = -1;

	priority_q_t *queue = PQCreate(IntCmp);

	PQEnqueue(queue, (void *)&y);
	PQEnqueue(queue, (void *)&z);
	PQEnqueue(queue, (void *)&x);

	peeked = *(int *)PQPeek(queue);

	if (5 == peeked)
	{
		printf("PQPeek working!                               V\n");
	}
	else
	{
		printf("PQPeek NOT working!                           X\n");
	}

	PQDestroy(queue);
}

static void TestClear()
{
	int x = 3;
	int y = 4;
	int z = 5;
	int empty = -1;
	int not_empty = -1;

	priority_q_t *queue = PQCreate(IntCmp);

	PQEnqueue(queue, (void *)&x);
	PQEnqueue(queue, (void *)&y);
	PQEnqueue(queue, (void *)&z);

	not_empty = PQIsEmpty(queue);

	PQClear(queue);

	empty = PQIsEmpty(queue);

	if (1 == empty && 0 == not_empty)
	{
		printf("PQClear working!                              V\n");
	}
	else
	{
		printf("PQClear NOT working!                          X\n");
	}

	PQDestroy(queue);
}

static int IntCmp(const void *data, const void *data2)
{
	return (*(int *)data - *(int *)data2);
}

static int ModCmp(const void *data, const void *param)
{
	return (0 == (*(int *)data % *(int *)param));
}
