#pragma once

#include <Windows.h>

// struct for points
struct coord
{
    int x;
    int y;

    coord()
        : x(0), y(0)
    {
    }

    coord(int a, int b)
        : x(a), y(b)
    {
    }
};

enum color
{
    dark_blue = 1, dark_green, dark_teal, dark_red, purple, 
    dark_yellow, dark_white, grey, blue, green, light_blue, 
    red, pink, yellow, white
};

enum direction
{
    up, down, left, right
};

namespace console
{
    void gotoxy(const short& x, const short& y)
    {
        HANDLE handle;
        COORD coord;

        handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle == INVALID_HANDLE_VALUE)
            return;

        coord = { x, y };

        SetConsoleCursorPosition(handle, coord);
    }

    void gotoxy(COORD pos)
    {
        HANDLE handle;

        handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle == INVALID_HANDLE_VALUE)
            return;

        SetConsoleCursorPosition(handle, pos);
    }

    void gotoxy(coord p)
    {
        HANDLE handle;
        COORD pos;
        pos.X = p.x;
        pos.Y = p.y;

        handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle == INVALID_HANDLE_VALUE)
            return;

        SetConsoleCursorPosition(handle, pos);
    }

    coord getCursorPos()
    {
        coord pos;
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);

        if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
            printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());

        pos.x = screenBufferInfo.dwCursorPosition.X;
        pos.y = screenBufferInfo.dwCursorPosition.Y;

        return pos;
    }

    void moveCursor(direction dir)
    {
        // store current cursor position
        coord pos = getCursorPos();

        // check which direction to move
        switch (dir)
        {
        case up:

            // make sure we can go up
            if (pos.y - 1 >= 0)
            {
                // go up
                gotoxy(pos.x, pos.y - 1);
            }

            break;

        case down:

            // go down
            gotoxy(pos.x, pos.y + 1);

            break;

        case left:

            // make sure we can go left
            if (pos.x - 1 >= 0)
            {
                // go left
                gotoxy(pos.x - 1, pos.y);
            }

            break;

        case right:

            // go right
            gotoxy(pos.x + 1, pos.y);

            break;

        default:
            std::cout << "Error: Cannot move cursor. Invalid direction.\n";
        }
    }

    void showCursor(const bool& flag)
    {
        HANDLE handle;
        CONSOLE_CURSOR_INFO consoleCursorInfo;

        handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle == INVALID_HANDLE_VALUE)
            return;

        GetConsoleCursorInfo(handle, &consoleCursorInfo);

        consoleCursorInfo.bVisible = flag;

        SetConsoleCursorInfo(handle, &consoleCursorInfo);
    }

    void clear()
    {
        HANDLE handle;
        CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
        DWORD count;
        DWORD cellCount;
        COORD origin;

        origin = { 0, 0 };

        handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (handle == INVALID_HANDLE_VALUE)
            return;

        if (!GetConsoleScreenBufferInfo(handle, &consoleScreenBufferInfo))
            return;

        cellCount = consoleScreenBufferInfo.dwSize.X * consoleScreenBufferInfo.dwSize.Y;

        if (!FillConsoleOutputCharacter(handle, (TCHAR)' ', cellCount, origin, &count))
            return;

        if (!FillConsoleOutputAttribute(handle, consoleScreenBufferInfo.wAttributes,
            cellCount, origin, &count))
            return;

        SetConsoleCursorPosition(handle, origin);
    }

    void setTextColor(color color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void sleep(int ms)
    {
        Sleep(ms);
    }
}
