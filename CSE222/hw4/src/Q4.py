def foo(integer1, integer2):
    if (integer1 < 10) or (integer2 < 10):
        return integer1 * integer2

    n = max(number_of_digits(integer1), number_of_digits(integer2))
    half = int(n / 2)

    # first integer = integer / 2^half
    # second integer = integer % 2^half

    int1, int2 = split_integer(integer1, half)
    int3, int4 = split_integer(integer2, half)
    sub0 = foo(int2, int4)
    sub1 = foo((int2 + int1), (int4 + int3))
    sub2 = foo(int1, int3)
    return (sub2 * 10 ^ (2 * half)) + ((sub1 - sub2 - sub0) * 10 ^ (half)) + (sub0)


def split_integer(integer1, half):
    return integer1 // pow(2, half), integer1 % pow(2, half)


def number_of_digits(integer1):
    digit = 1
    if integer1 < 0:
        integer1 *= -1

    if integer1 >= 0 and integer1 < 10:
        return 1

    while (1):
        integer1 //= 10
        if integer1 != 0:
            digit += 1
        else:
            break
    return digit


# print(number_of_digits())
print("Integer 1 : 15 and Integer 2 : 45632")
print(foo(15, 45632))
