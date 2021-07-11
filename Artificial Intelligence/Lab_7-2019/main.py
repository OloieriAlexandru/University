# pip install graphics.py

import re
import numpy as np
import random

from graphics import GraphWin, Text, Point, Entry, Rectangle, color_rgb

DATA_FILE_PATH = 'ignored/segments.data'

W_HEIGHT = 480
W_WIDTH = 720

DEFAULT_COLOR = color_rgb(255, 255, 255)
DIGIT_COLOR = color_rgb(127, 127, 127)
OUTLINE_COLOR = color_rgb(222, 222, 222)

DIGIT_BORDER_WIDTH = 50
DIGIT_BORDER_HEIGHT = 20
DIGIT_STICK_WIDTH = 30

rgx_numbers = re.compile(r"(\d+\.\d+|\d+)")


def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-z))


def sigmoid_derivative(z):
    return sigmoid(z) * (1 - sigmoid(z))


class NN:
    def __init__(self, input_layer_size, hidden_layer_size, output_layer_size):
        self.size_input = int(input_layer_size)
        self.size_hidden = int(hidden_layer_size)
        self.size_output = int(output_layer_size)

        self.weights = []
        self.weights.append(np.random.randn(self.size_hidden,
                                            self.size_input) / np.sqrt(self.size_input))
        self.weights.append(np.random.randn(self.size_output,
                                            self.size_hidden) / np.sqrt(self.size_hidden))

        self.biases = []
        self.biases.append(np.random.randn(self.size_hidden, 1))
        self.biases.append(np.random.randn(self.size_output, 1))

    def train(self, digits, epochs, learning_rate, max_err):
        epochs = int(epochs)

        np_digits = [(np.array(inp).reshape(self.size_input, 1),
                      np.array(outp).reshape(self.size_output, 1)) for inp, outp in digits]

        for _ in range(epochs):
            random.shuffle(np_digits)
            self.train_epoch(np_digits, learning_rate, max_err)

    def train_epoch(self, digits, learning_rate, max_err):
        weights_update = [np.zeros(weight.shape) for weight in self.weights]
        biases_update = [np.zeros(bias.shape) for bias in self.biases]

        for inp, outp in digits:
            delta_weights, delta_biases = self.backpropagation(inp, outp)

            for i in range(len(delta_weights)):
                weights_update[i] += delta_weights[i]
                biases_update[i] += delta_biases[i]

        rate = learning_rate / len(digits)
        for i in range(len(self.weights)):
            self.weights[i] -= rate * weights_update[i]
            self.biases[i] -= rate * biases_update[i]

    def cost_derivative(self, last_layer_activations, expected_output):
        return last_layer_activations - expected_output

    def backpropagation(self, inp, outp):
        delta_weights = [np.zeros(weight.shape) for weight in self.weights]
        delta_biases = [np.zeros(bias.shape) for bias in self.biases]

        last_layer_output = inp
        layers_outputs = [inp]
        layers_z = []
        for i in range(len(self.weights)):
            z = np.dot(self.weights[i], last_layer_output) + self.biases[i]
            layers_z.append(z)
            last_layer_output = sigmoid(z)
            layers_outputs.append(last_layer_output)

        delta = self.cost_derivative(
            layers_outputs[-1], outp) * sigmoid_derivative(layers_z[-1])

        delta_biases[-1] = delta
        delta_weights[-1] = np.dot(delta, layers_outputs[-2].transpose())
        for i in range(2, len(self.weights)):
            z = layers_z[-i]
            sig_der = sigmoid_derivative(z)
            delta = np.dot(self.weights[-i+1].transpose(), delta) * sig_der
            delta_biases[-i] = delta
            delta_weights[-i] = np.dot(delta, layers_outputs[-i-1].transpose())

        return (delta_weights, delta_biases)

    def feed_forward(self, inp):
        for i in range(len(self.weights)):
            inp = sigmoid(np.dot(self.weights[i], inp) + self.biases[i])
        return inp

    def predict(self, inp):
        inp = np.array(inp).reshape(self.size_input, 1)
        return self.feed_forward(inp)


