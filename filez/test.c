#include "files.c"
#include <stdio.h>

#define CR 13
#define ESC 27


void print_buff() {
    Node *tmp = HEAD;
    do {
        if (tmp == cursor) {
            printf("<%c", tmp->data);
        } else {
            printf("%c", tmp->data);
        }
        tmp = tmp->next;
    } while (tmp != TAIL);

    printf("%c", tmp->data);
}

void insert_mode() {
    int input = 0;
    while (1) {
        input = getc(stdin);
        switch (input) {
            case 72:
                select_previous_node();
                continue;
                break;
            case 76:
                select_next_node();
                continue;
                break;
            case CR:
                input = LF;
                break;
            case ESC:
                return;
                break;
            case 10:
                continue;
                break;
        }
        insert_node(input);
        print_buff();
    }
}

int main(int argc, char *argv[]) {
    init_buffer(argv[1]);
    cursor = HEAD;

    print_buff();
    insert_mode();

    printf("\n");
    return 0;
}
