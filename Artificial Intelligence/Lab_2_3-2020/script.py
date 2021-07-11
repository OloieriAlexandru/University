import copy
import queue
import heapq
import random
import math

MAZE_FILE_PATH = "./ignored/maze.txt"

MOVE_LINE = [-1, 0, 1, 0]
MOVE_COLUMN = [0, 1, 0, -1]
MOVE_SIZE = len(MOVE_LINE)

maze_matrix = None
num_lines = -1
num_lines = -1

visited = None
ans_BKT = None


class Problem:
    def __init__(self, maze, xs, ys, xd, yd):
        self.maze = maze
        self.xs = xs
        self.ys = ys
        self.xd = xd
        self.yd = yd


class State:
    def __init__(self, x, y, final_x, final_y):
        self.x = x
        self.y = y
        self.final_x = final_x
        self.final_y = final_y
        self.moves = [(self.x, self.y)]

    def __lt__(self, other):
        return (self.x + self.y) < (self.final_x - self.final_y)

    def remove_last(self):
        self.moves.pop()


def read_maze(path):
    with open(path, "r") as file_in:
        maze = []
        line = file_in.readline()

        while line:
            line = line.replace('[', '')
            line = line.replace(',', '')
            line = line.replace(']', '')
            maze.append(list(map(lambda x: int(x), line.split(' '))))
            line = file_in.readline()

        num_lines = len(maze)
        if num_lines == 0:
            print("The matrix has to have at least one line!")
            exit(1)
        num_columns = len(maze[0])
        if num_columns == 0:
            print("The matrix has to have at least one column!")
            exit(1)
        for line in maze:
            if len(line) != num_columns:
                print("All the lines should have the same number of columns!")
                exit(1)

        return (maze, num_lines, num_columns)


def get_initial_state(problem):
    return State(problem.xs, problem.ys, problem.xd, problem.yd)


def get_final_state(problem):
    return State(problem.xd, problem.yd, problem.xd, problem.yd)


def is_final_state(state):
    return state.x == state.final_x and state.y == state.final_y


def is_valid_tranzition(state, tranzition):
    if not (tranzition >= 0 and tranzition < MOVE_SIZE):
        return False
    new_x = state.x + MOVE_LINE[tranzition]
    new_y = state.y + MOVE_COLUMN[tranzition]
    if not (new_x >= 0 and new_x < num_lines and new_y >= 0 and new_y < num_columns):
        return False
    if maze_matrix[new_x][new_y] == 1:
        return False
    if visited[new_x][new_y] == True:
        return False
    return True


def apply_tranzition(state, tranzition):
    global visited
    new_state = copy.deepcopy(state)
    new_state.x += MOVE_LINE[tranzition]
    new_state.y += MOVE_COLUMN[tranzition]
    new_state.moves.append((new_state.x, new_state.y))
    visited[new_state.x][new_state.y] = True
    return new_state


def manhattan_distance(state_1, state_2):
    return abs(state_1.x - state_2.x) + abs(state_1.y - state_2.y)


def bfs(problem, maze, n, m):
    global visited
    visited = [[False for i in range(num_columns)] for j in range(num_lines)]
    initial_state = get_initial_state(problem)
    visited[initial_state.x][initial_state.y] = True
    q = queue.Queue()
    q.put(initial_state)
    answer = None
    while not q.empty():
        el = q.get()
        for i in range(MOVE_SIZE):
            if is_valid_tranzition(el, i):
                new_state = apply_tranzition(el, i)
                if is_final_state(new_state):
                    answer = new_state
                else:
                    q.put(new_state)
        if answer != None:
            break
    if answer != None:
        print("BFS: ", answer.moves)
    else:
        print("BFS: No answer")


def bkt(problem, maze, n, m):
    global visited
    global ans_BKT
    visited = [[False for i in range(num_columns)] for j in range(num_lines)]
    initial_state = get_initial_state(problem)
    visited[initial_state.x][initial_state.y] = True
    recursive_bkt(current_state=initial_state)
    if ans_BKT != None:
        print("BKT: ", ans_BKT.moves)
    else:
        print("BKT: No answer")


def recursive_bkt(current_state):
    global visited
    global ans_BKT
    if is_final_state(current_state):
        ans_BKT = current_state
        return
    visited[current_state.x][current_state.y] = True
    for i in range(MOVE_SIZE):
        if is_valid_tranzition(current_state, i):
            recursive_bkt(apply_tranzition(current_state, i))
            visited[current_state.x][current_state.y] = False
    current_state.remove_last()

