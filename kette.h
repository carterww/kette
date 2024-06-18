/*
 * Author: Carter Williams
 * Email: carterww@hotmail.com
 * Date: 2024-06-17
 *
 * This project will be contained in a single header and source file. The scope
 * of this library is simply to provide a mechanism for implementing linked
 * lists in C. This library was heavily inspired by the Linux kernel's linked
 * list implementation, and I wanted to provide a similar interface for my own
 * projects.
 *
 * The library will provide two types of linked lists: singly linked lists and
 * doubly linked lists. It may be simpler to just provide only doubly linked lists,
 * but I did not want force the user to take on unnecessary overhead if they only
 * needed a singly linked list. Functions or macros that are denoted with an 's'
 * at the beginning of the name are for singly linked lists. Functions or macros
 * that are denoted with a 'd' at the beginning of the name are for doubly linked
 * lists. If neither of these prefixes are present, the function or macro is
 * intended to be used for both types of linked lists.
 *
 * Both types of linked lists will be circular. This means that the last node in
 * the list will point to the first node in the list. This is done because there
 * is almost no downside to having a circular list, but there are many upsides in
 * some cases. Instead of having to check if the next node is NULL, the user can
 * simply check if the next node is the head of the list.
 *
 * The library will provide the following functions for singly linked lists:
 * - slist_add: Add a node right after the given node.
 * - slist_add_tail: Add a node to the end of the list. This function will be
 *   O(n) time complexity because the list must be traversed. If this is a common
 *   operation, consider using a doubly linked list.
 * - slist_del: Delete the given node from the list.
 * - slist_empty: Returns a non-zero value if the list is empty.
 * - slist_splice: Joins two lists together by adding first list after the head of
 *   the second list.
 * 
 * The library will provide the following functions for doubly linked lists:
 * - dlist_add: Similar to slist_add.
 * - dlist_add_tail: Similar to slist_add_tail, but O(1) time complexity.
 * - dlist_del: Similar to slist_del.
 * - dlist_empty: Similar to slist_empty.
 * - dlist_splice: Similar to slist_splice.
 */

#ifndef KETTE_H
#define KETTE_H

/* 
 * Singly linked list node. To make a singly linked list, give a struct
 * a member of type struct slink. The struct will be the node of the list.
 * Use the helper functions to manipulate the list through the struct's
 * singly linked list member. use the macro list_entry to get the pointer
 * to the node's struct.
 */
struct slink {
  struct slink *next;
};

/*
 * Doubly linked list node. Similar to the singly linked list node in every
 * aspect except the prev pointer.
 */
struct dlink {
  struct dlink *next;
  struct dlink *prev;
};

/*
 * From the Linux kernel. Get the container of a member of a struct.
 * This will allow the user to get the address of the actual struct
 * that contains the linked list node.
 * @param ptr: The pointer to the member of the struct.
 * @param type: The type of the struct.
 * @param member: The name of the member in the struct.
 * @return*: The address of the struct that contains the member.
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)((char *)__mptr - offsetof(type,member));})

/*
 * Rebrand container_of to make it more intuitive. I'm leaving container
 * of as well because it can be used in a more general sense.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/*
 * Returns a non-zero value if there are no links (empty).
 */
#define list_empty(ptr) (ptr->next == ptr)

/*
 * Initialize a singly linked list.
 */
#define SLIST_INIT(head) ({ &(head) })
#define SLIST_HEAD(name) struct slink name = SLIST_INIT(name)

static inline void slist_find_prev(struct slink *head, struct slink **prev)
{
  struct slink *next = head->next;
  *prev = head;
  while (next != head) {
    *prev = next;
    next = next->next;
  }
}

static inline void slist_add(struct slink *new, struct slink *head)
{
  struct slink *next = head->next;
  head->next = new;
  new->next = next;
}


static inline void slist_add_tail(struct slink *new, struct slink *head)
{
  struct slink *prev;
  slist_find_prev(head, &prev);
  prev->next = new;
  new->next = head;
}

static inline void slist_del(struct slink *node)
{
  struct slink *prev;
  slist_find_prev(node, &prev);
  prev->next = node->next;
}

static inline void slist_splice(struct slink *list, struct slink *head)
{
  struct slink *list_tail;
  slist_find_prev(list, &list_tail);

  struct slink *head_next = head->next;
  head->next = list;
  list_tail->next = head_next;
}

/*
 * Initialize a doubly linked list.
 */
#define DLIST_INIT(head) ({ &(head), &(head) })
#define DLIST_HEAD(name) struct dlink name = DLIST_INIT(name)

static inline void dlist_add(struct dlink *new, struct dlink *head)
{
  struct dlink *next = head->next;
  // Fix old
  next->prev = new;
  head->next = new;
  // Make new
  new->next = next;
  new->prev = head;
}

static inline void dlist_add_tail(struct dlink *new, struct dlink *head)
{
  dlist_add(new, head->prev);
}

static inline void dlist_del(struct dlink *node)
{
  struct dlink *prev = node->prev;
  prev->next = node->next;
  node->next->prev = prev;
}

static inline void dlist_splice(struct dlink *list, struct dlink *head)
{
  struct dlink *list_tail = list->prev;
  struct dlink *head_next = head->next;
  head->next = list;
  list->prev = head;

  list_tail->next = head_next;
  head_next->prev = list_tail;
}


#endif // KETTE_H
