#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

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
*   |                  ANIMATION                |
*   ---------------------------------------------
*/

// preprocessor definitions for wall and floor characters
#define wall (char)219
#define floor (char)219

// string for expression
std::string expression;

// create integer stack for numbers
stack<int> number_stack;

// create char stack for operators
stack<char> operator_stack;

// sleep time between each update
int sleep_time = 50;

// location of the beginning of the stacks
coord stack_origin = coord(20, 10);

// width of stacks
int stack_width = 7;

// height of stacks
int stack_height = 10;

// gap beween stacks
int stack_gap = 30;

// space gap between sub-expression arguments
int sub_expr_gap = 4;

// for expression loction above stacks
coord expression_origin;

// location of bottom of number stack
coord num_stack_bottom_pos = coord(stack_origin.x + 3, stack_origin.y + stack_height - 2);

// location of top of number stack
coord num_stack_top_pos = coord(stack_origin.x + 3, stack_origin.y);

// location of bottom of operator stack
coord op_stack_bottom_pos = coord(stack_origin.x + 3 + stack_width + stack_gap, stack_origin.y + stack_height - 2);

// location of top of operator stack
coord op_stack_top_pos = coord(stack_origin.x + 3 + stack_width + stack_gap, stack_origin.y);

// length of subexpression for aligning
int sub_expression_len = sub_expr_gap * 5;

// position beween stacks for the beginning of the subexpressions
coord sub_expression_pos = coord(stack_origin.x + stack_width + (stack_gap - sub_expression_len) / 2, 
                                stack_origin.y + stack_height / 2);

// print spaces over an integer passed
void printSpaces(int);

// draw initial stacks
void drawStacks();

// animation for pushing numbers to the number stack
void pushNumAnimation(int, int);

// animation for pushing sub-expression results to the number stack
void pushSubExpressionResultAnimation(int);

// animation for pushing operators to the operator stack
void pushOpAnimation(int, char);

// animation for popping num1 from the number stack for the sub-expression
void popNum1Animation(int);

// animation for popping num2 from the number stack for the sub-expression
void popNum2Animation(int);

// animation for popping op from the operator stack for the sub-expression
void popOpAnimation(char);

// animation for popping opening parentheses from operator stack
void popParenthesisAnimation(char);

// animation for popping the result from the stack and displaying it in front of original expression
void popResultAnimation(int);

// main driver
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

    // run evaluation and assign its result to a variable
    int result = evaluate(expression);

    // reset text color to white
    console::setTextColor(white);

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
            // run animation for pushing operator to op stack
            pushOpAnimation(i, expression[i]);

            // push opening parenthesis into operator stack
            operator_stack.push(expression[i]);
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

            // run animation for pushing number to num stack
            pushNumAnimation(i, num);

            // push resultant number into number stack
            number_stack.push(num);
        }
        else if (expression[i] == ')') // closing parenthesis
        {
            // evaluate expression back until last opening parenthesis
            while (!operator_stack.empty() && operator_stack.top() != '(')
            {
                // run animation for popping num2 to sub expression
                popNum2Animation(number_stack.top());

                // pop number into num2
                int num2 = number_stack.pop();

                // run animation for popping op to sub expression
                popOpAnimation(operator_stack.top());

                // pop operator
                char op = operator_stack.pop();

                // run animation for popping num1 to sub expression
                popNum1Animation(number_stack.top());

                // pop number into num1
                int num1 = number_stack.pop();

                // run animation for pushing sub expression result to num stack
                pushSubExpressionResultAnimation(applyOp(num1, num2, op));

                // evaluate the sub expression and push the result into the number stack
                number_stack.push(applyOp(num1, num2, op));
            }

            // if there is still a closing parenthesis to pop, do so
            if (!operator_stack.empty())
            {
                // run animation for popping parenthesis
                popParenthesisAnimation(operator_stack.top());

                // pop parenthesis from stack
                operator_stack.pop();
            }
        }
        else // operator
        {
            // while the last operator pushed to the operator stack has
            // higher precedence than the operator encountered,
            // evaluate the last sub-expression
            while ((!operator_stack.empty()) && precedence(operator_stack.top()) >= precedence(expression[i]))
            {
                // run animation for popping num2 to sub expression
                popNum2Animation(number_stack.top());

                // pop number into num2
                int num2 = number_stack.pop();

                // run animation for popping op to sub expression
                popOpAnimation(operator_stack.top());

                // pop operator
                char op = operator_stack.pop();

                // run animation for popping num1 to sub expression
                popNum1Animation(number_stack.top());

                // pop number into num1
                int num1 = number_stack.pop();

                // run animation for pushing sub expression result to num stack
                pushSubExpressionResultAnimation(applyOp(num1, num2, op));

                // evaluate the sub expression and push the result into the number stack
                number_stack.push(applyOp(num1, num2, op));
            }

            // run animation for pushing operator to op stack
            pushOpAnimation(i, expression[i]);

            // push operator to operator stack
            operator_stack.push(expression[i]);
        }
    }

    // evaluate what remains in the number and operator stack
    while (!operator_stack.empty())
    {
        // run animation for popping num2 to sub expression
        popNum2Animation(number_stack.top());

        // pop number into num2
        int num2 = number_stack.pop();

        // run animation for popping op to sub expression
        popOpAnimation(operator_stack.top());

        // pop operator
        char op = operator_stack.pop();

        // run animation for popping num1 to sub expression
        popNum1Animation(number_stack.top());

        // pop number into num1
        int num1 = number_stack.pop();

        // run animation for pushing sub expression result to num stack
        pushSubExpressionResultAnimation(applyOp(num1, num2, op));

        // evaluate the sub expression and push the result into the number stack
        number_stack.push(applyOp(num1, num2, op));

    }

    // run animation for popping result from num stack
    popResultAnimation(number_stack.top());

    // pop the final number and return it
    return number_stack.pop();
}

