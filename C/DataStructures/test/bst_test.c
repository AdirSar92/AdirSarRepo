#include <stdio.h> /* printf */

#include "bst.h"

static int IntCompare(const void *data1, const void *data2);
static int IntAdd(void *data, void *param);

static void TestAllFuncs();
static void TestCreate();
static void TestSizeAndEmpty();
static void TestInsert();
static void TestGetData();
static void TestRemove();
static void TestIterFuncs();
static void TestDestroy();
static void TestFind();
static void TestForEach();


int main()
{
	TestAllFuncs();
	return (0);
}

static void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestSizeAndEmpty();
	TestInsert();
	TestGetData();
	TestRemove();
	TestIterFuncs();
	TestFind();
	TestForEach();
	TestDestroy();
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestCreate()
{
	int x = 3;
	int y = 5;
	bst_t *tree = BSTCreate(IntCompare);
	
	printf ("\033[1;31m");
	
	if(NULL != tree)
	{
		printf("BSTCreate working!                                   V\n");
	}
	else
	{
		printf("BSTCreate NOT working!                               X\n");
	}
	
	BSTInsert(tree, (void*)&x);
	BSTInsert(tree, (void*)&y);
	
	BSTDestroy(tree);
}

static void TestDestroy()
{
	printf ("\033[1;36m");
	printf("*Run vlg to test BSTDestroy*\n");
	printf ("\033[0m");
}

static void TestSizeAndEmpty()
{	
	int x = 3;
	int y = 5;
	int before = -1;
	int after = -1;
	int empty = -1;
	int not_empty = -1;
	
	bst_t *tree = BSTCreate(IntCompare);
	
	before = BSTSize(tree);
	empty = BSTIsEmpty(tree);
	
	BSTInsert(tree, (void*)&x);
	BSTInsert(tree, (void*)&y);
	
	after = BSTSize(tree);
	not_empty = BSTIsEmpty(tree);
	printf ("\033[1;32m");
	if(0 == before && 2 == after)
	{
		printf("BSTSize working!          		             V\n");
	}
	else
	{
		printf("BSTSize NOT working!	                     X\n");
	}
	printf ("\033[1;37m");
	if(1 == empty && 0 == not_empty)
	{
		printf("BSTIsEmpty working!                      	     V\n");
	}
	else
	{
		printf("BSTIsEmpty NOT working!                      X\n");
	}
	
	BSTDestroy(tree);
}

