#include <stdio.h> /* printf */
#include <stdlib.h> /* free */

#include "../include/slist.h"

int AddToNum(void* ptr, void* param);
int IntMatch(const void *data, const void *param);
void TestAllFuncs();
void AppendTester();

int main()
{
	TestAllFuncs();
	AppendTester();
	return (0);
}

void AppendTester()
{
    int arr[20];
    int i = 0;
    int status = 1;
    slist_t * first_list = SListCreate();
    slist_t * second_list = SListCreate();
    slist_iter_t iter_first = SListBegin(first_list);
    slist_iter_t iter_second = SListBegin(second_list);

    printf("filling first_list....\n");
    for(i = 0;i < 10;++i)
    {
        arr[i] = i;
        iter_first = SListInsertBefore(iter_first, (void *)&arr[i]);
    }
    status = (SListCount(first_list) == 10);
    if(1 != status)
    {
        printf("filling list failed! using SListInsertBefore\n");
        return;
    }

    printf("filling second_list....\n");
    for(;i < 20;++i)
    {
        arr[i] = i;
        iter_second = SListInsertBefore(iter_second, (void *)&arr[i]);
    }
    status = (SListCount(second_list) == 10);
    if(1 != status)
    {
        printf("filling list failed! using SListInsertBefore\n");
        return;
    }
    SListAppend(first_list, second_list);

    status = (SListCount(first_list) == 20);
    if(1 != status)
    {
        printf("append failed!\n");
        return;
    }
    printf("destroying the first_list...\n");
    SListDestroy(first_list);
    printf("destroying the second_list...\n");
    SListDestroy(second_list);
    printf("SUCCESS");
}

void TestAllFuncs()
{
    int arr[100];
    int i = 0;
    int fifty = 50;
    int num = 10;
    int to_find = 45;
    slist_iter_t iter = NULL;
    slist_iter_t iter_end = NULL;
    slist_iter_t found_iter = NULL;
    slist_t * list = NULL;

	list = SListCreate();
	iter = SListBegin(list);
	
	if(NULL != list)
    {
    	printf("SListCreate working!                                 V\n");
	}
	else
	{
		printf("SListCreate NOT working!                             X\n");
	}
    
    for(i = 0;i < 100;++i)
    {
        arr[i] = i;
        iter = SListInsertBefore(iter, (void *)&arr[i]);
    }
    if(100 == SListCount(list))
    {
    	printf("SListCount working!                                  V\n");
	}
	else
	{
		printf("SListCount NOT working!                              X\n");
	}
    if(0 != SListCount(list))
    {
    	printf("SListInsertBefore working!                           V\n");
	}
	else
	{
		printf("SListInsertBefore NOT working!                       X\n");
	}
    
    iter = SListBegin(list);
    iter_end = SListEnd(list);
    
    if(99 == *(int *)SListGetData(iter))
    {
    	printf("SListBegin & SListGetData working!                   V\n");
	}
	else
	{
		printf("SListBegin & SListGetData NOT working!               X\n");
	}

	if(NULL == (iter_end = SListIterNext(iter_end)))
    {
    	printf("SListEnd working!                                    V\n");
	}
	else
	{
		printf("SListEnd NOT working!                                X\n");
	}
	
	SListRemove(iter);
	
    if(98 == *(int *)SListGetData(iter))
    {
    	printf("SListRemove working!                                 V\n");
	}
	else
	{
		printf("SListRemove NOT working!                             X\n");
	}
    
	SListSetData(iter, (void *)&fifty);
	
	if(50 == *(int *)SListGetData(iter))
    {
    	printf("SListSetData working!                                V\n");
	}
	else
	{
		printf("SListSetData NOT working!                            X\n");
	}
	
	iter = SListIterNext(iter);
	
	if(97 == *(int *)SListGetData(iter))
    {
    	printf("SListIterNext working!                               V\n");
	}
	else
	{
		printf("SListIterNext NOT working!                           X\n");
	}
	
	iter = SListBegin(list);
    iter_end = SListEnd(list);

	SListForEach(iter, iter_end, AddToNum, (void*)&num);
	
	if(60 == *(int *)SListGetData(iter))
    {
    	printf("SListForEach working!                                V\n");
	}
	else
	{
		printf("SListForEach NOT working!                            X\n");
	}
	
	found_iter = SListFind(iter, iter_end, IntMatch, (void*)&to_find);

	if(45 == *(int *)SListGetData(found_iter))
    {
    	printf("SListFind working!                                   V\n");
	}
	else
	{
		printf("SListFind NOT working!                               X\n");
	}
	
    SListDestroy(list);
}


int AddToNum(void* ptr, void* param)
{
	*(int*)ptr += *(int*)param;
	return (0);
}

int IntMatch(const void *data, const void *param)
{
    return (*(int *)data == *(int *)param);
}


