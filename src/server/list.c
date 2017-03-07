#include "list.h"

static int compare(KEY_TYPE k1, KEY_TYPE k2, List *list);
static Node* list_node_search(KEY_TYPE key, List* list);

Node* node_constructor(KEY_TYPE key, LIST_TYPE value){
	Node *node = malloc(sizeof(Node));
	node->next = NULL;
	node->prev = NULL;
  node->key = key;
	node->value = value;
  return node;
}

List* list_constructor( int (*compare)(void*, void*) ){
	List *list = malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
  list->compare = compare;
	return list;
}

void list_append(KEY_TYPE key, LIST_TYPE value, List* list){
	Node* newNode = node_constructor(key, value);
	if(list->head == NULL && list->tail == NULL){
		list->head = newNode;
		list->tail = newNode;
	}
	else{
		list->tail->next = newNode;
		newNode->prev = list->tail;
		list->tail = newNode;
	}
	list->size++;
}

LIST_TYPE list_remove(KEY_TYPE key, List *list)
{
  Node* node = list_node_search(key, list);
  if(node == NULL)
  {
    return NULL;
  }
  else
  {
    list->size--;
    // is there a way to do this without 4 conditions?
    if(list->head == node && list->tail == node)
    {
      list->head = NULL;
      list->tail = NULL;
      assert(node->next == NULL && node->prev == NULL);
    }
    else if(list->head == node)
    {
      Node* new_head = node->next;
      new_head->prev = NULL;
      node->next = NULL;
      list->head = new_head;
    }
    else if(list->tail = node)
    {
      Node* new_tail = node->prev;
      new_tail->next = NULL;
      node->prev = NULL;
      list->tail = new_tail;
    }
    else
    {
      Node* next = node->next;
      Node* prev = node->prev;
      next->prev = prev;
      prev->next = next;
    }
    return node->value;
  }
}

LIST_TYPE list_search(KEY_TYPE key, List *list)
{
  Node* node = list_node_search(key, list);
  if(node == NULL)
  {
    return NULL;
  }
  else
  {
    return node->value;
  }
}

static Node* list_node_search(KEY_TYPE key, List* list)
{
  Node* ptr;
  for(ptr = list->head; ptr != NULL; ptr = ptr->next)
  {
    if(compare(key, ptr->key, list) == 0)
    {
      return ptr;
    }
  }
  return NULL;
}

static int compare(KEY_TYPE k1, KEY_TYPE k2, List *list)
{
	return (*(list->compare))(k1, k2);
}