void printSpaces(int num)
{
    // save cursor position
    coord saved_pos = console::getCursorPos();

    if (num < 0) // number is negative
    {
        // print space for negative
        std::cout << " ";
    }

    if (std::abs(num) < 10) // 1 digit
    {
        std::cout << " ";
    }
    else if (std::abs(num) > 9) // 2 digits
    {
        std::cout << "  ";
    }
    else if (std::abs(num) > 99) // 3 digits
    {
        std::cout << "   ";
    }

    // go back to saved cursor position
    console::gotoxy(saved_pos);
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

    // reset text color to white
    console::setTextColor(yellow);
}

void pushNumAnimation(int index, int num)
{
    // variable for saving cursor position
    coord pos;

    // go index times forward from expression origin
    console::gotoxy(expression_origin.x + index, expression_origin.y);

    // go down and print num
    console::moveCursor(down);

    // save cursor position before printing
    pos = console::getCursorPos();

    // print num
    std::cout << num;

    // sleep
    console::sleep(sleep_time);

    // check where we are relative to num stack x coord
    if (pos.x > num_stack_top_pos.x) // we are right of the number stack
    {
        // go left until we are above the num stack
        while (pos.x > num_stack_top_pos.x)
        {
            // go back to position
            console::gotoxy(pos);

            // print space over last number
            printSpaces(num);

            // update x position
            pos.x--;

            // go to new position
            console::gotoxy(pos);

            // display num again
            std::cout << num;

            // sleep
            console::sleep(sleep_time);
        }
    }
    else if (pos.x < num_stack_top_pos.x) // we are left of number stack
    {
        // go right until we are above the num stack
        while (pos.x < num_stack_top_pos.x)
        {
            // go back to position
            console::gotoxy(pos);

            // print space over last number
            printSpaces(num);

            // update x position
            pos.x++;

            // go to new position
            console::gotoxy(pos);

            // display num again
            std::cout << num;

            // sleep
            console::sleep(sleep_time);
        }
    }

    // store y value for next available spot in stack
    int slot_y = num_stack_bottom_pos.y - number_stack.size();

    // go down until we hit the next available spot in the stack
    while (pos.y < slot_y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update y position
        pos.y++;

        // go to new position
        console::gotoxy(pos);

        // display num again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }
    
}

void pushSubExpressionResultAnimation(int num)
{
    // go to result position for sub expression
    console::gotoxy(sub_expression_pos.x + sub_expr_gap * 4, sub_expression_pos.y);

    // coord for saving position of result
    coord result = console::getCursorPos();

    // coord for tracking console position
    coord pos;

    // store destination (top of num stack) coord
    coord destination = coord(num_stack_bottom_pos.x, num_stack_bottom_pos.y - number_stack.size());

    // go back 2 and display equal sign
    console::gotoxy(result.x - sub_expr_gap, result.y);

    std::cout << "=";

    // go back to position for result
    console::gotoxy(result);

    // display result of sub-expression
    std::cout << num;

    // sleep for a few moments
    console::sleep(sleep_time * 4);

    // go back to beginning of sub expression
    console::gotoxy(sub_expression_pos.x, sub_expression_pos.y);

    pos = console::getCursorPos();

    // work towards the result printing spaces over the rest of the expression
    while (pos.x < result.x)
    {
        // display space
        std::cout << " ";

        // sleep
        console::sleep(sleep_time);

        // update x position
        pos.x++;
    }

    // got back to position of result
    console::gotoxy(result);

    // set position for moving result 
    pos = console::getCursorPos();

    // go up until we are 1 above the num stack
    while (pos.y > num_stack_top_pos.y - 1)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display num again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }

    // go left until we are above the destination
    while (pos.x > destination.x)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update x position
        pos.x--;

        // go to new position
        console::gotoxy(pos);

        // display num again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }

    // go down until we are in the destination location
    while (pos.y < destination.y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update y position
        pos.y++;

        // go to new position
        console::gotoxy(pos);

        // display num again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }
}

void pushOpAnimation(int index, char op)
{
    // variable for saving cursor position
    coord pos;

    // go index times forward from expression origin
    console::gotoxy(expression_origin.x + index, expression_origin.y);

    // go down and print num
    console::moveCursor(down);

    // save cursor position before printing
    pos = console::getCursorPos();

    // print operator
    std::cout << op;

    // sleep
    console::sleep(sleep_time);

    // check where we are relative to op stack x coord
    if (pos.x > op_stack_top_pos.x) // we are right of the number stack
    {
        // go left until we are above the op stack
        while (pos.x > op_stack_top_pos.x)
        {
            // go back to position
            console::gotoxy(pos);

            // print space over last op
            std::cout << " ";

            // update x position
            pos.x--;

            // go to new position
            console::gotoxy(pos);

            // display op again
            std::cout << op;

            // sleep
            console::sleep(sleep_time);
        }
    }
    else if (pos.x < op_stack_top_pos.x) // we are left of op stack
    {
        // go right until we are above the op stack
        while (pos.x < op_stack_top_pos.x)
        {
            // go back to position
            console::gotoxy(pos);

            // print space over last op
            std::cout << " ";

            // update x position
            pos.x++;

            // go to new position
            console::gotoxy(pos);

            // display num again
            std::cout << op;

            // sleep
            console::sleep(sleep_time);
        }
    }

    // store y value for next available spot in stack
    int slot_y = op_stack_bottom_pos.y - operator_stack.size();

    // go down until we hit the next available spot in the stack
    while (pos.y < slot_y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last op
        std::cout << " ";

        // update y position
        pos.y++;

        // go to new position
        console::gotoxy(pos);

        // display op again
        std::cout << op;

        // sleep
        console::sleep(sleep_time);
    }
}

void popNum1Animation(int num1)
{
    // go to position of top number in number stack
    console::gotoxy(num_stack_bottom_pos.x, num_stack_bottom_pos.y - number_stack.size() + 1);

    // coord for saving position
    coord pos = console::getCursorPos();

    // store destination coord
    coord destination = coord(sub_expression_pos);

    // go up until we are 1 above the stack
    while (pos.y > num_stack_top_pos.y - 1)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num1);

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display num1 again
        std::cout << num1;

        // sleep
        console::sleep(sleep_time);
    }

    // go right until we are above the num1 destination x coord
    while (pos.x < destination.x)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num1);

        // update x position
        pos.x++;

        // go to new position
        console::gotoxy(pos);

        // display num1 again
        std::cout << num1;

        // sleep
        console::sleep(sleep_time);
    }

    // go down until we are at the destination coord
    while (pos.y < destination.y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num1);

        // update y position
        pos.y++;

        // go to new position
        console::gotoxy(pos);

        // display num1 again
        std::cout << num1;

        // sleep
        console::sleep(sleep_time);
    }
}

