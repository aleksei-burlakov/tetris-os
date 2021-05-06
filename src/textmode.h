#ifndef TEXTMODE_H
#define TEXTMODE_H

#include "util.h"

void textMode_print(char *txt);
void textMode_printNoWait(char *txt);
void textMode_printChar(char c);
void textMode_clear();
void textMode_updateCursorPosition(u16 offset);
void textMode_nextRow();
void textMode_nextCol();

#endif TEXTMODE_H