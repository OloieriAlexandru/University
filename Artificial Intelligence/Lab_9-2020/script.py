# https://pypi.org/project/lightrdf/
# # pip install lightrdf

import sys
import lightrdf

ONTOLOGY_FILE = './ignored/ontology.owl'

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print('Invalid number of arguments! Expected one, the word to be searched in the ontology!')
        exit(1)

    word = sys.argv[1]

    with open(ONTOLOGY_FILE, "rb") as file_in:
        doc = lightrdf.RDFDocument(
            file_in, parser=lightrdf.xml.PatternParser, base_iri=None)

        for triple in doc.search_triples(None, None, None):
            if triple[2].endswith('/' + word):
                print(triple, triple[2].split('/')[-1])
                exit(0)
            elif triple[2].endswith('#' + word):
                print(triple, triple[2].split('#')[-1])
                exit(0)

    print('The "{}" word wasn\'t found!'.format(word))