void popNum2Animation(int num2)
{
    // go to position of top number in number stack
    console::gotoxy(num_stack_bottom_pos.x, num_stack_bottom_pos.y - number_stack.size() + 1);

    // coord for saving position
    coord pos = console::getCursorPos();

    // store destination coord
    coord destination = coord(sub_expression_pos.x + sub_expr_gap * 2, sub_expression_pos.y);

    // go up until we are 1 above the stack
    while (pos.y > num_stack_top_pos.y - 1)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num2);

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display num2 again
        std::cout << num2;

        // sleep
        console::sleep(sleep_time);
    }

    // go right until we are above the num1 destination x coord
    while (pos.x < destination.x)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num2);

        // update x position
        pos.x++;

        // go to new position
        console::gotoxy(pos);

        // display num1 again
        std::cout << num2;

        // sleep
        console::sleep(sleep_time);
    }

    // go down until we are at the destination coord
    while (pos.y < destination.y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num2);

        // update y position
        pos.y++;

        // go to new position
        console::gotoxy(pos);

        // display num2 again
        std::cout << num2;

        // sleep
        console::sleep(sleep_time);
    }
}

void popOpAnimation(char op)
{
    // go to position of top operator in op stack
    console::gotoxy(op_stack_bottom_pos.x, op_stack_bottom_pos.y - operator_stack.size() + 1);

    // coord for saving position
    coord pos = console::getCursorPos();

    // store destination coord
    coord destination = coord(sub_expression_pos.x + sub_expr_gap, sub_expression_pos.y);

    // go up until we are 1 above the stack
    while (pos.y > op_stack_top_pos.y - 1)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last operator
        std::cout << " ";

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display op again
        std::cout << op;

        // sleep
        console::sleep(sleep_time);
    }

    // go left until we are above the op destination x coord
    while (pos.x > destination.x)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last operator
        std::cout << " ";

        // update x position
        pos.x--;

        // go to new position
        console::gotoxy(pos);

        // display op again
        std::cout << op;

        // sleep
        console::sleep(sleep_time);
    }

    // go down until we are at the destination coord
    while (pos.y < destination.y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last operator
        std::cout << " ";

        // update y position
        pos.y++;

        // go to new position
        console::gotoxy(pos);

        // display op again
        std::cout << op;

        // sleep
        console::sleep(sleep_time);
    }
}

