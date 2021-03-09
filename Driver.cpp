#include <iostream>
#include <fstream>
#include <string>

#include "stack.h"

// function for getting precedence of an operator
int precedence(char op) 
{
    if (op == '+' || op == '-')
        return 1;
    else if (op == '*' || op == '/')
        return 2;

    // invalid operator
    return -1;
}

// function for applying an operator
int applyOp(int a, int b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;

    default:
        // invalid operator
        return -1;
    }
}

int evaluate(std::string expr) 
{
    // create integer stack for values
    stack<int> values;

    // create char stack for operators
    stack<char> ops;

    // iterate through the string for the expression
    for (int i = 0; i < expr.length(); i++)
    {
        if (expr[i] == ' ')
        {
            // if we have a space, pass over it
            continue;
        }
        else if (expr[i] == '(') 
        {
            // push opening parenthesis into operator stack
            std::cout << "pushing " << expr[i] << "\n";
            ops.push(expr[i]);
        }
        else if (isdigit(expr[i])) 
        {
            int val = 0;
            int j = i;

            while (j < expr.length() && isdigit(expr[j]))
            {
                val = (val * 10) + (expr[j] - '0');
                j++;
            }

            std::cout << "pushing " << val << "\n";
            values.push(val);
        } 
        else if (expr[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                std::cout << "popping " << values.top() << "\n";
                int val2 = values.pop();

                std::cout << "popping " << ops.top() << "\n";
                char op = ops.pop();

                std::cout << "popping " << values.top() << "\n";
                int val1 = values.pop();

                std::cout << "pushing " << applyOp(val1, val2, op) << "\n";
                values.push(applyOp(val1, val2, op));
            }

            if (!ops.empty())
            {
                std::cout << "popping " << ops.top() << "\n";
                ops.pop();
            }
        } 
        else
        {
            while ((!ops.empty()) && precedence(ops.top()) >= precedence(expr[i])) 
            {
                std::cout << "popping " << values.top() << "\n";
                int val2 = values.pop();

                std::cout << "popping " << ops.top() << "\n";
                char op = ops.pop();

                std::cout << "popping " << values.top() << "\n";
                int val1 = values.pop();

                std::cout << "pushing " << applyOp(val1, val2, op) << "\n";
                values.push(applyOp(val1, val2, op));
            }

            std::cout << "pushing " << expr[i] << "\n";
            ops.push(expr[i]);
        }
    }

    // evaluate what remains in the stacks
    while (!ops.empty()) 
    {
        // pop top value into val2
        std::cout << "popping " << values.top() << "\n";
        int val2 = values.pop();

        // pop last operator
        std::cout << "popping " << ops.top() << "\n";
        char op = ops.pop();

        // pop next value into val1
        std::cout << "popping " << values.top() << "\n";
        int val1 = values.pop();

        // evaluate and push the result into values stack
        std::cout << "pushing " << applyOp(val1, val2, op) << "\n";
        values.push(applyOp(val1, val2, op));

    }

    // pop the final value and return it
    std::cout << "popping " << values.top() << "\n";
    return values.pop();
}

int main()
{
    // open file containing expression
    std::ifstream file("c:\\temp\\input.txt");

    // create string for expression
    std::string expression;

    // make sure file opened correctly
    if (file.is_open())
    {
        // store expression in string
        std::getline(file, expression);

        // close file
        file.close();
    }
    else
    {
        // display error message for file not opening
        std::cout << "Error: File could not be opened.\n";
    }
    
    // display expression from file
    std::cout << expression << std::endl;

    // run evaluation and assign its result to a variable
    int result = evaluate(expression);

    // output the result of the evaluation
    std::cout << result << std::endl;

    // pause console
    std::cin.get();
    return 0;
}