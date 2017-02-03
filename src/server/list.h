#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void* LIST_TYPE;
typedef struct List List;
typedef struct Node Node;

struct Node {
	LIST_TYPE value;
	Node* next;
	Node* prev;
};

struct List{
	int size;
	Node* head;
	Node* tail;
	void (*list_print_function)(void*);
};

Node* node_constructor(LIST_TYPE value);
List* list_constructor(void);
List* list_constructor_print( void (*list_print_function)(void*) );
void list_append(LIST_TYPE value, List* list);
void list_print(List *list);
void list_insert(int index, LIST_TYPE value, List *list);
void list_prepend(LIST_TYPE value, List* list);
LIST_TYPE list_remove(int index, List *list);
LIST_TYPE list_get(int index, List *list);
LIST_TYPE list_removeFirst(List *list);
LIST_TYPE list_removeLast(List *list);
LIST_TYPE list_getFirst(List* list);
LIST_TYPE list_getLast(List* list);

#endif
