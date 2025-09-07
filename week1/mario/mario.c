#include <stdio.h>
#include <cs50.h>

int main() {
    int height = 0;
    while (true) {
        height = get_int("Heigh: ");
        if (1 <= height && height <= 8) {
            break;
        }
    }

    for (int i = 1; i < height+1 ; i++) {
        for (int _1 = 0; _1 < height - i; _1++) {
            printf(" ");
        }
        for (int _2 = 0; _2 < i; _2++) {
            printf("#");
        }

        printf("  ");

        for (int _3 = 0; _3 < i; _3++) {
            printf("#");
        }

        printf("\n");
    }
}
