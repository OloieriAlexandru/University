#include "DES.h"

const int DES::ip[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
						  60, 52, 44, 36, 28, 20, 12, 4,
						  62, 54, 46, 38, 30, 22, 14, 6,
						  64, 56, 48, 40, 32, 24, 16, 8,
						  57, 49, 41, 33, 25, 17, 9, 1,
						  59, 51, 43, 35, 27, 19, 11, 3,
						  61, 53, 45, 37, 29, 21, 13, 5,
						  63, 55, 47, 39, 31, 23, 15, 7 };

const int DES::fp[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
						  39, 7, 47, 15, 55, 23, 63, 31,
						  38, 6, 46, 14, 54, 22, 62, 30,
						  37, 5, 45, 13, 53, 21, 61, 29,
						  36, 4, 44, 12, 52, 20, 60, 28,
						  35, 3, 43, 11, 51, 19, 59, 27,
						  34, 2, 42, 10, 50, 18, 58, 26,
						  33, 1, 41, 9, 49, 17, 57, 25 };

const int DES::et[48] = { 32, 1, 2, 3, 4, 5,
						  4, 5, 6, 7, 8, 9,
						  8, 9, 10, 11, 12, 13,
						  12, 13, 14, 15, 16, 17,
						  16, 17, 18, 19, 20, 21,
						  20 , 21, 22, 23, 24, 25,
						  24, 25, 26, 27, 28, 29,
						  28, 29, 30, 31, 32, 1 };

