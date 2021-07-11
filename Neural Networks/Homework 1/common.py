
def is_int(s):
    if len(s) == 0:
        return False
    if s[0] in ('-', '+'):
        return s[1:].isdigit()
    return s.isdigit()


def check_int_message(s, message):
    if not is_int(s):
        raise Exception(message)


def get_argument_coefficient(s, coefficient):
    if coefficient not in s:
        return (0, s)

    parts = s.split(coefficient)
    if len(parts[0]) == 0:  # x
        return (1, parts[1] if len(parts) > 1 else '')
    elif len(parts[0]) == 1 and parts[0][0] in ('-', '+'):  # (+/-)x
        return (int(parts[0][0] + '1'), parts[1] if len(parts) > 1 else '')
    elif parts[0].startswith(coefficient):
        return (1, parts[1] if len(parts) > 1 else '')

    try:
        check_int_message(
            parts[0], 'The coefficient for {0} variable is not an integer!'.format(coefficient))
    except:
        raise

    return (int(parts[0]), parts[1] if len(parts) > 1 else '')  # (integer)x


def parse_equation(eq):
    eq = eq.replace(' ', '')
    parts = eq.split('=')

    if len(parts) <= 1:
        raise Exception('Missing right part of the equation')

    try:
        check_int_message(
            parts[1], 'The result of the equation is not an integer!')
        r = int(parts[1])

        parsing_result = get_argument_coefficient(parts[0], 'x')
        a = int(parsing_result[0])

        parsing_result = get_argument_coefficient(parsing_result[1], 'y')
        b = int(parsing_result[0])

        parsing_result = get_argument_coefficient(parsing_result[1], 'z')
        c = int(parsing_result[0])
    except:
        raise

    return (a, b, c, r)


def read_equations_from_file(file_name):
    with open(file_name, 'r') as file:
        equations = file.readlines()
        return [parse_equation(equation.strip()) for equation in list(filter(lambda el: len(el.strip()) > 0, equations))]


def build_matrices(equations, validate=False, expected_equations_count=3):
    if len(equations) != expected_equations_count:
        raise Exception('Expected {0} equations, found {1}'.format(
            expected_equations_count, len(equations)))

    m1 = []
    m2 = []
    for equation in equations:
        m1.append([equation[0], equation[1], equation[2]])
        m2.append([equation[3]])

    return (m1, m2)


def normalize(value):
    return int(round(value))


if __name__ == "__main__":
    print(read_equations_from_file('equations.txt'))
