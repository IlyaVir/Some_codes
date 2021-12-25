#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void error(void) {
    exit(1);
}

typedef size_t list_value_type;

struct Node {
    list_value_type value;
    struct Node *prev, *next;
};

typedef struct Node Node;

Node *change_node(Node *node, list_value_type value, Node *prev, Node *next) {
    node->value = value;
    node->prev = prev;
    node->next = next;

    return node;
}

Node *node(list_value_type value, Node *prev, Node *next) {
    Node *node = malloc(sizeof(Node));
    if (node == NULL) {
        error();
    }

    return change_node(node, value, prev, next);
}



struct List {
    Node *begin, *end;
    size_t size;
};

typedef struct List List;

List list(void) {
    Node *end_node = node(0, NULL, NULL);
    List list = {end_node, end_node, 0};
    return list;
}

void list_push_back(List *list, list_value_type value) {
    change_node(list->end, value, list->end->prev, node(0, list->end, NULL));
    list->end = list->end->next;
    ++list->size;
}

void list_push_front(List *list, list_value_type value) {
    if (list->begin == list->end) {
        list_push_back(list, value);
    } else {
        change_node(list->begin, list->begin->value, node(value, NULL, list->begin), list->begin->next);
        list->begin = list->begin->prev;
    }
    ++list->size;
}

bool list_pop_back(List *list) {
    if (list->begin != list->end) {
        list->end = list->end->prev;
        free(list->end->next);
        --list->size;
        return true;
    } else {
        return false;
    }
}

bool list_pop_front(List *list) {
    if (list->begin != list->end) {
        list->begin = list->begin->next;
        free(list->begin->prev);
        --list->size;
        return true;
    } else {
        return false;
    }
}

void list_clear(List *list) {
    while (list->begin != list->end) {
        list->begin = list->begin->next;
        free(list->begin->prev);
    }
    list->size = 0;
}

void list_print(List *list) {
    for (Node *now = list->begin; now != list->end; now = now->next) {
        printf("%zu ", now->value);
    }
}
