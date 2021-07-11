import numpy as np
import common


def solve(equations):
    m1, m2 = common.build_matrices(equations)

    np_m1 = np.array(m1)
    det = np.linalg.det(np_m1)

    if det == 0:
        raise Exception("The determinant of the matrix is 0")

    np_m1_inv = np.linalg.inv(np_m1)
    result_matrix = np.matmul(np_m1_inv, np.array(m2))

    return (common.normalize(result_matrix[0][0]), common.normalize(result_matrix[1][0]), common.normalize(result_matrix[2][0]))


if __name__ == '__main__':
    equations = common.read_equations_from_file('equations.txt')
    print(solve(equations[:3]))
