#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define l_1_t_04 main

/*
 * Dana jest liczba całkowita dodatnia n.
 * Napisz program, który znajduje wszystkie liczby pierwsze mniejsze od n, których cyfry tworzą ciąg niemalejący.
*/

bool isPrime(int number){
    if(number == 1){
        return false;
    }
    int i = 2;
    while (i * i < number){
        if (number % i == 0){
            return false;
        }
        i++;
    }
    return true;
}

bool isDigitsNotIncreasing(int number){
    while (floor(number/10) > 0) {
        int lastDigit = number % 10;
        number = floor(number / 10);
        if (lastDigit <= number % 10) {
            return false;
        }

    }
    return true;
}

void printPrimeNumbersWithNonIncreasingDigitsSmallerThan(int number){
    bool prime[number + 1];
    memset(prime, true, sizeof(prime));
    for(int i = 1; i * i <= number; i++) {
        if (isPrime(i)) {
            int multiplayer = 2;
            while (i * multiplayer <= number) {
                prime[i * multiplayer] = false;
                multiplayer++;
            }
        }
    }
    for(int j = 1; j < sizeof(prime); j++){
        if(prime[j] && isDigitsNotIncreasing(j)){
            printf("%d\n", j);
        }
    }
}

int l_1_t_04() {
    int number = 100;
    printPrimeNumbersWithNonIncreasingDigitsSmallerThan(number);
    return 0;
}
