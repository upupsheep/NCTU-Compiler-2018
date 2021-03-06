#include "genCode.h"
#define LABEL_STACK_SIZE 256

FILE *outputJ;
int limit = 100;
char *className;
int label = 0;
int top = -1;
int labelStack[LABEL_STACK_SIZE];
void pushLabel(int l) {
    if (top < LABEL_STACK_SIZE) {
        labelStack[++top] = l;
    } else {
        fprintf(stderr, "label stack full\n");
    }
}
int popLabel() {
    int l;
    if (top == -1) {
        fprintf(stderr, "label stack empty\n");
    } else {
        l = labelStack[top--];
    }
    return l;
}
int topLabel() { return labelStack[top]; }
void genClassByFileName(char *fn) {
    fprintf(outputJ, ";%s.j\n", fn);
    fprintf(outputJ, ".class public %s\n", fn);
    fprintf(outputJ, ".super java/lang/Object\n");
    fprintf(outputJ, ".field public static _sc Ljava/util/Scanner;\n");
}
char *parseFileName(char *fn) {
    char *fn_previous;
    char *fn_current;
    fn_previous = fn;
    fn_current = strtok(fn, "/");
    while (fn_current) {
        fn_previous = fn_current;
        fn_current = strtok(NULL, "/");
    }
    fn_previous[strlen(fn_previous) - 2] = '\0';
    return fn_previous;
}
void initCodeFile(char *fn) {
    size_t len = strlen(fn);
    char jFile[256];
    char filePath[256];
    if (fn[strlen(fn) - 2] != '.' || fn[strlen(fn) - 1] != 'c') {
        fprintf(stderr, "Not a .c file\n");
        exit(-1);
    }
    strcpy(filePath, fn);
    fn = parseFileName(fn);
    if (strncmp(fn, filePath, strlen(fn)) != 0) {
        filePath[strlen(filePath) - strlen(fn) - 2] = '\0';
        sprintf(jFile, "%s/%s.j", filePath, fn);
    } else {
        sprintf(jFile, "%s.j", fn);
    }
    outputJ = fopen(jFile, "w");
    genClassByFileName(fn);
    className = fn;
}

char SEMTYPE2TypeChar(SEMTYPE type) {
    switch (type) {
        case VOID_t:
            return 'V';
        case BOOLEAN_t:
            return 'Z';
        case INTEGER_t:
            return 'I';
        case FLOAT_t:
            return 'F';
        case DOUBLE_t:
            return 'F';
        default:
            return 'V';
    }
}
char *SEMTYPE2ReturnTypeStr(SEMTYPE type) {
    switch (type) {
        case VOID_t:
            return "return";
        case BOOLEAN_t:
            return "ireturn";
        case INTEGER_t:
            return "ireturn";
        case FLOAT_t:
            return "freturn";
        case DOUBLE_t:
            return "freturn";
        default:
            return "return";
    }
}
char *param_sem2TypeStr(struct param_sem *par) {
    char *params = malloc(sizeof(char) * 256);
    if (par != NULL) {
        char *p = params;
        for (; par != NULL; par = par->next, p++) {
            *p = SEMTYPE2TypeChar(par->pType->type);
        }
        *p = '\0';
    } else {
        strcpy(params, "");
    }
    return params;
}
char *pTypeList2Str(struct PTypeList *pType) {
    char *params = malloc(sizeof(char) * 256);
    if (pType != NULL) {
        char *p = params;
        for (; pType != NULL; pType = pType->next, p++) {
            *p = SEMTYPE2TypeChar(pType->value->type);
        }
        *p = '\0';
    } else {
        strcpy(params, "");
    }
    return params;
}
char *varLoad2Str(SEMTYPE type) {
    switch (type) {
        case BOOLEAN_t:
            return "iload";
        case INTEGER_t:
            return "iload";
        case FLOAT_t:
            return "fload";
        case DOUBLE_t:
            return "fload";
    }
}
char *varStore2Str(SEMTYPE type) {
    switch (type) {
        case BOOLEAN_t:
            return "istore";
        case INTEGER_t:
            return "istore";
        case FLOAT_t:
            return "fstore";
        case DOUBLE_t:
            return "fstore";
    }
}
char *intergerOP2Str(OPERATOR op) {
    switch (op) {
        case ADD_t:
            return "iadd";
        case SUB_t:
            return "isub";
        case MUL_t:
            return "imul";
        case DIV_t:
            return "idiv";
        case MOD_t:
            return "irem";
        case NEG_t:
            return "ineg";
    }
}
char *floatOP2Str(OPERATOR op) {
    switch (op) {
        case ADD_t:
            return "fadd";
        case SUB_t:
            return "fsub";
        case MUL_t:
            return "fmul";
        case DIV_t:
            return "fdiv";
        case NEG_t:
            return "fneg";
    }
}
char *doubleOP2Str(OPERATOR op) {
    switch (op) {
        case ADD_t:
            return "fadd";
        case SUB_t:
            return "fsub";
        case MUL_t:
            return "fmul";
        case DIV_t:
            return "fdiv";
        case NEG_t:
            return "fneg";
    }
}
char *boolOP2Str(OPERATOR op) {
    switch (op) {
        case AND_t:
            return "iand";
        case OR_t:
            return "ior";
        case NOT_t:
            return "ixor";
    }
}

