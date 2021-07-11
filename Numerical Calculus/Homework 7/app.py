import numpy as np

FILE_OUT_NAME = 'ignored/roots.txt'
NUMBER_OF_ITERATIONS = 10000
EPS = 1e-16
DELTA_LIMIT = int(1e8)

def f1(x):
    return (x-1)*(x-2)*(x-3)

def f2(x):
    return (x-2/3)*(x-1/7)*(x+1)*(x-3/2)

def f3(x):
    return (x-1)*(x-1/2)*(x-3)*(x-1/4)

def f4(x):
    return (x-1)*(x-1)*(x-2)*(x-2)

f1_info = {
    'fun': f1,
    'fun_str': 'f(x) = (x-1)*(x-2)*(x-3)',
    'coeff': [1.0, -6.0, 11.0, -6.0]
}

f2_info = {
    'fun': f2,
    'fun_str': 'f(x) = (x-2/3)*(x-1/7)*(x+1)*(x-3/2)',
    'coeff': [42.0, -55.0, -42.0, 49.0, -6.0]
}

f3_info = {
    'fun': f3,
    'fun_str': 'f(x) = (x-1)*(x-1/2)*(x-3)*(x-1/4)',
    'coeff': [8.0, -38.0, 49.0, -22.0, 3.0]
}

f4_info = {
    'fun': f4,
    'fun_str': 'f(x) = (x-1)*(x-1)*(x-2)*(x-2)',
    'coeff': [1.0, -6.0, 13.0, -12.0, 4.0]
}

fs = [f1_info, f2_info, f3_info, f4_info]

class Polynom:
    def __init__(self, f_info):
        self.info = f_info
        self.n = len(self.info['coeff'])-1
        
        self.derivative_1_coeff = self.compute_derivative_coeff(f_info['coeff'])
        self.derivative_2_coeff = self.compute_derivative_coeff(self.derivative_1_coeff)

        A = max(list(map(lambda x: abs(x), f_info['coeff'])))
        a0 = abs(f_info['coeff'][0])
        self.r = (a0 + A) / a0

    def olver(self, x):
        condition = True
        k = 0
        delta = 0

        while condition:
            delta = self.compute_delta(x)
            if delta == None:
                return None
            x -= delta
            k += 1
            condition = (k < NUMBER_OF_ITERATIONS and abs(delta) > EPS and abs(delta) <= DELTA_LIMIT)

        if abs(delta) <= EPS:
            return x # Convergent
        return None # Div.

    def compute_delta(self, x):
        p_x = self.pol_value_horner(self.info['coeff'], x)
        p_x_1 = self.pol_value_horner(self.derivative_1_coeff, x)
        p_x_2 = self.pol_value_horner(self.derivative_2_coeff, x)

        ck = ((p_x ** 2) * p_x_2) / (p_x_1 ** 3)
        return (p_x / p_x_1) + (1 / 2) * ck

    def pol_value_horner(self, coeff, x):
        res = coeff[0]
        for i in range(1, len(coeff)):
            res = coeff[i] + res * x
        return res

    def compute_derivative_coeff(self, coeff):
        if len(coeff) == 0:
            return []
        elif len(coeff) == 1:
            return [0]

        derivative_coeff = []
        x_pow = len(coeff)
        for i in range(len(coeff) - 1):
            derivative_coeff.append((x_pow - 1) * coeff[i])
            x_pow -= 1

        return derivative_coeff

    def pol_value_function(self, x):
        return self.info['fun'](x)

def print_poly_from_coeff(coeff):
    x_pow = len(coeff)
    print('Polynomial: ', end='')
    for i in range(len(coeff) - 1):
        print('{} * x^{} + '.format(coeff[i], x_pow - 1), end = '')
        x_pow -= 1
    print(coeff[-1])

if __name__ == "__main__":
    NUMBER_OF_POINTS = 100
    PRECISION = 5
    fun_roots = []
    n = len(fs)

    function_index = 1
    for fun_info in fs:
        roots_set = set()
        pol = Polynom(fun_info)

        delta = (2 * pol.r) / NUMBER_OF_POINTS
        x0 = -pol.r
        while x0 <= pol.r:
            x0_sol = pol.olver(x0)
            if x0_sol != None:
                x0_sol = round(x0_sol, PRECISION)
                roots_set.add(x0_sol)
            x0 += delta

        print('-------------------')
        print('Function {}'.format(function_index))
        print_poly_from_coeff(pol.info['coeff'])
        print_poly_from_coeff(pol.derivative_1_coeff)
        print_poly_from_coeff(pol.derivative_2_coeff)
        print('Computed roots:')
        olo_sol = list(roots_set)
        olo_sol.sort()
        print(str(olo_sol))
        print('numpy roots:')
        numpy_sol = list(set(map(lambda x: round(x, PRECISION), np.roots(fun_info['coeff']))))
        numpy_sol.sort()
        print(str(numpy_sol))
        fun_roots.append(list(roots_set))
        function_index += 1

    with open(FILE_OUT_NAME, 'w') as file_out:
        for i in range(len(fun_roots)):
            file_out.write('Function {}\n{}\nRoots: {}\n\n'.format(
                (i+1), fs[i]['fun_str'], str(fun_roots[i])) )
