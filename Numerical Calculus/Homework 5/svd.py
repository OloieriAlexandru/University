import numpy as np

if __name__ == '__main__':
    mat = np.array(
        [[0, 0, 1],
        [0, 0, 1],
        [0, 1, 1]]
    )

    u, s, vh = np.linalg.svd(mat, full_matrices=True)
    print(s)
    print(vh)
    print(u)
    