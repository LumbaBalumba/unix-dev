#include <locale.h>
#include <ncurses.h>

int
main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    initscr();
    move(LINES / 2, COLS / 3);
    addstr("Привет, мир!");
    getch();
    endwin();
}
