#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

double calc_L(const char *text);
double calc_S(const char *text);

int main(void){
    string text = get_string("Text: ");
    double L = calc_L(text);
    double S = calc_S(text);
    double grade = (0.0588 * L) - (0.296 * S) - 15.8;

    if (grade < 0) {
        printf("Before Grade 1\n");
    } else if (grade > 16) {
        printf("Grade 16+\n");
    } else {
        printf("Grade %i\n", (int) round(grade));
    }
}

double calc_L(const char *text) {
    int letter_count = 0;
    int word_count = 0;
    int curr_word = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            letter_count++;
        }
        if (!isspace(text[i]) && !curr_word) {
            curr_word = 1;
            word_count++;
        }
        if (isspace(text[i])) {
            curr_word = 0;
        }
    }

    if (word_count == 0) return 0.0;
    return ((double) letter_count / word_count) * 100.0;
}

double calc_S(const char *text) {
    int sentence_count = 0;
    int word_count = 0;
    int curr_word = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (!isspace(text[i]) && !curr_word) {
            curr_word = 1;
            word_count++;
        }
        if (isspace(text[i])) {
            curr_word = 0;
        }
        if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
            sentence_count++;
        }
    }

    if (word_count == 0) return 0.0;
    return ((double) sentence_count / word_count) * 100.0;
}
