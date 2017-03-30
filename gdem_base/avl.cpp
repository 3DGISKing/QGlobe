#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define __INLINE__	inline

/* AVL (Adelson-Velskii & Landis) node */
enum avl_Branch
{
	AVL_BOUGH_LEFT = 0,
	AVL_BOUGH_RIGHT,
	AVL_BOUGH_CNT
};

struct avl_Node
{
	struct avl_Node *	parent;		/* Parent node. */
	struct avl_Node *	left;		/* Left node. */
	struct avl_Node *	right;		/* Right node. */

	signed long		balance:4;	/* Balance factor. */
	unsigned long	rank:28;	/* Relative position of node in own subtree
								* i.e. number of nodes in left subtree + 1. */
	void *		data;		/* User Data. */
};

#define ALG_NAME_LEN	sizeof(unsigned long)

struct avl_Tree
{
	char		name[ALG_NAME_LEN];	/* AVL name, allways "AVL". */
	struct avl_Node *	root;			/* Root. */
	unsigned int	count;			/* Number of entries in tree. */
	unsigned int	size;	/* Size of internal data; 0, if external. */
	int (*compare)(void *, void *); /* Compare function. */
};

#define  __TREE_ALG_IMPLEMENTATION__
#include "avl.h"

/* Single Negative (LL - left) Rotation.
* We assumed that A node has a left-son named B and a right-son; and
* the B has a left-son and a right-son named C, which described as following:
*         A                     B
*        / \                   / \
*       B   Ar      ==>       Bl  A
*      / \                       / \
*     Bl  C                     C   Ar
*/

static __INLINE__ struct avl_Node* _avl_SingleNegative(struct avl_Tree * tree, struct avl_Node * A, struct avl_Node * B)
{
	struct avl_Node * C = B->right;
	struct avl_Node * P = A->parent;

	if (NULL == P)
		tree->root = B;
	else if (A == P->left)
		P->left = B; else P->right = B;
	B->parent = P;

	B->right = A; A->parent = B;
	A->left =  C; if (NULL != C) C->parent = A;

	B->balance ++;
	A->balance = (-B->balance);

	A->rank -= B->rank;

	return B;
}

/* Single Positive (RR - right) Rotation.
* We assume that the tree is looked like as the following:
*         A                     B
*        / \                   / \
*       Al  B       ==>       A   Br
*          / \               / \
*         C   Br            Al  C
*/
static __INLINE__ struct avl_Node*
_avl_SinglePositive(struct avl_Tree * tree, struct avl_Node * A, struct avl_Node * B)
{
	struct avl_Node * C = B->left;
	struct avl_Node * P = A->parent;

	if (NULL == P)
		tree->root = B;
	else if (A == P->left)
		P->left = B; else P->right = B;
	B->parent = P;

	B->left =  A; A->parent = B;
	A->right = C; if (NULL != C) C->parent = A;

	B->balance --;
	A->balance = (-B->balance);

	B->rank += A->rank;

	return B;
}

/* Double Negative (LR - left) Rotation.
* We assumed A node has a left-son named B and a right-son; and
* the B has a left-son and a right-son named C, also,
* C has a left-son named D and right-son named E.
*         A                     C
*        / \                  /   \
*       B   Ar      ==>      B     A
*      / \                  / \   / \
*     Bl  C                Bl  D E   Ar
*        / \
*       D   E
*/
static __INLINE__ struct avl_Node *
_avl_DoubleNegative(struct avl_Tree * tree, struct avl_Node * A, struct avl_Node * B)
{
	struct avl_Node * C = B->right;
	struct avl_Node * D = C->left, * E = C->right;
	struct avl_Node * P = A->parent;

	if (NULL == P)
		tree->root = C;
	else if (A == P->left)
		P->left = C; else P->right = C;
	C->parent = P;

	C->left =  B; B->parent = C;
	C->right = A; A->parent = C;

	B->right = D; if (NULL != D) D->parent = B;
	A->left =  E; if (NULL != E) E->parent = A;

	A->balance = (0 <= C->balance ? 0 : +1);
	B->balance = (0 >= C->balance ? 0 : -1);
	C->balance = 0;

	C->rank += B->rank;
	A->rank -= C->rank;

	return C;
}

