#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);
typedef struct DataWord {
    char *word;
    int counter;
} DataWord;
typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

void dump_int(const void *data){
    printf("%d ", *(int*) data);
}


// Print elements of the list
void dump_list(const List* p_list) {
    ListElement* ptr = p_list->head;
    while (ptr != NULL){
        p_list->dump_data(ptr->data);
        ptr = ptr->next;
    }
}


// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
    ListElement* ptr = p_list->head;
    while (ptr != NULL){
        if (p_list->compare_data(ptr->data, data) == 0){
            p_list->dump_data(ptr->data);
        }
        ptr = ptr->next;
    }
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
}

// Free all elements of the list
void free_list(List* p_list) {
	p_list->tail = NULL;
}
void init_first_element(List *p_list, void * data){
    ListElement* new_element = safe_malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = NULL;
    p_list->head = new_element;
    p_list->tail = new_element;
}
// Push element at the beginning of the list
void push_front(List *p_list, void *data){
    if (p_list->head == NULL) {
        init_first_element(p_list, data);
    }else{
        ListElement* new_element = safe_malloc(sizeof(ListElement));
        new_element->data = data;
        new_element->next = p_list->head;
        p_list->head = new_element;
    }

}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
    if (p_list->head == NULL) {
        init_first_element(p_list, data);
    }else{
        ListElement *new_element = safe_malloc(sizeof(ListElement));
        new_element->data = data;
        new_element->next = NULL;
        p_list->tail->next = new_element;
        p_list->tail = new_element;
    }
}

// Remove the first element
void pop_front(List *p_list) {
    ListElement *front = p_list->head;
    p_list->head = front->next;
    free(front);
}

// Reverse the list
void reverse(List *p_list) {
    ListElement* ptr = p_list->head;
    ListElement* shadow = NULL;
    ListElement* temp = NULL;
    temp = p_list->tail;
    p_list->tail = p_list->head;
    p_list->head = temp;
    while (ptr != NULL) {
        temp = ptr->next;
        ptr->next = shadow;
        shadow = ptr;
        ptr = temp;
    }
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, void *p_data) {
    ListElement* shadow = NULL;
    ListElement* ptr = p_list->head;
    while (ptr != NULL && p_list->compare_data(ptr->data, p_data) < 0) {
        shadow = ptr;
        ptr = ptr->next;
    }
    return shadow;
}

// Insert element after 'previous'
void push_after(void *data, ListElement *previous) {
    ListElement* new_element = safe_malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = previous->next;
    previous->next = new_element;
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    if (p_list->head == NULL) {
        init_first_element(p_list, p_data);
    }else{
        ListElement* insert_point = find_insertion_point(p_list, p_data);
        if (insert_point == NULL) {
            if (p_list->compare_data(p_list->head->data, p_data) != 0){
                push_front(p_list, p_data);
            }else{
                if (p_list->modify_data != NULL) p_list->modify_data(p_list->head->data);
            }
        }
        else if (insert_point == p_list->tail) {
            if (p_list->compare_data(p_list->tail->data, p_data) != 0){
                push_back(p_list, p_data);
            }else{
                if (p_list->modify_data != NULL) p_list->modify_data(p_list->tail->data);
            }
        }else {
            if (p_list->compare_data(insert_point->next->data, p_data) != 0) {
                push_after(p_data, insert_point);
            } else {
                if (p_list->modify_data != NULL) p_list->modify_data(insert_point->next->data);
            }
        }
    }

}

// -----------------------------------------------------------
// --- type-specific definitions

// int element


void free_int(void *d) {
    free(d);
}

int cmp_int(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int *create_data_int(int v) {
    int *p = malloc(sizeof(int));
    *p = v;
    return p;
}

// Word element



void dump_word(const void *data){
    DataWord* ptr = (DataWord*) data;
    printf("%s ", ptr->word);
}
char* to_lower(char* s) {
    for(char *p=s; *p; p++) {
        *p=tolower(*p);
    }
    return s;
}
void dump_word_lowercase (const void *d) {
    DataWord* ptr = (DataWord*) d;
    printf("%s ", to_lower(ptr->word));
}
void free_word(void *d) {
    free(((DataWord*)d)->word);
    free(d);
}

int cmp_word_alphabet(const void *a, const void *b) {
    return strcmp(to_lower(((DataWord*)a)->word), to_lower(((DataWord*)b)->word));
}

int cmp_word_counter(const void *a, const void *b) {
    return ((DataWord*)a)->counter - ((DataWord*)b)->counter;
}

void modify_word(void *p) {
    ((DataWord*)p)->counter++;
}

void *create_data_word(const char *string, int counter) {
    DataWord *p = safe_malloc(sizeof(DataWord));
    p->word = safe_strdup(string);
    p->counter = counter;
    return p;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
    char buffer[1024];
    char delimit[]=" \t\r\n\v\f.,?!:;-";
    while (fgets(buffer, sizeof(buffer), stream) != NULL) {
        char *word = strtok(buffer, delimit);
        while (word != NULL) {
            if (cmp != NULL) {
                insert_in_order(p_list, create_data_word(word, 1));
            } else {
                push_back(p_list, create_data_word(word, 1));
            }
            word = strtok(NULL, delimit);
        }
    }
}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word_lowercase, free_word, cmp_word_alphabet, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

