#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
    Node *new_node = create_node(data, array_size, list->tail, list->tail->prev);
    list->tail->prev->next = new_node;
    list->tail->prev = new_node;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
    for (size_t i = n; i > 0; i--){
        if (itr->position+1 == itr->node_ptr->array_size){
            itr->node_ptr = itr->node_ptr->next;
            itr->position = 0;
        }
        else{
            itr->position++;
        }
    }
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator itr = begin(list->head->next);
    skip_forward(&itr, n-1);
    return itr.node_ptr->data[itr.position];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
    for (size_t i = n; i > 0; i--){
        if (itr->position == 0){
            itr->node_ptr = itr->node_ptr->prev;
            itr->position = itr->node_ptr->array_size-1;
        }
        else{
            itr->position--;
        }
    }
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
    iterator itr = end(list->tail->prev);
    skip_backward(&itr, n);
    return itr.node_ptr->data[itr.position];
}

void remove_node(Node *node_ptr) {
    node_ptr->prev->next = node_ptr->next;
    node_ptr->next->prev = node_ptr->prev;
    free(node_ptr->data);
    free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
    iterator itr = begin(list->head->next);
    skip_forward(&itr, n-1);
    if (itr.node_ptr->array_size == 1){
        remove_node(itr.node_ptr);
    }
    else{
        int * dest = itr.node_ptr->data + itr.position;
        int * src = itr.node_ptr->data + itr.position + 1;
        memmove(dest, src, (itr.node_ptr->array_size - itr.position - 1) * sizeof(int));
        itr.node_ptr->array_size--;
    }
}

// return the number of digits of number n
size_t digits(int n) {
    size_t output=0;
    if (n == 0) return 1;
    while (n != 0){
        output++;
        n /= 10;
    }
    return output;
}
void insert_new_node(List *list, Node* pointer, int value) {
    int *data = safe_malloc(sizeof(int));
    data[0] = value;
    pointer->prev->next = create_node(data, 1, pointer, pointer->prev);
    pointer->prev = pointer->prev->next;
}
// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
    size_t digits_count = digits(value);
    Node *node_ptr = list->head->next;
    while (node_ptr != list->tail){
        if (digits(node_ptr->data[0]) == digits_count){
            node_ptr->data = safe_realloc(node_ptr->data, (node_ptr->array_size+1) * sizeof(int));
            for (int i = 0; i < node_ptr->array_size; i++){
                if (node_ptr->data[i] > value){
                    int * dest = node_ptr->data + i + 1;
                    int * src = node_ptr->data + i;
                    memmove(dest, src, (node_ptr->array_size - i) * sizeof(int));
                    node_ptr->data[i] = value;
                    node_ptr->array_size++;
                    return;
                }
            }
            node_ptr->data[node_ptr->array_size] = value;
            node_ptr->array_size++;
            return;
        }else if(digits(node_ptr->data[0]) > digits_count){
            insert_new_node(list, node_ptr, value);
            return;
        }
        node_ptr = node_ptr->next;
    }
    insert_new_node(list, node_ptr, value);
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}
