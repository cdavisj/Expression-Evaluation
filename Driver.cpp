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
int precedence(char);

// function for applying an operator
int applyOp(int, int, char);

// function for evaluating main expression
int evaluate(std::string);

/*
*   ---------------------------------------------
*   |          FUNCTIONS FOR ANIMATION          |
*   ---------------------------------------------
*/

#define wall (char)219
#define floor (char)219

std::string expression;

int sleep_time = 50;

coord stack_origin = coord(20, 10);

int stack_width = 5;

int stack_height = 7;

int stack_gap = 21;

coord expression_origin;

coord num_stack_bottom_pos = coord(stack_origin.x + 2, stack_origin.y + stack_height - 2);

coord num_stack_top_pos = coord(stack_origin.x + 2, stack_origin.y);

coord op_stack_bottom_pos = coord(stack_origin.x + 2 + stack_width + stack_gap, stack_origin.y + stack_height - 2);

coord op_stack_top_pos = coord(stack_origin.x + 2 + stack_width + stack_gap, stack_origin.y);

int sub_expression_len = 9;

coord sub_expression_pos = coord(stack_origin.x + stack_width + (stack_gap - sub_expression_len) / 2, stack_origin.y + stack_height / 2);

void drawStacks();

void pushAnimation();

void popAnimation();

int main()
{
    // open file containing expression
    std::ifstream file("c:\\temp\\input.txt");

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

    // assign expression_origin
    expression_origin = coord(stack_origin.x + ((stack_width * 2 + stack_gap) - (int)expression.size()) / 2,
                              stack_origin.y - 4);

    // draw stacks
    drawStacks();

    // pause console
    std::cin.get();

    // run evaluation and assign its result to a variable
    int result = evaluate(expression);

    // pause console
    std::cin.get();
    return 0;
}

int precedence(char op)
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

int applyOp(int num1, int num2, char op)
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

int evaluate(std::string expression)
{
    // create integer stack for numbers
    stack<int> nums;

    // create char stack for operators
    stack<char> ops;

    // iterate through the string for the expression
    for (int i = 0; i < (int)expression.length(); i++)
    {
        if (expression[i] == ' ') // space
        {
            // if we have a space, pass over it
            continue;
        }
        else if (expression[i] == '(') // opening parenthesis
        {
            // push opening parenthesis into operator stack
            std::cout << "pushing " << expression[i] << "\n";
            ops.push(expression[i]);
        }
        else if (isdigit(expression[i])) // digit
        {
            // initialize variable for the number found
            int num = 0;

            // iterate forward while we still have digits
            while (i < (int)expression.length() && isdigit(expression[i]))
            {
                // update num
                num = (num * 10) + (expression[i] - '0');

                // increment index
                i++;
            }

            // bring i back 1 to account for extra increment
            i--;

            // push resultant number into number stack
            std::cout << "pushing " << num << "\n";
            nums.push(num);
        }
        else if (expression[i] == ')') // closing parenthesis
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
                std::cout << "pushing " << applyOp(num1, num2, op) << "\n";
                nums.push(applyOp(num1, num2, op));
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
            while ((!ops.empty()) && precedence(ops.top()) >= precedence(expression[i]))
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
                std::cout << "pushing " << applyOp(num1, num2, op) << "\n";
                nums.push(applyOp(num1, num2, op));
            }

            // push operator to operator stack
            std::cout << "pushing " << expression[i] << "\n";
            ops.push(expression[i]);
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
        std::cout << "pushing " << applyOp(num1, num2, op) << "\n";
        nums.push(applyOp(num1, num2, op));

    }

    // pop the final number and return it
    std::cout << "popping " << nums.top() << "\n";
    return nums.pop();
}