/* Double Positive (RL - right) Rotation.
* We assume that the tree is looked like as the following:
*         A                     C
*        / \                  /   \
*       Al  B       ==>      A     B
*          / \              / \   / \
*         C   Br           Al  D E   Br
*        / \
*       D   E
*/
static __INLINE__ struct avl_Node *
_avl_DoublePositive(struct avl_Tree * tree, struct avl_Node * A, struct avl_Node * B)
{
	struct avl_Node * C = B->left;
	struct avl_Node * D = C->left, * E = C->right;
	struct avl_Node * P = A->parent;

	if (NULL == P)
		tree->root = C;
	else if (A == P->left)
		P->left = C; else P->right = C;
	C->parent = P;

	C->left =  A; A->parent = C;
	C->right = B; B->parent = C;

	A->right = D; if (NULL != D) D->parent = A;
	B->left =  E; if (NULL != E) E->parent = B;

	A->balance = (0 >= C->balance ? 0 : -1);
	B->balance = (0 <= C->balance ? 0 : +1);
	C->balance = 0;

	B->rank -= C->rank;
	C->rank += A->rank;

	return C;
}

static struct avl_Node *
_avl_NegativeRotate(struct avl_Tree * tree, struct avl_Node * node)
{
	struct avl_Node * child = node->left;

	/* (0 > node->balance) : Negative (left) Rotate */
	return (0 < child->balance
		? _avl_DoubleNegative(tree, node, child)
		: _avl_SingleNegative(tree, node, child));
}

static struct avl_Node *
_avl_PositiveRotate(struct avl_Tree * tree, struct avl_Node * node)
{
	struct avl_Node * child = node->right;

	/* (0 < node->balance) : Positive (right) Rotate */
	return (0 > child->balance
		? _avl_DoublePositive(tree, node, child)
		: _avl_SinglePositive(tree, node, child));
}

static struct avl_Node *
_avl_PrevDownNode(struct avl_Node * node)
{
	if (NULL != (node = node->left))
	{
		struct avl_Node * child;

		while (NULL != (child = node->right))
			node = child;
	}

	return node;
}

static struct avl_Node *
_avl_PrevUpNode(struct avl_Node * node)
{
	struct avl_Node * child = node;

	while (NULL != (node = child->parent))
	{
		if (child == node->right)
			break;

		child = node;
	}

	return node;
}

static struct avl_Node *
_avl_NextDownNode(struct avl_Node * node)
{
	if (NULL != (node = node->right))
	{
		struct avl_Node * child;

		while (NULL != (child = node->left))
			node = child;
	}

	return node;
}

static struct avl_Node *
_avl_NextUpNode(struct avl_Node * node)
{
	struct avl_Node * child = node;

	while (NULL != (node = child->parent))
	{
		if (child == node->left)
			break;

		child = node;
	}

	return node;
}

static struct avl_Node *
_avl_NextDepthNode(struct avl_Node * node, struct avl_Node * child, unsigned depth)
{
	while (0 < depth && NULL != node)
	{ /* At all times, client isn't NULL */
		if (child == node && NULL != node->left)
		{ /* Down-Left */
			node = node->left;
			child = node;
			depth --;
		}
		else if ((child == node || child == node->left) && NULL != node->right)
		{ /* Down-Right or Peer-Right */
			node = node->right;
			child = node;
			depth --;
		}
		else /* ((NULL == node->left && NULL == node->right) || child == node->right) */
		{ /* Down-Up(Parent) or Up-Up(Parent) */
			child = node;
			node = child->parent;
			depth ++;
		}
	}

	return node;
}

