#include "../include/queue.h"
#include <stdio.h> /* printf */

int main(void)
{
	int arr[20];
	int arr1[20];
	size_t i = 0;
	
	queue_t * new_q = QCreate();
	queue_t * new_q2 = QCreate();
	if (0 == QGetSize(new_q))
	{
		printf ("QCreate						V\n");
	}
	else
	{
		printf ("QCreate			        	X\n");
	}
	for (i = 0; i < 10; ++i)
	{
		arr[i] = i;
		QEnQueue(new_q, (void *)&arr[i]);

	}
	if (10 == QGetSize(new_q))
	{
		printf ("QEnQueue			    	    	V\n");
	}
	else
	{
		printf ("QEnQueue				        	X\n");
	}
	QDeQueue(new_q);
	if (9 == QGetSize(new_q))
	{
		printf ("QDeQueue			    	    	V\n");
	}
	else
	{
		printf ("QDeQueue				        	X\n");
	}
	if (1 == *(int *)QPeek(new_q))
	{
		printf ("QPeek					    	V\n");
	}
	else
	{
		printf ("QPeek						    X\n");
	}

	if (9 == QGetSize(new_q))
	{
		printf ("QGetSize					V\n");
	}
	else
	{
		printf ("QGetSize					X\n");
	}
	if (0 == QIsEmpty(new_q))
	{
		printf ("QIsEmpty					V\n");
	}
	else
	{
		printf ("QIsEmpty					X\n");
	}

	for (i = 0; i < 10; ++i)
	{
		arr1[i] = 10 + i;
		QEnQueue(new_q2, (void *)&arr1[i]);
	}
	QAppend(new_q, new_q2);

	for (i = 0; i < 19; ++i)
	{
		QDeQueue(new_q);
	}
	if (0 == QGetSize(new_q))
	{
		printf ("QAppend						V\n");
	}
	else
	{
		printf ("QAppend						X\n");
	}

	QDestroy(new_q);
	QDestroy(new_q2);
	return 0;
}
