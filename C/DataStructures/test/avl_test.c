#include <stdio.h> /* printf */
#include <stdlib.h> /* rand */
#include <time.h> /* time */

#include "avl.h" /* AVL */

#define SUCCESS (0)
#define TEST_SIZE (100)

typedef struct student
{
	long ID_number;
	int grade;
}student_t;

static void TestAllFuncs();
static void TestCreate();
static void TestSizeAndEmpty();
static void TestInsert();
static void TestRemove();
static void TestDestroy();
static void TestFind();
static void TestForEach();
static void TestHeight();
static void TestBalance();

static void AssignRand(int *arr, size_t size);
static void FillStudent(student_t *student);

static int IntCompare(const void *data1, const void *data2);
static int IntAdd(void *data, void *param);

int main(void)
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
	TestRemove();
	TestFind();
	TestHeight();
	TestForEach();
	TestBalance();
	TestDestroy();
	
	printf("      ~END OF TEST FUNCTION~ \n");
}

static void TestCreate()
{
	int arr[TEST_SIZE] = {0};
	size_t i = 0;

	avl_t *tree = AVLCreate(IntCompare);

	AssignRand(arr, sizeof(arr)/sizeof(arr[0]));

	for ( ; i < TEST_SIZE; ++i)
	{
		 
		AVLInsert(tree, (void*)&arr[i]);
	}

	printf ("\033[1;31m");
	
	if(NULL != tree)
	{
		printf("AVLCreate working!                                   V\n");
	}
	else
	{
		printf("AVLCreate NOT working!                               X\n");
	}
	
	AVLDestroy(tree);
}

static void TestFind()
{
	size_t i = 1;
	int not_there = 203126669;
	avl_t *tree = AVLCreate(IntCompare);

	student_t s1 = { 201370731, 95 };
	student_t students[TEST_SIZE]; 

	FillStudent(students);

	AVLInsert(tree, &s1);

	for ( ; i < TEST_SIZE; ++i)
	{
		AVLInsert(tree, &students[i].ID_number);
	}

	printf ("\033[1;33m");
	if(201370731 == *(int *)AVLFind(tree, &s1.ID_number) && NULL == AVLFind(tree, &not_there))
    {
    	printf("AVLFind working!                                     V\n");
	}
	else
	{
		printf("AVLFind NOT working!                                 X\n");
	}
	
	AVLDestroy(tree);
}

static void TestSizeAndEmpty()
{	
	int x = 3;
	int y = 5;
	int before = -1;
	int after = -1;
	int empty = -1;
	int not_empty = -1;
	
	avl_t *tree = AVLCreate(IntCompare);
	
	before = AVLSize(tree);
	empty = AVLIsEmpty(tree);
	
	AVLInsert(tree, (void *)&x);
	AVLInsert(tree, (void *)&y);
	
	after = AVLSize(tree);
	not_empty = AVLIsEmpty(tree);
	
	printf ("\033[1;32m");
	if(0 == before && 2 == after)
	{
		printf("AVLSize working!             		             V\n");
	}
	else
	{
		printf("AVLSize NOT working!     	                     X\n");
	}
	printf ("\033[1;37m");
	if(1 == empty && 0 == not_empty)
	{
		printf("AVLIsEmpty working!                      	     V\n");
	}
	else
	{
		printf("AVLIsEmpty NOT working!                      X\n");
	}
	
	AVLDestroy(tree);
}

static void TestInsert()
{
	int arr[TEST_SIZE] = {0};
	size_t i = 0;

	avl_t *tree = AVLCreate(IntCompare);

	AssignRand(arr, sizeof(arr)/sizeof(arr[0]));

	for ( ; i < TEST_SIZE; ++i)
	{
		AVLInsert(tree, (void*)&arr[i]);
	}

	printf ("\033[1;35m");
	
	if(TEST_SIZE == AVLSize(tree))
	{
		printf("AVLInsert working!                                   V\n");
	}
	else
	{
		printf("AVLInsert NOT working!                               X\n");
	}
	
	AVLDestroy(tree);
}