void genMethod(char *methodName, struct param_sem *par,
               struct PType *returnType) {
    char *params = param_sem2TypeStr(par);
    if (strcmp(methodName, "main") == 0) {
        fprintf(outputJ, ".method public static %s([Ljava/lang/String;)%c\n",
                methodName, SEMTYPE2TypeChar(returnType->type));
    } else {
        fprintf(outputJ, ".method public static %s(%s)%c\n", methodName, params,
                SEMTYPE2TypeChar(returnType->type));
    }
    free(params);
}
void genMethodInvoke(char *methodName, struct SymTable *table) {
    int scope = 0;
    struct SymNode *node = lookupSymbol(table, methodName, 0, __TRUE);
    if (node->category == FUNCTION_t) {
        if (node->attribute->formalParam->paramNum == 0)
            fprintf(outputJ, "invokestatic %s/%s(%s)%c\n", className,
                    methodName, "", SEMTYPE2TypeChar(node->type->type));
        else
            fprintf(outputJ, "invokestatic %s/%s(%s)%c\n", className,
                    methodName,
                    pTypeList2Str(node->attribute->formalParam->params),
                    SEMTYPE2TypeChar(node->type->type));
    }
}
void genEndMethod(struct PType *returnType) {
    if (returnType->type == VOID_t)
        fprintf(outputJ, "%s\n", SEMTYPE2ReturnTypeStr(returnType->type));
    fprintf(outputJ, ".end method\n");
}
void genReturn(struct PType *returnType) {
    fprintf(outputJ, "%s\n", SEMTYPE2ReturnTypeStr(returnType->type));
}
void limitStack() {
    fprintf(outputJ, ".limit stack %d\n", limit);
    fprintf(outputJ, ".limit locals %d\n", limit);
}
void genGlobalVariable(char *varName, struct PType *varType) {
    fprintf(outputJ, ".field public static %s %c\n", varName,
            SEMTYPE2TypeChar(varType->type));
}
void genNegative(struct expr_sem *expr, struct SymTable *table, int scope) {
    /*char *id = expr->varRef->id;
    struct SymNode *node = lookupSymbol( table, id, scope, __FALSE );
    if(node->category == CONSTANT_t){//constant
            if(expr->beginningOp == SUB_t){
                    if(node->attribute->constVal->category == FLOAT_t)
                            fprintf(outputJ, "%s\n", floatOP2Str(NEG_t));
                    else if(node->attribute->constVal->category == DOUBLE_t)
                            fprintf(outputJ, "%s\n", doubleOP2Str(NEG_t));
                    else if(node->attribute->constVal->category == INTEGER_t)
                            fprintf(outputJ, "%s\n", intergerOP2Str(NEG_t));
            }
    }
    else if(node->category == FUNCTION_t){
    }
    else{//local variable, global variable*/
    if (expr->beginningOp == SUB_t) {
        if (expr->pType->type == FLOAT_t)
            fprintf(outputJ, "%s\n", floatOP2Str(NEG_t));
        else if (expr->pType->type == DOUBLE_t)
            fprintf(outputJ, "%s\n", doubleOP2Str(NEG_t));
        else if (expr->pType->type == INTEGER_t)
            fprintf(outputJ, "%s\n", intergerOP2Str(NEG_t));
        expr->beginningOp = NONE_t;
    }
    //}
}
void genVariableLoad(struct expr_sem *expr, struct SymTable *table, int scope) {
    char *id = expr->varRef->id;
    struct SymNode *node = lookupSymbol(table, id, scope, __FALSE);
    if (node->category == CONSTANT_t) {  // constant
        if (node->attribute->constVal->category == INTEGER_t)
            fprintf(outputJ, "sipush %d\n",
                    node->attribute->constVal->value.integerVal);
        else if (node->attribute->constVal->category == DOUBLE_t)
            fprintf(outputJ, "ldc %lf\n",
                    node->attribute->constVal->value.doubleVal);
        else if (node->attribute->constVal->category == FLOAT_t) {
            fprintf(outputJ, "ldc %f\n",
                    node->attribute->constVal->value.floatVal);
            // if(node->type->type == DOUBLE_t)
            //	fprintf(outputJ, "f2d\n");
        } else if (node->attribute->constVal->category == STRING_t)
            fprintf(outputJ, "ldc \"%s\"\n",
                    node->attribute->constVal->value.stringVal);
        else if (node->attribute->constVal->category == BOOLEAN_t)
            fprintf(outputJ, "%s\n",
                    (node->attribute->constVal->value.booleanVal) ? "iconst_1"
                                                                  : "iconst_0");
    } else if (node->category == FUNCTION_t) {
    } else if (node->scope == 0) {  // global variable
        fprintf(outputJ, "getstatic %s/%s %c\n", className, node->name,
                SEMTYPE2TypeChar(node->type->type));
    } else {  // local variable
        fprintf(outputJ, "%s %d\n", varLoad2Str(expr->pType->type),
                node->localNumber);
    }
}
void genLiteralConst(struct expr_sem *expr, struct ConstAttr *constVal) {
    if (constVal->category == INTEGER_t)
        fprintf(outputJ, "sipush %d\n", constVal->value.integerVal);
    else if (constVal->category == DOUBLE_t) {
        fprintf(outputJ, "ldc %lf\n", constVal->value.doubleVal);
        fprintf(outputJ, "f2d\n");
    } else if (constVal->category == FLOAT_t)
        fprintf(outputJ, "ldc %f\n", constVal->value.floatVal);
    else if (constVal->category == STRING_t)
        fprintf(outputJ, "ldc \"%s\"\n", constVal->value.stringVal);
    else if (constVal->category == BOOLEAN_t)
        fprintf(outputJ, "%s\n",
                (constVal->value.booleanVal) ? "iconst_1" : "iconst_0");
}

