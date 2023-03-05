#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define l_1_t_07 main

/*
 * Znajdź pole powierzchni ograniczone osią OX i wykresem funkcji sin(x) w przedziale [a,b] metodą Monte Carlo.
 * Dane wejściowe:
 * a, b, N(liczba losowanych punktów)
*/

int l_1_t_07() {
    srand(time(NULL));
    double a = 0;
    double b = M_PI*10;
    int n = 10000000;
    int points = 0;
    for (int i = 0; i < n; i++) {
        float x = (b-a) * fabs(sin(rand())) + a;
        float y = fabs(sin(rand()));
        if (y <= fabs(sin(x))){
            points++;
        }
    }
    printf("%f\n", (b-a) * points/n);
    return 0;
}
