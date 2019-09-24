#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolTable* new_node(char _name[33], char _kind[10], int _level, char* _type,
                      char* _attr) {
    SymbolTable* node;
    node = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (node == NULL) return NULL;
    strcpy(node->name, _name);
    strcpy(node->kind, _kind);
    node->level = _level;
    node->type = strdup(_type);
    node->attr = strdup(_attr);
    node->next = NULL;
    return node;
}
SymbolTable* push_node(SymbolTable* list, char _name[33], char _kind[10],
                       int _level, char* _type, char* _attr) {
    SymbolTable* node = new_node(_name, _kind, _level, _type, _attr);
    if (list == NULL) {
        list = node;
    } else {
        SymbolTable* p = list;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = node;
    }
    return list;
}
int get_length(SymbolTable* list) {
    SymbolTable* p = list;
    int cnt = 0;
    while (p != NULL) {
        cnt++;
        p = p->next;
    }
    return cnt;
}
void display(SymbolTable* list) {
    if (!Opt_Symbol) return;
    SymbolTable* p = list;
    printf(
        "======================================================================"
        "=================");
    printf("\n");
    printf("%-33s%-11s%-12s%-19s%-24s\n", "Name", "Kind", "Level", "Type",
           "Attribute");
    printf(
        "----------------------------------------------------------------------"
        "-----------------");
    printf("\n");
    while (p != NULL) {
        printf("%-33s", p->name);
        printf("%-11s", p->kind);
        if (p->level <= 0)
            printf("%d%-11s", p->level, "(global)");
        else
            printf("%d%-11s", p->level, "(local)");
        printf("%-19s", p->type);
        printf("%-24s\n", p->attr);
        p = p->next;
    }
    printf(
        "======================================================================"
        "=================");
    printf("\n");
}

ST_stack* create() { return NULL; }
int isEmpty(ST_stack* top) { return (top == NULL); }
ST_stack* push_stack(ST_stack* top, SymbolTable* _st) {
    ST_stack* newnode;
    newnode = (ST_stack*)malloc(sizeof(ST_stack));
    if (newnode == NULL) {
        printf("stack allocation error!\n");
        exit(1);
    }
    newnode->st = _st;
    newnode->next = top;
    top = newnode;
    return top;
}
ST_stack* pop(ST_stack* top) {
    ST_stack* tmpnode;
    tmpnode = top;
    if (tmpnode == NULL) {
        printf("stack is emtpy!\n");
        return NULL;
    }
    top = top->next;
    free(tmpnode);
    return top;
}
SymbolTable* getTop(ST_stack* top) { return top->st; }
int getCurrentLevel(ST_stack* list) {
    ST_stack* p = list;
    int cnt = 0;
    while (p != NULL) {
        cnt++;
        p = p->next;
    }
    return cnt;
}