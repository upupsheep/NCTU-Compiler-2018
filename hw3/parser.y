%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"


extern int linenum;
extern FILE *yyin;
extern char *yytext;
extern char buf[256];

int yylex();
int yyerror( char *msg );

ST_stack* main_st = NULL;
//SymbolTable* current_top_list = NULL;

%}

%token <text> ID
%token <value> INT_CONST
%token <dvalue> FLOAT_CONST
%token <text> SCIENTIFIC
%token <text> STR_CONST

%token <text> LE_OP // <=
%token <text> NE_OP // !=
%token <text> GE_OP // >=
%token <text> EQ_OP // ==
%token <text> AND_OP // &&
%token <text> OR_OP // ||

%token <text> READ
%token <text> BOOLEAN
%token <text> WHILE
%token <text> DO
%token <text> IF
%token <text> ELSE
%token <text> TRUE
%token <text> FALSE
%token <text> FOR
%token <text> INT
%token <text> PRINT
%token <text> BOOL
%token <text> VOID
%token <text> FLOAT
%token <text> DOUBLE
%token <text> STRING
%token <text> CONTINUE
%token <text> BREAK
%token <text> RETURN
%token <text> CONST

%token <text> L_PAREN // (
%token <text> R_PAREN // )
%token <text> COMMA // ,
%token <text> SEMICOLON // ;
%token <text> ML_BRACE // [
%token <text> MR_BRACE // ]
%token <text> L_BRACE // {
%token <text> R_BRACE // }
%token <text> ADD_OP // +
%token <text> SUB_OP // -
%token <text> MUL_OP // *
%token <text> DIV_OP // /
%token <text> MOD_OP // %
%token <text> ASSIGN_OP // =
%token <text> LT_OP // <
%token <text> GT_OP // >
%token <text> NOT_OP // !

%type <return_type> scalar_type


// add yylval types
%union {
    int value;
    double dvalue;
    char *text;
    
    char *return_type;
}

/*  Program 
    Function 
    Array 
    Const 
    IF 
    ELSE 
    RETURN 
    FOR 
    WHILE
*/
%start program
%%

program: 
    decl_list funct_def decl_and_def_list
    {
        main_st = create();
    }
    ;

decl_list:
    decl_list var_decl
    | decl_list const_decl
    | decl_list funct_decl
    |
    ;


decl_and_def_list:
    decl_and_def_list var_decl
    | decl_and_def_list const_decl
    | decl_and_def_list funct_decl
    | decl_and_def_list funct_def
    | 
    ;

funct_def:
    scalar_type ID L_PAREN R_PAREN
    {  
        //printf("%s\n", $2);
        SymbolTable* current_top_list = NULL;
        int current_level = getCurrentLevel(main_st);
        current_top_list = push_node(current_top_list, $2, "function", current_level, $1, "");
        //display(current_top_list);
        main_st = push_stack(main_st, current_top_list); // enter a new scope
        //printf("now level: %d\n", getCurrentLevel(main_st));
        
    }
    compound_statement
    | scalar_type ID L_PAREN parameter_list R_PAREN
    {
        SymbolTable* current_top_list = NULL;
        int current_level = getCurrentLevel(main_st);
        current_top_list = push_node(current_top_list, $2, "function", current_level, $1, "");
        main_st = push_stack(main_st, current_top_list); // enter a new scope
        //printf("now level: %d\n", getCurrentLevel(main_st));
    }
    compound_statement
    | VOID ID L_PAREN R_PAREN 
    {
        SymbolTable* current_top_list = NULL;
        int current_level = getCurrentLevel(main_st);
        current_top_list = push_node(current_top_list, $2, "function", current_level, $1, "");
        main_st = push_stack(main_st, current_top_list); // enter a new scope
        //printf("now level: %d\n", getCurrentLevel(main_st));
    }
    compound_statement
    | VOID ID L_PAREN parameter_list R_PAREN
    {
        SymbolTable* current_top_list = NULL;
        int current_level = getCurrentLevel(main_st);
        current_top_list = push_node(current_top_list, $2, "function", current_level, $1, "");
        main_st = push_stack(main_st, current_top_list); // enter a new scope
        //printf("now level: %d\n", getCurrentLevel(main_st));
    }
    compound_statement
    ;

