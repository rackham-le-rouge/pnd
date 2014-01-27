/**
 * Program:   	pnd
 * File:	drawing.c
 * Brief:	All screen drawing functions
 * Description:	Here it is all of the drawing functions, including the splash and the help message.
 * 		All ncurses stuff is here too.
 * Date:	Somewhere in 2013 and near of 2014
 * Licence:	Beerware (rev 42)
 * Author:	Jerome GRARD - A bored engineering student...
 */


#include "conf.h"



// Blues lines - Top and bottom of the screen
void initBar()
{
        int l_iRow,l_iCol,i;                          /* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               /* get the number of rows and columns */
        start_color(); 	                              /* start color mode */
        init_pair(1, COLOR_BLACK, COLOR_LINE_BG_BOTTOM);   /* référence des couleurs */
        attron(COLOR_PAIR(1));
        // Bottom line
        for(i = 0; i<l_iCol ; i++)
                mvprintw(l_iRow-1,i," ");
        // Top line
        attroff(COLOR_PAIR(1));
        init_pair(2, COLOR_BLACK, COLOR_LINE_BG_TOP);   /* référence des couleurs */
        attron(COLOR_PAIR(2));

        for(i = 0; i<l_iCol ; i++)
                mvprintw(0,i," ");

        attroff(COLOR_PAIR(2));
}







// Text of the top line
void topText(char p_sString[])
{
        start_color();                          /* start color mode */
        attron(COLOR_PAIR(2));
        mvprintw(0,0,"%s", p_sString);
        attroff(COLOR_PAIR(2));
}





// Disable the two lines
void disableBar()
{
        start_color();                          /* start color mode */
        init_pair(1, COLOR_BLACK, COLOR_BLACK);   /* référence des couleurs */

        init_pair(2, COLOR_BLACK, COLOR_BLACK);   /* référence des couleurs */
}









// Text of the bottom line
void botText(char p_sString[])
{
        int l_iRow,l_iCol;                            /* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               /* get the number of rows and columns */
        start_color();                          /* start color mode */
        attron(COLOR_PAIR(1));
        mvprintw(l_iRow-1,0,"%s", p_sString);

        attroff(COLOR_PAIR(1));
	l_iCol++;
}




// Draw an element on the screen
int drawElement(int p_iX, int p_iY, char p_cChar, int p_iColor)
{
        int l_iRow,l_iCol;                            /* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               /* get the number of rows and columns */

	// Graphical binding
	if(p_iX>l_iCol) return -1;
	if(p_iY>l_iRow) return -1;

        start_color();                          /* start color mode */
        attron(COLOR_PAIR(p_iColor));

	// Draw the element
        mvprintw(p_iY,p_iX,"%c", p_cChar);

	attroff(COLOR_PAIR(p_iColor));
	return 0;
}



// Draw an element on the screen
int drawSentence(int p_iX, int p_iY, char* p_sString, int p_iColor)
{
        int l_iRow,l_iCol;                            /* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               /* get the number of rows and columns */

	// Graphical binding
	if(p_iX>l_iCol) return -1;
	if(p_iY>l_iRow) return -1;

        start_color();                          /* start color mode */
        attron(COLOR_PAIR(p_iColor));

	// Draw the sentence
        mvprintw(p_iY,p_iX,"%s", p_sString);

	attroff(COLOR_PAIR(p_iColor));
	return 0;
}


