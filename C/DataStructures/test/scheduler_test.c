#include <stdio.h> /* printf */

#include "scheduler.h"

static void TestAllFuncs();
static void TestCreate();
static void TestDestroy();
static void TestAddTask();
static void TestRemoveTask();
static void TestSizeNIsEmpty();
static void TestClear();
static void TestRunNStop();

static int PrintLuckyNum(void* param);
static int PrintHi(void* param);
static int StopScheduler(void* param);
static int PrintSomething(void* param);
static int GetSize(void *param);
static int ClearScheduler(void *param);

int main()
{
	TestAllFuncs();
	return(0);
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
	scheduler_t *scheduler = SchedulerCreate();
	
	if(NULL != scheduler)
	{
		printf("SchedulerCreate working!                             V\n");
	}
	else
	{
		printf("SchedulerCreate NOT working!                         X\n");
	}
	
	SchedulerDestroy(scheduler);
}

static void TestAddTask()
{
	int num1 = 5;
	int num2 = 15;
	int size_before = -1;
	int size_after = -1;
	scheduler_t *scheduler = SchedulerCreate();
	
	size_before = SchedulerSize(scheduler);
	
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num1, 17);
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num2, 15);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num1, 12);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num2, 18);
	
	size_after = SchedulerSize(scheduler);
	
	if((0 == size_before) && (4 == size_after))
	{
		printf("SchedulerAddTask working!                            V\n");
	}
	else
	{
		printf("SchedulerAddTask NOT working!                        X\n");
	}
	
	SchedulerDestroy(scheduler);
}

static void TestRemoveTask()
{
	int num1 = 5;
	int num2 = 15;
	UID_t to_remove1;
	UID_t to_remove2;
	int size_before = -1;
	int size_after = -1;
	scheduler_t *scheduler = SchedulerCreate();
	
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num1, 10);
	to_remove1 = SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num2, 15);
	to_remove2 = SchedulerAddTask(scheduler, PrintHi, (void*)&num1, 12);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num2, 18);
	
	size_before = SchedulerSize(scheduler);
	
	SchedulerRemoveTask(scheduler, to_remove1);
	SchedulerRemoveTask(scheduler, to_remove2);
	
	size_after = SchedulerSize(scheduler);
	
	if((4 == size_before) && (2 == size_after))
	{
		printf("SchedulerRemoveTask working!                         V\n");
	}
	else
	{
		printf("SchedulerRemoveTask NOT working!                     X\n");
	}
	
	SchedulerDestroy(scheduler);
}

static void TestSizeNIsEmpty()
{
	int num1 = 5;
	int num2 = 15;
	int empty = -1;
	int not_empty = -1;
	int size_before = -1;
	int size_after = -1;
	
	scheduler_t *scheduler = SchedulerCreate();
	
	size_before = SchedulerSize(scheduler);
	empty = SchedulerIsEmpty(scheduler);
	
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num1, 10);
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num2, 15);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num1, 12);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num2, 18);
	
	size_after = SchedulerSize(scheduler);
	not_empty = SchedulerIsEmpty(scheduler);
	
	if((0 == size_before) && (4 == size_after))
	{
		printf("SchedulerSize working!                               V\n");
	}
	else
	{
		printf("SchedulerSize NOT working!                           X\n");
	}
	
	if((1 == empty) && (0 == not_empty))
	{
		printf("SchedulerIsEmpty working!                            V\n");
	}
	else
	{
		printf("SchedulerIsEmpty NOT working!                        X\n");
	}
	
	SchedulerDestroy(scheduler);
}

static void TestClear()
{
	int num1 = 5;
	int num2 = 15;
	int size_before = -1;
	int size_after = -1;
	
	scheduler_t *scheduler = SchedulerCreate();
		
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num1, 10);
	SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num2, 15);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num1, 12);
	SchedulerAddTask(scheduler, PrintHi, (void*)&num2, 18);
	
	size_before = SchedulerSize(scheduler);

	SchedulerClear(scheduler);
	
	size_after = SchedulerSize(scheduler);
	
	if((4 == size_before) && (0 == size_after))
	{
		printf("SchedulerSize working!                               V\n");
	}
	else
	{
		printf("SchedulerSize NOT working!                           X\n");
	}
	
	SchedulerDestroy(scheduler);
}

static void TestRunNStop()
{
    int num1 = 5;
    int num2 = 123;
    int status = -1;
    int status2 = -1;
    
    scheduler_t *scheduler = SchedulerCreate();
    scheduler_t *scheduler2 = SchedulerCreate();
    
    SchedulerAddTask(scheduler, GetSize, (void*)scheduler, 2);
    SchedulerAddTask(scheduler, PrintLuckyNum, (void*)&num1, 2);
    SchedulerAddTask(scheduler, PrintHi, (void*)&num2, 3);
    SchedulerAddTask(scheduler, PrintSomething, (void*)&num1, 7);
    SchedulerAddTask(scheduler, ClearScheduler, (void*)scheduler, 10);
    SchedulerAddTask(scheduler, StopScheduler, (void*)scheduler, 15);
    	
    printf("*First scheduler should stop in 15 seconds*\n");
    status = SchedulerRun(scheduler);
    
    SchedulerAddTask(scheduler2, PrintSomething, (void*)&num1, 3);
    
    printf("*Second scheduler should stop in 5 seconds*\n");
    status2 = SchedulerRun(scheduler2);
    
    if((0 == status) && (0 == status2))
	{
		printf("SchedulerRun & SchedulerStop working!                V\n");
	}
	else
	{
		printf ("%d %d\n", status, status2);
		printf("SchedulerRun & SchedulerStop NOT working!            X\n");
	}
    
    SchedulerDestroy(scheduler);
    SchedulerDestroy(scheduler2);
}


static int PrintLuckyNum(void *param)
{
	printf("\033[1;32m");
	printf("Your Lucky number is: %d\n", *(int*)param);
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

static int PrintSomething(void* param)
{
	(void)param;
	printf("\033[1;35m");
	printf("I am a func that prints something only once (or twice)\n");
	printf("\033[0m");
	return (1);
}

static int StopScheduler(void *param)
{
    SchedulerStop((scheduler_t *)param);
    
    return (1);
}

static int GetSize(void *param)
{
	printf("\033[1;36m");
    printf ("List Size: %ld\n", SchedulerSize((scheduler_t *)param));
	printf("\033[0m");
    return (0);
}

static int ClearScheduler(void *param)
{
    SchedulerClear((scheduler_t *)param);
    
    return (1);
}