static struct avl_Node *
_avl_FirstNode(struct avl_Node * node)
{
	struct avl_Node * child = node->left;

	while (NULL != child)
	{
		node = child;
		child = node->left;
	}

	return node;
}

static __INLINE__ void
_avl_DecreaseRank(struct avl_Node * node)
{
	struct avl_Node * child = node;

	node->rank --;
	while (NULL != (node = child->parent))
	{
		if (child == node->left)
			node->rank --;

		child = node;
	}
}

static __INLINE__ void
_avl_IncreaseRank(struct avl_Node * node)
{
	struct avl_Node * child = node;

	node->rank ++;
	while (NULL != (node = child->parent))
	{
		if (child == node->left)
			node->rank ++;

		child = node;
	}
}

static __INLINE__ int
_avl_WeightBalance(struct avl_Tree * tree, struct avl_Node * node)
{
	unsigned rcount = node->rank;
	struct avl_Node * parent, * child = node;

	while (NULL != (parent = child->parent))
	{
		if (child == parent->left)
			break;

		rcount += parent->rank;
		child = parent;
	}

	rcount = (NULL == parent ? tree->count : (parent->rank - 1)) - rcount;

	return (int) (rcount - (node->rank - 1));
}

static __INLINE__ void
_avl_GrowTree(struct avl_Tree * tree, struct avl_Node * node, struct avl_Node * child)
{
	while (NULL != node)
	{
		if (child == node->left)
		{ /* the height of node's left subtree increased. */
			if (0 > node->balance)
			{
				_avl_NegativeRotate(tree, node);
				break;
			}
			else if (0 == node->balance)
			{
				node->balance = -1;
			}
			else /* (0 < node->balance) */
			{
				node->balance = 0;
				break;
			}
		}
		else /* (child == node->right) */
		{ /* the height of node's right subtree increased. */
			if (0 > node->balance)
			{
				node->balance = 0;
				break;
			}
			else if (0 == node->balance)
			{
				node->balance = +1;
			}
			else /* (0 < node->balance) */
			{
				_avl_PositiveRotate(tree, node);
				break;
			}
		}

		child = node;
		node = child->parent;
	}
}

static __INLINE__ void
_avl_ShrinkTree(struct avl_Tree * tree, struct avl_Node * node, struct avl_Node ** shrink)
{
	enum avl_Branch bough;

	if (NULL == node)
		return;

	bough = (&(node->left) == shrink) ? AVL_BOUGH_LEFT : AVL_BOUGH_RIGHT;
	for (;;)
	{
		struct avl_Node * child;

		if (AVL_BOUGH_LEFT == bough)
		{ /* the height of node's left subtree is decreased */
			if (0 > node->balance)
			{
				node->balance = 0;
			}
			else if (0 == node->balance)
			{
				node->balance = +1;
				break;
			}
			else /* (0 < node->balance) */
			{
				node = _avl_PositiveRotate(tree, node);
				if (0 > node->balance)
					break;
			}
		}
		else /* (AVL_BOUGH_RIGHT == bough) */
		{ /* the height of node's right subtree is decreased */
			if (0 > node->balance)
			{
				node = _avl_NegativeRotate(tree, node);
				if (0 < node->balance)
					break;
			}
			else if (0 == node->balance)
			{
				node->balance = -1;
				break;
			}
			else /* (0 < node->balance) */
			{
				node->balance = 0;
			}
		}

		child = node;
		if (NULL == (node = child->parent))
			break;

		bough = (child == node->left) ? AVL_BOUGH_LEFT : AVL_BOUGH_RIGHT;
	}
}

