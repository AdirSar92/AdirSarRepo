/* what is heap? best for getting max/min o(1)
    can use it in BST too, problem in vector is space complexity. tradeoff
    locality of reference - pro, not con.
    complexities of push o(logn), pop o(logn), remove o(n)
*/

/*========================== LIBRARIES && MACROS =============================*/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "vector.h" /* vector */
#include "heap.h"   /* Declerations */

#define ELEMENT_SIZE (sizeof(void *))
#define INITIAL_CAPACITY (10)
#define SUCCESS (0)
#define FAIL (1)
#define TRUE (1)

/*============================== ENUMS & STRUCTS =============================*/
struct binary_heap
{
    vector_t *vector;
    cmp_func_t cmp_fun;
};

typedef enum children
{
    LEFT,
    RIGHT
} children_t;

/*============================== STATIC FUNCS ================================*/
static size_t GetParentIndex(size_t index);
static size_t GetChildIndex(size_t index, children_t side);
static size_t GetLastIndex(vector_t *vector);
static void SwapPointers(void **ptr1, void **ptr2);
static void SwapElements(bin_heap_t *bin_heap, size_t index1, size_t index2);
static void HeapifyUp(bin_heap_t *bin_heap, size_t index);
static void HeapifyDown(bin_heap_t *bin_heap, size_t parent);
static void *GetElement(bin_heap_t *bin_heap, size_t index);
static int CompareData(bin_heap_t *bin_heap, size_t index1, size_t index2);
static size_t FindElement(bin_heap_t *bin_heap, heap_is_match_t is_match, void *param);

static size_t GetBiggerChild(bin_heap_t *bin_heap, size_t parent);
static int True(const void *data1, const void *data2);
/*================================ HEAP FUNCS ===============================*/
bin_heap_t *BinHeapCreate(cmp_func_t cmp_fun)
{
    bin_heap_t *heap = NULL;

    assert(NULL != cmp_fun);

    heap = (bin_heap_t *)malloc(sizeof(bin_heap_t));
    if (NULL != heap)
    {
        heap->vector = VectorCreate(INITIAL_CAPACITY, ELEMENT_SIZE);
        if (NULL == heap->vector)
        {
            free(heap);
            return (NULL);
        }
        heap->cmp_fun = cmp_fun;
    }
    return (heap);
}

void BinHeapDestroy(bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);
    VectorDestroy(bin_heap->vector);
    free(bin_heap);
}

int BinHeapIsEmpty(const bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);
    return (VectorIsEmpty(bin_heap->vector));
}

int BinHeapPush(bin_heap_t *bin_heap, void *data)
{
    assert(NULL != bin_heap);
    if (SUCCESS != VectorPushBack(bin_heap->vector, &data))
    {
        return (FAIL);
    }
    HeapifyUp(bin_heap, GetLastIndex(bin_heap->vector));

    return (SUCCESS);
}

void BinHeapPop(bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);
    BinHeapRemove(bin_heap, True, 0);
}

static int True(const void *data1, const void *data2)
{
    (void)data1;
    (void)data2;
    return (TRUE);
}

void *BinHeapPeek(const bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);
    assert(0 < BinHeapSize(bin_heap));
    return (GetElement((bin_heap_t *)bin_heap, 0));
}
/* remove function has to decide which heapify to do (depending on the data */
void *BinHeapRemove(bin_heap_t *bin_heap, heap_is_match_t is_match, void *param)
{
    size_t remove_index = 0;
    size_t last_index = GetLastIndex(bin_heap->vector);
    void *remove_data = NULL;

    assert(NULL != bin_heap);
    assert(NULL != is_match);

    remove_index = FindElement(bin_heap, is_match, param);

    if (remove_index <= last_index)
    {
        remove_data = GetElement(bin_heap, remove_index);
        SwapElements(bin_heap, remove_index, GetLastIndex(bin_heap->vector));
        VectorPopBack(bin_heap->vector);

        if (remove_index != last_index)
        {
            HeapifyDown(bin_heap, remove_index);
            HeapifyUp(bin_heap, remove_index);
        }
    }
    return (remove_data);
}

size_t BinHeapSize(const bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);
    return (VectorGetSize(bin_heap->vector));
}

static size_t GetParentIndex(size_t index)
{
    return ((0 == index) ? 0 : (index - 1) / 2);
}

static size_t GetChildIndex(size_t index, children_t side)
{
    return ((side == LEFT) ? (2 * index + 1) : (2 * index + 2));
}

static void SwapPointers(void **ptr1, void **ptr2)
{
    void *temp = NULL;
    assert(NULL != ptr1);
    assert(NULL != ptr2);

    temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

static void SwapElements(bin_heap_t *bin_heap, size_t index1, size_t index2)
{
    SwapPointers((void **)VectorAccessAt(bin_heap->vector, index1),
                 (void **)VectorAccessAt(bin_heap->vector, index2));
}

static size_t GetLastIndex(vector_t *vector)
{
    assert(NULL != vector);
    return (0 == VectorGetSize(vector) ? 0 : VectorGetSize(vector) - 1);
}

static void HeapifyUp(bin_heap_t *bin_heap, size_t index)
{
    size_t parent = GetParentIndex(index);

    if (0 < CompareData(bin_heap, index, parent))
    {
        SwapElements(bin_heap, index, parent);
        HeapifyUp(bin_heap, parent);
    }
}

static void *GetElement(bin_heap_t *bin_heap, size_t index)
{
    return (*(void **)VectorAccessAt(bin_heap->vector, index));
}

static void HeapifyDown(bin_heap_t *bin_heap, size_t parent)
{
    size_t larger = GetBiggerChild(bin_heap, parent);

    if (larger != parent)
    {
        SwapElements(bin_heap, parent, larger);
        HeapifyDown(bin_heap, larger);
    }
}

static size_t GetBiggerChild(bin_heap_t *bin_heap, size_t parent)
{
    size_t left_child = 0, right_child = 0, larger = parent;

    assert(NULL != bin_heap);

    left_child = GetChildIndex(parent, LEFT);
    right_child = GetChildIndex(parent, RIGHT);

    if (left_child <= GetLastIndex(bin_heap->vector) &&
        0 > CompareData(bin_heap, larger, left_child))
    {
        larger = left_child;
    }
    if (right_child <= GetLastIndex(bin_heap->vector) &&
        0 > CompareData(bin_heap, larger, right_child))
    {
        larger = right_child;
    }

    return (larger);
}
static int CompareData(bin_heap_t *bin_heap, size_t index1, size_t index2)
{
    return (bin_heap->cmp_fun(GetElement(bin_heap, index1), GetElement(bin_heap, index2)));
}

static size_t FindElement(bin_heap_t *bin_heap, heap_is_match_t is_match, void *param)
{
    size_t found = 0;
    int is_found = 0;
    for (; found < BinHeapSize(bin_heap); ++found)
    {
        if (is_match(GetElement(bin_heap, found), param))
        {
            is_found = 1;
            break;
        }
    }
    return ((is_found) ? found : BinHeapSize(bin_heap));
}
