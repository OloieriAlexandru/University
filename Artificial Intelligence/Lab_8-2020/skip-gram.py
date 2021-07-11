# Documentation for bonus task:
# https://www.kaggle.com/jeffd23/visualizing-word-vectors-with-t-sne/comments

# https://constitutioncenter.org/interactive-constitution/interpretation/article-ii/clauses/347
# https://constitutioncenter.org/interactive-constitution/interpretation/article-ii/clauses/345
# https://constitutioncenter.org/interactive-constitution/interpretation/article-ii/clauses/348

import numpy as np
import random

import sklearn.manifold
import matplotlib.pyplot as plt

import re
import copy

from gensim.models import Word2Vec


STOPWORDS_PATH = './ignored/english.txt'
# FILE_PATH = './ignored/text.txt'
FILE_PATH = './ignored/text_short.txt'
CONTEXT_SIZE = 3
ITERATIONS_COUNT = 10
SIMILAR_WORDS_COUNT = 8
HIDDEN_LAYER_SIZE = 150
LEARNING_RATE = 0.01


def softmax(z):
    v = np.exp(z - np.max(z))
    return v / np.sum(v)


class Randomizer:
    def __init__(self, size):
        self.permutation = [i for i in range(size)]

    def get_random_permutation(self):
        random.shuffle(self.permutation)
        return self.permutation


class Neural_Network:
    def __init__(self, words_count, hidden_layer_size):
        self.sizes = [words_count, hidden_layer_size]

        self.weights = []

        # self.weights.append(np.random.uniform(-1.0, 1.0, (words_count, hidden_layer_size)))
        # self.weights.append(np.random.uniform(-1.0, 1.0, (hidden_layer_size, words_count)))

        self.weights.append(np.random.randn(words_count,
                                            hidden_layer_size) / np.sqrt(words_count))
        self.weights.append(np.random.randn(hidden_layer_size,
                                            words_count) / np.sqrt(hidden_layer_size))

        self.v = words_count
        self.n = len(self.weights)

    def train(self, inputs, outputs, iterations, learning_rate):
        n = len(inputs)
        rnd = Randomizer(n)

        last_error = 99999999999999999999999999999999999999999999999999
        _ = 0
        tries = 0
        while _ < iterations:
            perm = rnd.get_random_permutation()

            oldweights = copy.deepcopy(self.weights)

            error = 0
            for wh in perm:
                error += self.process(inputs[wh], outputs[wh], learning_rate)

            if error > last_error:
                if tries > 5:
                    break
                tries += 1
                self.weights = oldweights
                continue
            tries = 0
            last_error = error
            print('Iteration {} - loss {}'.format(_, error))
            learning_rate *= 1.0 / (1.0 + learning_rate * (_+1))
            _ += 1

    def process(self, inpp, outt, learning_rate):
        delta_weights = [np.zeros(weight.shape) for weight in self.weights]
        layers_outputs, y = self.feed_forward(inpp)

        error = y - outt

        delta_weights[-1] = np.dot(layers_outputs[-2], error.T)
        delta_weights[-2] = np.dot(inpp, np.dot(self.weights[-1], error).T)

        for i in range(len(delta_weights)):
            self.weights[i] = self.weights[i] + \
                learning_rate * delta_weights[i]

        w_error = 0
        c = 0
        for i in range(self.v):
            if outt[i][0] == 1:
               error += -1 * layers_outputs[-1][i][0]
               c += 1
        w_error += c * np.log(np.sum(np.exp(layers_outputs[-1])))

        return w_error

    def feed_forward(self, x):
        layers_outputs = []

        for i in range(self.n):
            x = np.dot(self.weights[i].T, x)
            if i + 1 < self.n:
                x = x.reshape(self.sizes[i+1], 1)
            layers_outputs.append(x)

        return layers_outputs, softmax(layers_outputs[-1])

    def predict(self, inpp):
        _, y = self.feed_forward(inpp)
        return y


def is_number(string):
    if string.isdigit() or string[0] == '-' and string[1:].isdigit():
        return True
    return False


def load_stopwords():
    with open(STOPWORDS_PATH, "r") as file_in:
        file_content = file_in.readlines()
        return set([f.strip() for f in file_content])


def build_nn(words_idx, sentences):
    nn = Neural_Network(len(words_idx), HIDDEN_LAYER_SIZE)

    v = len(words_idx)
    inputs = []
    outputs = []
    for sentence in sentences:
        for idx, w in enumerate(sentence):
            inpp = np.zeros((v, 1)).reshape(v, 1)
            outt = np.zeros((v, 1)).reshape(v, 1)

            inpp[words_idx[w]][0] = 1

            start_idx = max(0, idx - CONTEXT_SIZE)
            end_idx = min(len(sentence) - 1, idx + CONTEXT_SIZE)

            while start_idx <= end_idx:
                if sentence[start_idx] != w:
                    outt[words_idx[sentence[start_idx]]][0] = 1
                start_idx += 1

            inputs.append(inpp)
            outputs.append(outt)

    nn.train(inputs, outputs, ITERATIONS_COUNT, LEARNING_RATE)

    return nn


