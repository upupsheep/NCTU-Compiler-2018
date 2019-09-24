#include "gencode.h"
#define OPERAND_STACK_SIZE 256

FILE *outputJ;
int labelStack[OPERAND_STACK_SIZE];
char *className;
int label = 0, top = -1, limit = 100;

void initCodeFile(char *fn) {
    char jFile[256] = "output.j";
    if (fn[strlen(fn) - 2] != '.' || fn[strlen(fn) - 1] != 'c') {
        fprintf(stderr, "Not a .c file\n");
        exit(-1);
    }
    fn = "output";
    outputJ = fopen(jFile, "w");
    fprintf(outputJ, ";%s.j\n", fn);
    fprintf(outputJ, ".class public %s\n", fn);
    fprintf(outputJ, ".super java/lang/Object\n");
    fprintf(outputJ, ".field public static _sc Ljava/util/Scanner;\n");
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

char *param_sem2TypeStr(struct param_sem *par) {
    char *params = malloc(sizeof(char) * 256);
    if (par != NULL) {
        char *ptr = params;
        while (par != NULL) {
            *ptr = SEMTYPE2TypeChar(par->pType->type);
            par = par->next;
            ptr++;
        }
        *ptr = '\0';
    } else {
        strcpy(params, "");
    }
    return params;
}
char *pTypeList2Str(struct PTypeList *pType) {
    char *params = malloc(sizeof(char) * 256);
    if (pType != NULL) {
        char *ptr = params;
        while (pType != NULL) {
            *ptr = SEMTYPE2TypeChar(pType->value->type);
            pType = pType->next;
            ptr++;
        }
        *ptr = '\0';
    } else {
        strcpy(params, "");
    }
    return params;
}
char *varLoad_jout(SEMTYPE type) {
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
char *varStore_jout(SEMTYPE type) {
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
char *intergerOP_jout(OPERATOR op) {
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
char *floatOP_jout(OPERATOR op) {
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
char *doubleOP_jout(OPERATOR op) {
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
char *boolOP_jout(OPERATOR op) {
    switch (op) {
        case AND_t:
            return "iand";
        case OR_t:
            return "ior";
        case NOT_t:
            return "ixor";
    }
}

void genFuncDecl(char *methodName, struct param_sem *param,
                 struct PType *rtnType) {
    char *params = param_sem2TypeStr(param);
    if (!strcmp(methodName, "main")) {
        // .method public static main([Ljava/lang/String;)V
        fprintf(outputJ, ".method public static %s([Ljava/lang/String;)%c\n",
                methodName, SEMTYPE2TypeChar(rtnType->type));
    } else {
        fprintf(outputJ, ".method public static %s(%s)%c\n", methodName, params,
                SEMTYPE2TypeChar(rtnType->type));
    }
    free(params);
}
void genFuncInvoke(char *methodName, struct SymTable *table) {
    struct SymNode *findNode = lookupSymbol(table, methodName, 0, __TRUE);
    if (findNode->category == FUNCTION_t) {
        if (findNode->attribute->formalParam->paramNum == 0)
            fprintf(outputJ, "invokestatic %s/%s(%s)%c\n", className,
                    methodName, "", SEMTYPE2TypeChar(findNode->type->type));
        else
            fprintf(outputJ, "invokestatic %s/%s(%s)%c\n", className,
                    methodName,
                    pTypeList2Str(findNode->attribute->formalParam->params),
                    SEMTYPE2TypeChar(findNode->type->type));
    }
}
void genEndFunc(struct PType *returnType) {
    if (returnType->type == VOID_t) fprintf(outputJ, "%s\n", "return");
    fprintf(outputJ, ".end method\n");
}
void genReturn(struct PType *returnType) {
    switch (returnType->type) {
        case VOID_t:
            fprintf(outputJ, "return\n");
            break;
        case BOOLEAN_t:
            fprintf(outputJ, "ireturn\n");
            break;
        case INTEGER_t:
            fprintf(outputJ, "ireturn\n");
            break;
        case FLOAT_t:
            fprintf(outputJ, "freturn\n");
            break;
        case DOUBLE_t:
            fprintf(outputJ, "freturn\n");
            break;
        default:
            fprintf(outputJ, "return\n");
            break;
    }
}
void limitStack() {
    fprintf(outputJ, ".limit stack %d\n", limit);
    fprintf(outputJ, ".limit locals %d\n", limit);
}
void genGlobalVar(char *varName, struct PType *varType) {
    fprintf(outputJ, ".field public static %s %c\n", varName,
            SEMTYPE2TypeChar(varType->type));
}
void genCheckNeg(struct expr_sem *expr, struct SymTable *table, int scope) {
    if (expr->beginningOp == SUB_t) {
        if (expr->pType->type == INTEGER_t)
            fprintf(outputJ, "%s\n", intergerOP_jout(NEG_t));
        else if (expr->pType->type == FLOAT_t)
            fprintf(outputJ, "%s\n", floatOP_jout(NEG_t));
        else if (expr->pType->type == DOUBLE_t)
            fprintf(outputJ, "%s\n", doubleOP_jout(NEG_t));
        expr->beginningOp = NONE_t;
    }
}
void genVarLoad(struct expr_sem *expr, struct SymTable *table, int scope) {
    char *id = expr->varRef->id;
    struct SymNode *node = lookupSymbol(table, id, scope, __FALSE);
    if (node->category == CONSTANT_t) {  // constant
        switch (node->attribute->constVal->category) {
            case INTEGER_t:
                fprintf(outputJ, "sipush %d\n",
                        node->attribute->constVal->value.integerVal);
                break;
            case DOUBLE_t:
                fprintf(outputJ, "ldc %lf\n",
                        node->attribute->constVal->value.doubleVal);
                break;
            case FLOAT_t:
                fprintf(outputJ, "ldc %f\n",
                        node->attribute->constVal->value.floatVal);
                break;
            case STRING_t:
                fprintf(outputJ, "ldc \"%s\"\n",
                        node->attribute->constVal->value.stringVal);
                break;
            case BOOLEAN_t:
                fprintf(outputJ, "%s\n",
                        (node->attribute->constVal->value.booleanVal)
                            ? "iconst_1"
                            : "iconst_0");
                break;
            default:
                break;
        }
    } else if (node->category == FUNCTION_t) {
    } else if (node->scope == 0) {  // global variable
        fprintf(outputJ, "getstatic %s/%s %c\n", className, node->name,
                SEMTYPE2TypeChar(node->type->type));
    } else {  // local variable
        fprintf(outputJ, "%s %d\n", varLoad_jout(expr->pType->type),
                node->localNumber);
    }
}
void genVarStore(struct expr_sem *expr1, struct expr_sem *expr2,
                 struct SymTable *table, int scope) {
    char *id = expr1->varRef->id;
    struct SymNode *node = lookupSymbol(table, id, scope, __FALSE);

    if ((expr1->pType->type == DOUBLE_t && expr2->pType->type == INTEGER_t) ||
        (expr1->pType->type == FLOAT_t && expr2->pType->type == INTEGER_t))
        fprintf(outputJ, "i2f\n");

    if (node->scope == 0)
        fprintf(outputJ, "putstatic %s/%s %c\n", className, node->name,
                SEMTYPE2TypeChar(node->type->type));
    else {
        fprintf(outputJ, "%s %d\n", varStore_jout(expr1->pType->type),
                node->localNumber);
    }
}
void genLiteralConst(struct expr_sem *expr, struct ConstAttr *constVal) {
    switch (constVal->category) {
        case INTEGER_t:
            fprintf(outputJ, "sipush %d\n", constVal->value.integerVal);
            break;
        case DOUBLE_t:
            fprintf(outputJ, "ldc %lf\n", constVal->value.doubleVal);
            fprintf(outputJ, "f2d\n");
            break;
        case FLOAT_t:
            fprintf(outputJ, "ldc %f\n", constVal->value.floatVal);
            break;
        case STRING_t:
            fprintf(outputJ, "ldc \"%s\"\n", constVal->value.stringVal);
            break;
        case BOOLEAN_t:
            fprintf(outputJ, "%s\n",
                    (constVal->value.booleanVal) ? "iconst_1" : "iconst_0");
            break;
        default:
            break;
    }
}

void genOperator(struct expr_sem *expr, OPERATOR op) {
    if (op == LT_t || op == LE_t || op == EQ_t || op == GT_t || op == GE_t ||
        op == NE_t) {
        label++;

        if (expr->pType->type == INTEGER_t)
            fprintf(outputJ, "isub\n");
        else if (expr->pType->type == FLOAT_t || expr->pType->type == DOUBLE_t)
            fprintf(outputJ, "fcmpl\n");
        else if (expr->pType->type == BOOLEAN_t && (op == EQ_t || op == NE_t))
            fprintf(outputJ, "isub\n");
        else
            return;

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
    switch (expr->pType->type) {
        case INTEGER_t:
            code = intergerOP_jout(op);
            break;
        case FLOAT_t:
            code = floatOP_jout(op);
            break;
        case DOUBLE_t:
            code = doubleOP_jout(op);
            break;
        case BOOLEAN_t:
            code = boolOP_jout(op);
            break;
        default:
            break;
    }
    fprintf(outputJ, "%s\n", code);
}
void castType(struct expr_sem *expr1, struct expr_sem *expr2) {
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
        fprintf(outputJ, "invokevirtual java/io/PrintStream/print(%c)V\n",
                SEMTYPE2TypeChar(expr->pType->type));
    } else {
        fprintf(
            outputJ,
            "invokevirtual java/io/PrintStream/print(Ljava/lang/String;)V\n");
    }
}
void genPrintStream() {
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
void genReadStream() {
    fprintf(outputJ, "new java/util/Scanner\n");
    fprintf(outputJ, "dup\n");
    fprintf(outputJ, "getstatic java/lang/System/in Ljava/io/InputStream;\n");
    fprintf(outputJ,
            "invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
    fprintf(outputJ, "putstatic %s/_sc Ljava/util/Scanner;\n", className);
}

void pushLabel(int l) {
    if (top < OPERAND_STACK_SIZE)
        labelStack[++top] = l;
    else
        fprintf(stderr, "label stack full\n");
}
int popLabel() {
    int afterpop;
    if (top == -1)
        fprintf(stderr, "label stack empty\n");
    else
        afterpop = labelStack[top--];
    return afterpop;
}
int topLabel() { return labelStack[top]; }

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
void genWhile() {
    label++;
    pushLabel(label);
    fprintf(outputJ, "Lbegin_%d:\n", label);
}
void genGotoWhileLabel() {
    int l = popLabel();
    fprintf(outputJ, "goto Lbegin_%d\n", popLabel());
    pushLabel(l);
}
void genFor() {
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
