import csv
import numpy as np
import re

from gensim.models import Word2Vec

DATASET_PATH = 'ignored/IMDB Dataset.csv'
rgx_words = re.compile(r"(\w[\w']*\w|\w)")


def print_delimiter_line():
    print('-----------------------------------')


if __name__ == "__main__":
    # https://docs.python.org/3/library/csv.html
    print_delimiter_line()
    print('Reading reviews from "{}" file'.format(DATASET_PATH))
    with open(DATASET_PATH, 'r', encoding="utf-8") as csv_file:
        reader = csv.reader(csv_file)
        dataset = list(reader)[1:]
    print('Reviews were read!')

    print_delimiter_line()
    print('Splitting the reviews into words!')
    words = [info[0] for info in dataset]
    words = [rgx_words.split(text.strip()) for text in words]
    print('Splitting the reviews into words! - done')

    print_delimiter_line()
    print('Building the Word2Vec model!')
    w2v_model = Word2Vec(words, min_count=1)
    print('Building the Word2Vec model! - done')

    # https://www.ef.com/ca/english-resources/english-vocabulary/top-50-nouns/
    print_delimiter_line()
    words_list = ['hand', 'family', 'night', 'number',
                  'student', 'world', 'book', 'job', 'man', 'money']
    for word in words_list:
        print('{} - {}'.format(word, w2v_model.wv.most_similar(positive=word, topn=3)))