const int DES::sb[8][4][16] = { { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                                  0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                                  4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                                  15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
                                { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                                  3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                                  0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                                  13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
                                { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                                  13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                                  13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                                  1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
                                { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                                  13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                                  10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                                  3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
                                { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                                  14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                                  4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                                  11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
                                { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                                  10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                                  9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                                  4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
                                { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                                  13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                                  1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                                  6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
                                { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                                  1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                                  7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                                  2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 } };

const int DES::pt[32] = { 16, 7, 20, 21,
						  29, 12, 28, 17,
						  1, 15, 23, 26,
						  5, 18, 31, 10,
						  2, 8, 24, 14,
						  32, 27, 3, 9,
						  19, 13, 30, 6,
						  22, 11, 4, 25 };

const int DES::st[16] = { 1, 1, 2, 2, 2, 2, 2, 2,
                          1, 2, 2, 2, 2, 2, 2, 1 };

const int DES::pc1[56] = { 57, 49, 41, 33, 25, 17, 9,
                           1, 58, 50, 42, 34, 26, 18,
                           10, 2, 59, 51, 43, 35, 27,
                           19, 11, 3, 60, 52, 44, 36,
                           63, 55, 47, 39, 31, 23, 15,
                           7, 62, 54, 46, 38, 30, 22,
                           14, 6, 61, 53, 45, 37, 29,
                           21, 13, 5, 28, 20, 12, 4 };

const int DES::pc2[48] = { 14, 17, 11, 24, 1, 5,
                           3, 28, 15, 6, 21, 10,
                           23, 19, 12, 4, 26, 8,
                           16, 7, 27, 20, 13, 2,
                           41, 52, 31, 37, 47, 55,
                           30, 40, 51, 45, 33, 48,
                           44, 49, 39, 56, 34, 53,
                           46, 42, 50, 36, 29, 32 };

std::vector<uc> DES::permute(const std::vector<uc>& arr, const int permutation[]) {
    std::vector<uc> res;

    for (int i = 0; i < arr.size(); ++i) {
        res.push_back(arr[permutation[i] - 1]);
    }

    return res;
}

std::vector<uc> DES::expand(const std::vector<uc>& arr, const int fe[], int len) {
    std::vector<uc> res;

    for (int i = 0; i < len; ++i) {
        res.push_back(arr[fe[i]-1]);
    }

    return res;
}

std::vector<uc> DES::shift(const std::vector<uc>& arr, int shiftValue) {
    std::vector<uc> res;

    for (int i = shiftValue; i < arr.size(); ++i) {
        res.push_back(arr[i]);
    }
    for (int i = 0; i < shiftValue; ++i) {
        res.push_back(arr[i]);
    }
    
    return res;
}

std::vector<uc> DES::combine(const std::vector<uc>& a, const std::vector<uc>& b) {
    std::vector<uc> res;

    for (auto x : a) {
        res.push_back(x);
    }
    for (auto x : b) {
        res.push_back(x);
    }

    return res;
}

std::vector<uc> DES:: xorr (const std::vector<uc>& a, const std::vector<uc>& b) {
    std::vector<uc> res;

    for (int i = 0; i < a.size(); ++i) {
        res.push_back((a[i] + b[i]) % 2);
    }

    return res;
}

std::vector<uc> DES::splitByteArr(const std::vector<uc>& arr, int half) {
    std::vector<uc> res;

    int from = (arr.size() / 2) * half;
    int to = from + (arr.size() / 2);
    for (int i = from; i < to; ++i) {
        res.push_back(arr[i]);
    }

    return res;
}

std::vector<uc> DES::byteArrRemoveParityBits(const std::vector<uc>& arr) {
    std::vector<uc> res;

    for (int i = 0; i < arr.size(); ++i) {
        if ((i + 1) % 8 == 0) {
            continue;
        }
        res.push_back(arr[i]);
    }

    return res;
}

std::vector<uc> DES::encryptChunk(const std::string& str, const std::string& key) {
    if (str.size() != 8) {
        throw new std::exception("The chunk supposed to be encrypted has to have 8 characters!");
    }

    std::vector<uc> byteStr = stringToByteArr(str);
    return encryptChunk(byteStr, key);
}

std::vector<uc> DES::encryptChunk(const std::vector<uc>& arr, const std::string& key) {
    return encryptChunk(arr, stringToByteArr(key));
}

std::vector<uc> DES::encryptChunk(const std::vector<uc>& arr, const std::vector<uc>& key) {
    std::vector<std::vector<uc>> keys = computeRoundKeys(key);

    std::vector<uc> permutedArr = permute(arr, ip);
    std::vector<uc> l = splitByteArr(permutedArr, 0);
    std::vector<uc> r = splitByteArr(permutedArr, 1);

    for (int i = 1; i <= 16; ++i) {
        std::vector<uc> xorr_ = xorr(l, f(r, keys[i - 1]));
        l = xorr_;

        if (i != 16) {
            std::vector<uc> aux = l;
            l = r;
            r = aux;
        }
    }

    return permute(combine(l, r), fp);
}

std::vector<uc> DES::decryptChunk(const std::vector<uc>& arr, const std::vector<uc>& key) {
    std::vector<std::vector<uc>> keys = computeRoundKeys(key);

    std::vector<uc> permutedArr = permute(arr, ip);
    std::vector<uc> l = splitByteArr(permutedArr, 0);
    std::vector<uc> r = splitByteArr(permutedArr, 1);

    for (int i = 16; i >= 1; --i) {
        std::vector<uc> xorr_ = xorr(l, f(r, keys[i - 1]));
        l = xorr_;

        if (i != 1) {
            std::vector<uc> aux = l;
            l = r;
            r = aux;
        }
    }

    return permute(combine(l, r), fp);
}

std::vector<uc> DES::decryptChunk(const std::vector<uc>& arr, const std::string& key) {
    return decryptChunk(arr, stringToByteArr(key));
}

std::vector<uc> DES::f(const std::vector<uc>& a, const std::vector<uc>& j) {
    std::vector<uc> b = xorr(expand(a, et, 48), j);

    std::vector<std::vector<uc>> bParts = splitIn6BitArrays(b);
    std::vector<uc> c;
    for (int i = 0; i < bParts.size(); ++i) {
        int line = getSBoxLine(bParts[i]);
        int column = getSBoxColumn(bParts[i]);

        std::vector<uc> bin = intToBinary(sb[i][line][column]);
        for (auto x : bin) {
            c.push_back(x);
        }
    }

    return permute(c, pt);
}

std::vector<std::vector<uc>> DES::splitIn6BitArrays(const std::vector<uc>& arr) {
    std::vector<std::vector<uc>> arrays;

    for (int i = 0; i < 8; ++i) {
        int from = i * 6;
        int to = from + 6;
        std::vector<uc> thisArr;
        for (int j = from; j < to; ++j) {
            thisArr.push_back(arr[j]);
        }
        arrays.push_back(thisArr);
    }

    return arrays;
}

std::vector<uc> DES::intToBinary(int e) {
    std::vector<uc> arr;

    std::bitset<4> bs(e);
    for (int i = 3; i >= 0; --i) {
        arr.push_back(bs.test(i));
    }

    return arr;
}

int DES::getSBoxLine(const std::vector<uc>& arr) {
    return 2 * arr[0] + arr[5];
}

int DES::getSBoxColumn(const std::vector<uc>& arr) {
    int res = 0;
    for (int i = 1; i < 5; ++i) {
        res = res * 2 + arr[i];
    }
    return res;
}

void DES::printRoundKeys(const std::vector<std::vector<uc>>& keys) {
    std::cout << "The round keys:\n";
    for (int i = 0; i < keys.size(); ++i) {
        std::cout << (i + 1) << ". ";
        for (int j = 0; j < keys[i].size(); ++j) {
            std::cout << (int)keys[i][j];
        }
        std::cout << '\n';
    }
}

std::vector<std::vector<uc>> DES::computeRoundKeys(const std::string& key) {
    if (key.size() != 8) {
        throw new std::exception("The key has to have 8 characters!");
    }

    std::vector<uc> byteKey = stringToByteArr(key);
    return computeRoundKeys(byteKey);
}

std::vector<std::vector<uc>> DES::computeRoundKeys(std::vector<uc> key) {
    std::vector<std::vector<uc>> keys;

    key = expand(key, pc1, 56);
    std::vector<uc> c = splitByteArr(key, 0);
    std::vector<uc> d = splitByteArr(key, 1);
    for (int i = 1; i <= 16; ++i) {
        c = shift(c, st[i - 1]);
        d = shift(d, st[i - 1]);
        
        std::vector<uc> roundKey = expand(combine(c, d), pc2, 48);
        keys.push_back(roundKey);
    }

    return keys;
}

std::vector<uc> DES::encrypt(const std::string& str, const std::string& key) {
    std::string copyStr = str;
    while (copyStr.size() % 8) {
        copyStr.push_back(0);
    }

    std::vector<uc> res;
    for (int i = 0; i < copyStr.size(); i += 8) {
        std::vector<uc> encrypted = encryptChunk(copyStr.substr(i, 8), key);
        for (auto x : encrypted) {
            res.push_back(x);
        }
    }

    return res;
}

std::vector<uc> DES::encrypt(const std::vector<uc>& arr, const std::string& key) {
    return encrypt(arr, stringToByteArr(key));
}

std::vector<uc> DES::encrypt(const std::vector<uc>& arr, const std::vector<uc>& key) {
    std::vector<uc> copyArr = arr;
    while (copyArr.size() % 64) {
        copyArr.push_back(0);
    }

    std::vector<uc> res;
    for (int i = 0; i < copyArr.size(); i += 64) {
        std::vector<uc> chunk;
        for (int j = i; j < i + 64; ++j) {
            chunk.push_back(copyArr[j]);
        }
        std::vector<uc> encrypted = encryptChunk(chunk, key);
        for (auto x : encrypted) {
            res.push_back(x);
        }
    }

    return res;
}

std::vector<uc> DES::decrypt(const std::vector<uc>& arr, const std::string& key) {
    return decrypt(arr, stringToByteArr(key));
}

std::vector<uc> DES::decrypt(const std::vector<uc>& arr, const std::vector<uc>& key) {
    std::vector<uc> copyArr = arr;
    while (copyArr.size() % 64) {
        copyArr.push_back(0);
    }

    std::vector<uc> res;
    for (int i = 0; i < copyArr.size(); i += 64) {
        std::vector<uc> chunk;
        for (int j = i; j < i + 64; ++j) {
            chunk.push_back(copyArr[j]);
        }
        std::vector<uc> decrypted = decryptChunk(chunk, key);
        for (auto x : decrypted) {
            res.push_back(x);
        }
    }

    return res;
}
