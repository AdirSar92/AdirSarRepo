#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "bst.h" /* Declerations */

#define SUCCESS (0)
#define FAIL (1)
#define TRUE (1)
#define FALSE (0)

/* =========================== INLINE FUNCTIONS ============================ */

__inline__ static bst_node_t *IterToNode(bst_iter_t iter); /* Make sure everybody do this */
__inline__ static bst_iter_t NodeToIter(bst_node_t *node);/* Make sure everybody do this */
__inline__ static bst_node_t *GetTreeRoot(const bst_t *tree);

/* =========================== STATIC FUNCTIONS ============================ */

static void InitDummyNode(bst_t *tree);
static bst_node_t *CreateNode();
static bst_node_t *GetDirection(const bst_t *tree, bst_node_t *node, const void *data);
static bst_node_t *GoToLeftSubTree(bst_node_t *where_node, bst_node_t *ret);
static bst_node_t *GoToRightSubTree(bst_node_t *where_node, bst_node_t *ret);
static bst_node_t *GoToRootFromRightChild(bst_node_t *where_node, bst_node_t *ret);
static bst_node_t *GoToRootFromLeftChild(bst_node_t *where_node, bst_node_t *ret);

/* ================================= STRUCTS =============================== */
struct bst_node
{
	void *data;
	bst_node_t *left;
	bst_node_t *right;
	bst_node_t *parent;
};

struct bst
{
	bst_node_t dummy;
	cmp_fun_t cmp_fun;
};

/* ========================= FUNCTION DEFINITION =========================== */

bst_t *BSTCreate(cmp_fun_t cmp_fun)
{
	bst_t *tree = NULL;

	assert (NULL != cmp_fun);
	
	tree = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == tree)
	{
		return (NULL);
	}
	
	InitDummyNode(tree);
	
	tree->cmp_fun = cmp_fun;

	return (tree);
}

void BSTDestroy(bst_t *tree)
{
	bst_iter_t to_remove = NULL;
	bst_iter_t next_to_remove = NULL;

	assert(NULL != tree);
	
	to_remove = BSTBegin(tree);
	/* in order */
	while(&tree->dummy != to_remove)
	{
		next_to_remove = BSTIterNext(to_remove);
		BSTRemove(to_remove);
		to_remove = next_to_remove;
	}
	free(tree);

}

int BSTIsEmpty(const bst_t *tree)
{
	assert (NULL != tree);
	return (NULL == GetTreeRoot(tree));	
}

bst_iter_t BSTInsert(bst_t *tree, const void *data)
{
	bst_node_t *new_node = NULL;
	bst_node_t *parent = NULL;
	bst_node_t *runner = NULL;
	
	assert(NULL != tree);

	assert(BSTEnd(tree) == BSTFind(tree, data));

	parent = GetTreeRoot(tree);
	runner = parent;

	new_node = CreateNode(data);
	if (NULL == new_node)
	{
		return (BSTEnd(tree));
	}

	if (NULL == runner) 
	{
		new_node->parent = &tree->dummy;
		tree->dummy.left = new_node;
	}

	else
	{
		while (NULL != runner)
		{
			parent = runner;
			runner = GetDirection(tree, runner, data);
		}

		if (0 > tree->cmp_fun(data, BSTGetData(parent)))
		{
				parent->left = new_node;
		}
		else
		{
			parent->right = new_node;
		}

		new_node->parent = parent;
	}
	
	return (NodeToIter(new_node));

}

void BSTRemove(bst_iter_t where)
{
	bst_node_t *temp = NULL;
	bst_node_t *where_node = NULL;
	assert(NULL != IterToNode(where));

	where_node = IterToNode(where);
	if (NULL != where_node->right && NULL != where_node->left)
	{
		temp = BSTIterNext(where_node);
		where_node->data = BSTGetData(temp);
		where_node = temp;
	}
	temp = ((NULL == where_node->left ) ? where_node->right : where_node->left);
	if (where_node == where_node->parent->left)
	{
		where_node->parent->left = temp;
	}
	else 
	{
		where_node->parent->right = temp;
	}

	if (NULL != temp)
	{
		temp->parent = where_node->parent;
	}

	free(where_node);
	
}


bst_iter_t BSTFind(bst_t *tree, const void *data)
{
	int result = SUCCESS;
	bst_node_t *found = NULL;

	assert(NULL != tree);

	found = GetTreeRoot(tree);

	do
	{
		if (NULL == found)
		{
			return (BSTEnd(tree));
		}
		result = tree->cmp_fun(data, BSTGetData(found));
		found = GetDirection(tree, found, data);
		
	} while (0 != result);

	return (NodeToIter(found));
	
}