def get_closest_words(w, words_idx, nn, idx_words):
    inpp = np.zeros((len(words_idx), 1))
    inpp[words_idx[w]][0] = 1

    nn_ans = nn.predict(inpp)
    probabilities = []
    for idx, pb in enumerate(nn_ans):
        probabilities.append((pb, idx))

    probabilities = sorted(probabilities, reverse=True, key=lambda x: x[0])

    ans = []
    to = min(len(probabilities), SIMILAR_WORDS_COUNT)
    found = False
    for i in range(to):
        if w == idx_words[probabilities[i][1]]:
            found = True
        else:
            ans.append(idx_words[probabilities[i][1]])
    if found and to < len(probabilities):
        ans.append(idx_words[probabilities[to][1]])

    return ans


def print_ans_for_word(nn, w, words_idx, idx_words, library_model):
    ans = "the word is missing from the training data!"
    ans_model = "the word is missing from the training data!"

    if w in words_idx:
        ans = get_closest_words(w, words_idx, nn, idx_words)
        ans_model = library_model.similar_by_vector(
            w, topn=SIMILAR_WORDS_COUNT)

    print('----')
    print('\"{}\" - {}'.format(w, ans))
    print('\"{}\" - {}'.format(w, ans_model))


def plot_model(model, sz):
    labels = []
    tokens = []

    for el in model:
        tokens.append(el[1].reshape(sz))
        labels.append(el[0])

    tsne_model = sklearn.manifold.TSNE(perplexity=40, n_components=2,
                                       init='pca', n_iter=2500, random_state=23)
    new_values = tsne_model.fit_transform(tokens)

    x = []
    y = []
    for value in new_values:
        x.append(value[0])
        y.append(value[1])

    plt.figure(figsize=(16, 16))
    for i in range(len(x)):
        plt.scatter(x[i], y[i])
        plt.annotate(labels[i],
                     xy=(x[i], y[i]),
                     xytext=(5, 2),
                     textcoords='offset points',
                     ha='right',
                     va='bottom')
    plt.show()


def solve(file_name):
    # https://stackoverflow.com/questions/12705293/regex-to-split-words-in-python
    # https://stackoverflow.com/questions/250271/python-regex-how-to-get-positions-and-values-of-matches
    # https://stackoverflow.com/questions/25735644/python-regex-for-splitting-text-into-sentences-sentence-tokenizing
    stopwords = load_stopwords()
    rgx_words = re.compile(r"(\w[\w']*\w|\w)")
    rgx_sentence = re.compile(r'([A-Z][^\.!?]*[\.!?])', re.M)
    sentences = []
    all_words = set()
    words_idx = {}
    idx_words = {}

    with open(file_name, encoding="UTF-8") as file_in:
        text = file_in.read().strip()
        text += '.'
        print('----- Initial text: ')
        print(text)

        print('----- Sentences: ')
        for sentence_info in rgx_sentence.finditer(text):
            sentence = sentence_info.group()
            print(sentence)

            sentence_words = []
            for word_info in rgx_words.finditer(sentence):
                word = word_info.group()

                if is_number(word):
                    continue
                word = word.lower()
                if word in stopwords:
                    # print('STOPWORD - {}'.format(word))
                    continue
                sentence_words.append(word)
                all_words.add(word)

            sentences.append(sentence_words)

            print('Words: {}'.format(sentence_words))

    for idx, word in enumerate(all_words):
        words_idx[word] = idx
        idx_words[idx] = word

    nn = build_nn(words_idx, sentences)
    v = len(all_words)
    model = []
    for w in all_words:
        inpp = np.zeros((len(words_idx), 1))
        inpp[words_idx[w]][0] = 1

        layers_outputs, _ = nn.feed_forward(inpp)

        model.append((w, layers_outputs[1]))
        # print(layers_outputs[0].shape)

        # c_words = get_closest_words(w, words_idx, nn, idx_words)
        # w_m = np.zeros((v, 1))
        # for c_word in c_words:
        #     w_m[words_idx[c_word]][0] = 1
        # model.append((w, w_m))
    plot_model(model, v)

    library_model = Word2Vec(sentences, min_count=1)

    print('------------------------------------')
    print('Enter words:')
    while True:
        line = input()
        words = [w for w in line.strip().split(' ') if len(w) > 0]

        if len(words) == 0:
            continue
        for w in words:
            print_ans_for_word(nn, w, words_idx, idx_words, library_model)


if __name__ == "__main__":
    solve(FILE_PATH)