// Initialisation of the colors of each kind of people
void initColor()
{
	start_color();

#ifdef GRAPHIC_MODE
        init_pair(enumNoir, COLOR_BLACK, COLOR_BLACK);
        init_pair(enumRouge, COLOR_BLACK, COLOR_RED);
        init_pair(enumVert, COLOR_BLACK, COLOR_GREEN);
        init_pair(enumJaune, COLOR_BLACK, COLOR_YELLOW);
        init_pair(enumBleu, COLOR_BLACK, COLOR_BLUE);
        init_pair(enumMagenta, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(enumCyan, COLOR_BLACK, COLOR_CYAN);
        init_pair(enumBlanc, COLOR_BLACK, COLOR_WHITE);
#else
        init_pair(enumNoir, COLOR_BLACK, COLOR_BLACK);
        init_pair(enumRouge, COLOR_RED, COLOR_BLACK);
        init_pair(enumVert, COLOR_GREEN, COLOR_BLACK);
        init_pair(enumJaune, COLOR_YELLOW, COLOR_BLACK);
        init_pair(enumBleu, COLOR_BLUE, COLOR_BLACK);
        init_pair(enumMagenta, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(enumCyan, COLOR_CYAN, COLOR_BLACK);
        init_pair(enumBlanc, COLOR_WHITE, COLOR_BLACK);
#endif
}



// Draw the intro panel
void drawIntro(int p_iRow, int p_iCol)
{
        start_color();                          /* start color mode */
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);   /* référence des couleurs */
        attron(COLOR_PAIR(7));

	p_iRow = (p_iRow - 30)/3;

	drawCenter("F.L.E.Y", p_iCol, p_iRow++);p_iRow++;

	drawCenter("C is for cookies, that's good enought for me and for this software !", p_iCol, p_iRow++);p_iRow++;

	drawCenter("                                    .,.                                              ", p_iCol, p_iRow++);
	drawCenter("                                ,nMMMMMMb.                                           ", p_iCol, p_iRow++);
	drawCenter("                     .,,,.     dP     MMMMMb            -                            ", p_iCol, p_iRow++);
	drawCenter("                  .nMMMMMMMn. `M       MMMMMM>                                       ", p_iCol, p_iRow++);
	drawCenter("                 uMMMMMMMMMMMb Mx     uMMMMMM   . .,,.                               ", p_iCol, p_iRow++);
	drawCenter("                 MMMMMMP     4M.`4MMMMMMMMM\' ?J$$$$ccc\"=                             ", p_iCol, p_iRow++);
	drawCenter("                 MMMMMM       M   \"4MMMMP\" z$c,\"?$c,\"\"$c$h=                         ", p_iCol, p_iRow++);
	drawCenter("                 \"MMMMMb,..,d\' hdzc   ,cc$$$$$$$$$$L.$c \"                            ", p_iCol, p_iRow++);
	drawCenter("     c\"`          `4MMMMMMP\" ,$$$$$$c,$$$$$$$$$$$$\"?? ,c?$.                          ", p_iCol, p_iRow++);
	drawCenter("                 ,cc,.  .,zc$$$$$3?$?$P\"?  \" \"  \"    $$$$h`                          ", p_iCol, p_iRow++);
	drawCenter("              ,c$$$\", c$$$$$$$$PP                   ,$$$$$c                          ", p_iCol, p_iRow++);
	drawCenter("              )$$$$$$$P\"$$$$$\"                       $c,\"$c/     *                   ", p_iCol, p_iRow++);
	drawCenter("    \'       ,cc$$$$$\"\".d$??                         J$$hc$=                         ", p_iCol, p_iRow++);
	drawCenter("           ,c\" d$$??c=\"                            z$$$$$??                          ", p_iCol, p_iRow++);
	drawCenter("=         `!?-$\"\"? ?\"                             d$$$\"\"$ccc                         ", p_iCol, p_iRow++);
	drawCenter("            =`\" \"                               ,d$$$$P $$$\"-                        ", p_iCol, p_iRow++);
	drawCenter("              d$$h.        ,;!               ,d$$$$P\" .$$$h.     =                   ", p_iCol, p_iRow++);
	drawCenter("             <L$$$$c     <!!!!>.         !> -$$$$$P zd$$$\"\"$.                        ", p_iCol, p_iRow++);
	drawCenter("              \" \"$\" $c  !  `!,!\'!;,,;!--!!!! `$$P .d$$$$$h $c\"                      ", p_iCol, p_iRow++);
	drawCenter("                 $ d$\".`!!  `\'\'\'`!!!!    !!,! $C,d$$$$$$$$.`$-                       ", p_iCol, p_iRow++);
	drawCenter("        \"        $.\"$ \'  ,zcc=cc ``\'``<;;!!!> $?$$$$$$$$\"$$F\"                        ", p_iCol, p_iRow++);
	drawCenter("                 \"=\" .J,??\"\" .$\",$$$$- \'!\'!!  $<$$$$$$$$h ?$=.                       ", p_iCol, p_iRow++);
	drawCenter("                 \".d$\"\"\",chd$$??\"\"\"\"\".r >  !\'.$?$$$$$$$$\"=,$                        ", p_iCol, p_iRow++);
	drawCenter("               ,c$$\",c,d$$$   ,c$$$$$$\"\'!;!\'.$F.$$$$$$$$. $$$.\"L                   ", p_iCol, p_iRow++);
	drawCenter("              d $$$c$$$$$  ,hd$$$$??\" z !!`.P\' `$$$$$$$?? $$$P ..                   ", p_iCol, p_iRow++);
	drawCenter("           ,cd J$$$$$$$P ,$$$$$$  ,c$$\" `.d$h h $$$$$$P\"?.?$$ ,$$\"\"               ", p_iCol, p_iRow++);
	drawCenter("          c$$\' $$$$$$$C,$$$$$$   ,$$$P\"z $$$$\"-$J\"$$$$$ ,$$?$F.$L?$$L               ", p_iCol, p_iRow++);
	drawCenter("         ,$\",J$$$$$$$$$$$$$$    ,$$$C,r\" $$$$$$$\"z$$$$$$$$$,`,$$\"d$$??              ", p_iCol, p_iRow++);
	drawCenter("        .dF $$$$$$$$$$$$$$P   $$$$$$$F.-,$$$$$$$$$$$$$$$$$$$%d$$F$$$$cc,-            ", p_iCol, p_iRow++);
	drawCenter("       ,$( c$$$$$$$$$$$$$$h,c$$$$$$c=\".d$$   \"\"\"\',,.. .,.`\"\"?$$$$$$.`?c .            ", p_iCol, p_iRow++);
	drawCenter("       d$$.$$$$$$$$$$$$$$$$$$$P\"\"P\" ,c$$$$$c,\"=d$$$$$h.\"$$$c,\"$$$$$$. $$c           ", p_iCol, p_iRow++);
	drawCenter("      $$$$$$$$$$$$$$$$$$$$$P\"\",c\"  ,$$$$$$$$$h. `?$$$$$$$$$$P\'.\"\"`?$$.3$cr           ", p_iCol, p_iRow++);
	drawCenter("    z$$$$$$$$$ 3?$$$$$$$$$ccc$P   ,$$$$$$$$$$$$$h.`\"$$$$$$$$$c?$c .`?$$$$c           ", p_iCol, p_iRow++);
	drawCenter("  dd$$\"J$$$$$$$P $$$$$$$$F?$$$\" .d$$$$$$$$$$$$$$$$L.`\"\"???$$$$$$$$$c  ?$C?           ", p_iCol, p_iRow++);
	drawCenter(" c$$$$ $$3$$$$$$ `$$$$$h$$P\"\".. $$$$$$$$$$$$$$$??\"\"\" ,ccc3$$$$$$$$$ hL`?$C           ", p_iCol, p_iRow++);
	drawCenter("h$$$$$>`$$$$$$$$$.$$$$$??\',c$\"  $$$$$P)$$$$$P\" ,$$$$$$$$$$$$$$$$$$$ \"$ .\"$           ", p_iCol, p_iRow++);



        attroff(COLOR_PAIR(7));

	topText("Free, simple, open-source and hightly configurable drawing program for linux shell - BEERWARE licence");
	botText("ASCII art from  http://www.retrojunkie.com/asciiart/tvmovies/sesamest.txt --  Program written by Jerome GRARD");


	refresh();getch();
        init_pair(7, COLOR_BLACK, COLOR_BLACK);   /* référence des couleurs */


	usleep(10000);
}




// Draw str to the center of the screen
int drawCenter(char* p_sString, int p_iCol, int p_iRow)
{
	// si on met -1 en col ou en row, il prendra comme valeur la première valeur non nulle qui
	// lui a été donnée.
	static int l_iBackupCol = 0;
	static int l_iBackupRow = 0;

	if(p_iCol == -1) p_iCol=l_iBackupCol;
	if(p_iRow == -1) p_iRow=l_iBackupRow;

        mvprintw(p_iRow, (p_iCol - strlen(p_sString))/2, "%s", p_sString);
	return p_iRow;
}



/**
  *  Draw the main menu, in order to swho all computation choices and a little in-line help
  */
void drawMainMenu(int p_iRow, int p_iCol)
{
	char* l_cBuffer = (char*)malloc(21*sizeof(char));

        start_color();                          /* start color mode */

	// this color pair seem to be useless...
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);   /* colors references */
        attron(COLOR_PAIR(7));

	p_iRow = (p_iRow - 30)/3;
	p_iCol = (p_iCol) / 4;


	strcpy(l_cBuffer, "+---------------------+");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "+---------------------+");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);


	p_iRow -= 7;

	strcpy(l_cBuffer, "1. Start/Stop search");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "2. Set an order     ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "3. Montoring screen ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "4. Settings         ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "5. XX               ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "6. About PND        ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);



        attroff(COLOR_PAIR(7));

	topText("Prime Number Discovery program, in order to find something new by yourself - BEERWARE licence");


	refresh();getch();
        init_pair(7, COLOR_BLACK, COLOR_BLACK);   /* référence des couleurs */


	usleep(10000);
}



