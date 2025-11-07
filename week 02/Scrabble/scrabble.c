#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    int points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    while (true) {
        string p1_word = get_string("Player 1: ");
        string p2_word = get_string("Player 2: ");

        int p1_score = 0;
        int p2_score = 0;

        int pos;

        for (int i = 0; p1_word[i] != '\0'; i++) {
            pos = (toupper((char) p1_word[i]) - 65);
            if (0 <= pos && pos < 26) {
                p1_score += points[pos];
            }
        }

        for (int i = 0; p2_word[i] != '\0'; i++) {
            pos = (toupper((char) p2_word[i]) - 65);
            if (0 <= pos && pos < 26) {
                p2_score += points[pos];
            }
        }

        if (p1_score == p2_score) {
            printf("Tie!\n");
        } else {
            printf("Player %i Wins!\n", p1_score > p2_score ? 1 : 2);
        }
        break;
    }
}
