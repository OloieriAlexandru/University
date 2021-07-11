#include "ExpressionParser.h"

double ExpressionParser::parseTerm(int& index) {
    if (index < len && exp[index] == '(') {
        ++index;
        double res = parseSumSub(index);
        ++index;
        return res;
    }
    if (!strncmp(exp+index, "pi", 2)) {
        index += 2;
        return PI;
    }
    double res = atof(exp+index);
    while (index < len && ((exp[index] >= '0' && exp[index] <= '9') || exp[index] == '.')) {
        ++index;
    }
    return res;
}

double ExpressionParser::parseMulDiv(int& index) {
    double res = parseTerm(index);
    while (index < len && (exp[index] == '*' || exp[index] == '/')) {
        bool mul = (exp[index++] == '*');
        double resRec = parseTerm(index);
        if (mul) {
            res *= resRec;
        } else {
            res /= resRec;
        }
    }
    return res;
}

double ExpressionParser::parseSumSub(int& index) {
    double res = parseMulDiv(index);
    while (index < len && (exp[index] == '+' || exp[index] == '-')) {
        bool add = (exp[index++] == '+');
        double resRec = parseMulDiv(index);
        if (add) {
            res += resRec;
        } else {
            res -= resRec;
        }
    }
    return res;
}

ExpressionParser::ExpressionParser(const char *e) {
    exp = e;
    len = strlen(e);
}

double ExpressionParser::parse() {
    int ind = 0;
    return parseSumSub(ind);
}