class NNAppButton:
    def __init__(self, x, y, width, height, text, callback):
        self.x1 = x
        self.y1 = y
        self.x2 = x + width
        self.y2 = y + height
        self.r = Rectangle(Point(x, y), Point(x+width, y+height))
        self.textObj = Text(Point(x + width // 2, y + height // 2), text)
        self.callback = callback

    def draw(self, win):
        self.r.draw(win)
        self.textObj.draw(win)

    def call(self):
        return self.callback

    def inside(self, x, y):
        return self.x1 <= x <= self.x2 and self.y1 <= y <= self.y2


class NNAppInputBox:
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


class NNApp:
    def __init__(self, training_data_file_path, window_width, window_height):
        self.read_data(training_data_file_path)

        self.win = GraphWin(width=window_width, height=window_height)
        self.win.setCoords(0, window_height, window_width, 0)

        self.compute_lines()
        self.draw_lines()

        self.init_buttons()
        self.draw_buttons()
        self.draw_input_boxes()
        self.draw_prediction()

        self.nn = None

    def read_data(self, file_path):
        with open(file_path, "r") as file_in:
            line = file_in.readline()
            nums = rgx_numbers.findall(line)
            if len(nums) != 3:
                raise Exception(
                    "Invalid file {}! Expected three integers on the first line!".format(file_path))

            self.digits_input_size = int(nums[0])
            self.digits_output_size = int(nums[1])
            self.digits_count = int(nums[2])
            self.digits = []
            for i in range(self.digits_count):
                line = file_in.readline()
                if not line:
                    raise Exception(
                        "Invalid file {}! Expected {} lines for digits in/out".format(file_path, self.digits_count))
                line_parts = line.split('|')
                str_in = line_parts[0].strip()
                str_out = line_parts[1].strip()
                digit_in = [int(num) for num in rgx_numbers.findall(str_in)]
                digit_out = [int(num) for num in rgx_numbers.findall(str_out)]
                self.digits.append((digit_in, digit_out))

    def init_buttons(self):
        self.buttons = []
        self.input_boxes = []

        self.prediction = self.create_prediction_text("?")
        self.pred_p1 = Point(W_WIDTH / 9 + 190, 140)
        self.pred_p2 = Point(W_WIDTH / 9 + 250, 190)

        self.buttons.append(NNAppButton(
            W_WIDTH / 20, 50, 140, 50, "Show", self.show_digit))
        self.buttons.append(NNAppButton(
            W_WIDTH / 9, 140, 140, 50, "Predict", self.predict))
        self.buttons.append(NNAppButton(
            W_WIDTH / 6, 230, 140, 50, "Train", self.train))

        self.input_boxes.append(NNAppInputBox(
            W_WIDTH / 20 + 200, 70, 100, 24, 0, "Digit"))
        self.input_boxes.append(NNAppInputBox(
            50, 340, 100, 24, 25000, "Epochs"))
        self.input_boxes.append(NNAppInputBox(
            240, 340, 100, 24, 0.001, "Max error"))
        self.input_boxes.append(NNAppInputBox(50, 400, 100, 24, 100, "Neurons"))
        self.input_boxes.append(NNAppInputBox(
            240, 400, 100, 24, 0.3, "Learning rate"))

    def create_prediction_text(self, text):
        pred = Text(Point(W_WIDTH / 9 + 220, 165), text)
        pred.setSize(24)
        return pred

    def clear_prediction(self):
        r = Rectangle(self.pred_p1, self.pred_p2)
        r.setFill(DEFAULT_COLOR)
        r.draw(self.win)

    def draw_prediction(self, new_text=None):
        if new_text is not None:
            if len(new_text) > 5:
                return
            self.prediction = self.create_prediction_text(new_text)
        self.clear_prediction()
        self.prediction.draw(self.win)

    def draw_buttons(self):
        for button in self.buttons:
            button.draw(self.win)

    def draw_input_boxes(self):
        for input_box in self.input_boxes:
            input_box.draw(self.win)

    def compute_lines(self):
        self.lines = []

        w = W_WIDTH / 2
        h = W_HEIGHT

        d_rectangle_height = h - 2 * DIGIT_BORDER_HEIGHT
        d_rectangle_width = w - 2 * DIGIT_BORDER_WIDTH

        line_horizontal_width = d_rectangle_width - 2 * DIGIT_STICK_WIDTH
        line_horizontal_height = DIGIT_STICK_WIDTH

        line_vertical_height = (d_rectangle_height -
                                3 * line_horizontal_height) / 2
        line_vertical_width = DIGIT_STICK_WIDTH

        x0 = w + DIGIT_BORDER_WIDTH
        y0 = DIGIT_BORDER_HEIGHT

        # 0
        self.lines.append([x0 + DIGIT_STICK_WIDTH, y0,
                           line_horizontal_width, line_horizontal_height])
        # 1
        self.lines.append([x0, y0 + DIGIT_STICK_WIDTH,
                           line_vertical_width, line_vertical_height])
        # 2
        self.lines.append([x0 + DIGIT_STICK_WIDTH + line_horizontal_width,
                           y0 + DIGIT_STICK_WIDTH, line_vertical_width, line_vertical_height])
        # 3
        self.lines.append([x0 + DIGIT_STICK_WIDTH, y0 + (line_horizontal_height + line_vertical_height),
                           line_horizontal_width, line_horizontal_height])
        # 4
        self.lines.append([x0, y0 + 2 * DIGIT_STICK_WIDTH +
                           line_vertical_height, line_vertical_width, line_vertical_height])
        # 5
        self.lines.append([x0 + DIGIT_STICK_WIDTH + line_horizontal_width,
                           y0 + 2 * DIGIT_STICK_WIDTH + line_vertical_height, line_vertical_width, line_vertical_height])
        # 6
        self.lines.append([x0 + DIGIT_STICK_WIDTH, y0 + 2 * (line_horizontal_height + line_vertical_height),
                           line_horizontal_width, line_horizontal_height])

        for i in range(len(self.lines)):
            if i in [0, 2, 3, 5, 6]:
                self.lines[i].append(1)
            else:
                self.lines[i].append(0)

    def draw_line_obj_c(self, line, color=DEFAULT_COLOR):
        self.draw_line_d(line[0], line[1], line[2], line[3], color)

    def draw_line_index_c(self, index, color=DEFAULT_COLOR):
        if not (0 <= index < len(self.lines)):
            return
        self.draw_line_obj_c(self.lines[index], color)

    def draw_empty_lines(self):
        for line in self.lines:
            self.draw_line_obj_c(line)

    def draw_full_lines(self):
        for line in self.lines:
            self.draw_line_obj_c(line, DIGIT_COLOR)

    def draw_line_obj(self, line):
        if line[4] == 1:
            self.draw_line_obj_c(line, DIGIT_COLOR)
        else:
            self.draw_line_obj_c(line)

    def draw_line_index(self, index):
        if not (0 <= index < len(self.lines)):
            return
        self.draw_line_obj(self.lines[index])

    def draw_lines(self):
        for line in self.lines:
            self.draw_line_obj(line)

    def draw_line_p(self, x1, y1, x2, y2, color=DEFAULT_COLOR):  # _p = points
        r = Rectangle(Point(x1, y1), Point(x2, y2))
        r.setFill(color)
        r.setOutline(OUTLINE_COLOR)
        r.draw(self.win)

    def draw_line_d(self, x1, y1, width, height, color=DEFAULT_COLOR):  # _d = dimensions
        self.draw_line_p(x1, y1, x1 + width, y1 + height, color)

    def draw_digit(self, digit):
        for i in range(self.digits_input_size):
            self.lines[i][4] = self.digits[digit][0][i]
        self.draw_lines()

    def switch_line_status(self, lineIndex):
        if not (0 <= lineIndex < len(self.lines)):
            return
        self.lines[lineIndex][4] = 1 - self.lines[lineIndex][4]
        self.draw_line_index(lineIndex)

    def get_digit_arr(self):
        return [line[4] for line in self.lines]

    # returns the index of the line or None
    def check_mouse_inside_line(self, x, y):
        for i in range(len(self.lines)):
            x1 = self.lines[i][0]
            y1 = self.lines[i][1]
            x2 = x1 + self.lines[i][2]
            y2 = y1 + self.lines[i][3]
            if x1 <= x <= x2 and y1 <= y <= y2:
                return i
        return None

    def check_mouse_inside_button(self, x, y):
        for button in self.buttons:
            if button.inside(x, y):
                return button
        return None

    def start(self):
        running = True
        while running:
            mouse_coordinate = self.win.checkMouse()
            while mouse_coordinate == None:
                mouse_coordinate = self.win.checkMouse()

            line = self.check_mouse_inside_line(
                mouse_coordinate.getX(), mouse_coordinate.getY())
            if line is not None:
                self.switch_line_status(line)
                continue

            button = self.check_mouse_inside_button(
                mouse_coordinate.getX(), mouse_coordinate.getY())
            if button is not None:
                button.call()()
                continue

    def get_input_box_value(self, input_box_index):
        if not (0 <= input_box_index < len(self.input_boxes)):
            return None
        digits_arr = rgx_numbers.findall(
            self.input_boxes[input_box_index].get_text())
        if len(digits_arr) == 0:
            return None
        return float(digits_arr[0])

    def show_digit(self):
        digit = self.get_input_box_value(0)
        if digit is None or not (0 <= digit < self.digits_count):
            return
        self.draw_digit(digit)

    def get_parameters(self):
        epoch = self.get_input_box_value(1)
        max_err = self.get_input_box_value(2)
        neurons = self.get_input_box_value(3)
        learning_rate = self.get_input_box_value(4)
        if epoch is None or max_err is None or neurons is None or learning_rate is None:
            return
        return (epoch, max_err, neurons, learning_rate)

    def predict(self):
        if self.nn is None:
            print("You have to train the network first!")
            return
        digit_input = self.get_digit_arr()

        output_layer_values = self.nn.predict(digit_input)
        digit_prediction = np.argmax(output_layer_values)
        self.draw_prediction(str(digit_prediction))

        output_layer_values = [round(value[0], 5)
                               for value in output_layer_values]
        print(output_layer_values)

    def train(self):
        parameters = self.get_parameters()
        # [0] = epochs, [1] = max_err, [2] = hidden neurons, [3] = learning rate
        self.nn = NN(self.digits_input_size, parameters[2], self.digits_count)

        print("Training started!")
        self.nn.train(self.digits, parameters[0], parameters[3], parameters[1])
        print("Training ended!")


if __name__ == "__main__":
    app = NNApp(DATA_FILE_PATH, W_WIDTH, W_HEIGHT)
    app.start()