static __INLINE__ struct avl_Node **
_avl_SwapNode(struct avl_Tree * tree, struct avl_Node * node, struct avl_Node ** branch)
{
	int balance, rank;
	struct avl_Node * child, * parent, * NL, * NR, * CL, * CR;

	child = (0 > _avl_WeightBalance(tree, node) /* (0 > node->balance) */
		? _avl_PrevDownNode(node) : _avl_NextDownNode(node));
	if (NULL == child)
		return branch;

	parent = child->parent;
	child->parent = node->parent;
	*branch = child;

	NL = node->left, NR = node->right;
	CL = child->left, CR = child->right;

	if (child == node->left)
	{
		child->left = node;  node->parent = child;
		child->right = NR; if (NULL != NR) NR->parent = child;
		branch = &(child->left);
	}
	else if (child == node->right)
	{
		child->left = NL; if (NULL != NL) NL->parent = child;
		child->right = node; node->parent = child;
		branch = &(child->right);
	}
	else /* descendants */
	{
		child->left = NL;  if (NULL != NL) NL->parent = child;
		child->right = NR; if (NULL != NR) NR->parent = child;
		branch = (child == parent->left ? &(parent->left) : &(parent->right));
		node->parent = parent; *branch = node;
	}

	node->left = CL;  if (NULL != CL) CL->parent = node;
	node->right = CR; if (NULL != CR) CR->parent = node;

	balance = node->balance; rank = node->rank;
	node->balance = child->balance; node->rank = child->rank;
	child->balance = balance; child->rank = rank;

	return branch;
}

static struct avl_Node **
_avl_LookupNode(struct avl_Tree * tree, void * key,
struct avl_Node ** ancestor, int * index)
{
	unsigned base = 0;
	struct avl_Node * node, ** branch = &(tree->root);

	*ancestor = NULL;
	while (NULL != (node = *branch))
	{
		int rc = (tree->compare)(key, node->data);

		if (0 == rc)
		{
			break;
		}
		else if (0 > rc)
		{
			branch = &(node->left);
		}
		else /* (0 < rc) */
		{
			base += node->rank;
			branch = &(node->right);
		}

		*ancestor = node;
	}

	if (NULL != index)
		*index = (NULL == node ? -EPERM : base + node->rank);

	return branch;
}

static __INLINE__ struct avl_Node *
_avl_NewNode(struct avl_Tree * tree, void * data)
{
	struct avl_Node * node;
	unsigned size = sizeof(struct avl_Node) + tree->size;

	if (NULL == (node = (struct avl_Node *) malloc(size)))
		return NULL;

	memset(node, 0, size);

	if (0 < tree->size)
	{
		node->data = (void *) ((char *) node + sizeof(struct avl_Node));
		memcpy(node->data, data, tree->size);
	}
	else
	{
		node->data = data;
	}

	return node;
}

/************************************************************************
*			Global Functions (APIs)				*
************************************************************************/

