#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MAX_LINES = 1000,
    MAX_LINE_LENGTH = 1024
};


void
display_file_content(WINDOW *win, char *lines[], int total_lines, int start_line, int start_col)
{
    int i;
    werase(win);
    for (i = 0; i < getmaxy(win) && (i + start_line) < total_lines; i++) {
        if ((int) strlen(lines[i + start_line]) > start_col) {
            mvwprintw(win, i, 0, "%.*s", getmaxx(win), lines[i + start_line] + start_col);
        }
    }
    wrefresh(win);
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    int start_line = 0;
    int start_col = 0;
    int ch;
    char *lines[MAX_LINES];
    int total_lines = 0;

    while (total_lines < MAX_LINES) {
        lines[total_lines] = (char *) malloc(MAX_LINE_LENGTH);
        if (fgets(lines[total_lines], MAX_LINE_LENGTH, file) == NULL) {
            break;
        }
        total_lines++;
    }

    fclose(file);

    int height = LINES - 2;
    int width = COLS - 2;
    int start_y = 1;
    int start_x = 1;

    WINDOW *file_win = newwin(height, width, start_y, start_x);
    box(file_win, 0, 0);
    mvwprintw(file_win, 0, 2, "[ %s ]", argv[1]);
    wrefresh(file_win);

    WINDOW *content_win = derwin(file_win, height - 2, width - 2, 1, 1);
    keypad(content_win, TRUE);

    display_file_content(content_win, lines, total_lines, start_line, start_col);

    while ((ch = wgetch(content_win)) != 27) {
        switch (ch) {
            case KEY_PPAGE:
            case KEY_UP:
                if (start_line > 0)
                    start_line--;
                break;
            case KEY_NPAGE:
            case KEY_DOWN:
                if (start_line + height - 2 < total_lines)
                    start_line++;
                break;
            case KEY_LEFT:
                if (start_col > 0)
                    start_col--;
                break;
            case KEY_RIGHT:
                start_col++;
                break;
        }
        display_file_content(content_win, lines, total_lines, start_line, start_col);
    }

    for (int i = 0; i < total_lines; i++) {
        free(lines[i]);
    }

    delwin(content_win);
    delwin(file_win);
    endwin();

    return 0;
}
