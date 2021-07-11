import pickle as cPickle
import gzip
import numpy as np
import random
import time
import json

LOG = 1
CUSTOM_INPUT_FILE_PATH = "ignored/custom.txt"


def log(text, end='\n'):
    if LOG == 1:
        print(text, end=end)


class Randomizer:
    def __init__(self, size):
        self.permutation = [i for i in range(size)]

    def get_random_permutation(self):
        random.shuffle(self.permutation)
        return self.permutation


def sigmoid(z):
    return 1.0 / (1.0 + np.exp(-z))


def sigmoid_derivated(z):
    return sigmoid(z) * (1 - sigmoid(z))


def softmax(z):
    v = np.exp(z)
    return v / np.sum(v)


class NeuralNetwork:
    def __init__(self, input_layer_size, hidden_layer_size, output_layer_size):
        self.size_input = int(input_layer_size)
        self.size_hidden = int(hidden_layer_size)
        self.size_output = int(output_layer_size)

        # Weight initialization
        # https://drive.google.com/file/d/1s-_90YtVNJx08-tHGIZaoltH_DmhkMuO/view
        # Slide: 39
        self.weights = []
        self.weights.append(np.random.randn(self.size_hidden,
                                            self.size_input) / np.sqrt(self.size_input))
        self.weights.append(np.random.randn(self.size_output,
                                            self.size_hidden) / np.sqrt(self.size_hidden))

        self.biases = [np.random.randn(
            self.size_hidden, 1), np.random.randn(self.size_output, 1)]

    def cost_derivate(self, last_layer_activations, expected_output):
        return last_layer_activations - expected_output

    def train(self, training_dataset, iterations, batch_size, learning_rate, l2_constant, momentum_coeff, testing_dataset=None):
        rnd = Randomizer(len(training_dataset[0]))
        n = len(training_dataset[0])
        batches_count = n // batch_size

        print('Batches count: {0}'.format(batches_count))

        for _ in range(iterations):
            perm = rnd.get_random_permutation()

            for i in range(batches_count):
                mini_batch_weights_update = [
                    np.zeros(weight.shape) for weight in self.weights]
                mini_batch_biases_update = [
                    np.zeros(bias.shape) for bias in self.biases]
                friction_values = [
                    np.zeros(weight.shape) for weight in self.weights]

                for j in range(batch_size):
                    layers_sums, layers_activations = self.train_feedforward(
                        training_dataset[0][perm[i*batch_size + j]])
                    add_weights, add_biases = self.train_backpropagation(training_dataset[1][perm[i*batch_size + j]],
                                                                         layers_sums, layers_activations)

                    for t in range(len(add_weights)):
                        mini_batch_weights_update[t] += add_weights[t]
                        mini_batch_biases_update[t] += add_biases[t]

                rate = learning_rate / batch_size
                l2_rate = l2_constant / n
                for t in range(len(mini_batch_weights_update)):
                    # Momentum
                    # https://docs.google.com/presentation/d/1taXQGV_Eu0FkeFXJSv8dJiCT39-kpmbM49bUsUk4yB0/present?slide=id.p12
                    friction_values[t] = momentum_coeff * friction_values[t] - (learning_rate / batch_size) * mini_batch_weights_update[t]
                    # L2 regularization
                    # https://docs.google.com/presentation/d/1tJiWjsvy6efTqcOC--ps1eBCtJKpJI0RrGijCotEwdE/present?slide=id.p24
                    self.weights[t] = self.weights[t] + friction_values[t] - l2_rate * learning_rate * self.weights[t]
                    self.biases[t] -= (mini_batch_biases_update[t] * rate)

                if (batch_size * (i + 1)) % 5000 == 0:
                    print('Batch training iteration {0}, finished processing {1} inputs'.format(
                        _+1, batch_size * (i+1)))

            log('Finished batch training iteration {0}'.format(_+1))

            if testing_dataset != None:
                self.test(testing_dataset, True)

    def train_feedforward(self, batch_input):
        last_layer_output = batch_input
        layers_activations = [last_layer_output]
        layers_sums = []

        for i in range(len(self.weights)):
            layer_sum = np.dot(
                self.weights[i], last_layer_output) + self.biases[i]
            layers_sums.append(layer_sum)
            if i + 1 == len(self.weights):
                # Softmax
                # https://drive.google.com/file/d/1s-_90YtVNJx08-tHGIZaoltH_DmhkMuO/view
                # Slide: 26
                last_layer_output = softmax(layer_sum)
            else:
                last_layer_output = sigmoid(layer_sum)
            layers_activations.append(last_layer_output)

        return (layers_sums, layers_activations)

    def train_backpropagation(self, batch_output, layers_sums, layers_activations):
        add_weights = [
            np.zeros(weight.shape) for weight in self.weights]
        add_biases = [
            np.zeros(bias.shape) for bias in self.biases]

        # Cross entropy
        # https://drive.google.com/file/d/1s-_90YtVNJx08-tHGIZaoltH_DmhkMuO/view
        # Slide: 14
        delta = layers_activations[-1] - batch_output

        # Backpropagation
        # https://docs.google.com/presentation/d/1dPXMnLI4Gy8lQRilgLik1YZMcanO2JWifGHlbiFSunA/present?slide=id.p34
        # Notations
        # https://docs.google.com/presentation/d/1dPXMnLI4Gy8lQRilgLik1YZMcanO2JWifGHlbiFSunA/present?slide=id.p23
        add_biases[-1] = delta
        add_weights[-1] = np.dot(delta, layers_activations[-2].transpose())
        for i in range(2, len(self.weights)+1):
            delta = np.dot(self.weights[-i+1].transpose(), delta) * sigmoid_derivated(layers_sums[-i])
            add_biases[-i] = delta
            add_weights[-i] = np.dot(delta,
                                     layers_activations[-i-1].transpose())

        return (add_weights, add_biases)

    def feed_forward(self, inp):
        for i in range(len(self.weights)):
            if i + 1 == len(self.weights):
                inp = softmax(np.dot(self.weights[i], inp) + self.biases[i])
            else:
                inp = sigmoid(np.dot(self.weights[i], inp) + self.biases[i])
        return inp

    def classify(self, inp):
        output_layer_values = self.feed_forward(inp)
        return np.argmax(output_layer_values)

    def test(self, dataset, validation=False):
        if validation:
            log('Neural Network validation started!')
        else:
            log('Neural Network testing started!')

        correct = sum(1 if (self.classify(dataset[0][i]) == np.argmax(
            dataset[1][i])) else 0 for i in range(len(dataset[0])))

        log('Total number of classifications: {}'.format(len(dataset[0])))
        log('Number of correct classifications: {}'.format(correct))
        log('Success percent: {}%'.format(
            round(100.0 * correct / len(dataset[0]), 2)))