funct_decl:
    scalar_type ID L_PAREN R_PAREN SEMICOLON
    | scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON
    | VOID ID L_PAREN R_PAREN SEMICOLON
    | VOID ID L_PAREN parameter_list R_PAREN SEMICOLON
    ;

parameter_list:
    parameter_list COMMA scalar_type ID
    | parameter_list COMMA scalar_type array_decl
    | scalar_type array_decl
    | scalar_type ID
    ;

var_decl:
    scalar_type identifier_list SEMICOLON
    ;

identifier_list:
    identifier_list COMMA ID
    | identifier_list COMMA ID ASSIGN_OP logical_expression
    | identifier_list COMMA array_decl ASSIGN_OP initial_array
    | identifier_list COMMA array_decl
    | array_decl ASSIGN_OP initial_array
    | array_decl
    | ID ASSIGN_OP logical_expression
    | ID
    ;

initial_array:
    L_BRACE literal_list R_BRACE
    ;

literal_list:
    literal_list COMMA logical_expression
    | logical_expression
    | 
    ;

const_decl:
    CONST scalar_type const_list SEMICOLON
    ;

const_list:
    const_list COMMA ID ASSIGN_OP literal_const
    | ID ASSIGN_OP literal_const
    ;

array_decl:
    ID dim
    ;

dim:
    dim ML_BRACE INT_CONST MR_BRACE
    | ML_BRACE INT_CONST MR_BRACE
    ;

compound_statement:
    L_BRACE var_const_stmt_list R_BRACE
    {
        SymbolTable* top = getTop(main_st);
        //printf("aaa\n");
        display(top);
        ST_stack* tmp = pop(main_st);   
        //printf("aaa\n");
        free(tmp);
        //printf("size after pop: %d\n", getCurrentLevel(main_st));
    }
    ;

var_const_stmt_list:
    var_const_stmt_list statement 
    | var_const_stmt_list var_decl
    | var_const_stmt_list const_decl
    |
    ;

statement:
    compound_statement
    | simple_statement
    | conditional_statement
    | while_statement
    | for_statement
    | function_invoke_statement
    | jump_statement
    ;     

simple_statement:
    variable_reference ASSIGN_OP logical_expression SEMICOLON
    | PRINT logical_expression SEMICOLON
    | READ variable_reference SEMICOLON
    ;

conditional_statement:
    IF L_PAREN logical_expression R_PAREN L_BRACE var_const_stmt_list R_BRACE
    | IF L_PAREN logical_expression R_PAREN 
        L_BRACE var_const_stmt_list R_BRACE
      ELSE
        L_BRACE var_const_stmt_list R_BRACE
    ;
while_statement:
    WHILE L_PAREN logical_expression R_PAREN
        L_BRACE var_const_stmt_list R_BRACE
    | DO L_BRACE
        var_const_stmt_list
      R_BRACE WHILE L_PAREN logical_expression R_PAREN SEMICOLON
    ;

for_statement:
    FOR L_PAREN initial_expression_list SEMICOLON control_expression_list SEMICOLON increment_expression_list R_PAREN 
        L_BRACE var_const_stmt_list R_BRACE
    ;

initial_expression_list:
    initial_expression
    |
    ;

initial_expression:
    initial_expression COMMA variable_reference ASSIGN_OP logical_expression
    | initial_expression COMMA logical_expression
    | logical_expression
    | variable_reference ASSIGN_OP logical_expression
    ;

control_expression_list:
    control_expression
    |
    ;

control_expression:
    control_expression COMMA variable_reference ASSIGN_OP logical_expression
    | control_expression COMMA logical_expression
    | logical_expression
    | variable_reference ASSIGN_OP logical_expression
    ;

