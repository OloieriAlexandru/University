
import numpy as np
import os
import random

TRAINING_FILE_INFO_PATH = "./training_info.txt"
TRAINING_FILE_EXPECTED_NUM_LINES = 6


class Randomizer:
    def __init__(self, size):
        self.permutation = [i for i in range(size)]

    def get_random_permutation(self):
        random.shuffle(self.permutation)
        return self.permutation


def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-z))


def sigmoid_derivative(z):
    return sigmoid(z) * (1.0 - sigmoid(z))


class NN:
    def __init__(self, input_layer_size, hidden_layer_size, output_layer_size):
        self.size_input = int(input_layer_size)
        self.size_hidden = int(hidden_layer_size)
        self.size_output = int(output_layer_size)

        self.weights = []
        self.weights.append(np.random.uniform(
            low=-0.2, high=0.2, size=(self.size_hidden, self.size_input)))
        self.weights.append(np.random.uniform(
            low=-0.2, high=0.2, size=(self.size_output, self.size_hidden)))

        self.biases = []
        self.biases.append(np.random.uniform(
            low=-0.5, high=0.5, size=(self.size_hidden, 1)))
        self.biases.append(np.random.uniform(
            low=-0.5, high=0.5, size=(self.size_output, 1)))

    def train(self, nn_info, epochs_count):
        n = len(nn_info.inp)
        rnd = Randomizer(n)

        for _ in range(epochs_count):
            randp = rnd.get_random_permutation()

            weights_update = [np.zeros(weight.shape)
                              for weight in self.weights]
            biases_update = [np.zeros(bias.shape) for bias in self.biases]

            error = 0
            for pe in randp:
                delta_weights, delta_biases, err = self.backpropagation(
                    nn_info.inp[pe], nn_info.out[pe])
                error += err

                for i in range(len(self.weights)):
                    weights_update[i] += delta_weights[i]
                    biases_update[i] += delta_biases[i]

            rate = nn_info.lr / n
            for i in range(len(self.weights)):
                self.weights[i] += rate * weights_update[i]
                self.biases[i] += rate * biases_update[i]

            if (_ + 1) % 5000 == 0:
                print('Finished training epoch {}'.format(_+1))

    def backpropagation(self, inp, out):
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

        error = np.array([[out]]) - layers_outputs[-1]

        delta = error * sigmoid_derivative(layers_z[-1])

        delta_biases[-1] = delta
        delta_weights[-1] = np.dot(delta, layers_outputs[-2].transpose())
        for i in range(2, len(self.weights)):
            z = layers_z[-i]
            sig_der = sigmoid_derivative(z)
            delta = np.dot(self.weights[-i+1].transpose(), delta) * sig_der
            delta_biases[-i] = delta
            delta_weights[-i] = np.dot(delta, layers_outputs[-i-1].transpose())

        return (delta_weights, delta_biases, error)

    def feed_forward(self, inp):
        x = inp
        for i in range(len(self.weights)):
            x = sigmoid(np.dot(self.weights[i], x) + self.biases[i])
        return x


class NN_Info:
    def __init__(self, file_name):
        if not os.path.exists(file_name):
            raise Exception("Invalid info file: \"{}\"".format(file_name))

        self.parse_file(file_name)
        self.build_in_arrays()

    def parse_file(self, file_name):
        with open(file_name, "r") as file_in:
            lines = file_in.readlines()
            lines = [line.strip() for line in lines if len(line) > 0]

            if len(lines) != TRAINING_FILE_EXPECTED_NUM_LINES:
                raise Exception("Invalid number of lines in info file, expecting: {}, found: {}".format(
                    TRAINING_FILE_EXPECTED_NUM_LINES, len(lines)))

            function = None
            lr = None
            max_error = None

            line_idx = 0
            while line_idx < TRAINING_FILE_EXPECTED_NUM_LINES:
                if line_idx == 0:
                    function = [int(value)
                                for value in lines[line_idx+1].strip().split(' ')]
                elif line_idx == 2:
                    lr = float(lines[line_idx+1])
                elif line_idx == 4:
                    max_error = float(lines[line_idx+1])

                line_idx += 2

            if function is None or len(function) != 4 or lr is None or max_error is None:
                raise Exception("Invalid info file!\n")

            self.out = function
            self.lr = lr
            self.max_error = max_error

    def build_in_arrays(self):
        self.inp = []
        for i in range(2):
            for j in range(2):
                self.inp.append(np.array([i, j]).reshape(2, 1))


if __name__ == "__main__":
    nn_info = NN_Info(TRAINING_FILE_INFO_PATH)

    print("Boolean function:")
    for i in range(len(nn_info.inp)):
        print('{} {} {}'.format(
            nn_info.inp[i][0][0], nn_info.inp[i][1][0], nn_info.out[i]))

    nn = NN(2, 2, 1)

    nn.train(nn_info, 200000)

    while True:
        line = input()
        f = [int(el) for el in line.strip().split(' ')]
        f = np.array(f).reshape(2, 1)
        prediction = nn.feed_forward(f)
        print(prediction)
