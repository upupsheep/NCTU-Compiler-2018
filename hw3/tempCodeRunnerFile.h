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

SymbolTable* new_node(char _name[33], char _kind[10], int, char*, char*);
SymbolTable* push_node(SymbolTable* list, char _name[33], char _kind[10], int,
                       char*, char*);
int get_length(SymbolTable*);
void display(SymbolTable*);