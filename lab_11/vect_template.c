#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
    printf("%zu \n", vector->capacity);
    for (size_t i = 0; i < vector->size; i++){
        print(vector->data + i * vector->element_size);
    }
}

// print integer value
void print_int(const void *v) {
    int value = *(int*) v;
    printf("%d ", value);
}

// print char value
void print_char(const void *v) {
    char value = *(char*) v;
    printf("%c ", value);
}
// print structure Person
void print_person(const void *v) {
    Person * person = (Person *) v;
    printf("%d %s %s\n", person->age, person->first_name, person->last_name);
}

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
    vector->data = malloc(block_size * element_size);
    vector->element_size = element_size;
    vector->size = 0;
    vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
    if (vector->capacity < new_capacity){
        vector->capacity = new_capacity;
        vector->data = realloc(vector->data, new_capacity * vector->element_size);
    }
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    value = (char *) value;
    if (vector->size == vector->capacity) {
        reserve(vector, vector->capacity * 2);
    }
    char *insertion_pointer = (char *) vector->data + vector->size * vector->element_size;
    memcpy(insertion_pointer, value, vector->element_size);
    vector->size++;
}
// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    if (new_size == vector->size) {
        return;
    }
    if (new_size < vector->size){
        vector->size = new_size;
        return;
    }
    if (new_size > vector->size){
        size_t old_size = vector->size;
        if (vector->capacity < new_size){
            reserve(vector, new_size);
        }
        size_t additional_elements = new_size - old_size;
        char *additional_data = (char *)vector->data + old_size * vector->element_size;
        memset(additional_data, 0, additional_elements * vector->element_size);
        vector->size = new_size;
    }
}
void clear(Vector *vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->element_size = 0;
    vector->capacity = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
    value = (char*) value;
    if (vector->size == vector->capacity) {
        reserve(vector, vector->capacity * 2);
    }
    char *source = (char *) vector->data + index * vector->element_size;
    char *destination = (char *) source + vector->element_size;
    size_t elements_to_shift = vector->size - index;
    memmove(destination, source, elements_to_shift * vector->element_size);
    memcpy(source, value, vector->element_size);
    vector->size++;
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
    char *destination = (char *)vector->data + (index * vector->element_size);
    void *source = (char *)vector->data + ((index + 1) * vector->element_size);
    size_t elements_to_shift = vector->size - index - 1;
    memmove(destination, source, elements_to_shift * vector->element_size);
    vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void * value, cmp_ptr cmp) {
    size_t i = 0;
    value = (char *) value;
    while (i < vector->size){
        char * val = vector->data + i * vector->element_size;
        if (cmp(value, val) == 0){
            erase(vector, i);
        }
        else {
            i++;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    size_t i = 0;
    void* current;
    while (i < vector->size) {
        current = vector->data + (i * vector->element_size);
        if (predicate(current)) {
            erase(vector, i);
        } else {
            i++;
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
    void* new_data = realloc(vector->data, vector->size * vector->element_size);
    vector->data = new_data;
    vector->capacity = vector->size;
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
    return *(int*) v1 - *(int*) v2;
}
// char comparator
int char_cmp(const void *v1, const void *v2) {
    return *(const char*)v1 - *(const char*)v2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2){
    Person* per1 = (Person *)p1;
    Person* per2 = (Person *)p2;
    if( per1->age == per2->age ){
        int first_name_cmp = strcmp(per1->first_name, per2->first_name);
        if (first_name_cmp == 0){
            return strcmp(per1->last_name, per2->last_name);
        }
        return first_name_cmp;
    }
    return (per1->age - per2->age) * -1;
}

// predicate: check if number is even
int is_even(void *value) {
    int val = *(int*) value;
    return val % 2 == 0;
}

// predicate: check if char is a vowel
int is_vowel(void *value) {
    char l = *(char*) value;
    if( l == 'a' || l == 'e' || l == 'i' || l == 'o'
        || l == 'u' || l == 'y' || l == 'A' || l == 'E' ||
        l == 'I' || l == 'O' || l == 'U' || l == 'Y'){
        return 1;
    }
    return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
    Person *p = (Person *) person;
    return p->age > 25;
}

// read int value
void read_int(void* value) {
    scanf("%d", value);
}

// read char value
void read_char(void* value) {
    char c[2];
    scanf("%s", c);
    *(char*)value = c[0];
}

// read struct Person
void read_person(void* value) {
    Person *person = (Person *) value;
    scanf("%d %s %s", &person->age, person->first_name, person->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

