/*========================================
 *    sl.c: SL version 5.03
 *        Copyright 1993,1998,2014-2015
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2014/06/03
 *
 *	Modified by ya0201: 2019/12/22
 *========================================
 */
/* be version 1.00 : Initial build executioner.                              */
/*                                              by ya0201         2019/12/22 */
/* sl version 5.03 : Fix some more compiler warnings.                        */
/*                                              by Ryan Jacobs    2015/01/19 */
/* sl version 5.02 : Fix compiler warnings.                                  */
/*                                              by Jeff Schwab    2014/06/03 */
/* sl version 5.01 : removed cursor and handling of IO                       */
/*                                              by Chris Seymour  2014/01/03 */
/* sl version 5.00 : add -c option                                           */
/*                                              by Toyoda Masashi 2013/05/05 */
/* sl version 4.00 : add C51, usleep(40000)                                  */
/*                                              by Toyoda Masashi 2002/12/31 */
/* sl version 3.03 : add usleep(20000)                                       */
/*                                              by Toyoda Masashi 1998/07/22 */
/* sl version 3.02 : D51 flies! Change options.                              */
/*                                              by Toyoda Masashi 1993/01/19 */
/* sl version 3.01 : Wheel turns smoother                                    */
/*                                              by Toyoda Masashi 1992/12/25 */
/* sl version 3.00 : Add d(D51) option                                       */
/*                                              by Toyoda Masashi 1992/12/24 */
/* sl version 2.02 : Bug fixed.(dust remains in screen)                      */
/*                                              by Toyoda Masashi 1992/12/17 */
/* sl version 2.01 : Smoke run and disappear.                                */
/*                   Change '-a' to accident option.                         */
/*                                              by Toyoda Masashi 1992/12/16 */
/* sl version 2.00 : Add a(all),l(long),F(Fly!) options.                     */
/*                                              by Toyoda Masashi 1992/12/15 */
/* sl version 1.02 : Add turning wheel.                                      */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.01 : Add more complex smoke.                                 */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.00 : SL runs vomiting out smoke.                             */
/*                                              by Toyoda Masashi 1992/12/11 */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include "be.h"

int add_BE();
/* void option(char *str); */
int my_mvaddstr(int y, int x, char *str);

int my_mvaddstr(int y, int x, char *str)
{
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

/* void option(char *str) */
/* { */
/*     extern int ACCIDENT, LOGO, FLY, C51; */

/*     while (*str != '\0') { */
/*         switch (*str++) { */
/*             case 'a': ACCIDENT = 1; break; */
/*             case 'F': FLY      = 1; break; */
/*             case 'l': LOGO     = 1; break; */
/*             case 'c': C51      = 1; break; */
/*             default:                break; */
/*         } */
/*     } */
/* } */

int main(int argc, char *argv[])
{
    int x, ctr = 0;
    int total_duration=150;
    int animation_duration=12;

    /* for (i = 1; i < argc; ++i) { */
    /*     if (*argv[i] == '-') { */
    /*         option(argv[i] + 1); */
    /*     } */
    /* } */
    initscr();
    start_color(); // color setting
    init_pair(1, 232, COLOR_BLACK);
    init_pair(2, 234, COLOR_BLACK);
    init_pair(3, 238, COLOR_BLACK);
    init_pair(4, 244, COLOR_BLACK);
    init_pair(5, 252, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    signal(SIGINT, SIG_IGN);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE);

    x = COLS/2;
    attrset(COLOR_PAIR(1));
    /* usleep(400000); */
    for (ctr=0; ctr < total_duration; ctr++) {
        if (add_BE() == ERR) break;
        getch();
        switch (ctr / (animation_duration / 5)) {
          case 0:
            attrset(COLOR_PAIR(1));
            break;
          case 1:
            attrset(COLOR_PAIR(2));
            break;
          case 2:
            attrset(COLOR_PAIR(3));
            break;
          case 3:
            attrset(COLOR_PAIR(4));
            break;
          case 4:
            attrset(COLOR_PAIR(5));
            break;
          default:
            attrset(COLOR_PAIR(6));
            break;
        }
        refresh();
        usleep(40000);
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    attrset(0);
    endwin();

    return 0;
}

int add_BE() {
  static char *BE[BEHEIGHT]
    = {BE01, 
      BE02, 
      BE03, 
      BE04, 
      BE05, 
      BE06, 
      BE07, 
      BE08, 
      BE09, 
      BE10, 
      BE11, 
      BE12, 
      BE13, 
      BE14, 
      BE15, 
      BE16, 
      BE17, 
      BE18, 
      BE19, 
      BE20, 
      BE21, 
      BE22, 
      BE23, 
      BE24, 
      BE25, 
      BE26, 
      BE27, 
      BE28, 
      BE29, 
      BE30, 
      BE31, 
      BE32, 
      BE33, 
      BE34, 
      BE35, 
      BE36, 
      BE37, 
      BE38, 
      BE39, 
      BE40, 
      BE41, 
      BE42, 
      BE43};
  int x, y, i;

  x = COLS / 2 - (BELENGTH / 2);
  y = LINES / 2 - (BEHEIGHT / 2);
  if (x < - BELENGTH)  return ERR;
  for (i = 0; i < BEHEIGHT; ++i) {
    my_mvaddstr(y + i, x, BE[i]);
  }
  return OK;
}
