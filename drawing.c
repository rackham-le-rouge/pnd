/**
 * @file	drawing.c
 * @program   	pnd
 * @brief	All screen drawing functions
 * @description	Here it is all of the drawing functions, including the splash and the help message.
 * 		All ncurses stuff is here too.
 * @date	Somewhere in 2013 and near of 2014
 * @licence	Beerware (rev 42)
 * @author	Jerome GRARD - A bored engineering student...
 */


#include "conf.h"



/** @brief Blues lines - Top and bottom of the screen */
void initBar(void)
{
        int l_iRow,l_iCol,i;                          	/* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               	/* get the number of rows and columns */
        start_color(); 	                              	/* start color mode */
        init_pair(1, COLOR_BLACK, COLOR_LINE_BG_BOTTOM);   /* référence des couleurs */
        attron(COLOR_PAIR(1));
        /* Bottom line */
        for(i = 0; i<l_iCol ; i++)
                mvprintw(l_iRow-1,i," ");
        /* Top line */
        attroff(COLOR_PAIR(1));
        init_pair(2, COLOR_BLACK, COLOR_LINE_BG_TOP);   /* référence des couleurs */
        attron(COLOR_PAIR(2));

        for(i = 0; i<l_iCol ; i++)
                mvprintw(0,i," ");

        attroff(COLOR_PAIR(2));
}







/** @brief Text of the top line */
void topText(const char* p_sString)
{
        start_color();                          	/* start color mode */
        attron(COLOR_PAIR(2));
        mvprintw(0,0,"%s", p_sString);
        attroff(COLOR_PAIR(2));
}





/** @brief Disable the two lines */
void disableBar(void)
{
        start_color();                          	/* start color mode */
        init_pair(1, COLOR_BLACK, COLOR_BLACK);   	/* référence des couleurs */

        init_pair(2, COLOR_BLACK, COLOR_BLACK);   	/* référence des couleurs */
}









/** @brief Text of the bottom line
  * @param p_sString : string to print at the bottom of the screen
  */
void botText(const char* p_sString)
{
        int l_iRow,l_iCol;                            	/* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               	/* get the number of rows and columns */
        start_color();                          	/* start color mode */
        attron(COLOR_PAIR(1));
        mvprintw(l_iRow-1,0,"%s", p_sString);

        attroff(COLOR_PAIR(1));
	l_iCol++;
}




/** @brief Draw an element on the screen
  * @param p_iX : X coord
  * @param p_iY : Y coord
  * @param p_cChar : the character to print
  * @param p_iColor : color, Cf the enum
  * @return 0
  */
int drawElement(int p_iX, int p_iY, char p_cChar, int p_iColor)
{
        int l_iRow,l_iCol;                            	/* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               	/* get the number of rows and columns */

	/* Graphical binding */
	if(p_iX>l_iCol) return -1;
	if(p_iY>l_iRow) return -1;

        start_color();                          	/* start color mode */
        attron(COLOR_PAIR(p_iColor));

	/* Draw the element */
        mvprintw(p_iY,p_iX,"%c", p_cChar);

	attroff(COLOR_PAIR(p_iColor));
	return 0;
}



/** @brief Draw an element on the screen
  * @param p_iX : X coord of the beginning of the sentence
  * @param p_iY : Y coord of the beginning of the sentence
  * @param p_sString : sentence to draw
  * @param p_iColor : color used, from the enum
  * @return 0
  */
int drawSentence(int p_iX, int p_iY, char* p_sString, int p_iColor)
{
        int l_iRow,l_iCol;                            	/* to store the number of rows and */
        getmaxyx(stdscr,l_iRow,l_iCol);               	/* get the number of rows and columns */

	/* Graphical binding */
	if(p_iX>l_iCol) return -1;
	if(p_iY>l_iRow) return -1;

        start_color();                          	/* start color mode */
        attron(COLOR_PAIR(p_iColor));

	/* Draw the sentence */
        mvprintw(p_iY,p_iX,"%s", p_sString);

	attroff(COLOR_PAIR(p_iColor));
	return 0;
}


/** @brief Initialisation of the colors of each kind of people
  */
void initColor(void)
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



/** @brief Draw the intro panel
  * @param p_iRow : Number of lines in the screen
  * @param p_iCol : Number of columns in the screen
  */
void drawIntro(int p_iRow, int p_iCol)
{
        start_color();                          	/* start color mode */
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);   	/* référence des couleurs */
        attron(COLOR_PAIR(7));

	p_iRow = (p_iRow - 30)/4;

	drawCenter("P.N.D", p_iCol, p_iRow++);
	p_iRow++;
	drawCenter("Make your computer usefull and tired", p_iCol, p_iRow++);
	p_iRow++;

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

	p_iRow++;
	drawCenter("Created and Dev by Jerome GRARD. Using OpenMP, ncurses and GMP libs.", p_iCol, p_iRow++);



        attroff(COLOR_PAIR(7));

	botText("ASCII art from  http://www.retrojunkie.com/asciiart/tvmovies/sesamest.txt");


	refresh();

        /* Ask user key press */
        nodelay(stdscr, FALSE);
        getch();
        nodelay(stdscr, TRUE);

        init_pair(7, COLOR_BLACK, COLOR_BLACK);   	/* référence des couleurs */
}




