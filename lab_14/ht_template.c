#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		 DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
    p_table->size = size;
    p_table->no_elements = 0;
    p_table->ht = malloc(size * sizeof(ht_element));
    p_table->dump_data = dump_data;
    p_table->create_data = create_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
    ht_element* p = p_table->ht[n].next;
    while (p != NULL){
        p_table->dump_data(p->data);
        p = p->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}
void print_all(hash_table *p_table){
    for (int i = 0; i < p_table->size; i++){
        dump_list(p_table, i);
        printf("\n");
    }
    printf("-------------------------------\n");
}
void rehash(hash_table *p_table) {

    ht_element list;
    ht_element* pointer = &list;
    for (int i = 0; i < p_table->size; i++){
        ht_element* p = p_table->ht[i].next;
        while (p != NULL){
            pointer->next = p;
            pointer->next->data = p->data;
            pointer = p;
            p = p->next;
        }
    }
    pointer = &list;
    pointer = pointer->next;
    p_table->size *= 2;
    pointer = &list;
    pointer = pointer->next;
    p_table->ht = malloc(p_table->size * sizeof(ht_element));
    while (pointer != NULL) {
        size_t index = p_table->hash_function(pointer->data, p_table->size);
        ht_element* p = p_table->ht + index;
        ht_element * new_element  = (ht_element *)malloc(sizeof(ht_element));
        new_element->next = p->next;
        new_element->data = pointer->data;
        p->next = new_element;
        pointer = pointer->next;
    }
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union *data) {
    size_t index = p_table->hash_function(*data, p_table->size);
    ht_element* shadow = p_table->ht+index;
    ht_element* p = shadow->next;
    while (p != NULL){
        if (p_table->compare_data(p->data, *data) == 0) {
            return shadow;
        }
        shadow = p;
        p = p->next;
    }
    return NULL;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
    size_t index = p_table->hash_function(*data, p_table->size);
    ht_element* p = p_table->ht + index;
    p = p->next;
    while (p != NULL){
        if (p_table->compare_data(p->data, *data) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
    if (get_element(p_table, data) != NULL) {
        return;
    }
    size_t index = p_table->hash_function(*data, p_table->size);
    ht_element* p = p_table->ht + index;
    ht_element * new_element  = (ht_element *)malloc(sizeof(ht_element));
    new_element->next = p->next;
    new_element->data = *data;
    p->next = new_element;
    p_table->no_elements++;
    if (p_table->no_elements / p_table->size > MAX_RATE){
        rehash(p_table);
    }
    //print_all(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
    ht_element * pointer = find_previous(p_table, &data);
    if (pointer == NULL) {
        return;
    }
    pointer->next = pointer->next->next;
}

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
    return a.int_data - b.int_data;
}

data_union create_int(void* value) {
    data_union new;
    int val;
    scanf("%d", &val);
    new.int_data = val;
    return new;
}

// char element

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
    return a.char_data - b.char_data;
}

data_union create_char(void* value) {
    data_union new;
    char val;
    scanf(" %c", &val);
    new.char_data = val;
    return new;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
    DataWord *dw = (DataWord*)data.ptr_data;
    printf("%s %d ", dw->word, dw->counter);
}

void free_word(data_union data) {
    DataWord *dw = (DataWord*)data.ptr_data;
    free(dw->word);
    free(dw);
}

int cmp_word(data_union a, data_union b) {
    DataWord *dw1 = (DataWord*)a.ptr_data;
    DataWord *dw2 = (DataWord*)b.ptr_data;
    return strcmp(dw1->word, dw2->word);
}

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

void modify_word(data_union *data) {
    DataWord *dw = (DataWord*)data->ptr_data;
    dw->counter++;
}

data_union create_data_word(void *value) {
    data_union data;
    DataWord* d1 = (DataWord* )malloc(sizeof(DataWord));

    d1->counter = 1;
    d1->word = (char *)malloc(strlen(value) + 1);

    strcpy(d1->word, value);
    data.ptr_data = d1;

    return data;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
    char buffer[BUFFER_SIZE];
    const char tokens[] = " .,?!:;-\n\t\r";
    char *token;

    while( fgets(buffer, BUFFER_SIZE, stream) != NULL ){
        token = strtok(buffer, tokens);
        while (token != NULL){
            if (strlen(token) != 0){
                char *str = strdup(token);
                for (int i = 0; i < strlen(str); i++){
                    str[i] = tolower(str[i]);
                }
                data_union data = create_data_word(str);
                ht_element *element = get_element(p_table, &data);
                if(element){
                    modify_word(&element->data);
                    free_word(data);
                }
                else{
                    insert_element(p_table, &data);
                }
                token = strtok(NULL, tokens);
            }
        }
        memset(buffer, 0, BUFFER_SIZE);

    }

}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 2: // test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		case 3: // read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	free_table(&table);

	return 0;
}

