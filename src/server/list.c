#include "list.h"

static void print(LIST_TYPE v, List *list);

Node* node_constructor(LIST_TYPE value){
	Node *node = malloc(sizeof(Node));
	node->next = NULL;
	node->prev = NULL;
	node->value = value;
    return node;
}

List* list_constructor_print( void (*list_print_function)(void*) ){
	List *list = malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->list_print_function = list_print_function;
	return list;
}

List* list_constructor(){
	List *list = malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->list_print_function = NULL;
	return list;
}

void list_append(LIST_TYPE value, List* list){
	Node* newNode = node_constructor(value);
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

void list_prepend(LIST_TYPE value, List* list){
	Node* newNode = node_constructor(value);
	if(list->head == NULL && list->tail == NULL){
		list->head = newNode;
		list->tail = newNode;
	}
	else{
		list->head->prev = newNode;
		newNode->next = list->head;
		list->head = newNode;
	}
	list->size++;
}

void list_set(int index, LIST_TYPE value, List *list){
	if(index >= list->size || index < 0){
		printf("index is out of bounds.\n");
		assert(0);
		return;
	}
	int counter = 0;
	Node* ptr;
	for(ptr = list->head; ptr != NULL; ptr = ptr->next){
		if(counter == index){
			ptr->value = value;
			return;
		}
		counter++;
	}
}

void list_insert(int index, LIST_TYPE value, List *list){
	if(index > list->size || index < 0){
		printf("index is out of bounds.\n");
		assert(0);
		return;
	}
	Node* newNode = node_constructor(value);
	// this wud break if it wasnt the case that the only way to insert something to a empty list is to insert it into the 0 index. 
	// whcih means prepend will handle this fine.
	if(index == 0){
		list_prepend(value, list);
	}
	else if(index == list->size){
		list_append(value, list);
	}
	else{
		int counter = 0;
		Node* ptr;
		for(ptr = list->head; ptr != NULL; ptr = ptr->next){
			if(counter == index){
				Node* left = ptr->prev;
				Node* right = ptr;
				left->next = newNode;
				newNode->prev = left;
				right->prev = newNode;
				newNode->next = right;
				list->size++;
				return;
			}
			counter++;
		}
	}
}

LIST_TYPE list_removeFirst(List *list){
	LIST_TYPE ret = list->head->value;
	Node* newHead = list->head->next;
	Node* oldHead = list->head;
	newHead->prev = NULL;
	oldHead->next = NULL;
	list->head = newHead;
	list->size--;
	return ret;
}
LIST_TYPE list_removeLast(List *list){
	LIST_TYPE ret = list->tail->value;
	Node* newTail = list->tail->prev;
	Node* oldTail = list->tail;
	newTail->next = NULL;
	oldTail->prev = NULL;
	list->tail = newTail;
	list->size--;
	return ret;
}

LIST_TYPE list_remove(int index, List *list){
	if(index >= list->size || index < 0){
		printf("index is out of bounds.\n");
		assert(0);
		return NULL;
	}
	if(list->size == 1){
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return NULL;
	}
	if(index == 0){
		return list_removeFirst(list);
	}
	else if(index == list->size-1){
		return list_removeLast(list);
	}
	else{
		int counter = 0;
		Node* ptr;
		for(ptr = list->head; ptr != NULL; ptr = ptr->next){
			if(counter == index){
				Node* left = ptr->prev;
				Node* right = ptr->next;
				left->next = right;
				right->prev = left;
				list->size--;
				return ptr;
			}
			counter++;
		}
        return NULL;
	}
}

LIST_TYPE list_get(int index, List *list){
	int counter = 0;
	Node* ptr;
	for(ptr = list->head; ptr != NULL; ptr = ptr->next){
		if(counter == index){
			return ptr->value;
		}
		counter++;
	}
	return NULL;
}

void list_print(List *list){
	Node *ptr;
	for(ptr = list->head; ptr != NULL; ptr = ptr->next){
		print(ptr->value, list);
		printf("|");
	}
	printf("[%d]\n", list->size);
}

static void print(LIST_TYPE v, List *list){
	(*(list->list_print_function))(v);
}
