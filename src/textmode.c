#include "textmode.h"
#include "timer.h"

#define TEXTMODE_WIDTH 80
#define TEXTMODE_HEIGHT 25

u8 row, col = 0;
u16 offset = 0;

void textMode_print(char *text)
{
    u8 *txt = text;
    while (*txt != 0) 
    {
        textMode_printChar(*txt);
        *txt++;

        timer_waitTicks(TIMER_TPS / 100);
    }
}

void textMode_printNoWait(char *text)
{
    u8 *txt = text;
    while (*txt != 0) 
    {
        textMode_printChar(*txt);
        txt++;
    }
}

void textMode_printChar(char c)
{
    switch (c) {
        case '\n':
            textMode_nextRow();
        break;
        case '\r':
            col = 0;
        break;
        default:
            *((u8*) 0xb8000 +  offset * 2) = c;

            textMode_nextCol();
        break;
    }

    offset = (row * 80 + col);
    textMode_updateCursorPosition(offset);
}

void textMode_nextCol()
{
    col++;
    if (col >= 80)
    {
        col = 0;
        textMode_nextRow();
    }
}

void textMode_nextRow()
{
    row++;
    /* if row > 25, scroll buffer one row up by memcpy, and set row = 25 */
}

void textMode_updateCursorPosition(u16 offset)
{
    outportw(0x3d4, (offset & 0xff00) | 0x0e);
    outportw(0x3d4, ((offset << 8) & 0xff00) | 0x0f);
}

void textMode_clear()
{
    u8 *ptr = (u8 *) 0xb8000;
    for (int y = 0; y < 25; y++)
    for (int x = 0; x < 80; x++, ptr+=2)
        *ptr = ' ';

    outportw(0x3d4, 0x000e);
    outportw(0x3d4, 0x000f);
}