void *
avl_Find(struct avl_Tree * tree, void * key, int condition, struct alg_Node * alg)
{
	struct avl_Node * node, * parent, ** branch;

	if (NULL == tree || NULL == key)
		return NULL;

	branch = _avl_LookupNode(tree, key, &parent, NULL);
	node = *branch;

	switch (condition)
	{
	case COND_LE: /* Less then or Equal */
		if (NULL != node)
			break;
	case COND_LT: /* Less Then */
		if (NULL != node)
			node = (NULL == node->left
			? _avl_PrevUpNode(node) : _avl_PrevDownNode(node));
		else if (NULL != parent)
			node = (branch == &(parent->left)
			? _avl_PrevUpNode(parent) : parent);
		break;

	case COND_GE: /* Great then or Equal */
		if (NULL != node)
			break;
	case COND_GT: /* Great Then */
		if (NULL != node)
			node = (NULL == node->right
			? _avl_NextUpNode(node) : _avl_NextDownNode(node));
		else if (NULL != parent)
			node = (branch == &(parent->right)
			? _avl_NextUpNode(parent) : parent);
		break;

	case COND_EQ: /* EQual */
		break;

	default:
		node = NULL;
		break;
	}

	if (NULL != alg) alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_FindByIndex(struct avl_Tree * tree, int index, struct alg_Node * alg)
{
	struct avl_Node * node;
	unsigned seq = (unsigned) index;

	if (0 >= index || NULL == tree || seq > tree->count)
		return NULL;

	node = tree->root;
	while (NULL != node && seq != node->rank)
	{
		if (seq < node->rank)
		{
			node = node->left;
		}
		else /* (seq > node->rank) */
		{
			seq -= node->rank;
			node = node->right;
		}
	}

	if (NULL != alg) alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_First(struct avl_Tree * tree, struct alg_Node * alg)
{
	struct avl_Node * node;

	if (NULL == tree)
		return NULL;

	if (NULL != (node = tree->root))
	{
		node = _avl_FirstNode(node);
	}

	if (NULL != alg) alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_Next(struct avl_Tree * tree, struct alg_Node * alg) /* must be */
{
	struct avl_Node * node;

	if (NULL == tree || NULL == alg)
		return NULL;

	if (NULL != (node = (struct avl_Node *) (alg->data)))
	{
		node = (NULL == node->right
			? _avl_NextUpNode(node) : _avl_NextDownNode(node));
	}

	alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_Prev(struct avl_Tree * tree, struct alg_Node * alg) /* must be */
{
	struct avl_Node * node;

	if (NULL == tree || NULL == alg)
		return NULL;

	if (NULL != (node = (struct avl_Node *) (alg->data)))
	{
		node = (NULL == node->left
			? _avl_PrevUpNode(node) : _avl_PrevDownNode(node));
	}

	alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_Top(struct avl_Tree * tree, struct alg_Node * alg)
{
	struct avl_Node * node;

	if (NULL == tree)
		return NULL;

	node = tree->root;

	if (NULL != alg) alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_Depth(struct avl_Tree * tree, unsigned depth, struct alg_Node * alg)
{
	struct avl_Node * node;

	if (NULL == tree)
		return NULL;

	if (NULL != (node = tree->root))
	{
		node = _avl_NextDepthNode(node, node, depth);
	}

	if (NULL != alg) alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_Peer(struct avl_Tree * tree, struct alg_Node * alg) /* must be */
{
	struct avl_Node * node;

	if (NULL == tree || NULL == alg)
		return NULL;

	if (NULL != (node = (struct avl_Node *) (alg->data)))
	{
		node = _avl_NextDepthNode(node->parent, node, 1);
	}

	alg->data = (void *) node;
	return (NULL == node ? NULL : node->data);
}

void *
avl_GetData(struct avl_Tree * tree, struct alg_Node * alg) /* must be */
{
	struct avl_Node * node;

	if (NULL == tree || NULL == alg)
		return NULL;

	node = (struct avl_Node *) (alg->data);
	return (NULL == node ? NULL : node->data);
}

int
avl_GetCount(struct avl_Tree * tree)
{
	if (NULL == tree)
		return -EPERM;

	return (int) (tree->count);
}

int
avl_GetIndex(struct avl_Tree * tree, void * key, struct alg_Node * alg)
{
	int index;
	struct avl_Node * node, * assist;

	if (NULL == tree)
		return -EPERM;

	if (NULL != key)
	{
		node = *(_avl_LookupNode(tree, key, &assist, &index));
		if (NULL != alg) alg->data = (void *) node;
	}
	else if (NULL != alg && NULL != alg->data)
	{
		assist = (struct avl_Node *) (alg->data);
		index = (int) (assist->rank);
		while (NULL != (node = assist->parent))
		{
			if (assist == node->right)
				index += (int) (node->rank);

			assist = node;
		}
	}
	else
		return -EPERM;

	return index;
}

/* Insert a node in the AVL tree. */
void *
avl_Insert(struct avl_Tree * tree, void * data, struct alg_Node * alg)
{
	struct avl_Node * node, * parent, ** branch;

	if (NULL == tree || NULL == data)
		return NULL;

	branch = _avl_LookupNode(tree, data, &parent, NULL);
	if (NULL == (node = *branch))
	{
		if (NULL == (node = _avl_NewNode(tree, data)))
			return NULL;

		*branch = node;
		node->parent = parent;

		_avl_IncreaseRank(node);
		tree->count ++;

		_avl_GrowTree(tree, parent, node);

	}

	if (NULL != alg) alg->data = (void *) node;
	return node->data;
}

/* Delete a node in the AVL tree. */
int
avl_Delete(struct avl_Tree * tree, void * key, alg_Fn_t clear, void * aux)
{
	struct avl_Node * node, * parent, ** branch;

	if (NULL == tree || NULL == key)
		return -EPERM;

	branch = _avl_LookupNode(tree, key, &parent, NULL);
	if (NULL == (node = *branch))
		return -ENOENT;

	/* If both subtrees of the specified node are non-empty,
	* the node should be interchanged with the next one,
	* at least one subtree of which is always empty. */
	if (NULL != node->left && NULL != node->right)
	{
		branch = _avl_SwapNode(tree, node, branch);
		parent = node->parent;
	}

	/* Now the specified node has at least one empty subtree;
	* go up to the root and adjust the rank field of all nodes affected by deletion */
	_avl_DecreaseRank(node);
	tree->count --;

	/* delete the specified node from the tree */
	*branch = (NULL == node->left ? node->right : node->left);
	if (NULL != *branch)
		(*branch)->parent = parent;

	_avl_ShrinkTree(tree, parent, branch);

	if (NULL != clear)
		clear(node->data, aux);

	free(node);
	return 0;
}

int
avl_Traverse(struct avl_Tree * tree, alg_Fn_t handle, void * aux)
{
	int rc = 0;
	struct avl_Node * node;

	if (NULL == tree || NULL == handle)
		return -EPERM;

	if (NULL == (node = tree->root))
		return rc;

	node = _avl_FirstNode(node);
	/* Traverse according to sequence of order,
	* so it allow to add or/and delete node in "handle()" module */
	while (NULL != node)
	{
		struct avl_Node * next = (NULL == node->right
			? _avl_NextUpNode(node) : _avl_NextDownNode(node));

		if (0 > (rc = handle(node->data, aux)))
			break;

		node = next;
	}

	return rc;
}

struct avl_Tree *
	avl_Create(alg_Fn_t compare, unsigned size)
{
	struct avl_Tree * tree;

	tree = (struct avl_Tree *) malloc(sizeof(struct avl_Tree));
	if (NULL == tree)
		return NULL;

	memset(tree, 0, sizeof(struct avl_Tree));

	strncpy(tree->name, "AVL", ALG_NAME_LEN);
	tree->compare = compare;
	tree->size = size;

	return tree;
}

int
avl_Destroy(struct avl_Tree * tree, alg_Fn_t clear, void * aux)
{
	struct avl_Node * node;

	if (NULL == tree)
		return -EPERM;

	/* Traverse in due sequence of leaves to root (children to parent),
	* so if add or/and delete node in "handle()" module,
	* it can't garrantee that it traverse all nodes on AVL tree. */
	node = tree->root; tree->root = NULL;
	/* _avl_Destory(struct avl_Node * node, alg_Fn_t clear, void * aux)
	* {
	*      if (NULL == node)
	*  	return;
	*
	*      _avl_Destory(node->left, clear, aux);
	*      _avl_Destory(node->right, clear, aux);
	*
	*      if (NULL != clear)
	*  	clear(child->data, aux);
	*
	*      free(node);
	* } */
	while (NULL != node)
	{
		struct avl_Node * parent, * child;

		do /* find most-left leaf-node on sub-tree */
		{
			parent = node;
			if (NULL == (node = parent->left))
				node = parent->right;
		}
		while (NULL != node);

		do /* process leaf-node or parent-node that children have been free. */
		{
			node = parent;
			parent = node->parent;

			if (NULL != clear)
				clear(node->data, aux);

			free(node);

			if (NULL == parent)
				goto _alg_out;

			child = parent->right;
		}
		while (NULL == child || node == child);

		node = child;
	}

_alg_out:
	free(tree);
	return 0;
}