def process_data_set(dataset):
    inputs = [el.reshape(len(el), 1) for el in dataset[0]]

    expected_outputs = []
    for output in dataset[1]:
        output_as_array = np.zeros((10, 1))
        output_as_array[output] = 1
        expected_outputs.append(output_as_array)

    return (inputs, expected_outputs)


def read_sets():
    f = gzip.open('./ignored/mnist.pkl.gz', 'rb')
    train_set, validation_set, test_set = cPickle.load(
        f, encoding='iso-8859-1')
    f.close()

    return (process_data_set(train_set), process_data_set(validation_set), process_data_set(test_set))


if __name__ == "__main__":
    ITERATIONS = 30
    MINI_BATCH_SIZE = 10
    LEARNING_RATE = 0.5
    L2_CONSTANT = 5.0
    MOMENTUM_CONSTANT = 0.2

    log('The execution has started! Reading training, validation and test data!')
    train_set, _, test_set = read_sets()
    log('Training, validation and test data loaded successfully!')

    nn = NeuralNetwork(len(train_set[0][0]), 100, 10)
    log('Starting network training')
    nn.train(train_set, ITERATIONS, MINI_BATCH_SIZE,
             LEARNING_RATE, L2_CONSTANT, MOMENTUM_CONSTANT, test_set)
    log('Network training ended')

    nn.test(test_set)

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
                inp = [np.array(el) for el in inp]
                inp = [el.reshape(len(el), 1) for el in inp]

                for i in range(len(inp)):
                    result = nn.classify(inp[i])
                    output_array = [
                        1 if i == result else 0 for i in range(10)]
                    print('{}. Digit: {}, array: {}'.format(
                        i+1, result, output_array))
        else:
            log("Invalid input!")
