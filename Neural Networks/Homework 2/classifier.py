import math
import pickle as cPickle
import gzip
import numpy as np
import random
import json

LOG = 1
CUSTOM_INPUT_FILE_PATH = "ignored/custom.txt"


def log(text, end='\n'):
    if LOG == 1:
        print(text,end=end)


class Randomizer:
    def __init__(self, size):
        self.permutation = [i for i in range(size)]

    def get_next(self):
        random.shuffle(self.permutation)
        return self.permutation


class Classifier:
    def __init__(self, learning_rate, perceptrons_count, input_size, use_activation_function=False):
        gen = np.random.Generator(np.random.PCG64())

        low = -1.0 if use_activation_function == False else 0.0
        high = 1.0
        self.weights = gen.uniform(
            low=low, high=high, size=(input_size, perceptrons_count))
        self.biases = gen.uniform(
            low=low, high=high, size=(1, perceptrons_count))

        self.expected = np.array([i for i in range(perceptrons_count)])

        self.use_activation_function = use_activation_function
        self.learning_rate = learning_rate
        self.input_size = input_size
        self.perceptrons_count = perceptrons_count

        else_value = -1 if use_activation_function == False else 0
        self.targets = [np.array([1 if i == j else else_value for i in range(perceptrons_count)]
                                 ).reshape((1, perceptrons_count)) for j in range(perceptrons_count)]

    def train(self, train_set, iterations):
        rnd = Randomizer(len(train_set[0]))

        log('Classifier training started!')
        for _ in range(iterations):
            perm = rnd.get_next()
            for training_element_index in perm:
                self.update_weights(train_set[0][training_element_index].reshape(
                    (1, self.input_size)), train_set[1][training_element_index])
            log('Finished training iteration {}'.format(_+1))

    def activate(self, act):
        if act > 0:
            return 1
        return 0

    def update_weights_no_activation(self, inp, digit):
        prod = np.dot(inp, self.weights) + self.biases
        diff = (self.targets[digit] - prod) * \
            self.learning_rate  # use the dot product
        add_mat = np.transpose(np.tile(inp, (self.perceptrons_count, 1)))
        for i in range(self.perceptrons_count):
            add_mat[:, i] *= diff[0][i]
        self.weights = self.weights + add_mat
        self.biases = self.biases + \
            (self.targets[digit] - prod) * \
            self.learning_rate  # use the dot product

    def update_weights_activation(self, inp, digit):
        prod = np.dot(inp, self.weights) + self.biases
        activated = np.array([self.activate(prod[0][i])
                              for i in range(self.perceptrons_count)])
        diff = (self.targets[digit] - activated) * \
            self.learning_rate  # use the activated array
        add_mat = np.transpose(np.tile(inp, (self.perceptrons_count, 1)))
        for i in range(self.perceptrons_count):
            add_mat[:, i] *= diff[0][i]
        self.weights = self.weights + add_mat
        self.biases = self.biases + \
            (self.targets[digit] - activated) * \
            self.learning_rate  # use the activated array

    def update_weights(self, inp, digit):
        if self.use_activation_function:
            self.update_weights_activation(inp, digit)
        else:
            self.update_weights_no_activation(inp, digit)

    def classify(self, inp):
        return np.argmax((np.dot(inp, self.weights) + self.biases))

    def get_classification_array(self, inp):
        return np.dot(inp, self.weights) + self.biases


def read_sets():
    f = gzip.open('mnist.pkl.gz', 'rb')
    train_set, _, test_set = cPickle.load(f, encoding='iso-8859-1')
    f.close()

    return (train_set, test_set)


if __name__ == "__main__":
    log('Program started! Reading training and test data!')
    train_set, test_set = read_sets()
    log('Training and test data loaded successfully!')

    act_function_iterations = 30
    no_act_function_iterations = 3

    classifier_no_activation = Classifier(0.001, 10, len(train_set[0][0]))
    log('Classifier without activation function started, {} iterations'.format(
        act_function_iterations))
    classifier_no_activation.train(train_set, act_function_iterations)

    classifier_activation = Classifier(0.01, 10, len(train_set[0][0]), True)
    log('Classifier with activation function started, {} iterations'.format(
        no_act_function_iterations))
    classifier_activation.train(train_set, no_act_function_iterations)

    correct_no_activation = sum([1 if (classifier_no_activation.classify(test_set[0][i]) ==
                                       test_set[1][i]) else 0 for i in range(len(test_set[0]))])
    correct_activation = sum([1 if (classifier_activation.classify(test_set[0][i]) ==
                                    test_set[1][i]) else 0 for i in range(len(test_set[0]))])

    log('Total number of classifications: {0}'.format(len(test_set[0])))
    log('Number of correct classifications with no activation function: {0}'.format(
        correct_no_activation))
    log('Number of correct classifications with activation function: {0}'.format(
        correct_activation))
    log('Success percent no activation: {0}%'.format(
        round(100.0 * correct_no_activation / len(test_set[0]), 2)))
    log('Success percent with activation: {}%'.format(
        round(100.0 * correct_activation / len(test_set[0]), 2)))

    while True:
        log("Type \"exit\" to stop the program or \"run\" to compute the precision on custom input: ", end='')
        entered = input()

        if entered == "exit":
            break
        elif entered == "run":
            with open(CUSTOM_INPUT_FILE_PATH, "r") as file_in:
                file_content = file_in.readlines()
                if len(file_content) < 1:
                    log("Invalid file!")
                inp = json.loads(file_content[0])
                log("Total instances: {}".format(len(inp)))
                for i in range(len(inp)):
                    array_no_activation = classifier_no_activation.get_classification_array(
                        inp[i])
                    array_activation = classifier_activation.get_classification_array(
                        inp[i])
                    result_no_activation = np.argmax(array_no_activation)
                    result_activation = np.argmax(array_activation)
                    output_array_no_activation = [
                        1 if i == result_no_activation else 0 for i in range(10)]
                    output_array_activation = [
                        1 if i == result_activation else 0 for i in range(10)]
                    log('{}. Array no activation: {}, prediction: {}'.format(
                        i+1, output_array_no_activation, result_no_activation))
                    log('{}. Array with activation: {}, prediction: {}'.format(
                        i+1, output_array_activation, result_activation))
        else:
            log("Invalid input!")
