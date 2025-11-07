#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string encrypt(int key, string plain_text) {
    int length = strlen(plain_text);
    string encripted_text = malloc(length + 1);

    for (int i = 0; i < length; i++) {
        if (isupper(plain_text[i])) {
            encripted_text[i] = ((plain_text[i] - 'A' + key) % 26) + 'A';
        } else if (islower(plain_text[i])) {
            encripted_text[i] = ((plain_text[i] - 'a' + key) % 26) + 'a';
        } else {
            encripted_text[i] = plain_text[i];
        }
    }

    encripted_text[length] = '\0';
    return encripted_text;
}

int main(int argc, string argv[]) {
    while (true) {
        if (argc != 2) {
            printf("Usage: ./caesar key\n");
            return 1;
        }
        for (int i = 0; argv[1][i] != '\0'; i++) {
            if (!isdigit(argv[1][i])) {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
        printf("ciphertext: %s\n", encrypt(atoi(argv[1]), get_string("plaintext: ")));
        break;
    }
}
