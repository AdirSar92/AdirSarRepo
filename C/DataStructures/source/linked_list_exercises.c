#include <stdio.h> /* printf */
#include <assert.h> /* assert */

/* Checked By Keren Slotin */
typedef struct node
{
    void *data;
    struct node *next;
} node_t;

typedef node_t *node_ptr_t;

/************************** Flip ***************************/
node_t *Flip(node_t *head);
static void FlipCheck();

/******************** Find Intersection ********************/
node_t *FindIntersection(node_t *head_1, node_t *head_2);
static node_t *FindNode(node_t *big_head, node_t *small_head, size_t node_diff);
static void FindInterCheck();
static size_t CountNumOfNodes(node_t *head);

/************************ Has Loop ************************/
int HasLoop(const node_ptr_t head);
static void HasLoopCheck();

/************************ Test Func ***********************/
static void TestAllFuncs();

/************************ Colours ***********************/
static void Purple();
static void Yellow();

int main(void)
{
    Purple();
    printf ("Test one:\n");
    FlipCheck();
    FindInterCheck();
    HasLoopCheck();
    Yellow();
    printf("\n");
    printf ("Test Two:\n");
    TestAllFuncs();
    return 0;
}

static void TestAllFuncs()
{
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    int num5 = 5;
    int num6 = 6;
    int num7 = 7;
    
    int flag_1 = -1;
    int flag_2 = -1;
    
    node_t node1;
    node_t node2;
    node_t node3;
    node_t node4;
    node_t node5;
    node_t node6;
    node_t node7;
    
    node_ptr_t intersected_1;
    node_ptr_t intersected_2;
    node_ptr_t ptr1 = &node1;
    node_ptr_t ptr5 = &node5;
    
    node1.data = (void*)&num1;
    node2.data = (void*)&num2;
    node3.data = (void*)&num3;
    node4.data = (void*)&num4;
    node5.data = (void*)&num5;
    node6.data = (void*)&num6;
    node7.data = (void*)&num7;
    
    node1.next = &node2;
    node2.next = &node3;
    node3.next = &node4;
    node4.next = NULL;
    node5.next = &node6;
    node6.next = &node7;
    node7.next = NULL;
    
    
    ptr1 = Flip(ptr1);
    
    if((*(int*)ptr1->data == *(int*)node4.data) && 
    (*(int*)ptr1->next->data == *(int*)node3.data))
    {
        printf("Flip Working!                                        V\n");
    }
    else
    {
        printf("Flip NOT Working!                                    X\n");
    }
    
    ptr1 = Flip(ptr1);
    flag_1 = HasLoop(ptr1);
    node4.next = &node2; 
    flag_2 = HasLoop(ptr1);
    
    if((0 == flag_1) && (1 == flag_2))
    {
        printf("HasLoop Working!                                     V\n");
    }
    else
    {
        printf("HasLoop NOT Working!                                  X\n");
    }
    
    node4.next = NULL;
    intersected_1 = FindIntersection(ptr1, ptr5);
    node7.next = &node3;
    intersected_2 = FindIntersection(ptr1, ptr5);
    
    if((NULL == intersected_1) && (intersected_2 == &node3))
    {
        printf("FindIntersection Working!                            V\n");
    }
    else
    {
        printf("FindIntersection NOT Working!                        X\n");
    }
}

void FlipCheck()
{
    size_t i = 0;
    int data_array[5] = {1, 2, 3, 4, 5};
    node_t node_array[5];
    node_t *head = NULL;
    for (i = 0; i < 5; ++i)
    {
        node_array[i].data = &data_array[i];
        node_array[i].next = &node_array[i+1];
    }
    node_array[4].next = NULL;

    head = Flip(node_array);
    if(node_array[4].next == head->next && 
    (node_array[3].next == head->next->next))
    {
        printf("Flip Working!                                        V\n");
    }
    else
    {
        printf("Flip NOT Working!                                    X\n");
    }
}

