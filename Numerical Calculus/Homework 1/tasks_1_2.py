import random

EPS = 1e-20

def eq(a, b):
    return abs(a-b) < EPS

def task_1():
    u = 1.0
    while True:
        if eq(1.0 + u, 1.0):
            return u * 10.0
        u /= 10.0

def task_2_1():
    a = 1.0
    b = task_1() / 10.0
    c = task_1() / 10.0
    return not eq(((a+b)+c), (a+(b+c)))

def gen_rand_value(min_value, max_value):
    return min_value + random.random() * (max_value-min_value)

def task_2_2():
    while True:
        a = gen_rand_value(1, 10)
        b = gen_rand_value(1, 10)
        c = gen_rand_value(1, 10)

        prod_1 = (a*b)*c
        prod_2 = a*(b*c)

        if not eq(prod_1, prod_2):
            break

    print('      a = {:.30f}'.format(a))
    print('      b = {:.30f}'.format(b))
    print('      c = {:.30f}'.format(c))
    print('(a*b)*c = {:.30f}'.format(prod_1))
    print('a*(b*c) = {:.30f}'.format(prod_2))

if __name__ == '__main__':
    print('--------------------------------------')
    print("Machine precision: {}".format(task_1()))
    print('--------------------------------------')
    print("((a+b)+c) != (a+(b+c)): {}".format(task_2_1()))
    print('--------------------------------------')
    task_2_2()
