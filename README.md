# kette
Kette is a header-only C library for creating and manipulating circular singly and doubly linked lists. It is designed to be simple, minimal, and easy to use. Using this library is as simple as copying a single header file into your project. Kette heavily borrows from the Linux kernel's linked list design and API.
## Features
- Singly and doubly linked lists
- Type-safe
- Minimal API
- Minimal code
- Functions and macros for common operations
- No dynamic memory allocation
- No dependencies
- Header-only
- Supports stacks and queues
## Usage
To use Kette, all you need it the `kette.h` header file. You can copy it into your project, add it as a git submodule, or whatever method you prefer. The library is written in C99 with no dependence on any external libraries or OS-specific APIs. It does, however, use offsetof(), so ensure your compiler supports this operation.

### Creating and Adding to a List
To use Kette, include the header file into your source file. To create a list, you need to define a struct that contains either a slink or dlink member. The member name can be anything you'd like. The following example demonstrates how to create a singly linked list:
```c
struct my_struct {
    int data;
    ...
    struct slink list_node;
};

struct my_struct foo = { 0, SLIST_INIT(foo.list_node) };
// or for after initialization
slist_init(&foo.list_node);
```
Now you have a struct, *my_struct*, that can be used to make a singly linked list. The *slink* member will allow us to chain multiple *my_struct* instances together. Adding an element to the list is simple:
```c
struct my_struct bar = { 0 };
          // New node     // List head (can be any node)
slist_add(&foo.list_node, &bar.list_node);
// or
slist_add_tail(&foo.list_node, &bar.list_node);
```
Keep in mind that slist_add() adds the node directly after the head, while slist_add_tail() adds the node to the end of the list. If slist_add_tail() is something you'll be doing often, switch to a doubly linked list.

### Using a List
It may seem strange to implement a linked list this way (especially if you come from a higher-level language like Java), but it is the best way to add generic linked list functionality to C. Imagine you want to do the simplest opeation, like get the next element in the list:
```c
struct my_struct *element;
...
struct slink *next = element->list_node.next;
```
Okay, so you have a pointer to the slink member, but that is not very useful. What you really want is the *my_struct* pointer. This is where the list_entry() macro comes in:
```c
struct my_struct *next_element = list_entry(next, struct my_struct, list_node);
```
Here we pass in a pointer to the slink member, the type of the containing struct, and the name of the slink member. The macro will return a pointer to the containing struct. This operation is type-safe and will not compile if the types do not match. You should be familiar with this macro because it will be used in almost every operation you perform on the list.

### Iterating Over a List
Kette provides a simple macro for iterating over the entire list. The following example demonstrates how to iterate over a singly linked list:
```c
struct my_struct *element;
slist_for_each_entry(&foo.list_node, element, struct my_struct, list_node) {
    // Do something with element
}
```
This is pretty busy for a simple operation. I could have eliminated the need for the struct type, but I did not want to use the non-standard typeof() operator.

### Further Usage
The above covered the basic of using Kette. It only covered singly linked lists, but doubly linked lists are just as easy to use, but have their own set of functions and macros. Other operations include
- list_empty(): Check if a list is empty
- slist_find_prev(): Finds the previous element in a singly linked list
- [s|d]list_del(): Remove an element from the list
- [s|d]list_splice(): Merge two lists
- dlist_for_each_reverse(): Iterate over a doubly linked list in reverse
