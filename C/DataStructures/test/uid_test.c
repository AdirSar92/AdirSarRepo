#include <stdio.h>	 /* printf */
#include <pthread.h> /* pthread_create */

#include "uid.h"

#define NUM_OF_THREADS (10)
#define UID_PER_THREAD (100)
#define TOTAL_UID_SIZE (NUM_OF_THREADS * UID_PER_THREAD)
UID_t uids[TOTAL_UID_SIZE];

void TestAllFuncs();
void TestCreate();
void TestIsSame();
void TestMultiThreads();
static void *UIDCreateWrapper(void *arg);

int main()
{
	TestAllFuncs();
	return (0);
}

void TestAllFuncs()
{
	printf("     ~START OF TEST FUNCTION~ \n");
	TestCreate();
	TestIsSame();
	TestMultiThreads();
	printf("      ~END OF TEST FUNCTION~ \n");
}

void TestCreate()
{
	UID_t uid = UIDCreate();
	UID_t uid2 = UIDCreate();
	if (uid2.counter > uid.counter && uid.pid == uid2.pid)
	{
		printf("UIDCreate working!                                   V\n");
	}
	else
	{
		printf("UIDCreate NOT working!                               X\n");
	}
}

void TestIsSame()
{
	UID_t uid = UIDCreate();
	UID_t uid2 = uid;

	if ((UIDIsSame(uid, uid2)) && (!UIDIsSame(uid, badUID)))
	{
		printf("UIDIsSame working!                                   V\n");
	}
	else
	{
		printf("UIDIsSame NOT working!                               X\n");
	}
}

static void *UIDCreateWrapper(void *arg)
{
	int i = (size_t)arg * UID_PER_THREAD + 1;
	int limit = i + UID_PER_THREAD;
	for (; i < limit; i++)
	{
		uids[i] = UIDCreate();
	}
	return NULL;
}

void TestMultiThreads()
{
	pthread_t threads[NUM_OF_THREADS];
	size_t i = 0, j = 0;
	int is_same = 0;

	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_create(&threads[i], NULL, UIDCreateWrapper, (void *)i);
	}

	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	for (i = 0; i < TOTAL_UID_SIZE; i++)
	{
		for (j = i + 1; j < TOTAL_UID_SIZE; j++)
		{

			if (UIDIsSame(uids[i], uids[j]) && i != j)
			{
				is_same = 1;
			}
		}
	}

	if (!is_same)
	{
		printf("UIDMultiThread working!                              V\n");
	}
	else
	{
		printf("UIDMultiThread NOT working!                          X\n");
	}
}
