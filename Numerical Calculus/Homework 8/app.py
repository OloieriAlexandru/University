import math
import random

import time

NUMBER_OF_ITERATIONS = 100
EPS = 1e-16
DELTA_LIMIT = int(1e10)

def build_fi_1(f):
    def fi(x, h):
        return ( 3 * f(x) - 4 * f(x - h) + f(x - 2 * h) ) / (2 * h)
    return fi

def build_fi_2(f):
    def fi(x, h):
        return ( -f(x + 2*h) + 8 * f(x + h) - 8*f(x-h) + f(x-2*h) ) / (12 * h)
    return fi

def build_fii(f):
    def fii(x, h):
        return ( -f(x + 2*h) + 16*f(x+h) - 30*f(x) + 16*f(x-h) - f(x - 2*h) ) / (12 * (h ** 2))
    return fii

def f1(x):
    return x ** 2 - 4 * x + 3

def f2(x):
    return x ** 2 + math.exp(x)

def f2i(x, *args):
    return 2 * x + math.exp(x)

def f3(x):
    return x ** 4 - 6 * (x ** 3) + 13 * (x ** 2) - 12 * x + 4

f1_info = {
    'fun': f1,
    'fun_str': 'f(x) = x^2 - 4*x + 3'
}

f2_info = {
    'fun': f2,
    'fun_str': 'f(x) = x^2 + e^x',
    'der1': f2i
}

f3_info = {
    'fun': f3,
    'fun_str': 'f(x) = x^4 - 6*(x^3) + 13*(x^2) - 12*x + 4'
}

fs = [f1_info, f2_info, f3_info]

RAND_MIN = -5.0
RAND_MAX = 5.0

class Function:
    def __init__(self, f, fi, fii, h):
        self.f = f
        self.fi = fi
        self.fii = fii
        self.h = h

    def get_critical_point(self):
        x = self.get_random()
        k = 0
        delta = (EPS + DELTA_LIMIT) / 2
        condition = True

        while condition:
            der_x = self.fi(x, self.h)
            denominator = self.fi(x + der_x, self.h) - der_x

            if abs(denominator) < 1e-6:
                return (x, k)
            if abs(denominator) > DELTA_LIMIT:
                return None
            z = x + (der_x ** 2) / denominator
            delta = (der_x * (self.fi(z, self.h) - der_x)) / denominator
            
            x -= delta         
            k += 1
            condition = (k <= NUMBER_OF_ITERATIONS and abs(delta) > EPS and abs(delta) <= DELTA_LIMIT)

        if abs(delta) <= EPS:
            return (x, k) # Convergent
        return None # Divergent

    def get_critical_points(self):
        ITER = 1000
        sol = {}

        for _ in range(ITER):
            p = self.get_critical_point()
            if p is not None:
                p = (round(p[0], 4), p[1])
                if p[0] in sol:
                    sol[p[0]] = min(sol[p[0]], p[1])
                else:
                    sol[p[0]] = p[1]

        res = []
        for key in sol.keys():
            min_p = self.fii(key, self.h) > 0
            res.append((key, sol[key], min_p))
        
        return res

    def get_random(self):
        return RAND_MIN + random.random() * (RAND_MAX - RAND_MIN)

if __name__ == "__main__":
    H = 1e-6
    function_index = 1

    for f_info in fs:
        print('-------------------------')
        print('Function {}\n{}'.format(function_index, f_info['fun_str']))

        f = f_info['fun']
        
        fi = build_fi_1(f)
        fii = build_fii(f)
        fun1 = Function(f, fi, fii, H)
        res1 = fun1.get_critical_points()

        fi = build_fi_2(f)
        fun2 = Function(f, fi, fii, H)
        res2 = fun2.get_critical_points()

        if res1 == []:
            print('Derivative approximation 1 provided no solution')
        else:
            print('Derivative approximation 1 solutions: (solution, steps, min_point)')
            print(str(res1))
        
        if res2 == []:
            print('Derivative approximation 2 provided no solution')
        else:
            print('Derivative approximation 2 solutions: (solution, steps, min_point)')
            print(str(res2))

        if 'der1' in f_info:
            res3 = Function(f, f_info['der1'], fii, H).get_critical_points()
            
            if res3 == []:
                print('Custom derivative provided no solution')
            else:
                print('Custom derivative solutions: (solution, steps, min_point)')
                print(str(res3))

        function_index += 1
