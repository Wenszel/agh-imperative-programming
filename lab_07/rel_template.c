#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_REL_SIZE 100

typedef struct {
	int first;
	int second;
} pair;

void print_int_array(int *array, int n) {
    for (int i = 0; i < n; i++){
        printf("%d ", array[i]);
    }
}

// The relation R is reflexive if xRx for every x in X
bool is_reflexive(pair* A, int n){
    for (int i = 0; i < n; i++){
        bool flag_first = false;
        bool flag_second = false;
        for (int j = 0; j < n; j++){
            if(A[i].second == A[j].first && A[j].first == A[j].second){
                flag_second = true;
            }
            if(A[i].first == A[j].first && A[j].first == A[j].second){
                flag_first = true;
           }
        }
        if (!(flag_first || flag_second)){
            return false;
        }
    }
    return true;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
bool is_irreflexive(pair *arr, int n){
    {
        for (int i = 0; i < n; i++) {
            if (arr[i].first == arr[i].second) {
                return false;
            }
        }
        return true;
    }
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
bool is_symmetric(pair arr[], int n)
{
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < n; j++) {
            if (arr[i].first == arr[j].second && arr[i].second == arr[j].first) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
bool is_antisymmetric(pair arr[], int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && arr[i].first == arr[j].second && arr[i].second == arr[j].first) {
                if (arr[i].first != arr[i].second) {
                    return false;
                }
            }
        }
    }
    return true;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
bool is_asymmetric(pair arr[], int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (arr[i].first == arr[j].second && arr[i].second == arr[j].first) {
                return false;
            }
        }
    }
    return true;
}


bool contains_pair(pair arr[], int a, int b, int n)
{
    for (int i = 0; i < n; i++) {
        if (arr[i].first == a && arr[i].second == b) {
            return true;
        }
    }
    return false;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
bool is_transitive(pair arr[], int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (arr[i].second == arr[j].first) {
                for (int k = 0; k < n; k++) {
                    if (i != k && j != k && arr[j].second == arr[k].first) {
                        if (!contains_pair(arr, arr[i].first, arr[k].second, n)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
bool is_partial_order(pair arr[], int n)
{
    if (!is_reflexive(arr, n)) {
        return false;
    }
    if (!is_antisymmetric(arr, n)) {
        return false;
    }
    if (!is_transitive(arr, n)) {
        return false;
    }
    return true;
}


// A total order relation is a partial order relation that is connected
bool is_total_order(pair arr[], int n)
{
    if (is_partial_order(arr, n)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    if (!(arr[i].first == arr[j].first || arr[i].first == arr[j].second || arr[i].second == arr[j].first || arr[i].second == arr[j].second)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

int find_max_elements(pair arr[], int n, int max_elements[])
{
    int max_elements_size = 0;
    bool is_max;

    for (int i = 0; i < n; i++) {
        is_max = true;
        for (int j = 0; j < n; j++) {
            if (arr[i].second == arr[j].first && arr[j].first != arr[j].second) {
                is_max = false;
                break;
            }
        }
        bool is_new = true;
        for (int j = 0; j < max_elements_size; j++) {
            if (arr[i].second == max_elements[j]) {
                is_new = false;
                break;
            }
        }
        if (is_max && is_new) {
            max_elements[max_elements_size] = arr[i].second;
            max_elements_size++;
        }
    }

    return max_elements_size;
}

int find_min_elements(pair arr[], int n, int min_elements[])
{
    int min_elements_size = 0;
    bool is_min;

    for (int i = 0; i < n; i++) {
        is_min = true;
        for (int j = 0; j < n; j++) {
            if (arr[i].first == arr[j].second && arr[j].first != arr[j].second) {
                is_min = false;
                break;
            }
        }
        bool is_new = true;
        for (int j = 0; j < min_elements_size; j++) {
            if (arr[i].first == min_elements[j]) {
                is_new = false;
                break;
            }
        }
        if (is_min && is_new) {
            min_elements[min_elements_size] = arr[i].first;
            min_elements_size++;
        }
    }

    return min_elements_size;
}

int cmpfunc(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

int get_domain(pair arr[], int n, int domain[])
{
    int domain_size = 0;
    for (int i = 0; i < n; i++) {
        bool is_new = true;
        for (int j = 0; j < domain_size; j++) {
            if (arr[i].first == domain[j]) {
                is_new = false;
                break;
            }
        }
        if (is_new) {
            domain[domain_size] = arr[i].first;
            domain_size++;
        }
    }
    for (int i = 0; i < n; i++) {
        bool is_new = true;
        for (int j = 0; j < domain_size; j++) {
            if (arr[i].second == domain[j]) {
                is_new = false;
                break;
            }
        }
        if (is_new) {
            domain[domain_size] = arr[i].second;
            domain_size++;
        }
    }
    qsort(domain, domain_size, sizeof(int), cmpfunc);
    return domain_size;
}

int composition (pair* tab1, int n1, pair* tab2, int n2, pair* com){
    int length = 0;
    for (int i = 0; i < n1; i++){
        for (int j = 0; j < n2; j++){
            if (tab1[i].second == tab2[j].first){

                bool is_new = true;
                for (int k = 0; k < length; k++) {
                    if (com[k].first == tab1[i].first && com[k].second == tab2[j].second) {
                        is_new = false;
                        break;
                    }
                }
                if (is_new) {
                    pair new_pair = {tab1[i].first, tab2[j].second};
                    com[length] = new_pair;
                    length++;
                }
            }

        }
    }
    return length;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n;
    scanf("%d",&n);
    for (int i = 0; i < n; i++){
        int a,b;
        scanf("%d %d", &a, &b);
        relation[i].first = a;
        relation[i].second = b;
    }
    return n;
}



int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			printf("%d\n", n_domain);
            print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
            printf("\n%d\n", no_max_elements);
            print_int_array(max_elements, no_max_elements);
            printf("\n%d\n", no_min_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