static void TestInsert()
{
	int a = 5, b = 3, c = 4, d = 2, e = 8, f = 12, g = 10, h = 9, i = 11, j = 14;
	bst_iter_t runner = NULL;
	int first, second, third;

	bst_t *tree = BSTCreate(IntCompare);

	BSTInsert(tree, &a);
	BSTInsert(tree, &b);
	BSTInsert(tree, &c);
	BSTInsert(tree, &d);
	BSTInsert(tree, &e);
	BSTInsert(tree, &f);
	BSTInsert(tree, &g);
	BSTInsert(tree, &h);
	BSTInsert(tree, &i);
	BSTInsert(tree, &j);
	
	runner = BSTBegin(tree);
	first = *(int*)BSTGetData(runner);
	runner = BSTIterNext(runner);
	second = *(int*)BSTGetData(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	third = *(int*)BSTGetData(runner);
	printf ("\033[1;33m");
	if((2 == first) && (3 == second) && (5 == third))
	{
		printf("BSTInsert working!                                   V\n");
	}
	else
	{
		printf("BSTInsert NOT working!                               X\n");
	}
	
	BSTDestroy(tree);
}


static void TestGetData()
{
	int a = 5, b = 3, c = 4;
	bst_iter_t runner = NULL;
	

	bst_t *tree = BSTCreate(IntCompare);

	BSTInsert(tree, &a);
	BSTInsert(tree, &b);
	BSTInsert(tree, &c);
	
	runner = BSTBegin(tree);
	runner = BSTIterNext(runner);
	
	printf ("\033[1;34m");
	if(3 == *(int*)BSTGetData(BSTBegin(tree)) && 4 == *(int *)BSTGetData(runner))
	{
		printf("BSTGetData working!                                  V\n");
	}
	else
	{
		printf("BSTGetData NOT working!                              X\n");
	}
	
	BSTDestroy(tree);
}

static void TestRemove()
{
	int a = 5, b = 3, c = 4;
	int size_before = -1, size_after = -1;

	bst_t *tree = BSTCreate(IntCompare);

	BSTInsert(tree, &a);
	BSTInsert(tree, &b);
	BSTInsert(tree, &c);
	
	size_before = BSTSize(tree);
	
	BSTRemove(BSTBegin(tree));
	BSTRemove(BSTBegin(tree));
	
	size_after = BSTSize(tree);
	printf ("\033[1;35m");
	if((2 == size_before - size_after) && 
	5 == *(int *)BSTGetData(BSTBegin(tree)))
	{
		printf("BSTRemove working!                                   V\n");
	}
	else
	{
		printf("BSTRemove NOT working!                               X\n");
	}
	
	BSTDestroy(tree);
}

static void TestIterFuncs()
{
	int a = 5, b = 3, c = 4, d = 2, e = 8, f = 12, g = 10, h = 9, i = 11, j = 14;
	bst_iter_t runner = NULL, runner2 = NULL;
	int first, second, third;

	bst_t *tree = BSTCreate(IntCompare);

	BSTInsert(tree, &a);
	BSTInsert(tree, &b);
	BSTInsert(tree, &c);
	BSTInsert(tree, &d);
	BSTInsert(tree, &e);
	BSTInsert(tree, &f);
	BSTInsert(tree, &g);
	BSTInsert(tree, &h);
	BSTInsert(tree, &i);
	BSTInsert(tree, &j);
	
	runner = BSTBegin(tree);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	first = *(int *)BSTGetData(runner);


	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterNext(runner);
	runner = BSTIterPrev(runner);	
	second = *(int *)BSTGetData(runner);

	runner = BSTIterNext(runner);
	runner = BSTIterPrev(runner);
	runner = BSTIterPrev(runner);
	third = *(int *)BSTGetData(runner);

	runner2 = BSTBegin(tree);
	runner2 = BSTIterNext(runner2);	
	runner2 = BSTIterNext(runner2);	
	runner2 = BSTIterNext(runner2);	
	runner2 = BSTIterNext(runner2);	
	runner2 = BSTIterNext(runner2);	
	runner2 = BSTIterNext(runner2);	
	runner = BSTIterPrev(runner);


	printf ("\033[1;36m");
	if(2 == *(int *)BSTGetData(BSTBegin(tree)))
	{
		printf("BSTBegin working!                                    V\n");
	}
	else
	{
		printf("BSTBegin NOT working!                                X\n");
	}
	printf ("\033[1;37m");
	if(NULL == (bst_t*)BSTGetData(BSTEnd(tree)))
	{
		printf("BSTEnd working!                                      V\n");
	}
	else
	{
		printf("BSTEnd NOT working!                                  X\n");
	}
	printf ("\033[1;35m");
	if(9 == first)
	{
		printf("BSTIterNext working!                                 V\n");
	}
	else
	{
		printf("BSTIterNext NOT working!                             X\n");
	}
	printf ("\033[1;31m");
	if(12 == second && 11 == third )
	{
		printf("BSTIterPrev working!                                 V\n");
	}
	else
	{
		printf("BSTIterPrev NOT working!                             X\n");
	}
	printf ("\033[1;32m");
	if(runner2 == runner)
	{
		printf("BSTIsSameIter working!                               V\n");
	}
	else
	{
		printf("BSTIsSameIter NOT working!                    X\n");
	}
	
	BSTDestroy(tree);
}


static void TestFind()
{
	int a = 5;
	int b = 9;
	int c = 3;
	int d = 7;
	int to_find = 5;
	int not_there = 12;

	bst_iter_t not_found;
	bst_iter_t found_iter;
	
	bst_t *tree = BSTCreate(IntCompare);
	
	BSTInsert(tree, (void*)&a);
	BSTInsert(tree, (void*)&b);
	BSTInsert(tree, (void*)&c);
	BSTInsert(tree, (void*)&d);
	
	found_iter = BSTFind(tree, (void *)&to_find);
	not_found = BSTFind(tree, (void *)&not_there);
	printf ("\033[1;33m");
	if(5 == *(int *)BSTGetData(found_iter) && BSTEnd(tree) == not_found)
    {
    	printf("BSTFind working!                                     V\n");
	}
	else
	{
		printf("BSTFind NOT working!                                 X\n");
	}
	
	BSTDestroy(tree);
}

static void TestForEach()
{
	int a = 5;
	int b = 9;
	int c = 3;
	int d = 7;
	int num_to_add = 10;
	
	bst_t *tree = BSTCreate(IntCompare);
	
	BSTInsert(tree, (void*)&a);
	BSTInsert(tree, (void*)&b);
	BSTInsert(tree, (void*)&c);
	BSTInsert(tree, (void*)&d);
	
	BSTForEach(BSTBegin(tree), BSTEnd(tree), IntAdd, (void *)&num_to_add);
	printf ("\033[1;34m");
	if(13 == *(int*)BSTGetData(BSTBegin(tree)) && 
		19 == *(int*)BSTGetData(BSTIterPrev(BSTEnd(tree))))
    {
    	printf("BSTForEach working!                                  V\n");
	}
	else
	{
		printf("BSTForEach NOT working!                              X\n");
	}
	
	BSTDestroy(tree);
}



static int IntCompare(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}

int IntAdd(void *data, void *param)
{
	*(int *)data += *(int *)param;

	return 0;
}


