/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : /include/ansi.h
 * Author : Cwlin
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 
155 6D m * SGR - Set Graphics Rendition (affects character attributes)
	 *	[0m = Clear all special attributes
	 *	[1m = Bold or increased intensity
	 *	[2m = Dim or secondary color on GIGI  (superscript on XXXXXX)
		[3m = Italic                          (subscript on XXXXXX)
	 *	[4m = Underscore, [0;4m = Clear, then set underline only
	 *	[5m = Slow blink
		[6m = Fast blink                      (overscore on XXXXXX)
	 *	[7m = Negative image, [0;1;7m = Bold + Inverse
		[8m = Concealed (do not display character echoed locally)
		[9m = Reserved for future standardization
	 *	[10m = Select primary font (LA100)
	 *	[11m - [19m = Selete alternate font (LA100 has 11 thru 14)
		[20m = FRAKTUR (whatever that means)
	 *	[22m = Cancel bold or dim attribute only (VT220)
	 *	[24m = Cancel underline attribute only (VT220)
	 *	[25m = Cancel fast or slow blink attribute only (VT220)
	 *	[27m = Cancel negative image attribute only (VT220)
	 *	[30m = Write with black,   [40m = Set background to black (GIGI)
	 *	[31m = Write with red,     [41m = Set background to red
	 *	[32m = Write with green,   [42m = Set background to green
	 *	[33m = Write with yellow,  [43m = Set background to yellow
	 *	[34m = Write with blue,    [44m = Set background to blue
	 *	[35m = Write with magenta, [45m = Set background to magenta
	 *	[36m = Write with cyan,    [46m = Set background to cyan
	 *	[37m = Write with white,   [47m = Set background to white

Minimum requirements for VT100 emulation:

  [A       Sent by the up-cursor key (alternately ESC O A)
  [B       Sent by the down-cursor key (alternately ESC O B)
  [C       Sent by the right-cursor key (alternately ESC O C)
  [D       Sent by the left-cursor key (alternately ESC O D)
  OP       PF1 key sends ESC O P
  OQ       PF2 key sends ESC O Q
  OR       PF3 key sends ESC O R
  OS       PF3 key sends ESC O S
  [c       Request for the terminal to identify itself
  [?1;0c   VT100 with memory for 24 by 80, inverse video character attribute
  [?1;2c   VT100 capable of 132 column mode, with bold+blink+underline+inverse

  [0J     Erase from current position to bottom of screen inclusive
  [1J     Erase from top of screen to current position inclusive
  [2J     Erase entire screen (without moving the cursor)
  [0K     Erase from current position to end of line inclusive
  [1K     Erase from beginning of line to current position inclusive
  [2K     Erase entire line (without moving cursor)
  [12;24r   Set scrolling region to lines 12 thru 24.  If a linefeed or an
            INDex is received while on line 24, the former line 12 is deleted
            and rows 13-24 move up.  If a RI (reverse Index) is received while
            on line 12, a blank line is inserted there as rows 12-13 move down.
            All VT100 compatible terminals (except GIGI) have this feature.
*/

#ifndef ANSI_H
#define ANSI_H		1

#define ESC     "\e"
#define CSI     ESC"["		/* Control Sequence Introducer */
#define SGR(x)	CSI + x + "m"	/* Set Graphics Rendition */


#define NOR		SGR("")		/* �M���Ҧ��S���ݩ� */
#define BOLD		SGR("1")	/* ����Ω���� */

#define ITALIC		SGR("3")	/* ���� */
#define U		SGR("4")	/* �U�� */
#define BLINK		SGR("5")	/* �{�{ (�C) */

#define REV		SGR("7")	/* �e/�I���� ���� */
#define HIREV		SGR("1;7")	/* ���� �e/�I���� ���� */


/* �e���� */
 
#define BLK SGR("30")		/* �� */
#define RED SGR("31")		/* �� */
#define GRN SGR("32")		/* �� */
#define YEL SGR("33")		/* �� */
#define BLU SGR("34")		/* �� */
#define MAG SGR("35")		/* �� */
#define CYN SGR("36")		/* �C */
//#define WHT SGR("37")		/* �� */
#define WHT SGR("1;30")		/* �� */

/* �I���� */
 
#define BBLK SGR("40")		/* �� */
#define BRED SGR("41")		/* �� */
#define BGRN SGR("42")		/* �� */
#define BYEL SGR("43")		/* �� */
#define BBLU SGR("44")		/* �� */
#define BMAG SGR("45")		/* �� */
#define BCYN SGR("46")		/* �C */
#define BWHT SGR("47")		/* �� */

/* ���� �e���� */
#define HBLK SGR("1;30")	/* �� */
#define HIR SGR("1;31")		/* �� */
#define HIG SGR("1;32")		/* �� */
#define HIY SGR("1;33")		/* �� */
#define HIB SGR("1;34")		/* �� */
#define HIM SGR("1;35")		/* �� */
#define HIC SGR("1;36")		/* �C */
#define HIW SGR("1;37")		/* �� */

/* ���� �I���� */
#define HBBLK SGR("1;40")	/* �H�� */
#define HBRED SGR("1;41")	/* �� */
#define HBGRN SGR("1;42")	/* �� */
#define HBYEL SGR("1;43")	/* �� */
#define HBBLU SGR("1;44")	/* �� */
#define HBMAG SGR("1;45")	/* �� */
#define HBCYN SGR("1;46")	/* �C */
#define HBWHT SGR("1;47")	/* �� */

 
/*  Additional ansi Esc codes added to ansi.h by Gothic  april 23,1993 */
/* Note, these are Esc codes for VT100 terminals, and emmulators */
/*       and they may not all work within the mud               */
#define BEEP		"\a"		/* ĵ�i�n */
#define CLR		CSI"2J"		/* Clear the screen  */
#define HOME		CSI"H"		/* Send cursor to home position */
#define SAVEC		CSI"s"		/* Save cursor position */
#define REST		CSI"u"		/* Restore cursor to saved position */
#define FRTOP		CSI"2;25r"	/* Freeze top line */
#define FRBOT		CSI"1;24r"	/* Freeze bottom line */
#define UNFR		CSI"r"		/* Unfreeze top and bottom lines */
#define FREEZE(x,y)	CSI+x+";"+y+"r" /* Freeze ��q x �� y �o�X��@�� screen Added by Clode@RW */
#define MOVEC(x,y)	CSI+x+";"+y+"H" /* �N cursor ���ܲ� x ��� y �r Added by Clode@RW */

#define REF		CLR+HOME	/* Clear screen and home cursor */

#define REVINDEX	ESC+"M"		/* Scroll screen in opposite direction */
#define BIGTOP		ESC+"#3"	/* Dbl height characters, top half */
#define BIGBOT		ESC+"#4"	/* Dbl height characters, bottem half */
#define SINGW		ESC+"#5"	/* Normal, single-width characters */
#define DBL		ESC+"#6"	/* Creates double-width characters */

#endif
