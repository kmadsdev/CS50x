height = None

while True:
    try:
        height = int(input("Height: "))
    except ValueError:
        continue

    if 1 <= height <= 8:
        break

for i in range(1, height + 1):
    print(" " * (height - i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)
