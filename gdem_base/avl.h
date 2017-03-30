#ifndef AVL_H
#define AVL_H 1

#include "gdem_base_global.h"

typedef int (* alg_Fn_t)(void *,void *);

#define COND_LT		(-2)	/* Less Then */
#define COND_LE		(-1)	/* Less then or Equal */
#define COND_EQ		(0)	/* EQual */
#define COND_GE		(1)	/* Great then or Equal */
#define COND_GT		(2)	/* Great Then */

#define COND_LP 	(3)	/* Longest Prefix */

struct alg_Node
{
	void *	data;	/* Pointer to exact node */
};

#if ! defined(__TREE_ALG_IMPLEMENTATION__)

#define ALG_NAME_LEN	sizeof(unsigned long)

struct avl_Tree
{
	char	name[ALG_NAME_LEN];	/* AVL-Tree name, allways "AVL". */
};

#endif

GDEM_BASE_EXPORT void * avl_Find(struct avl_Tree * tree, void * key,
				int condition, struct alg_Node * node);
GDEM_BASE_EXPORT void * avl_FindByIndex(struct avl_Tree * tree, int index, struct alg_Node * node);
GDEM_BASE_EXPORT void * avl_First(struct avl_Tree * tree, struct alg_Node * node);
GDEM_BASE_EXPORT void * avl_Next(struct avl_Tree * tree, struct alg_Node * node); /* must be */
GDEM_BASE_EXPORT void * avl_Prev(struct avl_Tree * tree, struct alg_Node * node); /* must be */
GDEM_BASE_EXPORT void * avl_Top(struct avl_Tree * tree, struct alg_Node * node);
GDEM_BASE_EXPORT void * avl_Depth(struct avl_Tree * tree, unsigned depth, struct alg_Node * node);
GDEM_BASE_EXPORT void * avl_Peer(struct avl_Tree * tree, struct alg_Node * node); /* must be */
GDEM_BASE_EXPORT void * avl_GetData(struct avl_Tree * tree, struct alg_Node * node); /* must be */
GDEM_BASE_EXPORT int    avl_GetCount(struct avl_Tree * tree);
GDEM_BASE_EXPORT int    avl_GetIndex(struct avl_Tree * tree, void * key, struct alg_Node * node);
GDEM_BASE_EXPORT void * avl_Insert(struct avl_Tree * tree, void * data, struct alg_Node * node);
GDEM_BASE_EXPORT int    avl_Delete(struct avl_Tree * tree, void * key, alg_Fn_t clear, void * aux);

GDEM_BASE_EXPORT int    avl_Traverse(struct avl_Tree * tree, alg_Fn_t handle, void * aux);
GDEM_BASE_EXPORT struct avl_Tree * avl_Create(alg_Fn_t compare, unsigned size);
GDEM_BASE_EXPORT int    avl_Destroy(struct avl_Tree * tree, alg_Fn_t clear, void * aux);


#endif /* avl.h */