increment_expression_list:
    increment_expression 
    |
    ;

increment_expression:
    increment_expression COMMA variable_reference ASSIGN_OP logical_expression
    | increment_expression COMMA logical_expression
    | logical_expression
    | variable_reference ASSIGN_OP logical_expression
    ;

function_invoke_statement:
    ID L_PAREN logical_expression_list R_PAREN SEMICOLON
    | ID L_PAREN R_PAREN SEMICOLON
    ;

jump_statement:
    CONTINUE SEMICOLON
    | BREAK SEMICOLON
    | RETURN logical_expression SEMICOLON
    ;

variable_reference:
    array_list
    | ID
    ;


logical_expression:
    logical_expression OR_OP logical_term
    | logical_term
    ;

logical_term:
    logical_term AND_OP logical_factor
    | logical_factor
    ;

logical_factor:
    NOT_OP logical_factor
    | relation_expression
    ;

relation_expression:
    relation_expression relation_operator arithmetic_expression
    | arithmetic_expression
    ;

relation_operator:
    LT_OP
    | LE_OP
    | EQ_OP
    | GE_OP
    | GT_OP
    | NE_OP
    ;

arithmetic_expression:
    arithmetic_expression ADD_OP term
    | arithmetic_expression SUB_OP term
    | term
    ;

term:
    term MUL_OP factor
    | term DIV_OP factor
    | term MOD_OP factor
    | factor
    ;

factor:
    SUB_OP factor
    | literal_const
    | variable_reference
    | L_PAREN logical_expression R_PAREN
    | ID L_PAREN logical_expression_list R_PAREN
    | ID L_PAREN R_PAREN
    ;

logical_expression_list:
    logical_expression_list COMMA logical_expression
    | logical_expression
    ;

array_list:
    ID dimension
    ;

dimension:
    dimension ML_BRACE logical_expression MR_BRACE         
    | ML_BRACE logical_expression MR_BRACE
    ;



scalar_type:
      INT {$$ = "int";}
    | DOUBLE {$$ = "double";}
    | STRING {$$ = "string";}
    | BOOL {$$ = "bool";}
    | FLOAT {$$ = "float";}
    ;
 
literal_const:
    INT_CONST
    | FLOAT_CONST
    | SCIENTIFIC
    | STR_CONST
    | TRUE
    | FALSE
    ;


%%

int yyerror( char *msg )
{
    fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
    fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
    fprintf( stderr, "|\n" );
    fprintf( stderr, "| Unmatched token: %s\n", yytext );
    fprintf( stderr, "|--------------------------------------------------------------------------\n" );
    exit(-1);
    //  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}

int  main( int argc, char **argv )
{
    //////////////////
    // ST_stack* list = NULL;
    // SymbolTable* st_list = NULL;
    // st_list = push_node(st_list, "aa", "bb", 1, "dd", "ee");
    // printf("length: %d\n", get_length(st_list));
    // st_list = push_node(st_list, "aaa", "bbb", 2, "ddd", "eee");
    // SymbolTable* pop_result = NULL;
    // list = push_stack(list, st_list);
    // pop_result = getTop(list);
    //display(st_list);
    // display(pop_result);
    // printf("current level: %d\n", getCurrentLevel(list));
    /////////////////
    if( argc == 1 )
    {
        yyin = stdin;
    }
    else if( argc == 2 )
    {
        FILE *fp = fopen( argv[1], "r" );
        if( fp == NULL ) {
                fprintf( stderr, "Open file error\n" );
                exit(-1);
        }
        yyin = fp;
    }
    else
    {
        fprintf( stderr, "Usage: ./parser [filename]\n" );
        exit(0);
    } 

    yyparse();  /* primary procedure of parser */

    
    fprintf( stdout, "\n|--------------------------------|\n" );
    fprintf( stdout, "|  There is no syntactic error!  |\n" );
    fprintf( stdout, "|--------------------------------|\n" );
    exit(0);
}


