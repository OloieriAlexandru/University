
import solver_numpy
import solver_olo
import common


def check_equality(solution_1, solution_2):
    if len(solution_1) != len(solution_2):
        return False

    for i in range(len(solution_1)):
        if solution_1[i] != solution_2[i]:
            return False

    return True


def print_equation_from_coefficients(coeff):
    print(
        '({0})x + ({1})y + ({2})z = {3}'.format(coeff[0], coeff[1], coeff[2], coeff[3]))


if __name__ == "__main__":
    equations = common.read_equations_from_file('equations.txt')

    for i in range(0, len(equations), 3):
        if i + 2 >= len(equations):
            break

        current_equations = equations[i:i+3]

        print('---------------------------------------------')
        print_equation_from_coefficients(current_equations[0])
        print_equation_from_coefficients(current_equations[1])
        print_equation_from_coefficients(current_equations[2])

        solution_numpy = solver_numpy.solve(current_equations)
        solution_olo = solver_olo.solve(current_equations)

        print('Numpy solution: ', end='')
        print(solution_numpy)

        print('Olo solution: ', end='')
        print(solution_olo)

        if check_equality(solution_olo, solution_numpy):
            print('The solutions are the same!')
        else:
            print('The solutions are not the same!')