static void FindInterCheck()
{
    size_t i = 0, j = 0;
    int data_array1[5] = {1, 2, 3, 4, 5};
    int data_array2[10] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int data_array3[5] = {11, 12, 13, 14, 15};
    int data_array4[5] = {20, 21, 22, 23, 24};
    node_t node_array1[5], node_array2[10], node_array3[10], node_array4[5];

    node_t *head1= NULL, *head2 = NULL, *head3 = NULL, *head4 = NULL;
    node_t *mutual_node1 = NULL, *mutual_node2 = NULL, *mutual_node3 = NULL;

    for (i = 0, j = 5; j < 10; ++i, ++j)
    {
        node_array1[i].data = &data_array1[i];
        node_array1[i].next = &node_array1[i+1];
        node_array2[i].data = &data_array2[i];
        node_array2[i].next = &node_array2[i+1];
        node_array2[j].data = &data_array2[j];
        node_array2[j].next = &node_array2[j+1];
        node_array3[i].data = &data_array3[i];
        node_array3[i].next = &node_array3[i+1];
        node_array4[i].data = &data_array4[i];
        node_array4[i].next = &node_array4[i+1];
    }
    
    node_array1[4].next = &node_array2[6];
    node_array2[9].next = NULL;
    node_array3[2].next = &node_array1[2];
    node_array4[4].next = NULL;

    head1 = node_array1;
    head2 = node_array2;
    head3 = node_array3;
    head4 = node_array4;

    mutual_node1 = FindIntersection(head1, head2);
    mutual_node2 = FindIntersection(head1, head3);
    mutual_node3 = FindIntersection(head1, head4);

    if((&node_array2[6] == mutual_node1) &&
        (&node_array1[2] == mutual_node2) &&
         NULL == mutual_node3)
    {
        printf("FindIntersection Working!                            V\n");
    }
    else
    {
        printf("FindIntersection NOT Working!                        X\n");
    }
}

static void HasLoopCheck()
{
    size_t i = 0;
    int data_array1[5] = {1, 2, 3, 4, 5};
    int data_array2[5] = {-3, 5, 12, 4, 2};
    node_t node_array1[5], node_array2[5];
    node_t *head1 = NULL, *head2 = NULL;

    for (i = 0; i < 5; ++i)
    {
        node_array1[i].data = &data_array1[i];
        node_array1[i].next = &node_array1[i+1];
        node_array2[i].data = &data_array2[i];
        node_array2[i].next = &node_array2[i+1];
    }
    node_array1[4].next = &node_array1[2];
    head1 = node_array1;

    node_array2[3].next = &node_array2[1];
    head2 = node_array2;

    if(1 == HasLoop(head1) && 1 == HasLoop(head2))
    {
        printf("HasLoop Working!                                     V\n");
    }
    else
    {
        printf("HasLoop NOT Working!                                 X\n");
    }
}


node_t *FindIntersection(node_t *head_1, node_t *head_2)
{
    node_t *node_ptr1 = head_1;
    node_t *node_ptr2 = head_2;
    node_t *mutual_node = NULL;

    size_t node_count1 = 0;
    size_t node_count2 = 0;
    size_t node_diff = 0;

    assert (NULL != head_1);
    assert (NULL != head_2);

    node_count1 = CountNumOfNodes(node_ptr1);
    node_count2 = CountNumOfNodes(node_ptr2);

    if (node_count1 > node_count2)
    {
        node_diff = node_count1 - node_count2;
        mutual_node = FindNode(head_1, head_2, node_diff);
    }
    else
    {
        node_diff = node_count2 - node_count1;
        mutual_node = FindNode(head_2, head_1, node_diff);
    }

    return (mutual_node);
}


static size_t CountNumOfNodes(node_t *head)
{
    size_t node_count = 0;
    
    assert (NULL != head);
    while (NULL != head)
    {
        ++node_count;
        head = head->next;
    }

    return (node_count);
}


static node_t *FindNode(node_t *big_head, node_t *small_head, size_t node_diff)
{
    assert (NULL != big_head);
    assert (NULL != small_head);

    while (0 < node_diff)
    {
        --node_diff;
        big_head = big_head->next;
    }

    while (NULL != big_head && NULL != small_head)
    {
        if (big_head == small_head)
        {
            break;
        }
        big_head = big_head -> next;
        small_head = small_head -> next;
    }

    return (big_head);

}


node_t *Flip(node_t *head)
{
    node_t *previous = NULL, *next = NULL, *current = head;

    assert (NULL != head);

    while (NULL != current)
    {
        next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    head = previous;

    return (head);
}


int HasLoop(const node_ptr_t head)
{
    int has_loop = 0;
    node_ptr_t slow_runner = head;
    node_ptr_t fast_runner = head;

    assert (NULL != head);
    
    while (NULL != slow_runner && fast_runner != NULL)
    {
        slow_runner = slow_runner -> next;
        fast_runner = fast_runner -> next -> next;

        if (slow_runner == fast_runner)
        {
            has_loop = 1;
            break;
        }
    }

    return (has_loop);
}

static void Purple()
{
    printf("\033[1;35m");
}

static void Yellow()
{
    printf("\033[1;33m");
}