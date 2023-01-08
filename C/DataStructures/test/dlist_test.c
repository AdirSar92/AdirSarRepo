#include "dlist.h"
#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <string.h> /* strcmp */


int AddInt(void *data, void *param);
int IntMatch(const void *data, const void *param);

static void TestAllFuncs();
static void TestCreate();
static void TestSizeAndEmpty();
static void TestInsertBefore();
static void TestGetAndSetData();
static void TestRemove();
static void TestPushAndPop();
static void TestSpilce();
static void TestDestroy();

int main()
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	TestCreate();
	TestSizeAndEmpty();
	TestGetAndSetData();
	TestInsertBefore();
	TestPushAndPop();
	TestRemove();
	TestSpilce();
	TestDestroy();
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestCreate()
{
	int x = 3;
	int y = 5;
	
	dlist_t *list = DoublyListCreate();
	
	if(NULL != list)
	{
		printf("DoublyListCreate working!                            V\n");
	}
	else
	{
		printf("DoublyListCreate NOT working!                        X\n");
	}
	
	DoublyListPushBack(list, (void*)&x);
	DoublyListPushBack(list, (void*)&y);
	
	DoublyListDestroy(list);
}

static void TestSizeAndEmpty()
{
	int x = 3;
	int y = 5;
	int before = -1;
	int after = -1;
	int empty = -1;
	int not_empty = -1;
	
	dlist_t *list = DoublyListCreate();
	
	before = DoublyListSize(list);
	empty = DoublyListIsEmpty(list);
	
	DoublyListPushBack(list, (void*)&x);
	DoublyListPushBack(list, (void*)&y);
	
	after = DoublyListSize(list);
	not_empty = DoublyListIsEmpty(list);
	
	if(0 == before && 2 == after)
	{
		printf("DoublyListSize working!                              V\n");
	}
	else
	{
		printf("DoublyListSize NOT working!                          X\n");
	}
	
	if(1 == empty && 0 == not_empty)
	{
		printf("DoublyListIsEmpty working!                           V\n");
	}
	else
	{
		printf("DoublyListIsEmpty NOT working!                       X\n");
	}
	
	DoublyListDestroy(list);
}

static void TestInsertBefore()
{
	int x = 3;
	int y = 5;
	int between = 4;
	dlist_iter_t last_elem = NULL;
	
	dlist_t *list = DoublyListCreate();
	
	DoublyListPushBack(list, (void*)&x);
	DoublyListPushBack(list, (void*)&y);
	
	last_elem = DoublyListIterPrev(DoublyListEnd(list));
	DoublyListInsertBefore(list, last_elem, (void*)&between);
	
	if((*(int*)(DoublyListGetData(DoublyListBegin(list))) == 3) &&
	(*(int*)(DoublyListGetData(DoublyListIterNext(DoublyListBegin(list)))) == 4))
	{
		printf("DoublyListInsertBefore working!                      V\n");
	}
	else
	{
		printf("DoublyListInsertBefore NOT working!                  X\n");
	}
	
	DoublyListDestroy(list);
}

static void TestDestroy()
{
	printf("*Run vlg to test DoublyListDestroy*\n");
}

static void TestGetAndSetData()
{
	int x = 3;
	int y = 5;
	char to_set[15] = "hello world";
	int old_data = -1;
	char *new_data = "";
	
	dlist_t *list = DoublyListCreate();
	
	DoublyListPushBack(list, (void*)&x);
	DoublyListPushBack(list, (void*)&y);
	
	old_data = *(int*)DoublyListGetData(DoublyListBegin(list));
	
	if(3 == old_data)
	{
		printf("DoublyListGetData working!                           V\n");
	}
	else
	{
		printf("DoublyListGetData NOT working!                       X\n");
	}
	
	DoublyListSetData(DoublyListBegin(list), (void*)&to_set);
	new_data = (char*)DoublyListGetData(DoublyListBegin(list));
	
	if(0 == strcmp(new_data, to_set))
	{
		printf("DoublyListSetData working!                           V\n");
	}
	else
	{
		printf("DoublyListSetData NOT working!                       X\n");
	}
	
	DoublyListDestroy(list);
}

static void TestRemove()
{
	int x = 3;
	int y = 5;
	int z = 7;
	int size_before = -1;
	int size_after = -1;
	
	dlist_t *list = DoublyListCreate();
	
	DoublyListPushBack(list, (void*)&x);
	DoublyListPushBack(list, (void*)&y);
	DoublyListPushBack(list, (void*)&z);
	
	size_before = DoublyListSize(list);
	
	DoublyListRemove(DoublyListIterNext(DoublyListBegin(list)));
	
	size_after = DoublyListSize(list);
	
	if((1 == size_before - size_after) && 
	7 == *(int*)DoublyListGetData(DoublyListIterNext(DoublyListBegin(list))))
	{
		printf("DoublyListRemove working!                            V\n");
	}
	else
	{
		printf("DoublyListRemove NOT working!                        X\n");
	}
	
	DoublyListDestroy(list);
}

static void TestPushAndPop()
{
	int x = 3;
	int y = 5;
	int push_to_back = 88;
	int push_to_front = 22;
	int pop_from_back = -1;
	int pop_from_front = -1;
	
	dlist_t *list = DoublyListCreate();
	
	DoublyListPushBack(list, (void*)&x);
	DoublyListPushBack(list, (void*)&y);
	
	DoublyListPushBack(list, (void*)&push_to_back);
	DoublyListPushFront(list, (void*)&push_to_front);
	
	pop_from_back = *(int*)DoublyListPopBack(list);
	pop_from_front = *(int*)DoublyListPopFront(list);
	
	if(push_to_back == pop_from_back)
	{
		printf("DoublyListPushBack & DoublyListPopBack working!      V\n");
	}
	else
	{
		printf("DoublyListPushBack & DoublyListPopBack NOT working!  X\n");
	}
	
	if(push_to_front == pop_from_front)
	{
		printf("DoublyListPushFront & DoublyListPopFront working!    V\n");
	}
	else
	{
		printf("DoublyListPushFront & DoublyListPopFront NOT working!X\n");
	}
	
	DoublyListDestroy(list);
}

static void TestSpilce()
{

	int arr[10] = {2,3,4,5,6,7,8,9,10,11};
	int arr2[10] = {4,5,6,7,8,9,10,11,12,13};
	int i = 0;
	dlist_t *list = DoublyListCreate();
	dlist_t *list2 = DoublyListCreate();
	
	for(; i < 10; ++i)
	{
		DoublyListPushBack(list, (void*)&arr[i]);
		DoublyListPushBack(list2, (void*)&arr2[i]);
	}
	
	DoublyListSplice(DoublyListBegin(list), DoublyListEnd(list), DoublyListBegin(list2));

	if((0 == DoublyListSize(list)) && (20 == DoublyListSize(list2)))
    {
    	printf("DoublyListSplice working!                            V\n");
	}
	else
	{
		printf("DoublyListSplice NOT working!                        X\n");
	}
	
	DoublyListDestroy(list);
	DoublyListDestroy(list2);
}

int IntMatch(const void *data, const void *param)
{
	return (*(int *)data == *(int *)param);
}

int AddInt(void *data, void *param)
{
	int status = 0;
	assert (NULL != param);

	if (NULL == data)
	{
		status = 1;
	}

	*(int *)data += *(int *)param;

	return (status);
}

