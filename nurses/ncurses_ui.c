#include <stdio.h>
#include <ncurses.h>
#include "../filez/files.c"

int move_cursor_left() {
    int success = 0;
    success = select_previous_node();
    if (!(success)) return 0;

    move(getcury(stdscr), (getcurx(stdscr)-1));
    return 1;
}

int move_cursor_right() {
    int success = 0;
    success = select_next_node();
    if (!(success)) return 0;

    move(getcury(stdscr), (getcurx(stdscr)+1));
    return 1;
}

void move_cursor_up() {
    int offset = 0;
    offset = select_above_node();
    if (offset == -1) return;

    move((getcury(stdscr)-1), offset);
    return;
}

void move_cursor_down() {
    int offset = 0;
    offset = select_below_node();
    if (offset == -1) {
        return;
    }

    move((getcury(stdscr)+1), offset);
    return;
}

int print_buffer() {
    if (HEAD == NULL) return 0;

    Node *current_node = HEAD;

    while (current_node->next != NULL) {
        if (current_node->data == LF) {
            waddch(stdscr, *"\\");
        }
        waddch(stdscr, current_node->data);
        current_node = current_node->next;
    }

    move(0,0);
    return 1;
}

int print_from_cursor() {
    int y = getcury(stdscr);
    int x = getcurx(stdscr);

    Node *tmp = cursor;
    while (tmp->next != NULL) {
        if (tmp->data == LF) {
            waddch(stdscr, *"\\");
            wclrtoeol(stdscr);
        }
        waddch(stdscr, tmp->data);
        tmp = tmp->next;
    }

    move(getcury(stdscr)+1, 0);
    wclrtoeol(stdscr);

    move(y, x);
    return 1;
}

int print_line() {
    if (cursor->data == LF) return 0;

    int distance_from_start = 0;
    while (cursor->data != LF && cursor != TAIL) {
        waddch(stdscr, cursor->data);
        cursor = cursor->next;
        distance_from_start++;
    }

    while (distance_from_start != 0) {
        move(getcury(stdscr), (getcurx(stdscr)-1));
        cursor = cursor->previous;
        distance_from_start--;
    }

    return 1;
}

int insert_char(int data) {
    int status = 0;
    status = insert_node(data);
    if (!(status)) return 0;

    waddch(stdscr, data);

    if (data == LF) {
        print_from_cursor();
    } else {
        print_line();
    }

    return 1;
}

int remove_char() {
    if (cursor == HEAD) return 0;

    Node *cursor_prev = cursor->previous;
    if (cursor_prev->data == LF) {
        int length = get_previous_line_length();
        if (length == -1) return -1;

        int status;
        status = remove_node();
        if (!(status)) return 0;

        current_column = length;

        move(getcury(stdscr)-1, length);

        print_from_cursor();
    } else {
        
        int status;
        status = remove_node();
        if (!(status)) return 0;

        move(getcury(stdscr), (getcurx(stdscr)-1));

        wclrtoeol(stdscr);
        print_line();
    }

    return 1;
}