void genVariableStore(struct expr_sem *expr1, struct expr_sem *expr2,
                      struct SymTable *table, int scope) {
    char *id = expr1->varRef->id;
    struct SymNode *node = lookupSymbol(table, id, scope, __FALSE);
    // cast before store in case type is inconsist

    // if(expr1->pType->type == DOUBLE_t && expr2->pType->type == FLOAT_t)
    //	fprintf(outputJ, "f2d\n");
    if (expr1->pType->type == DOUBLE_t && expr2->pType->type == INTEGER_t)
        fprintf(outputJ, "i2f\n");
    else if (expr1->pType->type == FLOAT_t && expr2->pType->type == INTEGER_t)
        fprintf(outputJ, "i2f\n");

    if (node->scope == 0)
        fprintf(outputJ, "putstatic %s/%s %c\n", className, node->name,
                SEMTYPE2TypeChar(node->type->type));
    else {
        fprintf(outputJ, "%s %d\n", varStore2Str(expr1->pType->type),
                node->localNumber);
    }
}
void genOP(struct expr_sem *expr, OPERATOR op) {
    if (op == LT_t || op == LE_t || op == EQ_t || op == GT_t || op == GE_t ||
        op == NE_t) {
        if (expr->pType->type == INTEGER_t)
            fprintf(outputJ, "isub\n");
        else if (expr->pType->type == FLOAT_t || expr->pType->type == DOUBLE_t)
            fprintf(outputJ, "fcmpl\n");
        else if (expr->pType->type == BOOLEAN_t && (op == EQ_t || op == NE_t))
            fprintf(outputJ, "isub\n");
        else
            return;
        label++;
        switch (op) {
            case LT_t:
                fprintf(outputJ, "iflt Ltrue_%d\n", label);
                break;
            case LE_t:
                fprintf(outputJ, "ifle Ltrue_%d\n", label);
                break;
            case EQ_t:
                fprintf(outputJ, "ifeq Ltrue_%d\n", label);
                break;
            case GT_t:
                fprintf(outputJ, "ifgt Ltrue_%d\n", label);
                break;
            case GE_t:
                fprintf(outputJ, "ifge Ltrue_%d\n", label);
                break;
            case NE_t:
                fprintf(outputJ, "ifne Ltrue_%d\n", label);
                break;
        }
        fprintf(outputJ, "iconst_0\n");
        fprintf(outputJ, "goto Lfalse_%d\n", label);
        fprintf(outputJ, "Ltrue_%d:\n", label);
        fprintf(outputJ, "iconst_1\n");
        fprintf(outputJ, "Lfalse_%d:\n", label);
        return;
    }
    char *code;
    if (expr->pType->type == INTEGER_t)
        code = intergerOP2Str(op);
    else if (expr->pType->type == FLOAT_t)
        code = floatOP2Str(op);
    else if (expr->pType->type == DOUBLE_t)
        code = doubleOP2Str(op);
    else if (expr->pType->type == BOOLEAN_t)
        code = boolOP2Str(op);
    fprintf(outputJ, "%s\n", code);
}
void castType(struct expr_sem *expr1, struct expr_sem *expr2) {
    /*if(expr1->pType->type == FLOAT_t && expr2->pType->type == DOUBLE_t){
            fprintf(outputJ, "dstore 50\n");
            fprintf(outputJ, "f2d\n");
            fprintf(outputJ, "dload 50\n");
    }
    else if(expr1->pType->type == DOUBLE_t && expr2->pType->type == FLOAT_t){
            fprintf(outputJ, "f2d\n");
    }*/
    if (expr1->pType->type == INTEGER_t && expr2->pType->type == DOUBLE_t) {
        fprintf(outputJ, "fstore 50\n");
        fprintf(outputJ, "i2f\n");
        fprintf(outputJ, "fload 50\n");
    } else if (expr1->pType->type == DOUBLE_t &&
               expr2->pType->type == INTEGER_t) {
        fprintf(outputJ, "i2f\n");
    } else if (expr1->pType->type == INTEGER_t &&
               expr2->pType->type == FLOAT_t) {
        fprintf(outputJ, "fstore 50\n");
        fprintf(outputJ, "i2f\n");
        fprintf(outputJ, "fload 50\n");
    } else if (expr1->pType->type == FLOAT_t &&
               expr2->pType->type == INTEGER_t) {
        fprintf(outputJ, "i2f\n");
    }
}
void genPrint(struct expr_sem *expr, struct SymTable *table, int scope) {
    if (expr->pType->type != STRING_t) {
        /*if(expr->varRef != NULL){
                fprintf(outputJ, "pop\n");
                fprintf(outputJ, "getstatic java/lang/System/out
        Ljava/io/PrintStream;\n"); genVariableLoad(expr, table, scope);
        }
        else{*/
        if (expr->pType->type == DOUBLE_t) {
            fprintf(outputJ, "fstore 50\n");
            fprintf(outputJ,
                    "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
            fprintf(outputJ, "fload 50\n");
        } else if (expr->pType->type == FLOAT_t) {
            fprintf(outputJ, "fstore 50\n");
            fprintf(outputJ,
                    "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
            fprintf(outputJ, "fload 50\n");
        } else if (expr->pType->type == INTEGER_t ||
                   expr->pType->type == BOOLEAN_t) {
            fprintf(outputJ, "istore 50\n");
            fprintf(outputJ,
                    "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
            fprintf(outputJ, "iload 50\n");
        }
        //}
        fprintf(outputJ, "invokevirtual java/io/PrintStream/print(%c)V\n",
                SEMTYPE2TypeChar(expr->pType->type));
    } else {
        fprintf(
            outputJ,
            "invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
    }
}
void genPrinterObj() {
    fprintf(outputJ, "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
}
void genRead(struct expr_sem *expr, struct SymTable *table, int scope) {
    fprintf(outputJ, "getstatic %s/_sc Ljava/util/Scanner;\n", className);
    char *id = expr->varRef->id;
    struct SymNode *node = lookupSymbol(table, id, scope, __FALSE);
    if (expr->pType->type == INTEGER_t) {
        fprintf(outputJ, "invokevirtual java/util/Scanner/nextInt()I\n");
        fprintf(outputJ, "istore %d\n", node->localNumber);
    } else if (expr->pType->type == FLOAT_t) {
        fprintf(outputJ, "invokevirtual java/util/Scanner/nextFloat()F\n");
        fprintf(outputJ, "fstore %d\n", node->localNumber);
    } else if (expr->pType->type == DOUBLE_t) {
        fprintf(outputJ, "invokevirtual java/util/Scanner/nextFloat()F\n");
        fprintf(outputJ, "fstore %d\n", node->localNumber);
    } else if (expr->pType->type == BOOLEAN_t) {
        fprintf(outputJ, "invokevirtual java/util/Scanner/nextBoolean()Z\n");
        fprintf(outputJ, "istore %d\n", node->localNumber);
    }
}
void genReaderObj() {
    fprintf(outputJ, "new java/util/Scanner\n");
    fprintf(outputJ, "dup\n");
    fprintf(outputJ, "getstatic java/lang/System/in Ljava/io/InputStream;\n");
    fprintf(outputJ,
            "invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
    fprintf(outputJ, "putstatic %s/_sc Ljava/util/Scanner;\n", className);
}
void genCondition() {
    label++;
    pushLabel(label);
    fprintf(outputJ, "ifeq Lelse_%d\n", label);
}
void genGoToExitLabel() { fprintf(outputJ, "goto Lexit_%d\n", topLabel()); }
void genElseLabel(int pop) {
    if (pop)
        fprintf(outputJ, "Lelse_%d:\n", popLabel());
    else
        fprintf(outputJ, "Lelse_%d:\n", topLabel());
}
void genExitLabel() { fprintf(outputJ, "Lexit_%d:\n", popLabel()); }
void genWhileLabel() {
    label++;
    pushLabel(label);
    fprintf(outputJ, "Lbegin_%d:\n", label);
}
void genGotoWhileLabel() {
    int l = popLabel();
    fprintf(outputJ, "goto Lbegin_%d\n", popLabel());
    pushLabel(l);
}
void genForBeginLabel() {
    label++;
    pushLabel(label);
    fprintf(outputJ, "L_for_Begin_%d:\n", label);
}
void genForConditionLabel() {
    fprintf(outputJ, "L_for_cond_%d:\n", topLabel());
}
void genForConditionEnd() {
    fprintf(outputJ, "ifeq L_for_exit_%d\n", topLabel());
    fprintf(outputJ, "goto L_for_body_%d\n", topLabel());
}
void genForBodyLabel() { fprintf(outputJ, "L_for_body_%d:\n", topLabel()); }
void genForBodyEnd() {
    fprintf(outputJ, "goto L_for_incr_%d\n", topLabel());
    fprintf(outputJ, "L_for_exit_%d:\n", popLabel());
}
void genForIncrLabel() { fprintf(outputJ, "L_for_incr_%d:\n", topLabel()); }
void genForIncrEnd() { fprintf(outputJ, "goto L_for_cond_%d\n", topLabel()); }