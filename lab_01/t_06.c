#include <stdio.h>

#define l_1_t_06 main

/*
 * Szyfr Cezara polega na szyfrowaniu kolejnych liter (pozostałe znaki pozostawiamy bez zmian).
 * Każda litera zostaje zamieniona w k-tą następną w alfabecie (k jest stałą szyfru),
 * przy czym jeżeli taka nie istnieje (wychodzimy za 'z'), to odliczanie jest kontynuowane z powrotem od 'a'.
 * Szyfrowanie zachowuje wielkość liter. Napisz funkcję, która szyfruje ciąg znaków podany jako argument.
*/

int encryptWithCaesarCipher(char letter, int key){
    short LETTERS_IN_ALPHABET = 26;
    if (letter <= 90) {
        return (letter + key - 'A') % LETTERS_IN_ALPHABET + 'A';
    }else{
        return (letter + key - 'a') % LETTERS_IN_ALPHABET + 'a';
    }

}

int l_1_t_06() {
    char password[] = "helloWorld";
    char KEY = 1;
    for (int i = 0; i < sizeof(password) - 1; i++) {
        printf("%c", encryptWithCaesarCipher(password[i], KEY));
    }
    return 0;
}
