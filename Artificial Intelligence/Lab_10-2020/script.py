import os
import nltk
import re
import json
from nltk.corpus import stopwords

stopwords = stopwords.words('romanian')
rgx_words = re.compile(r"(\w[\w']*\w|\w)")

MUL = 1000000


def process_file(file_name):
  if not os.path.isfile(file_name):
    print("Invalid file!")
    return

  file_words = {}
  file_scores = {}
  number_of_words = 0

  with open(file_name, "r") as file_in:
    lines = file_in.readlines()
    content = ' '.join(
        list(filter(lambda x: len(x) > 0, [l.strip() for l in lines])))

    for word_info in rgx_words.finditer(content):
      word = word_info.group().lower()
      if word in stopwords or len(word) < 2:
        continue
      if word not in file_words:
        file_words[word] = 0
      file_words[word] += 1
      number_of_words += 1

  for key in file_words:
    score = file_words[key] / number_of_words
    file_scores[key] = score * MUL

  new_name = 'ignored/' + \
      os.path.splitext(os.path.basename(file_name))[0] + '.json'

  with open(new_name, "w") as file_out:
    file_out.write(json.dumps(file_scores))


def load_json(file_name):
  with open(file_name, "r") as file_in:
    content = file_in.read().replace('\n', '').replace('\t', '').replace(' ', '')
    return json.loads(content)


def compare_files(file_name_1, file_name_2):
  full_name_1 = 'ignored/' + file_name_1
  full_name_2 = 'ignored/' + file_name_2

  if not os.path.isfile(full_name_1) or not os.path.isfile(full_name_2):
    print("Invalid file names!")
    return

  json_1 = load_json(full_name_1)
  json_2 = load_json(full_name_2)

  score = 0
  number_of_words = 0

  for key in json_1:
    if key in json_2:
      score += abs(json_1[key] - json_2[key])
    else:
      score += abs(json_1[key])
    number_of_words += 1

  for key in json_2:
    if key not in json_1:
      score += abs(json_2[key])
      number_of_words += 1

  score_mean = score / number_of_words

  print('Scor de similitudine: {}\nMedia diferentei: {}'.format(score, score_mean))


if __name__ == "__main__":
  commands = [
      'process file_name',
      'compare file_name_1, file_name_2',
      'quit'
  ]
  print('Available commands:')
  for command in commands:
    print('- {}'.format(command))
  while True:
    s = input('Enter the command: ')
    parts = s.split(' ')

    if len(parts) == 1 and parts[0] == 'quit':
      break
    elif len(parts) == 2 and parts[0] == 'process':
      process_file(parts[1])
    elif len(parts) == 3 and parts[0] == 'compare':
      compare_files(parts[1], parts[2])
