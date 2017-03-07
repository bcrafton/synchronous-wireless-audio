#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef void* KEY_TYPE;
typedef void* LIST_TYPE;
typedef struct List List;
typedef struct Node Node;

struct Node {
  KEY_TYPE key;
	LIST_TYPE value;
	Node* next;
	Node* prev;
};

struct List{
	int size;
	Node* head;
	Node* tail;
  int (*compare)(void*, void*);
};

Node* node_constructor(KEY_TYPE key, LIST_TYPE value);
List* list_constructor( int (*compare)(void*, void*) );
void list_append(KEY_TYPE key, LIST_TYPE value, List* list);
LIST_TYPE list_remove(KEY_TYPE key, List *list);
LIST_TYPE list_search(KEY_TYPE key, List* list);

#endif
