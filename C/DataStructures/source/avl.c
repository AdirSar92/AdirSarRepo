
/*========================== LIBRARIES && MACROS =============================*/

#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "avl.h" /* AVL */

#define SUCCESS (0)
#define FAIL (1)
#define MAX(a, b) ((a > b) ? a : b)
#define FLIP(x) (LEFT == x ? RIGHT : LEFT)

/*============================== ENUMS & STRUCTS =============================*/

typedef struct avl_node avl_node_t;

typedef enum children
{
	LEFT = 0,
	RIGHT,
	NUM_CHILDREN
}children_t;


struct avl_node
{
    void *data;
    avl_node_t *child[NUM_CHILDREN];
    size_t height;
};

struct avl
{
    avl_node_t *root;
    cmp_func_t cmp_func;
};

/*============================ STATIC DECLERATIONS ===========================*/

static avl_node_t *CreateNode(void *data);
static avl_node_t *AVLInsertRec(cmp_func_t cmp_func, avl_node_t *next_node, avl_node_t *new_node);
static avl_node_t *FindNextSuccessor(avl_node_t *curr_node);
static avl_node_t *AVLRemoveRec(cmp_func_t cmp_func, avl_node_t *curr_node, const void *key);
static avl_node_t *BalanceNode(avl_node_t *node);
static avl_node_t *TreeRotate(avl_node_t *parent, children_t direction);

static int ForEachInOrder(avl_node_t *node,action_func_t action,void *param);
static int ForEachPostOrder(avl_node_t *node,action_func_t action,void *param);
static int ForEachPreOrder(avl_node_t *node,action_func_t action,void *param);

static void *AVLGetData(avl_node_t *node);
static void *AVLFindRec(cmp_func_t cmp_func, avl_node_t *curr_node, const void *key);
static void AVLRemoveNode(avl_node_t *curr_node);
static size_t AVLSizeRec(const avl_node_t *node);
static size_t UpdateHeight(avl_node_t *node);
static long GetChildHeight(avl_node_t *node, children_t direction);
static long GetHeightDiff(avl_node_t *node);

/*============================ FUNCTION DEFINITIONS ==========================*/

avl_t *AVLCreate(cmp_func_t cmp_func)
{
	avl_t *tree = NULL;
	assert (NULL != cmp_func);

	tree = (avl_t *)malloc(sizeof(avl_t));
	if (NULL != tree)
	{
		tree->cmp_func = cmp_func;
		tree->root = NULL;
	}

	return (tree);

}

void AVLDestroy(avl_t *tree)
{
	assert (NULL != tree);
	AVLRemoveNode(tree->root);
	free(tree);
}

static void AVLRemoveNode(avl_node_t *curr_node)
{
	if (NULL == curr_node)
	{
		return;
	}

	AVLRemoveNode(curr_node->child[LEFT]);
	curr_node->child[LEFT] = NULL;
	AVLRemoveNode(curr_node->child[RIGHT]);
	curr_node->child[RIGHT] = NULL;
	free (curr_node);
}

int AVLIsEmpty(const avl_t *tree)
{
	assert (NULL != tree);

	return (NULL == tree->root);
}


int AVLInsert(avl_t *tree, void *data)
{
	avl_node_t *new_node = NULL;
	assert (NULL != tree);
	assert (NULL != tree->cmp_func);

	new_node = CreateNode(data);
	if(NULL == new_node)
	{
		return (FAIL);
	}
	tree->root = AVLInsertRec(tree->cmp_func, tree->root, new_node);
	return (SUCCESS);
}

static avl_node_t *AVLInsertRec(cmp_func_t cmp_func, avl_node_t *curr_node, avl_node_t *new_node)
{
	if (NULL == curr_node)
	{
		return (new_node);
	}
	if (0 > cmp_func(AVLGetData(new_node), AVLGetData(curr_node)))
	{
		curr_node->child[LEFT] = AVLInsertRec(cmp_func, curr_node->child[LEFT], new_node);
	}
	else
	{
		curr_node->child[RIGHT] = AVLInsertRec(cmp_func, curr_node->child[RIGHT], new_node);
	}
	curr_node->height = UpdateHeight(curr_node);
	return (BalanceNode(curr_node));
}

void AVLRemove(avl_t *tree, const void *key)
{	
	assert (NULL != tree);
	assert (NULL != tree->root);

	tree->root = AVLRemoveRec(tree->cmp_func, tree->root, key);
}


static avl_node_t *AVLRemoveRec(cmp_func_t cmp_func, avl_node_t *curr_node, const void *key)
{
	int compare = 0;
	if (NULL == curr_node)
	{
		return (curr_node);
	}

	compare = cmp_func(key, AVLGetData(curr_node));
	if (0 > compare)
	{
		curr_node->child[LEFT] = AVLRemoveRec(cmp_func, curr_node->child[LEFT], key);
	}
	else if (0 < compare)
	{
		curr_node->child[RIGHT] = AVLRemoveRec(cmp_func, curr_node->child[RIGHT], key);
	}
	else 
	{	
		avl_node_t *temp = NULL;
		if (NULL == curr_node->child[RIGHT] || NULL == curr_node->child[LEFT])
		{
			temp = ((NULL == curr_node->child[LEFT]) ? curr_node->child[RIGHT] : curr_node->child[LEFT]);
			free(curr_node);
			return (temp);
		} 
		temp = FindNextSuccessor(curr_node->child[RIGHT]);
		curr_node->data = AVLGetData(temp);
		curr_node->child[RIGHT] = AVLRemoveRec(cmp_func, curr_node->child[RIGHT], AVLGetData(temp));
	}
	curr_node->height = UpdateHeight(curr_node);

	return (BalanceNode(curr_node));
}


