#include <stdio.h> /* printf */

#include "scheduler_heap.h"

static void TestAllFuncs();
static void TestCreate();
static void TestDestroy();
static void TestAddTask();
static void TestRemoveTask();
static void TestSizeNIsEmpty();
static void TestClear();
static void TestRunNStop();

static int PrintLuckyNum(void *param);
static int PrintHi(void *param);
static int StopScheduler(void *param);
static int PrintSomething(void *param);
static int GetSize(void *param);
static int ClearScheduler(void *param);

int main()
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestAddTask();
	TestRemoveTask();
	TestSizeNIsEmpty();
	TestClear();
	TestRunNStop();
	TestDestroy();
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestDestroy()
{
	printf("*Run vlg to test SortedListDestroy*\n");
}

static void TestCreate()
{
	scheduler_t *scheduler = HSchedulerCreate();

	if (NULL != scheduler)
	{
		printf("HSchedulerCreate working!                             V\n");
	}
	else
	{
		printf("HSchedulerCreate NOT working!                         X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestAddTask()
{
	int num1 = 5;
	int num2 = 15;
	int size_before = -1;
	int size_after = -1;
	scheduler_t *scheduler = HSchedulerCreate();

	size_before = HSchedulerSize(scheduler);

	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num1, 17);
	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num2, 15);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num2, 18);

	size_after = HSchedulerSize(scheduler);

	if ((0 == size_before) && (4 == size_after))
	{
		printf("HSchedulerAddTask working!                            V\n");
	}
	else
	{
		printf("HSchedulerAddTask NOT working!                        X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestRemoveTask()
{
	int num1 = 5;
	int num2 = 15;
	UID_t to_remove1;
	UID_t to_remove2;
	int size_before = -1;
	int size_after = -1;
	scheduler_t *scheduler = HSchedulerCreate();

	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num1, 10);
	to_remove1 = HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num2, 15);
	to_remove2 = HSchedulerAddTask(scheduler, PrintHi, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num2, 18);

	size_before = HSchedulerSize(scheduler);

	HSchedulerRemoveTask(scheduler, to_remove1);
	HSchedulerRemoveTask(scheduler, to_remove2);

	size_after = HSchedulerSize(scheduler);

	if ((4 == size_before) && (2 == size_after))
	{
		printf("HSchedulerRemoveTask working!                         V\n");
	}
	else
	{
		printf("HSchedulerRemoveTask NOT working!                     X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestSizeNIsEmpty()
{
	int num1 = 5;
	int num2 = 15;
	int empty = -1;
	int not_empty = -1;
	int size_before = -1;
	int size_after = -1;

	scheduler_t *scheduler = HSchedulerCreate();

	size_before = HSchedulerSize(scheduler);
	empty = HSchedulerIsEmpty(scheduler);

	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num1, 10);
	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num2, 15);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num2, 18);

	size_after = HSchedulerSize(scheduler);
	not_empty = HSchedulerIsEmpty(scheduler);

	if ((0 == size_before) && (4 == size_after))
	{
		printf("HSchedulerSize working!                               V\n");
	}
	else
	{
		printf("HSchedulerSize NOT working!                           X\n");
	}

	if ((1 == empty) && (0 == not_empty))
	{
		printf("HSchedulerIsEmpty working!                            V\n");
	}
	else
	{
		printf("HSchedulerIsEmpty NOT working!                        X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestClear()
{
	int num1 = 5;
	int num2 = 15;
	int size_before = -1;
	int size_after = -1;

	scheduler_t *scheduler = HSchedulerCreate();

	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num1, 10);
	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num2, 15);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num1, 12);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num2, 18);

	size_before = HSchedulerSize(scheduler);

	HSchedulerClear(scheduler);

	size_after = HSchedulerSize(scheduler);

	if ((4 == size_before) && (0 == size_after))
	{
		printf("HSchedulerSize working!                               V\n");
	}
	else
	{
		printf("HSchedulerSize NOT working!                           X\n");
	}

	HSchedulerDestroy(scheduler);
}

static void TestRunNStop()
{
	int num1 = 5;
	int num2 = 123;
	int status = -1;
	int status2 = -1;

	scheduler_t *scheduler = HSchedulerCreate();
	scheduler_t *scheduler2 = HSchedulerCreate();

	HSchedulerAddTask(scheduler, GetSize, (void *)scheduler, 2);
	HSchedulerAddTask(scheduler, PrintLuckyNum, (void *)&num1, 2);
	HSchedulerAddTask(scheduler, PrintHi, (void *)&num2, 3);
	HSchedulerAddTask(scheduler, PrintSomething, (void *)&num1, 7);
	HSchedulerAddTask(scheduler, ClearScheduler, (void *)scheduler, 10);
	HSchedulerAddTask(scheduler, StopScheduler, (void *)scheduler, 15);

	printf("*First scheduler should stop in 15 seconds*\n");
	status = HSchedulerRun(scheduler);

	HSchedulerAddTask(scheduler2, PrintSomething, (void *)&num1, 3);

	printf("*Second scheduler should stop in 5 seconds*\n");
	status2 = HSchedulerRun(scheduler2);

	if ((0 == status) && (0 == status2))
	{
		printf("HSchedulerRun & HSchedulerStop working!                V\n");
	}
	else
	{
		printf("%d %d\n", status, status2);
		printf("HSchedulerRun & HSchedulerStop NOT working!            X\n");
	}

	HSchedulerDestroy(scheduler);
	HSchedulerDestroy(scheduler2);
}

static int PrintLuckyNum(void *param)
{
	printf("\033[1;32m");
	printf("Your Lucky number is: %d\n", *(int *)param);
	printf("\033[0m");
	return (0);
}

static int PrintHi(void *param)
{
	(void)param;
	printf("\033[1;33m");
	printf("Hi :)\n");
	printf("\033[0m");
	return (0);
}

static int PrintSomething(void *param)
{
	(void)param;
	printf("\033[1;35m");
	printf("I am a func that prints something only once (or twice)\n");
	printf("\033[0m");
	return (1);
}

static int StopScheduler(void *param)
{
	HSchedulerStop((scheduler_t *)param);

	return (1);
}

static int GetSize(void *param)
{
	printf("\033[1;36m");
	printf("List Size: %ld\n", HSchedulerSize((scheduler_t *)param));
	printf("\033[0m");
	return (0);
}

static int ClearScheduler(void *param)
{
	HSchedulerClear((scheduler_t *)param);

	return (1);
}
