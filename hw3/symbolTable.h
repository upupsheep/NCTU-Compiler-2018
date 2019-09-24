extern int linenum;  /* declared in lex.l */
extern char* yytext; /* declared by lex */
extern int Opt_Symbol;

struct SymbolTable {
    char name[33];
    char kind[10];
    int level;
    char* type;
    char* attr;
    struct SymbolTable* next;
};
typedef struct SymbolTable SymbolTable;

struct ST_stack {
    struct SymbolTable* st;
    struct ST_stack* next;
};
typedef struct ST_stack ST_stack;

SymbolTable* new_node(char _name[33], char _kind[10], int, char*, char*);
SymbolTable* push_node(SymbolTable* list, char _name[33], char _kind[10], int,
                       char*, char*);
int get_length(SymbolTable*);
void display(SymbolTable*);

ST_stack* create();
int isEmpty(ST_stack*);
ST_stack* push_stack(ST_stack*, SymbolTable*);
ST_stack* pop(ST_stack*);
SymbolTable* getTop(ST_stack*);
int getCurrentLevel(ST_stack*);