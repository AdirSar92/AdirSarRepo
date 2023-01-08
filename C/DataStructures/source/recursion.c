#include <assert.h> /* assert */
#include <string.h> /* strncmp */

#include "recursion.h" /* declerations */

static node_t *FlipListRec(node_t *curr, node_t *next, node_t *prev);
static char *StrCpyIdx(char *dest, const char *src, size_t idx);
static void PushSorted(stack_t *stack, int num);

int Fibonacci(int element_index)
{
	if (1 >= element_index)
	{
		return (element_index);
	}
	return (Fibonacci(element_index - 1) + Fibonacci(element_index - 2));
}

int FibonacciVer2(int element_index)
{
	int first = 0, next = 1, c = first + next, i = 2;

	if (0 == element_index)
	{
		return (first);
	}

	for (i = 2; i <= element_index; i++)
	{
		c = first + next;
		first = next;
		next = c;
	}

	return (next);
}

node_t *FlipList(node_t *head)
{
	return FlipListRec(head, head->next, NULL);
}

static node_t *FlipListRec(node_t *curr, node_t *next, node_t *prev)
{

	curr->next = prev;
	if (NULL == next)
	{
		return (curr);
	}
	return FlipListRec(next, next->next, curr);
}

size_t StrLen(const char *str)
{
	if (*str == '\0')
	{
		return 0;
	}
	return (1 + StrLen(++str));
}

int StrCmp(const char *s1, const char *s2)
{
	assert(NULL != s1);
	assert(NULL != s2);

	if (*s1 != *s2 || '\0' == *s1)
	{
		return (*s1 - *s2);
	}
	return (StrCmp(++s1, ++s2));
}

char *StrCpy(char *dest, const char *src)
{
	return (StrCpyIdx(dest, src, 0));
}

static char *StrCpyIdx(char *dest, const char *src, size_t idx)
{
	dest[idx] = src[idx];
	return (('\0' == src[idx]) ? dest : StrCpyIdx(dest, src, idx + 1));
}

char *StrCat(char *dest, const char *src)
{
	return (StrCpy(dest + StrLen(dest), src));
}

char *StrStr(const char *haystack, const char *needle)
{
	assert(NULL != haystack);
	assert(NULL != needle);

	if (0 == strncmp(haystack, needle, StrLen(needle)))
	{
		return ((char *)haystack);
	}
	else
	{
		if (*haystack == '\0')
		{
			return (NULL);
		}
		return (StrStr(++haystack, needle));
	}
}

void SortStack(stack_t *stack)
{
	int num;
	if (!StackIsEmpty(stack))
	{
		num = *(int *)StackPop(stack);
		SortStack(stack);
		PushSorted(stack, num);
	}
	(void)num;
}

static void PushSorted(stack_t *stack, int num)
{
	int temp;
	if (StackIsEmpty(stack) || num > *(int *)StackPeek(stack))
	{
		StackPush(stack, &num);
	}
	else
	{
		temp = *(int *)StackPop(stack);
		PushSorted(stack, num);
		StackPush(stack, &temp);
		(void)temp;
	}
}
