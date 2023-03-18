#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

int cards_A[52];
int cards_B[52];
int curr_A = 0;
int curr_B = 0;
int len_A = 26;
int len_B = 26;

int rand_from_interval(int a, int b) {
    if (a > b){
        return INT_MIN;
    }
    if (b-a > RAND_MAX){
        return INT_MAX;
    }
    if (a == b) {
        return a;
    }
    return rand()%(b - a + 1) + a;
}

void swap (int array[], int i, int k) {
    int temp = array[i];
    array[i] = array[k];
    array[k] = temp;
}

void shuffle_cards(int n, int array[]) {
    for(int i = 0; i < n; i++){
        array[i] = i;
    }
    for(int i = 0; i < n - 1; i++){
        int k = rand_from_interval(i, n - 1);
        swap(array, i, k);
    }
}

void print_card_symbol(int index){
    switch(index/4){
        case 0 ... 8:
            printf("%d ", (index / 4) + 2);
            break;
        case 9:
            printf("J ");
            break;
        case 10:
            printf("Q ");
            break;
        case 11:
            printf("K ");
            break;
        case 12:
            printf("A ");
            break;
    }
}

void print_cards(){
    printf("\nCards of player A: \n");
    for(int i = 0; i < 26; i++) {
        print_card_symbol(cards_A[(curr_A + i) % 52]);
    }
    printf("\n\nCards of player: B \n");
    for(int i = 0; i < 26; i++) {
        print_card_symbol(cards_B[(curr_A + i) % 52]);
    }
    printf("\n\n");
}

void deal_cards(const int cards[]){
    for(int i = 0; i < 26; i++){
        cards_A[i] = cards[i];
    }
    for(int j = 0; j < 26; j++){
        cards_B[j] = cards[26 + j];
    }
}

void insert(int cards[], int value, const int * curr, int * len){
    cards[(*curr + *len) % 52] = value;
    *len = *len + 1;
}

void insert_array(int cards[], int inserted_values[], const int amount_of_values, const int * curr, int * len){
    for (int i = 0; i < amount_of_values; i++) {
        insert(cards, inserted_values[i], curr, len);
    }
}

int pop(const int cards[], int * curr, int * len){
    int output = cards[* curr];
    *len = *len - 1;
    *curr = (*curr + 1)% 52;
    return output;
}

void resolve_conflict(char result){
    switch(result){
        case 'A':
            insert(cards_A, pop(cards_A, &curr_A, &len_A), &curr_A, &len_A);
            insert(cards_A, pop(cards_B, &curr_B, &len_B), &curr_A, &len_A);
            break;
        case 'B':
            insert(cards_B, pop(cards_B, &curr_B, &len_B), &curr_B, &len_B);
            insert(cards_B, pop(cards_A, &curr_A, &len_A), &curr_B, &len_B);
            break;
        case 'T':
            insert(cards_A, pop(cards_A, &curr_A, &len_A), &curr_A, &len_A);
            insert(cards_B, pop(cards_B, &curr_B, &len_B), &curr_B, &len_B);
            break;
        default:
            printf("Player not found! Given player: %c \n", result);
            break;
    };
}

char round_result(){
    if (cards_A[curr_A]/4 > cards_B[curr_B]/4){
        return 'A';
    }else if (cards_A[curr_A]/4 < cards_B[curr_B]/4){
        return 'B';
    }else{
        return 'T';
    }
}

bool are_decks_non_empty(){
    return len_A > 0 && len_B > 0;
}

void add_cards_to_war_stacks(int stack_A[], int stack_B[], int *cards_on_stack){
    stack_A[* cards_on_stack] = pop(cards_A, &curr_A, &len_A);
    stack_A[* cards_on_stack + 1] = pop(cards_A, &curr_A, &len_A);
    stack_B[* cards_on_stack] = pop(cards_B, &curr_B, &len_B);
    stack_B[* cards_on_stack + 1] = pop(cards_B, &curr_B, &len_B);
    * cards_on_stack = * cards_on_stack + 2;
}

void resolve_war(char result, int stack_A[], int stack_B[], int cards_on_stack){
    int * winner_cards = (result == 'A') ? cards_A : cards_B;
    int * loser_cards = (result == 'A') ? cards_B : cards_A;
    int * winner_stack = (result == 'A') ? stack_A : stack_B;
    int * loser_stack = (result == 'A') ? stack_B : stack_A;
    int * winner_curr = (result == 'A') ? &curr_A : &curr_B;
    int * loser_curr = (result == 'A') ? &curr_B : &curr_A;
    int * winner_len = (result == 'A') ? &len_A : &len_B;
    int * loser_len = (result == 'A') ? &len_B : &len_A;
    insert_array(winner_cards, winner_stack, cards_on_stack,winner_curr , winner_len);
    insert(winner_cards, pop(winner_cards, winner_curr, winner_len), winner_curr, winner_len);
    insert_array(winner_cards, loser_stack, cards_on_stack, winner_curr, winner_len);
    insert(winner_cards, pop(loser_cards, loser_curr, loser_len), winner_curr, winner_len);
}

void start_war(int * conflicts_counter){
    int stack_A[26];
    int stack_B[26];
    int cards_on_stack = 0;

    while (round_result() == 'T') {
        if (len_B <= 1 || len_A <= 1){
            len_A -= 2;
            len_B -= 2;
            return;
        }
        add_cards_to_war_stacks(stack_A, stack_B, &cards_on_stack);
        * conflicts_counter = * conflicts_counter + 1;
    }
    resolve_war(round_result(), stack_A, stack_B, cards_on_stack);
}

char get_winner(){
    if(len_A == len_B || (len_A > 0 && len_B > 0)){
        return 'T';
    }
    return (len_A > len_B) ? 'A' : 'B';
}

void start_game(int max_conflicts, int type_of_game) {
    int conflicts_counter = 0;
    while (are_decks_non_empty() && conflicts_counter < max_conflicts) {
        conflicts_counter++;
        switch (round_result()) {
            case 'A':
                resolve_conflict('A');
                break;
            case 'B':
                resolve_conflict('B');
                break;
            case 'T':
                if (type_of_game == 1) {
                    resolve_conflict('T');
                } else {
                    start_war(&conflicts_counter);
                }
                break;
        }
    }
    char winner = get_winner();
    if (winner == 'T'){
        printf("Game undecided in given number of conflicts");
    }else{
        printf("The winner is: player %c\n", winner);
        printf("The number of conflicts needed to complete the game: %d", conflicts_counter);
    }
}

int main(void) {
    int type_of_game, seed;
    int max_conflicts;
    int cards[52];

    printf("Enter srand seed: ");
    scanf("%d", &seed);
    printf("Game modes: \n (0) - \"full\" with wars \n (1) - \"simplified\" without wars");
    printf("\nEnter game mode number: ");
    scanf("%d", &type_of_game);
    printf("Enter the maximum number of conflicts: ");
    scanf("%d", &max_conflicts);

    srand((unsigned) seed);

    shuffle_cards(52, cards);
    deal_cards(cards);
    print_cards();
    start_game(max_conflicts, type_of_game);

    return 0;
}