def hill_climbing_heap(problem, maze, n, m):
    global visited
    visited = [[False for i in range(num_columns)] for j in range(num_lines)]
    initial_state = get_initial_state(problem)
    final_state = get_final_state(problem)
    visited[initial_state.x][initial_state.y] = True

    pq = []
    pq.append((0, initial_state))
    answer = None

    while len(pq) > 0:
        pq_el = heapq.heappop(pq)
        current_state = pq_el[1]
        for i in range(MOVE_SIZE):
            if is_valid_tranzition(current_state, i):
                new_state = apply_tranzition(current_state, i)
                if is_final_state(new_state):
                    answer = new_state
                else:
                    distance = manhattan_distance(new_state, final_state)
                    heapq.heappush(pq, (distance, new_state))
        if answer != None:
            break
    if answer is None:
        print("The Hill climbing algorithm couldn't find a path!")
    else:
        print("HC_heap:   ", end='')
        print(answer.moves)


def hill_climbing(problem, maze, n, m):
    global visited
    visited = [[False for i in range(num_columns)] for j in range(num_lines)]
    current_state = get_initial_state(problem)
    final_state = get_final_state(problem)
    visited[current_state.x][current_state.y] = True

    answer = None

    while True:
        min_dist = None
        next_state = None
        order_neighbors = list(range(MOVE_SIZE))
        random.shuffle(order_neighbors)
        for i in order_neighbors:
            if is_valid_tranzition(current_state, i):
                new_state = apply_tranzition(current_state, i)
                if is_final_state(new_state):
                    answer = new_state
                else:
                    distance = manhattan_distance(new_state, final_state)
                    if min_dist == None:
                        min_dist = distance
                        next_state = new_state
                    else:
                        if distance < min_dist:
                            min_dist = distance
                            next_state = new_state
                
        if answer != None:
            break
        if next_state != None:
            current_state = next_state
        else:
            break
    
    if answer is None:
        print("HC: No answer")
    else:
        print("HC:   ", end='')
        print(answer.moves)


def simulated_annealing(problem, maze, n, m):
    global visited
    visited = [[False for i in range(num_columns)] for j in range(num_lines)]
    current_state = get_initial_state(problem)
    final_state = get_final_state(problem)
    visited[current_state.x][current_state.y] = True
    answer = None

    initial_temp = 5
    final_temp = 0.1
    alpha_temp = 0.1

    current_temp = initial_temp

    while current_temp > final_temp:
        min_dist = None
        next_state = None
        order_neighbors = list(range(MOVE_SIZE))
        random.shuffle(order_neighbors)
        for i in order_neighbors:
            if is_valid_tranzition(current_state, i):
                new_state = apply_tranzition(current_state, i)
                if is_final_state(new_state):
                    answer = new_state
                else:
                    distance = manhattan_distance(new_state, final_state)
                    if min_dist == None:
                        min_dist = distance
                        next_state = new_state
                    else:
                        if distance < min_dist:
                            min_dist = distance
                            next_state = new_state
                        else:
                            uniform_value = random.uniform(0, 1)
                            comparison_value = math.exp(-abs(distance - min_dist) / current_temp)
                            if uniform_value < comparison_value:
                                min_dist = distance
                                next_state = new_state
        if answer != None:
            break
        if next_state != None:
            current_state = next_state
        else:
            break
        current_temp -= alpha_temp

    if answer == None:
        print("Simulated annealing: No answer")
    else:
        print("Simulated annealing: ", answer.moves)



if __name__ == "__main__":
    maze_matrix, num_lines, num_columns = read_maze(MAZE_FILE_PATH)
    problem = Problem(maze_matrix, 2, 3, 0, 3)

    strategies = [bfs, bkt, hill_climbing, hill_climbing_heap, simulated_annealing]

    for strategy in strategies:
        strategy(problem, maze_matrix, num_lines, num_columns)

    exit(1)

    visited = [[False for i in range(num_columns)] for j in range(num_lines)]
    initial_state = get_initial_state(problem)
    visited[initial_state.x][initial_state.y] = True
    q = queue.Queue()
    q.put(initial_state)
    answer = None

    while not q.empty():
        el = q.get()
        for i in range(MOVE_SIZE):
            if is_valid_tranzition(el, i):
                new_state = apply_tranzition(el, i)
                if is_final_state(new_state):
                    answer = new_state
                else:
                    q.put(new_state)
        if answer != None:
            break

    if answer != None:
        print(answer.moves)
