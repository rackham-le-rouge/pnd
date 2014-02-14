/**
 * Program:     pnd
 * File:        drawing.h
 * Brief:       All screen drawing functions prototypes
 * Description: All is in the brief
 * Date:        1/2014
 * Licence:     Beerware (rev 42)
 * Author:      Jerome GRARD - A bored engineering student...
 */



#ifndef DRAWING_H
#define DRAWING_H


/*

        Header of the graphical part of the program

*/


void initBar(void);
void disableBar(void);
void enableBar(void);
void topText(const char* p_sString);
void botText(const char* p_sString);
int drawElement(int, int, char, int);
int drawSentence(int p_iX, int p_iY, char* p_sString, int p_iColor);
void initColor(void);
void drawIntro(int, int);
int drawCenter(const char*, int, int);
void drawMainMenu(int p_iRow, int p_iCol);
void drawLoadingBar(int p_iLine, int p_iCompleted, int p_iMax, int p_iScreenLenght, int p_iColor);
void eraseWorkingScreen(int p_iRow, int p_iCol);
void drawSubMenu(int p_iRow, int p_iCol, int p_iMenuSelector);
void drawCurrentMersenneOrder(structProgramInfo* structCommon);

#endif

