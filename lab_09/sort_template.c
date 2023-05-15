#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stddef.h>
#include <time.h>

typedef struct {
    int day;
    int month;
    int year;
} Date;

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
    char name[20];
    float price;
    int amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const Food *, const Food *);

int cmp_date(const Date *d1, const Date *d2) {
    if(d1->year != d2->year) return d1->year - d2->year;
    if(d1->month != d2->month) return d1->month - d2->month;
    return d1->day - d2->day;
}

int cmp_foods(const Food *a, const Food *b) {
    int name_comp = strcmp(a->name, b->name);
    if (name_comp == 0){
        if (fabsf(a->price - b->price) < 0.0001) return cmp_date(&a->valid_date, &b->valid_date);
        if (a->price > b->price) return 1;
        if (a->price < b->price) return -1;
    }
    return name_comp;
}

void *bsearch2 (const void *key, const void *base0, size_t nmemb, size_t size, ComparFp compar, int *result) {
    const char *base = (const char *) base0;
    int lim, compare_result;
    const void *p;
    for (lim = nmemb; lim != 0; lim>>=1){
        p = base + (lim >> 1) * size;
        compare_result = (*compar)(key, p);

        if (compare_result == 0){
            *result = 1;
            return (void *) p;
        }
        if (compare_result > 0){
            base = (const char *)p + size;
            lim--;
        }
    }
    * result = 0;
    if (compar(key, p) > 0) return (void *) (p + size);
    return (void *) p;
}

void print_art(Food *p, int n, char *art) {
    for(int i = 0; i < n; i++){
        if((strcmp(p[i].name, art)) == 0){
            if (p[i].valid_date.day < 10){
                printf("%.2f %d 0%d.", p[i].price, p[i].amount, p[i].valid_date.day);
            }
            else{
                printf("%.2f %d %d.", p[i].price, p[i].amount, p[i].valid_date.day);
            }
            if (p[i].valid_date.month < 10){
                printf("0%d.%d\n", p[i].valid_date.month, p[i].valid_date.year);
            }
            else{
                printf("%d.%d\n", p[i].valid_date.month, p[i].valid_date.year);
            }
        }
    }
}

void shift_array(Food *tab, ptrdiff_t index, int size){
    for (int i = size - 1; i >= index; i--) {
        tab[i + 1] = tab[i];
    }
}
Food * add_record(Food *tab, int *np, ComparFp compar, Food *new) {
    int result;
    if (*np == 0){
        tab[0] = *new;
        (*np)++;
        return tab;
    }
    Food* pos = bsearch2(new, tab, *np, sizeof(Food), compar, &result);
    ptrdiff_t index = pos - tab;
    if (result == 1){
        tab[index].amount = tab[index].amount + new->amount;
        return pos;
    }
    if (result == 0){
        shift_array(tab, index, *np);
        (*np)++;
        tab[index] = *new;
        return pos;
    }
}

int read_goods(Food *tab, int no, FILE *stream, int sorted) {
    int elements_counter = 0;
    for (int i = 0; i < no; i++) {
        Food new_food;
        fscanf(stream, "%s %f %d %d.%d.%d", new_food.name, &new_food.price, &new_food.amount, &new_food.valid_date.day,
               &new_food.valid_date.month, &new_food.valid_date.year);
        if (sorted == 1) {
            add_record(tab, &elements_counter, cmp_foods, &new_food);
        } else {
            tab[elements_counter] = new_food;
            elements_counter++;
        }
    }
    return elements_counter;
}

int cmp_qs(const void *a, const void *b) {
    Food *fa = (Food*)a, *fb = (Food*)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
    Date *pd = (Date*)a;
    Food *fb = (Food*)b;
    return cmp_date(pd, &fb->valid_date);
}

