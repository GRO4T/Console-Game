#ifndef MENU_H
#define MENU_H

#include<ncurses.h>

extern WINDOW * win;

int n_choices = 3;

void PrintMenu(WINDOW * menu_win, int highlight, vector<string> choices, int n_choices){

    int x, y, i;

    y = (BOUND_UP - 2) / 2;

    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i)
    {
        x = (BOUND_RIGHT - choices[i].length()) / 2;
        if(highlight == i + 1) /* High light the present choice */
        {	wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
        y += 2;
    }
    wrefresh(menu_win);
}
int Menu(vector<string> choices, int n_choices){
    int highlight = 1;
    int choice = 0;
    int c;
    
    wclear(win);
    PrintMenu(win, highlight, choices, n_choices);

    while(1){
        c = wgetch(win);
        switch(c)
        {
            case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
        }

        PrintMenu(win, highlight, choices, n_choices);

        if (choice != 0) //user did a choice
            break;
    }

    if (choice == n_choices)
        return 0;
    return choice;
}
#endif
