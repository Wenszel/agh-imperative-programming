#include <stdio.h>
#include <stdbool.h>
#define l_1_t_03 main

/*
 * Liczba doskonała jest to taka liczba naturalna,
 * która jest sumą wszystkich swych dzielników właściwych (to znaczy od niej mniejszych).
 * Najmniejszą liczbą doskonałą jest 6, ponieważ jej dzielnikami właściwymi są 1, 2, 3 i 1 + 2 + 3 = 6.
 * Napisz program, który znajduje wszystkie liczby doskonałe w zadanym przedziale oraz ich liczbę.
*/

bool isPerfect(int number){
    int sumOfDivs = 0;
    int div = number - 1;
    while (div > 0){
        if (number % div == 0){
            sumOfDivs += div;
        }
        div--;
    }
    return sumOfDivs == number;
}

int getQuantityOfPerfectNumbersInRange(int start, int end){
    int quantityOfPerfectNumbers = 0;
    for (int n = start; n <= end; n++){
        if (isPerfect(n)){
            printf("%d\n",n);
            quantityOfPerfectNumbers ++;
        }
    }
    return quantityOfPerfectNumbers;
}

int l_1_t_03() {
    int start, end;
    scanf("%d", &start);
    scanf("%d", &end);
    printf("Perfect numbers in given range: \n");
    printf("Quantity: %d\n", getQuantityOfPerfectNumbersInRange(start, end));
    return 0;
}