/** @brief  Draw str to the center of the screen
  * @param p_sString : sentence to write on the screen
  * @param p_iCol : column of the screen
  * @param p_iRow : rows in the screen
  * @return p_iRow
  */
int drawCenter(const char* p_sString, int p_iCol, int p_iRow)
{
	/* si on met -1 en col ou en row, il prendra comme valeur la première valeur non nulle qui
	   lui a été donnée. */
	static int l_iBackupCol = 0;
	static int l_iBackupRow = 0;

	if(p_iCol == -1) p_iCol=l_iBackupCol;
	if(p_iRow == -1) p_iRow=l_iBackupRow;

        mvprintw(p_iRow, (p_iCol - strlen(p_sString))/2, "%s", p_sString);
	return p_iRow;
}



/**
  * @brief Draw the main menu, in order to swho all computation choices and a little in-line help
  * @param p_iRow : number of row in the screen
  * @param p_iCol : number of column in the screen
  */
void drawMainMenu(int p_iRow, int p_iCol)
{
	char* l_cBuffer = (char*)malloc(21*sizeof(char));

	if(l_cBuffer == NULL)
	{
			perror("Not enought memory - Stop all now");
			exit(ENOMEM);
	}

        start_color();                          	/* start color mode */

	/* this color pair seem to be useless... */
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);   	/* colors references */
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

	strcpy(l_cBuffer, "1. Start search");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "2. Set an order     ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "3. Set thread count ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "4. Prospecting      ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "5. About PND        ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "6. Quit             ");
	drawSentence(p_iCol + 2 , p_iRow++, l_cBuffer, enumJaune);



        attroff(COLOR_PAIR(7));

	topText("Prime Number Discovery program, in order to find something new by yourself - BEERWARE licence");

	free(l_cBuffer);
	l_cBuffer = NULL;

	refresh();
        init_pair(7, COLOR_BLACK, COLOR_BLACK);   	/* color reference */
}

/**
  * @brief Drawing function to draw all messages and sub menus of the screen
  * @param p_iRow : number of row in the screen
  * @param p_iCol : number of column in the screen
  * @param p_iMenuSelector : select the wanted menu to draw
  * @param p_structCommon : usefull data on the program
  */
void drawSubMenu(int p_iRow, int p_iCol, int p_iMenuSelector, structProgramInfo* p_structCommon)
{
	char* l_cBuffer = (char*)malloc(21*sizeof(char));

	if(l_cBuffer == NULL)
	{
			perror("Not enought memory - Stop all now");
			exit(ENOMEM);
	}

        start_color();                          	/* start color mode */

	/* this color pair seem to be useless... */
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);   /* colors references */
        attron(COLOR_PAIR(7));

	/* The +4 is for shift menu rather than the main menu */
	p_iRow = (p_iRow - 30)/3 + 4;
	p_iCol = (p_iCol) / 4 + 4;


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
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "|                     |");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);
	strcpy(l_cBuffer, "+---------------------+");
	drawSentence(p_iCol, p_iRow++, l_cBuffer, enumJaune);

	p_iRow-= 9;

	switch(p_iMenuSelector)
	{
		case MENU_NEW_ORDER:
		{
			strcpy(l_cBuffer, "Type the new Mers-");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "enne order and not");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "the number itself.");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "Cursor is in vaca-");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "tion. Check your");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "number in the bot-");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "tom bar after Enter");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "key pressed.");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_THIS_IS_A_PRIME_NUMBER:
		{
			strcpy(l_cBuffer, "This is a prime");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "number !!!!!!!!");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);

			strcpy(l_cBuffer, "Press any key");
			drawSentence(p_iCol + 2, ++p_iRow, l_cBuffer, enumJaune);

			p_iRow+=3;
			strcpy(l_cBuffer, "   Prime number ?");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "         YES");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_THIS_IS_NOT_A_PRIME_NUMBER:
		{
			strcpy(l_cBuffer, "This is not a ");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "prime number.");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);

			strcpy(l_cBuffer, "Press any key");
			drawSentence(p_iCol + 2, ++p_iRow, l_cBuffer, enumJaune);

			p_iRow+=3;
			strcpy(l_cBuffer, "   Prime number ?");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "         NO");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_GIVE_UP_SEARCH:
		{
			strcpy(l_cBuffer, "User gives up");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "search.");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);

			strcpy(l_cBuffer, "Press any key");
			drawSentence(p_iCol + 2, ++p_iRow, l_cBuffer, enumJaune);

			p_iRow+=3;
			strcpy(l_cBuffer, "   Prime number ?");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "         ??");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_SET_THREAD_NUMBER:
		{
			strcpy(l_cBuffer, "Type the wanted  ");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "thread number. By");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "default this is  ");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			snprintf(l_cBuffer, (strlen(l_cBuffer) - 1)*sizeof(char), "%d threads.", p_structCommon->iThreadNumber);
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "You can check new");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "value by display");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "log with this line");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "$tail -n99 pnd.log");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_WAIT_CHECK_MERSENNE_ORDER:
		{
			strcpy(l_cBuffer, "Please wait until");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "the end of the");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "Mersenne order");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "checking. Because");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "to be right, this");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "number need to be");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "a prime number.");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_INFO_STD_ALGO_SET:
		{
			strcpy(l_cBuffer, "STANDART ALGO");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "selected !");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_INFO_MR_ALGO_SET:
		{
			strcpy(l_cBuffer, "MILLER-RABIN ALGO");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "selected !");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_MERSENNE_ORDER:
		{
			strcpy(l_cBuffer, "Fast computation");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "selected ! Press Q");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			strcpy(l_cBuffer, "and wait to escape.");
			drawSentence(p_iCol + 2, p_iRow++, l_cBuffer, enumJaune);
			break;
		}
		case MENU_ABOUT:
		{
			drawIntro(p_structCommon->iRow,p_structCommon->iCol);
			break;
		}
		case MENU_NEW_UNDEFINED:
		{
			break;
		}
		default:
		{
			break;
		}
	}


        attroff(COLOR_PAIR(7));

	free(l_cBuffer);
	l_cBuffer = NULL;

	refresh();
        init_pair(7, COLOR_BLACK, COLOR_BLACK);   /* référence des couleurs */
}





