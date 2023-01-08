#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h> /* size_t */
#include "stack.h" /* Stack */

typedef struct node node_t;

struct node
{
	int data;
	node_t *next;
};

int Fibonacci(int element_index);
int FibonacciVer2(int element_index);
node_t *FlipList(node_t *head);
size_t StrLen(const char *str);
int StrCmp(const char *s1,const char *s2);
char *StrCpy(char *dest, const char *src);
char *StrCat(char *dest, const char *src);
char *StrStr(const char *haystack, const char *needle);
void SortStack(stack_t *stack);

#endif /* __RECURSION_H__ */