%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}

%token ID           /* identifier */
%token INT FLOAT SCIENTIFIC
%token TYPE			/* data type decla */
%token READ WHILE DO IF TRUE FALSE FOR CONST PRINT CONTINUE BREAK RETURN
%token STRING
%token VOID
%token AND OR LE NE GE EQ

%nonassoc ELSE
%nonassoc ';'

%right '='
%right '!'
%left PARENTHESES
%left UNARY
%left NOT
%left '+' '-'
%left '*' '/' '%'
%left '>' '<'
%left AND OR
%left LE NE GE EQ

%%
program
	: declaration_list funct_def decl_and_def_list
	| /* epsilon */
	;

decl_and_def_list
	: decl_and_def_list funct_decl
	| decl_and_def_list var_decl
	| decl_and_def_list const_decl
	| decl_and_def_list funct_def
	| /* epsilon */
	;

declaration_list
	: declaration_list const_decl
    | declaration_list var_decl
    | declaration_list funct_decl
	| /* epsilon */
	;

funct_decl
	: TYPE ID '(' arguments ')' ';'
	| procedure_decl
	;

procedure_decl
	: VOID ID '(' arguments ')' ';'
	;

arguments
	: /* epsilon */
	| nonEmptyArgc
	;

nonEmptyArgc
	: nonEmptyArgc ',' argument_whole
	| argument_whole
	;

argument_whole
	: TYPE ident_no_init
	;



var_decl
	: TYPE ident_list ';'
	;

ident_list
	: ident_list ',' ident_decl
	| ident_decl
	;

ident_decl
	: ident_no_init
	| ident_with_init
	;


ident_with_init
	: ID '=' expression
	| ID arrWithSize '=' initial_array
	;

ident_no_init
	: ID
	| ID arrWithSize
	;

arrWithSize
	: '[' INT ']'
	| '[' INT ']' arrWithSize
	;

initial_array
	: '{' zero_or_more_exp '}'
	;

zero_or_more_exp
	: /* epsilon */
	| more_expression
	;

more_expression
	: expression
	| expression ',' more_expression
	;

const_decl
	: CONST TYPE const_list ';'
	;

/* shift/reduce conflicts */
const_list
	: const_list ',' const_list_elim
	| const_list_elim
	;

const_list_elim
	: ID '=' literal_constant
	;

literal_constant
	: INT
	| FLOAT
	| SCIENTIFIC
	| TRUE
	| FALSE
	| STRING
	;

funct_def
	: TYPE ID '(' arguments ')' compound_stmt
	| procedure_def
	;

procedure_def
	: VOID ID '(' arguments ')' compound_stmt

statement
	: compound_stmt
	| simple_stmt
	| conditional_stmt
	| while_stmt
	| for_stmt
	| jump_stmt
	| funct_invoc ';'
	;

compound_stmt
	: '{' compound_content '}'
	;

compound_content
	: compound_content const_decl
	| compound_content var_decl
	| compound_content statement
	| /* epsilon */
	;

variable_ref
	: ID
	| array_ref
	;

array_ref
	: ID array_ref_square
	;

array_ref_square
	: array_ref_square square_expression
	| square_expression
	;

square_expression
	: '[' expression ']'
	;

simple_stmt
	: simple_body ';'
	;

simple_body
	: variable_ref '=' expression
	// | PRINT variable_ref
	| PRINT expression
	| READ expression
	;

expression
	: '-' expression %prec UNARY
	| '!' expression %prec NOT
	| '(' expression ')' %prec PARENTHESES
	| expression '+' expression
	| expression '-' expression
	| expression '*' expression
	| expression '/' expression
	| expression '%' expression
	| expression '<' expression
	| expression '>' expression
	| expression LE expression
	| expression GE expression
	| expression EQ expression
	| expression NE expression
	| expression AND expression
	| expression OR expression
	| variable_ref
	| literal_constant
	| funct_invoc
	;

funct_invoc
	: ID '(' zero_or_more_exp ')'
	;

conditional_stmt
	: IF '(' boolean_expression ')' compound_stmt ELSE compound_stmt
	| IF '(' boolean_expression ')' compound_stmt
	;

boolean_expression
	: expression
	;

while_stmt
	: WHILE '(' boolean_expression ')' compound_stmt
	| DO compound_stmt WHILE '(' boolean_expression ')' ';'
	;

for_stmt
	: FOR '(' initial_exp ';' control_exp ';' increment_exp ')' compound_stmt
	;

initial_exp
	: expression
	| ID '=' expression
	;

control_exp
	: expression
	| ID '=' expression
	;

increment_exp
	: expression
	| ID '=' expression
	;

jump_stmt
	: RETURN expression ';'
	| BREAK ';'
	| CONTINUE ';'
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
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );
	
	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}
	
	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}