/**
  * Draw a loading bar on a complete line
  * it's used to represent computation progression...
  *
  * You need to inititalise it -> 				e.g : drawLoadingBar(0, -1, 0, yourScreenLenght, 0);
  * And after you can use it with the previous screen Lenght   	e.g : drawLoadingBar(2, 25, 100, -1, enumRed);		// with the previous screen lenght
  * You can also modify the screen lenght 			e.g : drawLoadingBar(2, 25, 100, newScreenLenght, enumRouge); // progress bar drawed with the new screen lenght
  */
void drawLoadingBar(int p_iLine, int p_iCompleted, int p_iMax, int p_iScreenLenght, int p_iColor)
{
	int l_iIterateur = 0;
	int l_iPercent = 0;
	int l_iColumn = 0;
	char l_cNumber[5];
	char l_cCaracterBody = PROGRESS_BAR_BODY_CHARACTER;
	char l_cCaracterHead = PROGRESS_BAR_HEAD_CHARACTER;
	static int l_iScreenLenght = -1;

	// In order to avoid to carry the Screen Lenght each time, we set it in static at the beginning
	// and to keep it put -1 in the call function. To modify it, put any number != -1
	if(p_iScreenLenght != -1)
	{
		l_iScreenLenght = p_iScreenLenght;
		LOG_WRITE("Now screen lenght is :")
		LOG_WRITE_LONG((long int)l_iScreenLenght)
	}

	// In order to quit the function just after setting  the screen lenght
	if(p_iCompleted < 0)
	{
		return;
	}

	l_iPercent = (int)(((float)p_iCompleted /(float)p_iMax)*(float)l_iScreenLenght);

	// If the bar is taller than the screen
	l_iPercent = (l_iPercent > l_iScreenLenght) ? l_iScreenLenght : l_iPercent;

	// If lenght == 0 then we need to add 1 in order to avoid infinite loop
	l_iPercent = (l_iPercent == 0) ? 1 : l_iPercent;

	// draw body
	for(l_iIterateur = 0; l_iIterateur < l_iPercent - 1; l_iIterateur++)
	{
		drawElement(l_iIterateur, p_iLine, l_cCaracterBody, p_iColor);
	}
	// draw the head
	drawElement(l_iIterateur++, p_iLine, l_cCaracterHead, p_iColor);


	// If pair number we need to substract one (more pretty)
	l_iColumn = l_iScreenLenght/2;
	l_iColumn -= (l_iScreenLenght % 2 == 0) ? 1 : 0;

	l_iPercent = (int)(((float)p_iCompleted /(float)p_iMax)*(float)100);
	snprintf(l_cNumber, 4, "%d", l_iPercent);


	drawElement(l_iColumn++, p_iLine, *(l_cNumber + 0), p_iColor);
	if(*(l_cNumber + 0) != '\0')
	{
		drawElement(l_iColumn++, p_iLine, *(l_cNumber + 1), p_iColor);
		if(*(l_cNumber + 1) != '\0')
		{
			drawElement(l_iColumn++, p_iLine, *(l_cNumber + 2), p_iColor);
		}
	}

	// We need to see each time the bar is changed
	refresh();
}
