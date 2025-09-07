#include <cs50.h>
#include <stdio.h>

int main() {
    int cents;

    do {
        cents = get_int("Change owed: ");
    }
    while (cents < 0);

    int qnt = 0;

    qnt += cents / 25;
    cents %= 25;

    qnt += cents / 10;
    cents %= 10;

    qnt += cents / 5;
    cents %= 5;

    qnt += cents;

    printf("%i\n", qnt);
}
