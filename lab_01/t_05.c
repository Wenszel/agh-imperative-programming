#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define l_1_t_05 main

/*
 * Program losuje liczbę 0 <= X <= 100,Napisz funkcję, która zgaduje wartość X.
 * W pętli losujemy 0 <= N <= 100.
 * Jeżeli X=N, zgadliśmy X,
 * jeżeli nie to ograniczamy przedział do X, N z którego losujemy kolejne N
*/

int generateNumber(int lower, int upper){
    return rand()%(upper - lower + 1) + lower;
}

int l_1_t_05() {
    srand(time(NULL));
    int drawnNumber = generateNumber(0, 100);
    int guess = generateNumber(0, 100);
    do {
        printf("N = %d\n", guess);
        if (guess > drawnNumber){
            guess = generateNumber(drawnNumber, guess);

        }else if(guess < drawnNumber) {
            guess = generateNumber(guess, drawnNumber);
        }
    }while(guess!=drawnNumber);
    printf("N = %d\n", guess);
    printf("X is %d", drawnNumber);
    return 0;
}
