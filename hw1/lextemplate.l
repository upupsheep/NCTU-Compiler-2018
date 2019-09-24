%{
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<\"%s\">\n", t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char buf[MAX_LINE_LENGTH];
char bufStr[MAX_LINE_LENGTH];
char *s;
int ID_cnt = 0;
int isExist = 0;
struct ID{
    char context[MAX_ID_LENGTH];
    int occur;
};
struct ID IDs[1000];
//IDs[0],context[0] = '\0'; 
void ID_freq(char *);
void printID();
%}
%x STRING
%x COMMENT
%x PRAGMA
%x SOURCE
%x TOKEN
%x STATISTIC

digit [0-9]
letter [_a-zA-Z]
keyword ("while"|"do"|"if"|"else"|"true"|"false"|"for"|"int"|"print"|"const"|"read"|"boolean"|"bool"|"void"|"float"|"double"|"string"|"continue"|"break"|"return")
delimiter [,;\(\)\[\]\{\}]
space [ \t]+
identifier {letter}({letter}|{digit})*
integer (0|([1-9][0-9]*))
float {digit}+"."{digit}+
scientific ({float}|{integer})[eE][\+\-]?{digit}+
operator [\+\-\*\/\%=]
logical ("<"|"<="|"!="|">="|">"|"=="|"&&"|"||"|"!")

%%

 /* STRING */
\"              { BEGIN STRING; s = bufStr; LIST; }
<STRING>\\\\    { *s++ = '\\'; LIST; }
<STRING>\\\"    { *s++ = '\"'; LIST;}
<STRING>\"      { *s = 0; BEGIN(0); tokenString("string", bufStr); }
<STRING>\n      { printf("Error at line %d:\n", linenum); exit(1); }
<STRING>.       { *s++ = *yytext; LIST; }

 /* COMMENT */
"/*"            { BEGIN COMMENT; LIST; }
<COMMENT>.      { LIST; }
<COMMENT>(.)*"*/"   { LIST; BEGIN(0); }
<COMMENT>"\n"   { 
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

 /* c++ style comment */
"//".*\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

 /* PRAGMA */
^"#pragma"                  { LIST; BEGIN PRAGMA; }
<PRAGMA>{space}"source"     { LIST; BEGIN SOURCE; }
<PRAGMA>{space}"token"      { LIST; BEGIN TOKEN; }
<PRAGMA>{space}"statistic"  { LIST; BEGIN STATISTIC; }

 /* PRAGMA_COMMENT */
<SOURCE>{space}"on"({space})*("/*") {
    LIST;
    Opt_Source = 1;
    BEGIN COMMENT;
}
<SOURCE>{space}"off"({space})*("/*") {
    LIST;
    Opt_Source = 0;
    BEGIN COMMENT;
}
<TOKEN>{space}"on"({space})*("/*") {
    LIST;
    Opt_Token = 1;
    BEGIN COMMENT;
}
<TOKEN>{space}"off"({space})*("/*") {
    LIST;
    Opt_Token = 0;
    BEGIN COMMENT;
}
<STATISTIC>{space}"on"({space})*("/*") {
    LIST;
    Opt_Statistic = 1;
    BEGIN COMMENT;
}
<STATISTIC>{space}"off"({space})*("/*") {
    LIST;
    Opt_Statistic = 0;
    BEGIN COMMENT;
}
 /* SOURCE */
<SOURCE>{space}"on"({space})*((("//")(.)*"\n")|("\n")) {
     LIST;
     Opt_Source = 1;
     if(Opt_Source)
         printf("%d:%s\n", linenum, buf);
     ++linenum;
     buf[0] = '\0';
     BEGIN(0);
}
<SOURCE>{space}"off"({space})*((("//")(.)*"\n")|("\n")) {
     LIST;
     Opt_Source = 0;
     if(Opt_Source)
        printf("%d:%s\n", linenum, buf);
     ++linenum;
     buf[0] = '\0';
     BEGIN(0);
}
<SOURCE>. {
     fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
     exit(1);
}

 /* TOKEN */
<TOKEN>{space}"on"({space})*((("//")(.)*"\n")|("\n")) {
     LIST;
     Opt_Token = 1;
     if(Opt_Source)
         printf("%d:%s\n", linenum, buf);
     ++linenum;
     buf[0] = '\0';
     BEGIN(0);
}
<TOKEN>{space}"off"({space})*((("//")(.)*"\n")|("\n")) {
     LIST;
     Opt_Token = 0;
     if(Opt_Source)
         printf("%d:%s\n", linenum, buf);
     ++linenum;
     buf[0] = '\0';
     BEGIN(0);
}
<TOKEN>. {
     fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
     exit(1);
}

 /* STATISTIC */
<STATISTIC>{space}"on"({space})*((("//")(.)*"\n")|("\n")) {
    LIST;
    Opt_Statistic = 1;
    if(Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
    BEGIN(0);
}
<STATISTIC>{space}"off"({space})*((("//")(.)*"\n")|("\n")) {
    LIST;
    Opt_Statistic = 0;
    if(Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
    BEGIN(0);
}
<STATISTIC>. {
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

{delimiter} {
    tokenString("delim", yytext);
}

{keyword} {
    tokenString("KM", yytext);
}

{identifier} {
    tokenString("id", yytext);
    ID_freq(yytext);
    //printf("This is ID: %s\n", yytext);
}

{integer} {
    tokenString("integer", yytext);
}

{float} {
    tokenString("float", yytext);
}

{scientific} {
    tokenString("scientific", yytext);
}

{operator}|{logical} {
    token(yytext);
}

{space} {
    LIST;
}

\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

%%

void ID_freq(char *s) {
    int i;
    isExist = 0;
    //printf("Original ID_cnt: %d\n", ID_cnt);
    for(i = 0; i < ID_cnt; i++){
        if(strcmp(s, IDs[i].context)==0) {
            IDs[i].occur++;
            isExist = 1;
			printf("s: %s, context: %s\n", s, IDs[i].context);
            break;
        }
    }
    if(!isExist) {
        IDs[ID_cnt].context[0] = '\0';
        strcpy(IDs[ID_cnt].context, s);
		//printf("New ID Len: %lu\n", strlen(IDs[ID_cnt].context));
        IDs[ID_cnt].occur = 1;
		ID_cnt++;
    }
    //printf("Now ID_cnt: %d\n", ID_cnt);
}

void printID(){
    int i;
    printf("frenquencies of identifier:\n");
    for(i = 0; i < ID_cnt; i++){
        if(IDs[i].occur > 0)
            printf("%s\t%d\n", IDs[i].context, IDs[i].occur);
    }
}

int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();

    if (Opt_Statistic)
    {
        // Print frequency table
        printID();
    }

    exit(0);
}
