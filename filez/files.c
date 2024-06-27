#include <stdio.h>
#include "../bufferz/buffer.c"

int init_buffer(char file_name[]) {
    FILE *file;
    file = fopen(file_name, "r");
    int c, status;
    
    while (c != EOF) {
        c = getc(file);
        status = add_node_tail(c);
        if (!(status)) {
            fclose(file);
            return 0;
        }
    }

    fclose(file);

    return 1;
}

int write_buffer(char file_name[]) {
    if (HEAD == NULL) return 0;
    FILE *file;
    file = fopen(file_name, "w");
    
    Node *current_node = HEAD;
    do {
        putc(current_node->data, file);
        current_node = current_node->next;
    } while (current_node != TAIL);
    
    fclose(file);
    
    return 1;
}

