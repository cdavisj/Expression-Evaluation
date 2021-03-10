#pragma once

#include <Windows.h>

enum Color
{
    yellow = 6, green = 10, red = 12, purple = 13, white = 15
};

enum Direction
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

    COORD getCursorPos()
    {
        COORD pos;
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);

        if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
            printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());

        pos.X = screenBufferInfo.dwCursorPosition.X;
        pos.Y = screenBufferInfo.dwCursorPosition.Y;

        return pos;
    }

    void moveCursor(Direction dir)
    {
        // store current cursor position
        COORD pos = getCursorPos();
        pos.X--;

        // check which direction to move
        switch (dir)
        {
        case up:

            // make sure we can go up
            if (pos.Y - 1 >= 0)
            {
                // go up
                gotoxy(pos.X, pos.Y - 1);
            }

            break;

        case down:

            // go down
            gotoxy(pos.X, pos.Y + 1);

            break;

        case left:

            // make sure we can go left
            if (pos.X - 1 >= 0)
            {
                // go left
                gotoxy(pos.X - 1, pos.Y);
            }

            break;

        case right:

            // go right
            gotoxy(pos.X + 1, pos.Y);

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

    void setTextColor(Color color)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void sleep(int ms)
    {
        Sleep(ms);
    }
}
