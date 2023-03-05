#include <stdio.h>
#include <stdbool.h>
#define l_1_t_02 main

/*
 * Napisz program, który dla zadanej liczby naturalnej n
 * odpowiada na pytanie, czy liczba ta jest iloczynem dowolnych dwóch kolejnych wyrazów ciągu Fibonacciego.
 * Zakładamy, że pierwsze dwa wyrazy ciągu Fibonacciego to 0 i 1.
*/

bool isProductOfTwoConsecutiveElementsInFibbonaciSequence(int n){
    int a, b, h;
    a = 0;
    b = 1;
    while (n > a * b) {
        h = a;
        a = b;
        b = h + b;
    };
    return n == a * b;
}

int l_1_t_02() {
    int n;
    scanf("%d", &n);
    printf("%d", isProductOfTwoConsecutiveElementsInFibbonaciSequence(n));
    return 0;
}
