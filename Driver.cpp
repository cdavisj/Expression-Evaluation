#include <iostream>
#include <fstream>
#include <string>

#include "stack.h"
#include "Console.h"

/*
*   ---------------------------------------------
*   |            FUNCTIONS FOR LOGIC            |
*   ---------------------------------------------
*/

// function for getting precedence of an operator
int getOperatorPrecedence(char op);

// function for applying an operator
int evaluateSubExpression(int num1, int num2, char op);

// function for evaluating main expression
int evaluateExpression(std::string);

/*
*   ---------------------------------------------
*   |          FUNCTIONS FOR ANIMATION          |
*   ---------------------------------------------
*/

void drawStacks(int startx, int starty);

void pushAnimation();

void popAnimation();

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
    int result = evaluateExpression(expression);

    // output the result of the evaluation
    std::cout << result << std::endl;

    drawStacks(10, 5);

    // pause console
    std::cin.get();
    return 0;
}

int getOperatorPrecedence(char op)
{
    if (op == '+' || op == '-') // addition or subtraction
    {
        // return 1 for lower precedence than multiplication and division
        return 1;
    }
    else if (op == '*' || op == '/') // multiplication or division
    {
        // return 2 for higher precedence than addition and subtraction
        return 2;
    }
    else if (op == '(' || op == ')') // opening or closing parenthesis
    {
        // return any number with no error message
        return 0;
    }
    else // invalid operator
    {
        // display error message
        std::cout << "Error: Invalid operator.\n";

        // return error number
        return -1;
    }
}

int evaluateSubExpression(int num1, int num2, char op)
{
    if (op == '+') // addition
    {
        // return the sum
        return num1 + num2;
    }
    else if (op == '-') // subtraction
    {
        // return the difference
        return num1 - num2;
    }
    else if (op == '*') // multiplication
    {
        // return the product
        return num1 * num2;
    }
    else if (op == '/') // division
    {
        // return the quotient
        return num1 / num2;
    }
    else // invalid operator
    {
        // display error message
        std::cout << "Error: Invalid operator.\n";

        // return error number
        return -1;
    }
}

int evaluateExpression(std::string expr)
{
    // create integer stack for numbers
    stack<int> nums;

    // create char stack for operators
    stack<char> ops;

    // iterate through the string for the expression
    for (int i = 0; i < (int)expr.length(); i++)
    {
        if (expr[i] == ' ') // space
        {
            // if we have a space, pass over it
            continue;
        }
        else if (expr[i] == '(') // opening parenthesis
        {
            // push opening parenthesis into operator stack
            std::cout << "pushing " << expr[i] << "\n";
            ops.push(expr[i]);
        }
        else if (isdigit(expr[i])) // digit
        {
            // initialize variable for the number found
            int num = 0;

            // iterate forward while we still have digits
            while (i < (int)expr.length() && isdigit(expr[i]))
            {
                // update num
                num = (num * 10) + (expr[i] - '0');

                // increment index
                i++;
            }

            // bring i back 1 to account for extra increment
            i--;

            // push resultant number into number stack
            std::cout << "pushing " << num << "\n";
            nums.push(num);
        }
        else if (expr[i] == ')') // closing parenthesis
        {
            // evaluate expression back until last opening parenthesis
            while (!ops.empty() && ops.top() != '(')
            {
                // pop number into num2
                std::cout << "popping " << nums.top() << "\n";
                int num2 = nums.pop();

                // pop operator
                std::cout << "popping " << ops.top() << "\n";
                char op = ops.pop();

                // pop number into num1
                std::cout << "popping " << nums.top() << "\n";
                int num1 = nums.pop();

                // evaluate the sub expression and push the result into the number stack
                std::cout << "pushing " << evaluateSubExpression(num1, num2, op) << "\n";
                nums.push(evaluateSubExpression(num1, num2, op));
            }

            // if there is still a closing parenthesis to pop, do so
            if (!ops.empty())
            {
                std::cout << "popping " << ops.top() << "\n";
                ops.pop();
            }
        }
        else // operator
        {
            // while the last operator pushed to the operator stack has
            // higher precedence than the operator encountered,
            // evaluate the last sub-expression
            while ((!ops.empty()) && getOperatorPrecedence(ops.top()) >= getOperatorPrecedence(expr[i]))
            {
                // pop number from number stack
                std::cout << "popping " << nums.top() << "\n";
                int num2 = nums.pop();

                // pop operator from operator stack
                std::cout << "popping " << ops.top() << "\n";
                char op = ops.pop();

                // pop another number from number stack
                std::cout << "popping " << nums.top() << "\n";
                int num1 = nums.pop();

                // evaluate sub-expression and push result to number stack
                std::cout << "pushing " << evaluateSubExpression(num1, num2, op) << "\n";
                nums.push(evaluateSubExpression(num1, num2, op));
            }

            // push operator to operator stack
            std::cout << "pushing " << expr[i] << "\n";
            ops.push(expr[i]);
        }
    }

    // evaluate what remains in the number and operator stack
    while (!ops.empty())
    {
        // pop top number into num2
        std::cout << "popping " << nums.top() << "\n";
        int num2 = nums.pop();

        // pop last operator
        std::cout << "popping " << ops.top() << "\n";
        char op = ops.pop();

        // pop next number into num1
        std::cout << "popping " << nums.top() << "\n";
        int num1 = nums.pop();

        // evaluate and push the result into numbers stack
        std::cout << "pushing " << evaluateSubExpression(num1, num2, op) << "\n";
        nums.push(evaluateSubExpression(num1, num2, op));

    }

    // pop the final number and return it
    std::cout << "popping " << nums.top() << "\n";
    return nums.pop();
}

void drawStacks(int startx, int starty)
{
    // for saving console cursor position at any time
    COORD pos;

    console::clear();
    console::showCursor(false);
    console::setTextColor(red);

    // starting point
    console::gotoxy(startx, starty);

    // left wall
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(down);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(down);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(down);
    std::cout << (char)219;
    console::sleep(250);
    
    // name of stack
    // save cursor position
    pos = console::getCursorPos();

    console::moveCursor(down);
    std::cout << "Nums";

    // go back to previous cursor position
    console::gotoxy(pos);

    // floor
    console::moveCursor(right);
    std::cout << (char)220;
    console::sleep(250);

    console::moveCursor(right);
    std::cout << (char)220;
    console::sleep(250);

    // right wall
    console::moveCursor(right);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(up);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(up);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(up);
    std::cout << (char)219;
    console::sleep(250);

    // starting point for second stack
    console::gotoxy(startx + 8, starty);

    // left wall
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(down);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(down);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(down);
    std::cout << (char)219;
    console::sleep(250);

    // name of stack
    // save cursor position
    pos = console::getCursorPos();

    console::moveCursor(down);
    std::cout << "Ops";

    // go back to previous cursor position
    console::gotoxy(pos);

    // floor
    console::moveCursor(right);
    std::cout << (char)220;
    console::sleep(250);

    console::moveCursor(right);
    std::cout << (char)220;
    console::sleep(250);

    // right wall
    console::moveCursor(right);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(up);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(up);
    std::cout << (char)219;
    console::sleep(250);

    console::moveCursor(up);
    std::cout << (char)219;
    console::sleep(250);

    console::setTextColor(white);
}

void pushAnimation()
{
}

void popAnimation()
{
}
