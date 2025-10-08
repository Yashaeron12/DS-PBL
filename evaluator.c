#include <ctype.h>
#include <math.h>
#include "evaluator.h"
#include<stdlib.h>

typedef struct {
double items[256];
int top;
} NumStack;


static void pushNum(NumStack* s, double val) {
s->items[++(s->top)] = val;
}


static double popNum(NumStack* s) {
return s->items[(s->top)--];
}


double evaluate_postfix(char* postfix) {
    NumStack s;
    s.top = -1;

    for (int i = 0; postfix[i]; ) {
        if (isdigit(postfix[i])) {
            char numStr[32];
            int j = 0;
            // Read full number
            while (isdigit(postfix[i])) {
                numStr[j++] = postfix[i++];
            }
            numStr[j] = '\0';
            pushNum(&s, atof(numStr));
        }
        else if (postfix[i] == ' ') {
            i++; // skip spaces
        }
        else {
            double b = popNum(&s);
            double a = popNum(&s);
            switch (postfix[i]) {
                case '+': pushNum(&s, a + b); break;
                case '-': pushNum(&s, a - b); break;
                case '*': pushNum(&s, a * b); break;
                case '/': pushNum(&s, a / b); break;
                case '^': pushNum(&s, pow(a, b)); break;
            }
            i++;
        }
    }
    return popNum(&s);
}