static void TestRemove()
{
	int arr[TEST_SIZE] = {0};
	size_t i = 0;

	avl_t *tree = AVLCreate(IntCompare);

	AssignRand(arr, sizeof(arr)/sizeof(arr[0]));

	for ( ; i < TEST_SIZE; ++i)
	{
		AVLInsert(tree, (void *)&arr[i]);
	}

	for (i = 10 ; i < 20; ++i)
	{
		AVLRemove(tree, (void *)&arr[i]);
	}

	printf ("\033[1;36m");
	
	if(TEST_SIZE - 10 == AVLSize(tree))
	{
		printf("AVLRemove working!                                   V\n");
	}
	else
	{
		printf("AVLRemove NOT working!                               X\n");
	}
	
	AVLDestroy(tree);
}

static void TestHeight()
{
	int arr[] = {12, 7, 15, 4, 8, 9, 18, 13, 17, 16};
	size_t height_before = 0, height_after = 0;
	size_t i = 0;

	avl_t *tree = AVLCreate(IntCompare);
	
	AVLInsert(tree, &arr[0]);
	height_before = AVLHeight(tree);

	for (i = 1 ; i < 10; ++i)
	{
		AVLInsert(tree, &arr[i]);
	}
	
	height_after = AVLHeight(tree);
	printf ("\033[1;34m");
	if(1 == height_before && 4 == height_after)
	{
		printf("AVLHeight working!                                   V\n");
	}
	else
	{
		printf("AVLHeight NOT working!                               X\n");
	}
	
	AVLDestroy(tree);

}

static void TestForEach()
{
	int a = 5;
	int b = 9;
	int c = 3;
	int d = 7;
	int not_there = 3;
	int num_to_add = 10;
	
	avl_t *tree = AVLCreate(IntCompare);
	
	AVLInsert(tree, (void *)&a);
	AVLInsert(tree, (void *)&b);
	AVLInsert(tree, (void *)&c);
	AVLInsert(tree, (void *)&d);
	
	AVLForEach(tree, IntAdd, (void *)&num_to_add, IN_ORDER);
	printf ("\033[1;35m");
	if(15 == *(int *)AVLFind(tree, &a) && NULL == AVLFind(tree, &not_there))
    {
    	printf("AVLForEach working!                                  V\n");
	}
	else
	{
		printf("AVLForEach NOT working!                              X\n");
	}
	
	AVLDestroy(tree);
}

static void TestBalance()
{
	int arr[] = { 12, 7, 15, 4, 8, 9, 18 };
	size_t height_before = 0, height_after_balance = 0;
	size_t i = 0;

	avl_t *tree = AVLCreate(IntCompare);
	
	AVLInsert(tree, &arr[0]);
	height_before = AVLHeight(tree);

	for (i = 1 ; i < 7; ++i)
	{
		AVLInsert(tree, &arr[i]);
	}
	height_after_balance = AVLHeight(tree);

	printf ("\033[1;34m");
	if(1 == height_before && 4 == height_after_balance)
	{
		printf("AVLBalance working!                                  V\n");
	}
	else
	{
		printf("AVLBalance NOT working!                              X\n");
	}
	
	AVLDestroy(tree);

}

static void TestDestroy()
{
	printf ("\033[1;36m");
	printf("*Run vlg to test AVLDestroy*\n");
	printf ("\033[0m");
}

static int IntCompare(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}

static int IntAdd(void *data, void *param)
{
	*(int *)data += *(int *)param;
	return (SUCCESS);
}

static void AssignRand(int *arr, size_t size)
{
	size_t i = 0;
	(void)size;

	srand(time(NULL));
	for (i = 0; i < TEST_SIZE; ++i)
	{
		arr[i] = rand() % 10000;
	}
}

static void FillStudent(student_t *student)
{
	size_t i = 0, j = 0;

	srand(time(NULL));
	for (i = 0; i < TEST_SIZE; ++i)
	{
		student[i].ID_number = rand() % 100000000;
		student[i].grade = rand() % 100;
		for (j = 0; j < i; ++j)
		{
			if (student[j].grade == student[i].grade)
			{
				student[i].grade = rand() % 100;
				--j;
			}
		}
	}
}


