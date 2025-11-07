import math

def calc_L(text: str) -> float:
    letter_count = 0
    word_count = 0
    curr_word = False


    for ch in text:
        if ch.isalpha():
            letter_count += 1
        if not ch.isspace() and not curr_word:
            curr_word = True
            word_count += 1
        if ch.isspace():
            curr_word = False


    if word_count == 0:
        return 0.0
    return (letter_count / word_count) * 100.0


def calc_S(text: str) -> float:
    sentence_count = 0
    word_count = 0
    curr_word = False


    for c in text:
        if not c.isspace() and not curr_word:
            curr_word = True
            word_count += 1
        if c.isspace():
            curr_word = False
        if c in '.!?':
            sentence_count += 1


    if word_count == 0:
        return 0.0
    return (sentence_count / word_count) * 100.0



text = input('Text: ')
L = calc_L(text)
S = calc_S(text)
grade = (0.0588 * L) - (0.296 * S) - 15.8

if grade < 0:
    print('Before Grade 1')
elif grade > 16:
    print('Grade 16+')
else:
    print(f'Grade {round(grade)}')
