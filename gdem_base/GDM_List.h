#ifndef _GDM_LIST_H_
#define _GDM_LIST_H_

typedef struct list_head {
	list_head *next, *prev;
} list_head;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
		list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
		(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_entry( _list_, _type_, _member_ ) \
	((_type_ *)((char *)(_list_)-(unsigned long)(&((_type_ *)0)->_member_)))

#define list_item_is_valid(ptr) ((ptr)->prev != 0 && (ptr)->next != 0)


/*
* Insert a new entry between two known consecutive entries. 
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/
static inline void __list_add(list_head *new_entry, list_head *prev, list_head *next)
{
	next->prev = new_entry;
	new_entry->next = next;
	new_entry->prev = prev;
	prev->next = new_entry;
}

/**
* list_add - add a new entry
* @new: new entry to be added
* @head: list head to add it after
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
static inline void list_add(list_head *new_entry, list_head *head)
{
	__list_add(new_entry, head, head->next);
}

/**
* list_add_tail - add a new entry
* @new: new entry to be added
* @head: list head to add it before
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
static inline void list_add_tail(list_head *new_entry, list_head *head)
{
	__list_add(new_entry, head->prev, head);
}

/*
* Delete a list entry by making the prev/next entries
* point to each other.
*
* This is only for internal list manipulation where we know
* the prev/next entries already!
*/
static inline void __list_del(list_head *prev, list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

/**
* list_del - deletes entry from list.
* @entry: the element to delete from the list.
* Note: list_empty on entry does not return true after this, the entry is in an undefined state.
*/
static inline void list_del(list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = (list_head *) 0;
	entry->prev = (list_head *) 0;
}

/**
* list_del_init - deletes entry from list and reinitialize it.
* @entry: the element to delete from the list.
*/
static inline void list_del_init(list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry); 
}

/**
* list_move - delete from one list and add as another's head
* @list: the entry to move
* @head: the head that will precede our entry
*/
static inline void list_move(list_head *list, list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

/**
* list_move_tail - delete from one list and add as another's tail
* @list: the entry to move
* @head: the head that will follow our entry
*/
static inline void list_move_tail(list_head *list,
list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

/**
* list_empty - tests whether a list is empty
* @head: the list to test.
*/
static inline int list_empty(list_head *head)
{
	return head->next == head;
}

static inline void __list_splice(list_head *list,
list_head *head)
{
	list_head *first = list->next;
	list_head *last = list->prev;
	list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}

/**
* list_splice - join two lists
* @list: the new list to add.
* @head: the place to add it in the first list.
*/
static inline void list_splice(list_head *list, list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}

/**
* list_splice_init - join two lists and reinitialise the emptied list.
* @list: the new list to add.
* @head: the place to add it in the first list.
*
* The list at @list is reinitialised
*/
static inline void list_splice_init(list_head *list,
list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}


#endif /* _GDM_LIST_H_ */