size_t BSTSize(const bst_t *tree)
{
	size_t count = 0;
	bst_iter_t runner = NULL;

	assert(NULL != tree);

	runner = BSTBegin(tree);
	while (BSTEnd(tree) != runner)
	{
		++count;
		runner = BSTIterNext(runner);
	}

	return (count);
}

void *BSTGetData(bst_iter_t where)
{
	bst_node_t *where_node = NULL;
	
	assert(NULL != IterToNode(where));

	where_node = IterToNode(where);
	return (where_node->data);
}

int BSTForEach(bst_iter_t from, bst_iter_t to, action_func_t action_func, void *param)
{
	int status = SUCCESS;

	assert(NULL != action_func);
	assert(NULL != param);

	while (!BSTIsEqual(from, to) && SUCCESS == status)
	{
		status = action_func(BSTGetData(from), param);
		from = BSTIterNext(from);
	}

	return (status);

}

bst_iter_t BSTIterNext(bst_iter_t where)
{
	bst_node_t *ret = NULL;
	bst_node_t *where_node = NULL;
	assert(NULL != IterToNode(where));

	where_node = IterToNode(where);

	if (NULL != where_node->right)
	{
		ret = GoToLeftSubTree(where_node, ret);
	}
	else
	{
		ret = GoToRootFromRightChild(where_node, ret);
	}

	return (NodeToIter(ret));
}

bst_iter_t BSTIterPrev(bst_iter_t where)
{
	bst_node_t *ret = NULL;
	bst_node_t *where_node = NULL;

	assert (NULL != where);

	where_node = IterToNode(where);

	if (NULL != where_node->left)
	{
		ret = GoToRightSubTree(where_node, ret);
	}

	else
	{
		ret = GoToRootFromLeftChild(where_node, ret);
	}

	return (NodeToIter(ret));
}

bst_iter_t BSTBegin(const bst_t *tree)
{
	bst_node_t *runner = NULL;

	assert (NULL != tree);
	
	runner = &((bst_t *)tree)->dummy;
	while(NULL != runner->left)
	{
		runner = runner->left;
	}
	
	return (NodeToIter(runner));
}

bst_iter_t BSTEnd(const bst_t *tree)
{
	assert (NULL != tree);
	return ((bst_iter_t)(&(tree->dummy)));
}

int BSTIsEqual(bst_iter_t iter_one, bst_iter_t iter_two)
{
	return (IterToNode(iter_one) == IterToNode(iter_two));
}


static bst_node_t *CreateNode(const void *data)
{
	bst_node_t *new_node = malloc(sizeof(bst_node_t));
	if(NULL != new_node)
	{
		new_node->left = NULL;
		new_node->right = NULL;
		new_node->data = (void *)data;
	}
	return (new_node);
}

static void InitDummyNode(bst_t *tree)
{
	tree->dummy.data = NULL;
	tree->dummy.left = NULL;
	tree->dummy.right = NULL;
	tree->dummy.parent = NULL;
}

static bst_node_t *GetDirection(const bst_t *tree, bst_node_t *node, const void *data)
{
	int result = 0;

	result = tree->cmp_fun(data, BSTGetData(node));
	
	if (0 > result)
	{
		node = node->left;
	}
	if (0 < result)
	{
		node = node->right;
	}	
	return (node);
}

static bst_node_t *GoToLeftSubTree(bst_node_t *where_node, bst_node_t *ret)
{
	ret = where_node->right;

	while (NULL != ret->left)
	{
		ret = ret->left;
	}

	return (ret);
}

static bst_node_t *GoToRootFromRightChild(bst_node_t *where_node, bst_node_t *ret)
{
	ret = where_node;
	while (NULL != ret->parent->parent && ret == ret->parent->right)
	{
		ret = ret->parent;
	}

	ret = ret->parent;
	return (ret);
}

static bst_node_t *GoToRightSubTree(bst_node_t *where_node, bst_node_t *ret)
{
	ret = where_node->left;
	while (NULL != ret->right)
	{
		ret = ret->right;
	}

	return (ret);
}

static bst_node_t *GoToRootFromLeftChild(bst_node_t *where_node, bst_node_t *ret)
{
	ret = where_node;
	while (NULL != ret->parent->parent && ret == ret->parent->left)
	{
		ret = ret->parent;
	}
	ret = ret->parent;
	return (ret);
}

__inline__ static bst_node_t *IterToNode(bst_iter_t iter)
{
	return ((bst_node_t *)iter);
}

__inline__ static bst_iter_t NodeToIter(bst_node_t *node)
{
	return ((bst_iter_t)node);
}

__inline__ static bst_node_t *GetTreeRoot(const bst_t *tree)
{
	return (tree->dummy.left);
}