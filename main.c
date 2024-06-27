#include <ncurses.h>
#include <stdio.h>
#include "nurses/ncurses_ui.c"

#define ESC 27
#define BACKSPACE 127
#define CR 13
#define h 104
#define l 108
#define j 106
#define k 107
#define i 105


void print_stat() {
    int x = getcurx(stdscr);
    int y = getcury(stdscr);

    move(32,95);
    printw("(%d, %d) -- [%d] {%c}  {%d} --%d--", y, x, cursor->data, HEAD->data, TAIL->data, current_column);
    move(y,x);

    return;
}

void insert_mode() {
    int input = 0;
    while (1) {
        print_stat();
        refresh();
        input = getc(stdin);
        switch (input) {
            case CR:
                insert_char(LF);
                break;
            case ESC:
                return;
                break;
            case BACKSPACE:
                remove_char();
                break;
            default:
                insert_char(input);
                break;
        }
    }
}

void nav_mode() {
    int input = 0;
    while (1) {
        print_stat();
        refresh();
        input = getc(stdin);
        switch (input) {
            case h:
                move_cursor_left();
                break;
            case l:
                move_cursor_right();
                break;
            case k:
                move_cursor_up();
                break;
            case j:
                move_cursor_down();
                break;
            case i:
                insert_mode();
                break;
            case ESC:
                return;
                break;
        }
    }
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No file selected!\n");
        return 0;
    } else if (argc > 2) {
        printf("Too many arguments!\n");
        return 0;
    }

    initscr();
    cbreak();
    noecho();
    
    int status;
    status = init_buffer(argv[1]);
    if (!(status)) {
        printf("Error initializing buffer!\n");
        return 0;
    }

    cursor = HEAD;
    status = print_buffer();
    if (!(status)) {
        printf("Error printing buffer!\n");
        return 0;
    }

    nav_mode();

    status = write_buffer(argv[1]);
    if (!(status)) {
        printf("Error writing buffer!\n");
        return 0;
    }

    endwin();
    return 0;
}