void popParenthesisAnimation(char op)
{
    // go to position of top operator in op stack
    console::gotoxy(op_stack_bottom_pos.x, op_stack_bottom_pos.y - operator_stack.size() + 1);

    // coord for saving position
    coord pos = console::getCursorPos();

    // store destination coord
    coord destination = coord(op_stack_top_pos.x, op_stack_top_pos.y - 2);

    // go up until we are 1 above the stack
    while (pos.y > destination.y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last operator
        std::cout << " ";

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display op again
        std::cout << op;

        // sleep
        console::sleep(sleep_time);
    }

    // go to destinaion
    console::gotoxy(destination.x - 2, destination.y);

    console::setTextColor(red);

    // display boom
    std::cout << "BOOM!";

    console::setTextColor(yellow);

    // sleep for a few moments
    console::sleep(sleep_time * 4);

    // go to print boom
    console::gotoxy(destination.x - 2, destination.y);

    // display spaces over boom
    std::cout << "     ";
}

void popResultAnimation(int num)
{
    // go to position of top number in number stack
    console::gotoxy(num_stack_bottom_pos.x, num_stack_bottom_pos.y - number_stack.size() + 1);

    // coord for saving position
    coord pos = console::getCursorPos();

    // store destination coord
    coord destination = coord(expression_origin.x + expression.size() + 3, expression_origin.y);

    // go up until we are 1 above the stack
    while (pos.y > num_stack_top_pos.y - 1)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display num again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }

    // go right until we are under the num destination x coord
    while (pos.x < destination.x)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update x position
        pos.x++;

        // go to new position
        console::gotoxy(pos);

        // display num1 again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }

    // go up until we are at the destination coord
    while (pos.y > destination.y)
    {
        // go back to position
        console::gotoxy(pos);

        // print space over last number
        printSpaces(num);

        // update y position
        pos.y--;

        // go to new position
        console::gotoxy(pos);

        // display num1 again
        std::cout << num;

        // sleep
        console::sleep(sleep_time);
    }

    // go back two
    console::gotoxy(pos.x - 2, pos.y);

    // print equal sign
    std::cout << "=";
}
