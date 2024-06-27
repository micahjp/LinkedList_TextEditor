#include <stdio.h>
#include <stdlib.h>

#define LF 10
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4


typedef struct {
    void *previous;
    void *next;
    int data;
} Node;

Node *HEAD = NULL;
Node *TAIL = NULL;
Node *cursor = NULL;
int current_column = 0;


int add_node_tail(int data) {
    Node *new_node = NULL;

    new_node = malloc(sizeof(Node));
    if (new_node == NULL) return 0;

    new_node->data = data;


    if (HEAD == NULL) {
        HEAD = new_node;
        TAIL = new_node;

        return 1;
    } else {
        new_node->previous = TAIL;
        TAIL->next = new_node;
        TAIL = new_node;

        return 1;
    }
}

int insert_node(int data) {
    Node *new_node = NULL;

    new_node = malloc(sizeof(Node));
    if (new_node == NULL) return 0;

    new_node->data = data;

    if (cursor == HEAD) {
        HEAD->previous = new_node;
        new_node->next = HEAD;

        HEAD = new_node;
    } else {
        new_node->previous = cursor->previous;
        cursor->previous = new_node;
        new_node->next = cursor;
        
        cursor = new_node->previous;
        cursor->next = new_node;

        cursor = new_node->next;
        
    }
    
    if (data != LF) {
        current_column++;
    } else {
        current_column = 0;
    }
    return 1;
}

int remove_node() {
    if (cursor == HEAD) return 0;

    Node *dying_node = cursor->previous;

    if (dying_node == HEAD) {
        HEAD = cursor;
        cursor->previous = NULL;

        free(dying_node);
        dying_node = NULL;
    } else {
        cursor->previous = dying_node->previous;
        cursor = cursor->previous;
        cursor->next = dying_node->next;
        cursor = cursor->next;

        free(dying_node);
        dying_node = NULL;
    }

    if (current_column != 0) current_column--;
    return 1;
}

// returns -1 for no movement
int select_above_node() {
    if (cursor == HEAD) return -1;

    int distance_from_start = 0;

    // move cursor backwards to start of current line unless current line is 
    // first in file
    do {
        cursor = cursor->previous;
        distance_from_start++;
    } while (cursor->data != LF && cursor != HEAD);


    if (cursor == HEAD) {
        // puts cursor back to where it was and exits
        while (distance_from_start != 0) {
            cursor = cursor->next;
            distance_from_start--;
        }
        return -1;
    }

    // moves cursor backwards to start of previous line
    do {
        cursor = cursor->previous;
        distance_from_start++;
        // the statement below represents 2 different distances from the
        // destination
    } while (cursor->data != LF && cursor != HEAD);
    
    if (cursor == HEAD) {
        for (int i = 1; i <= current_column; i++) {
            cursor = cursor->next;
            distance_from_start--;
            if (cursor->data == LF || cursor == TAIL) {
                return i;
            }
        }
    } else {
        for (int i = 0; i <= current_column; i++) {
            cursor = cursor->next;
            distance_from_start--;
            if (cursor->data == LF || cursor == TAIL) {
                return i;
            }
        }
    }

    return current_column;
}

// returns -1 for no movement
int select_below_node() {
    if (cursor == TAIL) return -1;

    int distance_from_start = 0;
    // move cursor to the end of the line
    while (cursor->data != LF && cursor != TAIL) {
        cursor = cursor->next;
        distance_from_start++;
    }

    // if the cursor is the last in the file
    if (cursor == TAIL || cursor->next == TAIL) {
        // return cursor to previous position
        while (distance_from_start != 0) {
            cursor = cursor->previous;
            distance_from_start--;
        }
        return -1;
    } else {
        // move cursor to the first char on the next line
        cursor = cursor->next;
    }

    for (int i = 0; i < current_column; i++) {
        if (cursor->data == LF || cursor == TAIL) {
            return i;
        }
        cursor = cursor->next;
    }

    return current_column;
}

int select_previous_node() {
    if (cursor == HEAD) return 0;

    cursor = cursor->previous;

    if (cursor->data == LF) {
        cursor = cursor->next;
    } else {
        current_column--;
    }

    return 1;
}

int select_next_node() {
    if (cursor->data == LF || cursor->next == NULL) return 0;

    cursor = cursor->next;
    current_column++;

    return 1;
}

int get_previous_line_length() {
    int distance_from_start = 0;
    int length = -1;

    Node *tmp = cursor->previous;
    while (tmp->data != LF && tmp != HEAD) {
        tmp = tmp->previous;
        distance_from_start++;
    }

    if (tmp == HEAD) {
        while (distance_from_start) {
            tmp = tmp->next;
            distance_from_start--;
        }
        return length;
    }

    do {
        tmp = tmp->previous;
        length++;
        distance_from_start++;

        if (tmp == HEAD) {
            length++;
            break;
        }
    } while (tmp->data != LF);

    // put cursor back to original place
    // while (distance_from_start) {
    //     cursor = cursor->next;
    //     distance_from_start--;
    // }

    return length;
}