static avl_node_t *FindNextSuccessor(avl_node_t *curr_node)
{
	return (NULL == curr_node->child[LEFT] ? curr_node : FindNextSuccessor(curr_node->child[LEFT]));
}

void *AVLFind(const avl_t *tree, const void *key)
{
	assert (NULL != tree);
	assert (NULL != tree->cmp_func);
	assert (NULL != tree->root);

	return (AVLFindRec(tree->cmp_func, tree->root, key));

}

static void *AVLFindRec(cmp_func_t cmp_func, avl_node_t *curr_node, const void *key)
{
	int compare = 0;
	if (NULL == curr_node)
	{
		return NULL;
	}
	compare = cmp_func(key, AVLGetData(curr_node));
	if (0 == compare)
	{
		return (AVLGetData (curr_node));
	}
	if (0 > compare)
	{
		return (AVLFindRec(cmp_func, curr_node->child[LEFT], key));
	}
	else
	{
		return (AVLFindRec(cmp_func, curr_node->child[RIGHT], key));
	}

}

size_t AVLSize(const avl_t *tree)
{
	assert (NULL != tree);
	return (AVLSizeRec(tree->root));
}

static size_t AVLSizeRec(const avl_node_t *node)
{
	return (NULL == node ? 0 : (1 + AVLSizeRec(node->child[LEFT]) + AVLSizeRec(node->child[RIGHT])));
}

size_t AVLHeight(const avl_t *tree)
{
	assert (NULL != tree);

	return ((NULL == tree->root) ? 0 : tree->root->height);
}

int AVLForEach(avl_t *tree, action_func_t action_func, void *param, traverse_t type)
{
	switch (type)
	{
		case PRE_ORDER:
			return (ForEachPreOrder(tree->root, action_func, param));

		case POST_ORDER:
			return (ForEachPostOrder(tree->root, action_func, param));
		
		case IN_ORDER:
			return (ForEachInOrder(tree->root, action_func, param));
		
		default:
			return (FAIL);
	}
}

static int ForEachInOrder(avl_node_t *node, action_func_t action, void *param)
{
	if (NULL == node)
	{
		return (SUCCESS);
	}
	if (SUCCESS != ForEachInOrder(node->child[LEFT], action, param))
	{
		return (FAIL);
	}
	if (SUCCESS != action(AVLGetData(node), param))
	{
		return (FAIL);
	}
	if (SUCCESS != ForEachInOrder(node->child[RIGHT], action, param))
	{
		return (FAIL);
	}
	return (SUCCESS);
}

static int ForEachPostOrder(avl_node_t *node, action_func_t action, void *param)
{
	if (NULL == node)
	{
		return (SUCCESS);
	}
	if (SUCCESS != ForEachPostOrder(node->child[LEFT], action, param))
	{
		return (FAIL);
	}
	if (SUCCESS != ForEachPostOrder(node->child[RIGHT], action, param))
	{
		return (FAIL);
	}
	if (SUCCESS != action(AVLGetData(node), param))
	{
		return (FAIL);
	}
	return (SUCCESS);
}


static int ForEachPreOrder(avl_node_t *node, action_func_t action, void *param)
{                                       
	if (NULL == node)
	{
		return (SUCCESS);
	}
	if (SUCCESS != action(AVLGetData(node), param))
	{
		return (FAIL);
	}
	if (SUCCESS != ForEachPreOrder(node->child[LEFT], action, param))
	{
		return (FAIL);
	}
	if (SUCCESS != ForEachPreOrder(node->child[RIGHT], action, param))
	{
		return (FAIL);
	}
	return (SUCCESS);
}


static avl_node_t *CreateNode(void *data)
{
	avl_node_t *new_node = (avl_node_t *)malloc(sizeof(avl_node_t));
	if (NULL == new_node)
	{
		return (NULL);
	}

	new_node->data = data;
	new_node->child[LEFT] = NULL;
	new_node->child[RIGHT] = NULL;
	new_node->height = 1;

	return (new_node);
}

static void *AVLGetData(avl_node_t *node)
{
	return (node->data);
}


static size_t UpdateHeight(avl_node_t *node)
{
	size_t left_height = 0, right_height = 0;
	left_height = GetChildHeight(node, LEFT);  
	right_height = GetChildHeight(node, RIGHT); 
	return (1 + MAX(left_height, right_height));
}

static long GetChildHeight(avl_node_t *node, children_t direction)
{
	return (NULL == node->child[direction] ? 0 : node->child[direction]->height);
}

static long GetHeightDiff(avl_node_t *node)
{
	return (GetChildHeight(node, LEFT) - GetChildHeight(node, RIGHT));
}

static avl_node_t *BalanceNode(avl_node_t *node)
{
	long height_diff = GetHeightDiff(node);
	if (1 < height_diff)
	{
		if(GetHeightDiff(node->child[LEFT]) < 0)
        {
            node->child[LEFT] = TreeRotate(node->child[LEFT], LEFT);
        }
		return(TreeRotate(node, RIGHT));
	}
	if (-1 > height_diff)
	{
		if(GetHeightDiff(node->child[RIGHT]) > 0)
        {
            node->child[RIGHT] = TreeRotate(node->child[RIGHT], RIGHT);
        }
		return(TreeRotate(node, LEFT));
	}
	return (node);
}

static avl_node_t *TreeRotate(avl_node_t *parent, children_t direction)
{
	avl_node_t *pivot = parent->child[FLIP(direction)];
	parent->child[FLIP(direction)] = pivot->child[direction];
	pivot->child[direction] = parent;

	parent->height = UpdateHeight(parent);
	pivot->height = UpdateHeight(pivot);

	return (pivot);
}

