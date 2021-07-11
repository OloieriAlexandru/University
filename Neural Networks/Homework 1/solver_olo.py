import common


def compute_matrix_determinant(matrix):
    result = matrix[0][0] * matrix[1][1] * matrix[2][2]
    result += matrix[1][0] * matrix[2][1] * matrix[0][2]
    result += matrix[2][0] * matrix[0][1] * matrix[1][2]
    result -= matrix[0][2] * matrix[1][1] * matrix[2][0]
    result -= matrix[1][2] * matrix[2][1] * matrix[0][0]
    result -= matrix[2][2] * matrix[0][1] * matrix[1][0]

    return result


def build_matrix_transpose(matrix):
    result = []
    n = len(matrix)
    m = len(matrix[0])

    for j in range(m):
        line = []
        for i in range(n):
            line.append(matrix[i][j])
        result.append(line)

    return result


def compute_2_by_2_matrix_determinant(matrix):
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]


def build_2_by_2_matrix(matrix, ignored_line, ignored_column):
    result = [[0, 0], [0, 0]]
    line = 0
    column = 0

    for i in range(3):
        for j in range(3):
            if i == ignored_line or j == ignored_column:
                continue

            result[line][column] = matrix[i][j]

            column += 1
            if column == 2:
                line += 1
                column = 0

    return result


def build_matrix_of_minors(matrix):
    result = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]

    for i in range(3):
        for j in range(3):
            result[i][j] = compute_2_by_2_matrix_determinant(
                build_2_by_2_matrix(matrix, i, j))

    return result


def build_matrix_inverse(matrix):
    det = compute_matrix_determinant(matrix)
    if det == 0:
        raise Exception("The determinant of the matrix is 0")

    tr = build_matrix_transpose(matrix)
    minors_matrix = build_matrix_of_minors(tr)

    for i in range(3):
        for j in range(3):
            if (i + j) % 2 == 1:
                minors_matrix[i][j] *= -1
            minors_matrix[i][j] /= det

    return minors_matrix


def multiply_matrices(mat1, mat2):
    n1 = len(mat1)
    m1 = len(mat1[0])

    m2 = len(mat2[0])

    res = []
    for i in range(n1):
        line = []
        for j in range(m2):
            line.append(0)
        res.append(line)

    for i in range(n1):
        for j in range(m2):
            for k in range(m1):
                res[i][j] += mat1[i][k] * mat2[k][j]

    return res


def solve(equations):
    m1, m2 = common.build_matrices(equations)

    m1_inv = build_matrix_inverse(m1)
    result_matrix = multiply_matrices(m1_inv, m2)

    return (common.normalize(result_matrix[0][0]), common.normalize(result_matrix[1][0]), common.normalize(result_matrix[2][0]))


if __name__ == '__main__':
    equations = common.read_equations_from_file('equations.txt')
    print(solve(equations[:3]))
