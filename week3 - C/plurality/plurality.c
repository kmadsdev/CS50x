#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define max 9

typedef struct {
    string name;
    int votes;
} candidate;

// Variables
candidate candidates[max];
int candidate_count;

// Functions
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[]) {
    candidate_count = argc-1;

    if (argc < 2) {
        printf("Usage: ./plurality [candidate ...]\n");
        return 1;
    }

    for (int i = 0; i < candidate_count; i++) {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    for (int i = 0; i < voter_count; i++) {
        string name = get_string("Vote: ");
        if (!vote(name)) {
            printf("Invalid vote.\n");
        }
    }

    print_winner();
}

bool vote(string name) {
    for (int i = 0; i < candidate_count; i++) {
        if (strcmp(candidates[i].name, name) == 0) {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

void print_winner(void) {
    int max_votes = 0;
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes > max_votes) {
            max_votes = candidates[i].votes;
        }
    }
    for (int i = 0; i < candidate_count; i++) {
        if (candidates[i].votes == max_votes) {
            printf("%s\n", candidates[i].name);
        }
    }
}