float value(Food *food_tab, size_t n, Date curr_date, int days) {
    struct tm cdate = {0};
    cdate.tm_year = curr_date.year - 1900;
    cdate.tm_mon = curr_date.month - 1;
    cdate.tm_mday = curr_date.day;
    time_t ctime = mktime(&cdate);

    float val = 0.0;

    for(int i=0; i < n; i++){
        struct tm fdate = {0};
        fdate.tm_year = food_tab[i].valid_date.year - 1900;
        fdate.tm_mon = food_tab[i].valid_date.month - 1;
        fdate.tm_mday = food_tab[i].valid_date.day;
        time_t ftime = mktime(&fdate);

        if(ctime + days *24*60*60 == ftime){
            val+=food_tab[i].amount * food_tab[i].price;
        }
    }

    return val;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
    enum Sex sex:1;
    enum BOOL in_line:1;
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
    if (person1->bits.sex == person2->bits.sex) return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
    return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
    Person *pa = (Person*)a, *pb = (Person*)b;
    if (pa->parent == NULL) return -1;
    if (pb->parent == NULL) return 1;
    int result = strcmp(pa->parent, pb->parent);
    if (result == 0){
        result = cmp_primo(pa, pb);
        if (result == 0) return cmp_date(&pa->born, &pb->born);
        return result;
    }
    return result;
}

Person* find_in_array(Person *pers_tab, int n, char* name){
    for (int i = 0; i < n; i++){
        if (strcmp(pers_tab[i].name, name) == 0){
            return &pers_tab[i];
        }
    }
}

int height_in_tree(Person *pers_tab, int n, Person *person){
    int height = 0;
    while (person->parent != NULL){
        person = find_in_array(pers_tab, n, person->parent);
        height++;
    }
    return height;
}
Person pers_tab[] = {
        {"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
        {"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
        {"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
        {"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
        {"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
        {"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
        {"William", {M,yes}, {21, 6, 1982}, "Charles III"},
        {"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
        {"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
        {"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
        {"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
        {"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
        {"James", {M,yes}, {17, 12, 2007}, "Edward"},
        {"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
        {"Charles", {M,yes}, {1, 7, 1999}, "David"},
        {"Margarita", {F,yes}, {14, 5, 2002}, "David"},
        {"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
        {"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
        {"George", {M,yes}, {22, 7, 2013}, "William"},
        {"George VI", {M,no}, {14, 12, 1895}, NULL},
        {"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
        {"Louis", {M,yes}, {23, 4, 2018}, "William"},
        {"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
        {"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
        {"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
        {"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
        {"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
        {"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
        {"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
        {"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
        {"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
        {"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
        {"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
};
int n = 33;
int cmp_position(const void *a, const void *b){
    //printf("cmp_position %s %s\n", ((Person *)a)->name, ((Person *)b)->name);
    Person *pa = (Person *)a, *pb = (Person *)b;
    int ha = height_in_tree(pers_tab, n, pa);
    int hb = height_in_tree(pers_tab, n, pb);
    if (pa->parent == NULL) return -1;
    if (pb->parent == NULL) return 1;
    while (ha > hb){
        pa = find_in_array(pers_tab, n, pa->parent);
        ha--;
        if (pa == pb) return 1;
    }
    while (hb > ha){
        pb = find_in_array(pers_tab, n, pb->parent);
        hb--;
        if (pa == pb) return -1;
    }
    if (ha == hb) {
        while (strcmp(pa->parent, pb->parent) != 0){
            pa = find_in_array(pers_tab, n, pa->parent);
            pb = find_in_array(pers_tab, n, pb->parent);
        }
        return cmp_person(pa, pb);
    }
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
    for (int i = 0; i <  size; i++)
    {
        idx_tab[i].par_name = pers_tab[i].parent;
        idx_tab[i].index = i;
    }
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {


}

int cleaning(Person *person_tab) {
    int cleaned = 0;
    for (int i = 0; i < n; i++){
        if (person_tab[i].bits.in_line == 0){
            for (int j = i; j < n - 1; j++){
                person_tab[j] = person_tab[j+1];
            }
            i--;
            cleaned++;
        }

    }
    n = n - cleaned;

    return n ;
}

void print_person(const Person *p) {
    printf("%s", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for (int i = 0; i < n; i++)
        print_person(&person_tab[i]);
}

int create_list(Person *person_tab) {
    qsort(person_tab, n, sizeof(Person), cmp_position);
    cleaning(person_tab);
}

int main(void) {


    int to_do, no;
    int n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    fgets(buff, ART_MAX, stdin);
    sscanf(buff, "%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 1);
            scanf("%s",buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // qsort
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 0);
            qsort(food_tab, n, sizeof(Food), cmp_qs);
            Date curr_date;
            int days;
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            scanf("%d", &days);
            printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
            break;
        case 3: // succession
            scanf("%d",&no);
            int no_persons = sizeof(pers_tab) / sizeof(Person);
            create_list(pers_tab);
            print_person(pers_tab + no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}