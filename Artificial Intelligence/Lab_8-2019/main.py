# pip install graphics.py

import re
import random
import time

from graphics import GraphWin, Circle, Line, Oval, Text, Point, Entry, Rectangle, color_rgb

INITIAL_MAZE_FILE = "ignored/initial_maze.txt"

CELL_COLOR_EMPTY = color_rgb(222, 255, 224)
CELL_COLOR_OBSTACLE = color_rgb(20, 16, 64)
CELL_COLOR_START = color_rgb(140, 9, 8)
CELL_COLOR_END = color_rgb(20, 238, 8)
CELL_COLOR_PATH = color_rgb(243, 222, 78)
COLORS = [CELL_COLOR_EMPTY, CELL_COLOR_OBSTACLE,
          CELL_COLOR_START, CELL_COLOR_END, CELL_COLOR_PATH]

WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720
WINDOW_OPTION_BAR_GAP = 30
WINDOW_OPTION_BAR_WIDTH = 280

D_NUM = 4
D_LIN = [-1, 0, 1, 0]
D_COL = [0, 1, 0, -1]

rgx_numbers = re.compile(r"(\d+\.\d+|\d+)")


class MazeAppInputBox:
    def __init__(self, x, y, width, height, default_value="0.0", default_text="None"):
        self.textObj = Text(
            Point(x + width // 2, y - height // 2), default_text)
        self.box = Entry(Point(x + width // 2, y + height // 2), 10)
        self.box.setText(default_value)
        self.box.setSize(14)

    def draw(self, win):
        self.box.draw(win)
        self.textObj.draw(win)

    def get_text(self):
        return self.box.getText()


class MazeAppButton:
    def __init__(self, button_index, x, y, width, height, text, callback, cell_callback=None):
        self.x1 = x
        self.y1 = y
        self.x2 = x + width
        self.y2 = y + height
        self.r = Rectangle(Point(x, y), Point(x+width, y+height))
        self.textObj = Text(Point(x + width // 2, y + height // 2), text)
        self.callback = callback
        self.cell_callback = cell_callback
        self.button_index = button_index

    def index(self):
        return self.button_index

    def draw(self, win):
        self.r.draw(win)
        self.textObj.draw(win)

    def call(self):
        return self.callback

    def get_matrix_cell_handler(self):
        return self.cell_callback

    def inside(self, x, y):
        return self.x1 <= x <= self.x2 and self.y1 <= y <= self.y2


class MazeApp:
    def __init__(self, maze_file_path, window_width, window_height):
        self.wwidth = window_width
        self.wheight = window_height

        self.read_maze(maze_file_path)
        self.compute_maze_dimensions(window_width, window_height)
        self.compute_maze_points()
        self.init_buttons()

        self.win = GraphWin(width=window_width, height=window_height)
        self.win.setCoords(0, window_height, window_width, 0)

        self.draw_maze()
        self.draw_buttons()
        self.draw_input_boxes()

        self.cell_handler = None

        self.q = None

    def read_maze(self, maze_file_path):
        with open(maze_file_path, "r") as file_in:
            line = file_in.readline()
            nums = rgx_numbers.findall(line)
            if len(nums) != 3:
                raise Exception(
                    "Invalid file {}! Expected three integers on the first line of the file!".format(maze_file_path))
            self.n = int(nums[0])
            self.m = int(nums[1])
            from_file = int(nums[2])

            if from_file != 0:
                self.maze = []

                for i in range(self.n):
                    line = file_in.readline()
                    if not line:
                        raise Exception(
                            "Invalid file {}! Expected {} lines with {} columns!".format(
                                maze_file_path, self.n, self.m))
                    maze_line = [int(num) for num in rgx_numbers.findall(line)]
                    if len(maze_line) != self.m:
                        raise Exception(
                            "Invalid file {}! Line {} has {} columns, instead of {}!".format(
                                maze_file_path, i, len(maze_line), self.m))
                    self.maze.append(maze_line)
            else:
                self.maze = [[0] * self.m for _ in range(self.n)]

    def compute_maze_dimensions(self, width, height):
        self.maze_height = height
        self.maze_width = width - WINDOW_OPTION_BAR_WIDTH
        self.maze_cell_dim = min(
            self.maze_height // self.n, self.maze_width // self.m)

    def compute_maze_points(self):
        self.maze_points = []
        for i in range(self.n):
            line_points = []
            for j in range(self.m):
                p1 = Point(self.maze_cell_dim * j, self.maze_cell_dim * i)
                p2 = Point(self.maze_cell_dim * j + self.maze_cell_dim - 1,
                           self.maze_cell_dim * i + self.maze_cell_dim - 1)
                line_points.append((p1, p2))
            self.maze_points.append(line_points)

    def init_buttons(self):
        self.buttons = []
        self.input_boxes = []

        button_width = WINDOW_OPTION_BAR_WIDTH - 2 * WINDOW_OPTION_BAR_GAP
        self.buttons.append(MazeAppButton(
            0, self.maze_width + WINDOW_OPTION_BAR_GAP, 60, button_width, 50,
            "Train", self.button_callback_train))
        self.buttons.append(MazeAppButton(
            1, self.maze_width + WINDOW_OPTION_BAR_GAP, 140, button_width, 50,
            "Find path", self.button_callback_find_path))
        self.buttons.append(MazeAppButton(
            2, self.maze_width + WINDOW_OPTION_BAR_GAP, 220, button_width, 50,
            "Add obstacle", self.button_callback_update_cell_handler, self.button_callback_add_obstacle))
        self.buttons.append(MazeAppButton(
            3, self.maze_width + WINDOW_OPTION_BAR_GAP, 300, button_width, 50,
            "Remove obstacle", self.button_callback_update_cell_handler, self.button_callback_remove_obstacle))
        self.buttons.append(MazeAppButton(
            4, self.maze_width + WINDOW_OPTION_BAR_GAP, 380, button_width, 50,
            "Select start cell", self.button_callback_update_cell_handler, self.button_callback_select_start))
        self.buttons.append(MazeAppButton(
            5, self.maze_width + WINDOW_OPTION_BAR_GAP, 460, button_width, 50,
            "Select end cell", self.button_callback_update_cell_handler, self.button_callback_select_end))

        self.input_boxes.append(MazeAppInputBox(
            self.maze_width + WINDOW_OPTION_BAR_GAP, 540, button_width, 24, "1000", "Iterations"))
        self.input_boxes.append(MazeAppInputBox(
            self.maze_width + WINDOW_OPTION_BAR_GAP, 600, button_width, 24, "0.05", "Alpha"))
        self.input_boxes.append(MazeAppInputBox(
            self.maze_width + WINDOW_OPTION_BAR_GAP, 660, button_width, 24, "0.8", "Gamma"))

    def run(self):
        running = True
        while running:
            mouse_coordinate = self.win.checkMouse()
            while mouse_coordinate == None:
                mouse_coordinate = self.win.checkMouse()

            button = self.check_mouse_inside_button(
                mouse_coordinate.getX(), mouse_coordinate.getY())
            if button is not None:
                self.draw_maze()
                button.call()(button.index())
                continue

            maze_cell = self.check_mouse_inside_maze(
                mouse_coordinate.getX(), mouse_coordinate.getY())
            if maze_cell is not None:
                if self.cell_handler is not None:
                    self.cell_handler(int(maze_cell[1]), int(maze_cell[0]))
                    self.draw_maze()

    def draw_maze(self):
        for i in range(self.n):
            for j in range(self.m):
                self.draw_maze_cell(i, j)

    def draw_maze_cell(self, line, column, value=None):
        r = Rectangle(self.maze_points[line][column]
                      [0], self.maze_points[line][column][1])
        if value is None:
            value = self.maze[line][column]
        if value < len(COLORS):
            r.setFill(COLORS[value])
        else:
            raise Exception("Logic error! Invalid value in the maze matrix! cell[{}][{}] = {}".format(
                line, column, value))
        r.draw(self.win)

    def draw_buttons(self):
        for button in self.buttons:
            button.draw(self.win)

    def draw_input_boxes(self):
        for input_box in self.input_boxes:
            input_box.draw(self.win)

    def get_maze_position(self, cell_value):
        for i in range(self.n):
            for j in range(self.m):
                if self.maze[i][j] == cell_value:
                    return (i, j)
        return None

    def valid_maze_indexes(self, line, column):
        return 0 <= line < self.n and 0 <= column < self.m

    def button_callback_train(self, *args):
        iterations = self.get_input_box_value(0)
        alpha = self.get_input_box_value(1)
        gamma = self.get_input_box_value(2)
        if iterations is None or alpha is None or gamma is None:
            return
        iterations = int(iterations)
        print("Training started!")
        self.q_learning(iterations, alpha, gamma)
        print("Training ended!")

    def get_sorted(self, arr):
        ret_arr = []
        for i in range(len(arr)):
            ret_arr.append((i, arr[i]))
        ret_arr.sort(reverse=True, key=lambda x: float(x[1]))
        return ret_arr

    def button_callback_find_path(self, *args):
        if self.q is None:
            print("You have to train first!")
            return

        pos = self.get_maze_position(2)
        end_pos = self.get_maze_position(3)
        visited = [[0] * self.m for _ in range(self.n)]

        total_steps = 0
        found = False
        # the path cannot have more cells than the total cells in the maze
        for _ in range(self.n * self.m):
            visited[pos[0]][pos[1]] = 1  # mark the current position as visited
            # get next positions sorted, to avoid the situation
            next_steps = self.get_sorted(self.q[pos[0]][pos[1]])
            # to avoid the situation when two adjacent positions have the maximum value a cycle
            next_step = None  # find a next position that wasn't visited, the best one
            for n_s in next_steps:
                nl = pos[0] + D_LIN[n_s[0]]
                nc = pos[1] + D_COL[n_s[0]]
                if not self.in_interior(nl, nc) or visited[nl][nc] == 1 or self.maze[nl][nc] not in [0, 3]:
                    continue
                # a position is good when is in interior, is not visited and is an empty cell or the end cell
                next_step = n_s[0]  # save the direction of the move
                break
            if next_step is None:
                break
            nl = pos[0] + D_LIN[next_step]
            nc = pos[1] + D_COL[next_step]
            if nl == end_pos[0] and nc == end_pos[1]:
                found = True
                break
            self.draw_maze_cell(nl, nc, 4)  # draw the cell
            pos = (nl, nc)
            total_steps += 1
        if found:
            print("Path length: {}".format(total_steps))
        else:
            print("Invalid training or there is no path!")

    def button_callback_update_cell_handler(self, index):
        if 0 <= index < len(self.buttons):
            self.cell_handler = self.buttons[index].get_matrix_cell_handler()

    def button_callback_add_obstacle(self, line, column):
        if not self.valid_maze_indexes(line, column):
            return
        if self.maze[line][column] != 0:
            return
        self.maze[line][column] = 1

    def button_callback_remove_obstacle(self, line, column):
        if not self.valid_maze_indexes(line, column):
            return
        if self.maze[line][column] != 1:
            return
        self.maze[line][column] = 0

    def button_callback_select_start(self, line, column):
        if not self.valid_maze_indexes(line, column):
            return
        if self.maze[line][column] in [2, 3]:
            return
        current_position = self.get_maze_position(2)
        self.maze[current_position[0]][current_position[1]] = 0
        self.maze[line][column] = 2

    def button_callback_select_end(self, line, column):
        if not self.valid_maze_indexes(line, column):
            return
        if self.maze[line][column] in [2, 3]:
            return
        current_position = self.get_maze_position(3)
        self.maze[current_position[0]][current_position[1]] = 0
        self.maze[line][column] = 3

    def check_mouse_inside_button(self, x, y):
        for button in self.buttons:
            if button.inside(x, y):
                return button
        return None

    def check_mouse_inside_maze(self, x, y):
        if not (0 <= x < self.maze_width and 0 <= y < self.maze_height):
            return None
        return (x // self.maze_cell_dim, y // self.maze_cell_dim)

    def get_input_box_value(self, input_box_index):
        if not (0 <= input_box_index < len(self.input_boxes)):
            return None
        digits_arr = rgx_numbers.findall(
            self.input_boxes[input_box_index].get_text())
        if len(digits_arr) == 0:
            return None
        return float(digits_arr[0])

    def create_q_matrix(self):
        q_matrix = []
        for _1 in range(self.n):
            line = []
            for _2 in range(self.m):
                line.append([0.0] * 4)
            q_matrix.append(line)
        return q_matrix

    def get_reward(self, line, col):
        if self.maze[line][col] == 0:
            return -1
        if self.maze[line][col] == 1:
            return -100
        if self.maze[line][col] == 3:
            return 1000000
        return 0

    def in_interior(self, line, column):
        return 0 <= line < self.n and 0 <= column < self.m

    def get_valid(self, line, column, visited):
        valid = []
        invalid = []
        for i in range(D_NUM):
            nl = line + D_LIN[i]
            nc = column + D_COL[i]
            if not self.in_interior(nl, nc) or self.maze[nl][nc] not in [0, 3]:
                invalid.append((nl, nc, i))
                continue
            if visited[nl][nc] == 1:
                continue
            valid.append((nl, nc, i))
        return (valid, invalid)

    def get_max(self, line, column):
        return max(self.q[line][column])

    def q_learning(self, iter, alpha, gamma):
        self.q = self.create_q_matrix()
        start_pos = self.get_maze_position(2)
        end_pos = self.get_maze_position(3)

        # for a fixed number of iterations
        for _ in range(iter):
            # init the "visited" array
            visited = [[0] * self.m for _ in range(self.n)]
            state = start_pos  # the start position

            while True:
                visited[state[0]][state[1]] = 1
                if end_pos[0] == state[0] and end_pos[1] == state[1]:
                    break
                valid_moves, invalid_moves = self.get_valid(
                    state[0], state[1], visited)  # get valid and invalid moves
                for invalid_move in invalid_moves:
                    self.q[state[0]][state[1]][invalid_move[2]] = -100000
                    # mark the invalid moves as having a very bad score
                if len(valid_moves) == 0:  # there are no valid moves
                    break
                # select a random next move
                s = random.randint(0, len(valid_moves)-1)

                nl = valid_moves[s][0]
                nc = valid_moves[s][1]

                mul = self.get_reward(
                    nl, nc) + gamma * self.get_max(nl, nc) - self.q[state[0]][state[1]][valid_moves[s][2]]
                delta = alpha * mul
                self.q[state[0]][state[1]][valid_moves[s][2]] += delta
                state = (nl, nc)


if __name__ == "__main__":
    maze_app = MazeApp(INITIAL_MAZE_FILE, WINDOW_WIDTH, WINDOW_HEIGHT)
    maze_app.run()