void drawStacks()
{
    // strings for stack tags
    std::string numTag = "Numbers";
    std::string opTag = "Operators";

    // clear screen and hide cursor
    console::clear();
    console::showCursor(false);

    // go to expression origin
    console::gotoxy(expression_origin);

    // set color for expression
    console::setTextColor(yellow);

    // display expression
    for (int i = 0; i < (int)expression.size(); i++)
    {
        // display character
        std::cout << expression[i];

        // sleep for non-space characters
        if (expression[i] != ' ')
            console::sleep(sleep_time);
    }


    /*
    *   NUMBER STACK ANIMATION
    */

    // starting point for stacks
    console::gotoxy(stack_origin);

    // set color for nums stack
    console::setTextColor(blue);

    // left wall
    std::cout << wall;
    console::sleep(sleep_time);

    for (int i = 0; i < stack_height - 1; i++)
    {
        console::moveCursor(left);
        console::moveCursor(down);
        std::cout << wall;
        console::sleep(sleep_time);
    }
    
    // save cursor position
    coord pos = console::getCursorPos();

    // position cursor for num stack tag
    console::moveCursor(left);
    console::moveCursor(left);
    console::moveCursor(down);
    
    // save position num stack tag
    coord numTagPos = console::getCursorPos();

    // go back to previous cursor position
    console::gotoxy(pos);

    // output floor
    for (int i = 0; i < stack_width - 2; i++)
    {
        std::cout << floor;
        console::sleep(sleep_time);
    }

    // right wall
    std::cout << wall;
    console::sleep(sleep_time);

    for (int i = 0; i < stack_height - 1; i++)
    {
        console::moveCursor(left);
        console::moveCursor(up);
        std::cout << wall;
        console::sleep(sleep_time);
    }


    /*
    *   OPERATOR STACK ANIMATION
    */

    // set color for nums stack
    console::setTextColor(red);

    // starting point for second stack
    console::gotoxy(stack_origin.x + stack_width + stack_gap, stack_origin.y);

    // left wall
    std::cout << wall;
    console::sleep(sleep_time);

    for (int i = 0; i < stack_height - 1; i++)
    {
        console::moveCursor(left);
        console::moveCursor(down);
        std::cout << wall;
        console::sleep(sleep_time);
    }

    // save cursor position
    pos = console::getCursorPos();

    // position cursor for operator tag
    console::moveCursor(left);
    console::moveCursor(left);
    console::moveCursor(left);
    console::moveCursor(down);

    // save position for operator tag
    coord opTagPos = console::getCursorPos();

    // go back to previous cursor position
    console::gotoxy(pos);

    // floor
    for (int i = 0; i < stack_width - 2; i++)
    {
        std::cout << floor;
        console::sleep(sleep_time);
    }

    // right wall
    std::cout << wall;
    console::sleep(sleep_time);

    for (int i = 0; i < stack_height - 1; i++)
    {
        console::moveCursor(left);
        console::moveCursor(up);
        std::cout << wall;
        console::sleep(sleep_time);
    }

    // go to coord for number stack tag
    console::gotoxy(numTagPos);

    // set color for number stack tag
    console::setTextColor(blue);

    // output number stack tag
    for (int i = 0; i < (int)numTag.size(); i++)
    {
        std::cout << numTag[i];
        console::sleep(sleep_time);
    }

    // go to coord for number stack tag
    console::gotoxy(opTagPos);

    // set color for operator stack tag
    console::setTextColor(red);

    // output operator stack tag
    for (int i = 0; i < (int)opTag.size(); i++)
    {
        std::cout << opTag[i];
        console::sleep(sleep_time);
    }

    // testing
    console::setTextColor(yellow);

    console::gotoxy(num_stack_bottom_pos);
    std::cout << "n";

    console::gotoxy(num_stack_top_pos);
    std::cout << "n";

    console::gotoxy(op_stack_bottom_pos);
    std::cout << "o";

    console::gotoxy(op_stack_top_pos);
    std::cout << "o";

    console::gotoxy(sub_expression_pos);
    std::cout << "s u b = r";

    // reset text color to white
    console::setTextColor(white);
}

void pushAnimation()
{
}

void popAnimation()
{
}
