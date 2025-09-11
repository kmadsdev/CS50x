while True:
    try:
        dollars = float(input('Change owed: '))
    except ValueError:
        continue
    if dollars >= 0:
        break


cents = round(dollars * 100)

qnt = 0

qnt += cents // 25
cents %= 25

qnt += cents // 10
cents %= 10

qnt += cents // 5
cents %= 5

qnt += cents

print(qnt)
