#include <stdio.h>
#define l_1_t_01 main

/*
 * Napisz program, który wczytuje liczbę całkowitą N, a następnie oblicza i drukuje na ekran wartość N!.
 * Uwaga: silnia bardzo szybko powoduje przepełnienie całkowitoliczbowe.
 * Sprawdź dla jakich wartości N program wyprowadza prawidłowy wynik.
 * Przetestuj działanie programu dla różnych typów danych (dla ułatwienia można zastosować definicję typedef).
*/

unsigned long factorial(int n){
    int output = 1;
    for (int i = 1; i <= n; i++){
        output = output * i;
    }
    return output;
}

int l_1_t_01() {
    int n;
    scanf("%d", &n);
    printf("%lu", factorial(n));
    return 0;
}
