from nltk.corpus import wordnet as wn

import sys
import re

# https://stackoverflow.com/questions/12705293/regex-to-split-words-in-python
# https://stackoverflow.com/questions/250271/python-regex-how-to-get-positions-and-values-of-matches
# https://stackoverflow.com/questions/25735644/python-regex-for-splitting-text-into-sentences-sentence-tokenizing
def solve(file_name):
    rgx_words = re.compile(r"(\w[\w']*\w|\w)")
    rgx_sentence = re.compile(r'([A-Z][^\.!?]*[\.!?])', re.M)
    with open(file_name) as file_in:
        text = file_in.read().strip()
        initial_text = list(text)
        print('----- Initial text: ')
        print(text)

        print('----- Sentences: ')
        sentence_replacements = []
        for sentence_info in rgx_sentence.finditer(text):
            sentence = sentence_info.group()
            print(sentence)
            initial_sentence = list(sentence)

            replacements = []
            for word_info in rgx_words.finditer(sentence):
                word = word_info.group()
                syn_found = False
                for syn in wn.synsets(word):
                    if str(syn).split('.')[1] == 'n': # the first noun we found
                        syn_found = True
                        break
                    
                if syn_found == False or len(syn.hypernyms()) <= 0 or len(syn.hypernyms()[0].lemma_names()) <= 0: # there aren't any nouns
                    continue

                repl = syn.hypernyms()[0].lemma_names()[0] # the replacement string
                replacements.append((word_info.start(), word_info.end(), repl, syn)) # information to replace the word
            
            replacements.reverse()
            for repl in replacements: # replace the words
                initial_sentence[repl[0]:repl[1]] = list(repl[2])

            max_distance = 0
            for i in range(len(replacements)-1):
                for j in range(i+1, len(replacements)):
                    max_distance = max(max_distance, replacements[i][3].shortest_path_distance(replacements[j][3]))

            initial_sentence = ''.join(initial_sentence)
            initial_sentence += (' ' + str(max_distance))
            sentence_replacements.append((sentence_info.start(), sentence_info.end(), initial_sentence)) # information to replace the sentence

        sentence_replacements.reverse()
        for repl in sentence_replacements: # replace the sentences
            initial_text[repl[0]:repl[1]] = list(repl[2]) 

        initial_text = ''.join(initial_text)
        print('----- Final text: ')
        print(initial_text)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Invalid number of arguments!")
    else:
        solve(sys.argv[1])
