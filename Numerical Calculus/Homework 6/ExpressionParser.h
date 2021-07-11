#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <cstring>
#include <cstdlib>

extern const double PI;

class ExpressionParser {
    private:
        int len;

        const char *exp;

        double parseT(int& index);

        double parseTerm(int& index);

        double parseMulDiv(int& index);

        double parseSumSub(int& index);
    public:
        ExpressionParser(const char *e);

        double parse();
};

#endif // EXPRESSION_PARSER_H