/** @brief
  * Draw a loading bar on a complete line
  * it's used to represent computation progression...\n\n
  *
  * You need to inititalise it -> 				e.g : drawLoadingBar(0, -1, 0, yourScreenLenght, 0);\n
  * And after you can use it with the previous screen Lenght   	e.g : drawLoadingBar(2, 25, 100, -1, enumRed);		// with the previous screen lenght\n
  * You can also modify the screen lenght 			e.g : drawLoadingBar(2, 25, 100, newScreenLenght, enumRouge); // progress bar drawed with the new screen lenght\n\n
  *
  * @param p_iLine : selected row to draw the progressbar
  * @param p_iCompleted : already done
  * @param p_iMax : max value of the progress bar
  * @param p_iScreenLenght : screen size
  * @param p_iColor : color of the progressbar
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

	/* In order to avoid to carry the Screen Lenght each time, we set it in static at the beginning
	   and to keep it put -1 in the call function. To modify it, put any number != -1 */
	if(p_iScreenLenght != -1)
	{
		l_iScreenLenght = p_iScreenLenght;
		LOG_WRITE("Now screen lenght is :")
		LOG_WRITE_LONG((long int)l_iScreenLenght)
	}

	/* In order to quit the function just after setting  the screen lenght */
	if(p_iCompleted < 0)
	{
		return;
	}

	l_iPercent = (int)(((float)p_iCompleted /(float)p_iMax)*(float)l_iScreenLenght);

	/* If the bar is taller than the screen */
	l_iPercent = (l_iPercent > l_iScreenLenght) ? l_iScreenLenght : l_iPercent;

	/* If lenght == 0 then we need to add 1 in order to avoid infinite loop */
	l_iPercent = (l_iPercent == 0) ? 1 : l_iPercent;

	/* draw body */
	for(l_iIterateur = 0; l_iIterateur < l_iPercent - 1; l_iIterateur++)
	{
		drawElement(l_iIterateur, p_iLine, l_cCaracterBody, p_iColor);
	}
	/* draw the head */
	drawElement(l_iIterateur++, p_iLine, l_cCaracterHead, p_iColor);


	/* If pair number we need to substract one (more pretty) */
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

	/* We need to see each time the bar is changed */
	refresh();
}


/**
  * Cleaning the working screen
  *
  * In order to provide a clean screen for other application, without erase the first and the last line
  * of the screen.
  *
  */
void eraseWorkingScreen(int p_iRow, int p_iCol)
{
	int l_iIterator;
	int l_iMatrixIterator;

	for(l_iIterator = 1; l_iIterator < p_iRow - 1; l_iIterator++)
	{
		for(l_iMatrixIterator = 0; l_iMatrixIterator < p_iCol ; l_iMatrixIterator++)
		{
			drawElement(l_iMatrixIterator, l_iIterator, ' ', enumNoir);
		}
	}

	refresh();
}


/**
  * @brief
  * In order to display the current mersenne order in the bottom bar
  *
  * Because user can change it, thus, we need to know it evrytime
  *
  * @param structCommon : all usefull informations on the program, in order to find the mersenne order
  */
void drawCurrentMersenneOrder(structProgramInfo* structCommon)
{
	char* l_cBuffer;
	l_cBuffer = (char*)malloc(structCommon->iCol*sizeof(char));

	if(l_cBuffer == NULL)
	{
			perror("Not enought memory - Stop all now");
			exit(ENOMEM);
	}

	snprintf(l_cBuffer, (structCommon->iCol - 1)*sizeof(char), "Current Mersenne order : %d   ", structCommon->iMersenneOrder);

	botText(l_cBuffer);

	free(l_cBuffer);
}
