/*
 * traversal.c
 * -------------
 * 트리 노드, Stack, 파서, ASCII 출력, 순회 함수의 실제 구현.
 * 프로그램 전체에서 재귀를 사용하지 않는다 (파싱, ASCII 출력, 메모리 해제,
 * 그리고 3가지 순회 함수 모두 명시적인 반복문과 명시적인 Stack으로 구현한다).
 */

#include <stdio.h>
#include <stdlib.h>
#include "traversal.h"

Node *createNode(char data) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    n->col = 0;
    n->depth = 0;
    return n;
}

/* ---------- Stack #1 ---------- */
void initStack(Stack *s) { s->top = -1; }
int  isEmpty(Stack *s)   { return s->top == -1; }

void push(Stack *s, Node *node) {
    if (s->top >= MAX_STACK - 1) { fprintf(stderr, "Stack overflow\n"); exit(1); }
    s->items[++(s->top)] = node;
}

Node *pop(Stack *s) {
    if (isEmpty(s)) { fprintf(stderr, "Stack underflow\n"); exit(1); }
    return s->items[(s->top)--];
}

/* ---------- Stack #2 ---------- */
void initStackND(StackND *s) { s->top = -1; }
int  isEmptyND(StackND *s)   { return s->top == -1; }

void pushND(StackND *s, Node *node, int depth) {
    if (s->top >= MAX_STACK - 1) { fprintf(stderr, "Stack overflow\n"); exit(1); }
    s->top++;
    s->items[s->top].node = node;
    s->items[s->top].depth = depth;
}

NodeDepth popND(StackND *s) {
    if (isEmptyND(s)) { fprintf(stderr, "Stack underflow\n"); exit(1); }
    return s->items[(s->top)--];
}

/* ---------- Stack #3: 파서 프레임 ---------- */
void initFrameStack(FrameStack *s) { s->top = -1; }
int  isEmptyFrameStack(FrameStack *s) { return s->top == -1; }

void pushFrame(FrameStack *s, Node *node, int expecting) {
    if (s->top >= MAX_STACK - 1) { fprintf(stderr, "Stack overflow\n"); exit(1); }
    s->top++;
    s->items[s->top].node = node;
    s->items[s->top].expecting = expecting;
}

Frame *peekFrame(FrameStack *s) { return &s->items[s->top]; }
void   popFrame(FrameStack *s)  { s->top--; }

/* ---------- 파서: 괄호 표기법 -> 연결 트리 (완전히 반복적) ---------- */
void skipSpaces(const char **s) {
    while (**s == ' ' || **s == '\t' || **s == '\n') (*s)++;
}

#define PARSE  0
#define ATTACH 1
#define LEFT   0
#define RIGHT  1

Node *buildTreeFromString(const char *input) {
    const char *p = input;
    FrameStack fs;
    initFrameStack(&fs);

    Node *result = NULL;
    Node *root = NULL;
    int mode = PARSE;
    int done = 0;

    while (!done) {
        if (mode == PARSE) {
            skipSpaces(&p);
            if (*p == '\0' || *p == ',' || *p == ')') {
                result = NULL;
                mode = ATTACH;
            } else {
                char value = *p;
                p++;
                Node *node = createNode(value);
                skipSpaces(&p);
                if (*p == '(') {
                    p++;
                    pushFrame(&fs, node, LEFT);
                    mode = PARSE;
                } else {
                    result = node;
                    mode = ATTACH;
                }
            }
        } else {
            if (isEmptyFrameStack(&fs)) {
                root = result;
                done = 1;
            } else {
                Frame *top = peekFrame(&fs);
                if (top->expecting == LEFT) {
                    top->node->left = result;
                    skipSpaces(&p);
                    if (*p != ',') {
                        fprintf(stderr, "Invalid input: expected ',' near \"%s\"\n", p);
                        exit(1);
                    }
                    p++;
                    top->expecting = RIGHT;
                    mode = PARSE;
                } else {
                    top->node->right = result;
                    skipSpaces(&p);
                    if (*p != ')') {
                        fprintf(stderr, "Invalid input: expected ')' near \"%s\"\n", p);
                        exit(1);
                    }
                    p++;
                    Node *completed = top->node;
                    popFrame(&fs);
                    result = completed;
                    mode = ATTACH;
                }
            }
        }
    }

    skipSpaces(&p);
    if (*p != '\0') {
        fprintf(stderr, "Invalid input: unexpected trailing characters \"%s\"\n", p);
        exit(1);
    }
    return root;
}

/* ---------- ASCII 트리 출력 ---------- */
static int g_col_counter;
static int g_max_depth;

void assignPositions(Node *root) {
    StackND s;
    initStackND(&s);
    Node *cur = root;
    int depth = 0;

    while (cur != NULL || !isEmptyND(&s)) {
        while (cur != NULL) {
            pushND(&s, cur, depth);
            depth++;
            cur = cur->left;
        }
        NodeDepth nd = popND(&s);
        nd.node->col = g_col_counter++;
        nd.node->depth = nd.depth;
        if (nd.depth > g_max_depth) g_max_depth = nd.depth;
        cur = nd.node->right;
        depth = nd.depth + 1;
    }
}

void fillGrid(Node *root, char grid[MAX_ROWS][MAX_COLS]) {
    Stack s;
    initStack(&s);
    push(&s, root);
    while (!isEmpty(&s)) {
        Node *cur = pop(&s);
        int r = cur->depth * 2;
        int c = cur->col * SPACING + SPACING / 2;
        grid[r][c] = cur->data;
        if (cur->left) {
            grid[r + 1][c - 1] = '/';
            push(&s, cur->left);
        }
        if (cur->right) {
            grid[r + 1][c + 1] = '\\';
            push(&s, cur->right);
        }
    }
}

void drawTreeASCII(Node *root) {
    static char grid[MAX_ROWS][MAX_COLS];
    g_col_counter = 0;
    g_max_depth = 0;
    assignPositions(root);

    int rows = (g_max_depth + 1) * 2;
    int cols = g_col_counter * SPACING + SPACING;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            grid[i][j] = ' ';

    fillGrid(root, grid);

    for (int i = 0; i < rows; i++) {
        int lastNonSpace = -1;
        for (int j = 0; j < cols; j++)
            if (grid[i][j] != ' ') lastNonSpace = j;
        for (int j = 0; j <= lastNonSpace; j++)
            putchar(grid[i][j]);
        putchar('\n');
    }
}

/* ================= 반복적 순회 (채점 대상 함수) ================= */

void preorder(Node *root) {
    if (root == NULL) return;
    Stack s;
    initStack(&s);
    push(&s, root);

    printf("Preorder  : ");
    while (!isEmpty(&s)) {
        Node *cur = pop(&s);
        printf("%c ", cur->data);
        if (cur->right != NULL) push(&s, cur->right);
        if (cur->left  != NULL) push(&s, cur->left);
    }
    printf("\n");
}

void inorder(Node *root) {
    Stack s;
    initStack(&s);
    Node *cur = root;

    printf("Inorder   : ");
    while (cur != NULL || !isEmpty(&s)) {
        while (cur != NULL) {
            push(&s, cur);
            cur = cur->left;
        }
        cur = pop(&s);
        printf("%c ", cur->data);
        cur = cur->right;
    }
    printf("\n");
}

void postorder(Node *root) {
    if (root == NULL) return;
    Stack s1, s2;
    initStack(&s1);
    initStack(&s2);
    push(&s1, root);

    while (!isEmpty(&s1)) {
        Node *cur = pop(&s1);
        push(&s2, cur);
        if (cur->left  != NULL) push(&s1, cur->left);
        if (cur->right != NULL) push(&s1, cur->right);
    }

    printf("Postorder : ");
    while (!isEmpty(&s2)) {
        Node *cur = pop(&s2);
        printf("%c ", cur->data);
    }
    printf("\n");
}

void freeTree(Node *root) {
    if (root == NULL) return;
    Stack s;
    initStack(&s);
    push(&s, root);
    while (!isEmpty(&s)) {
        Node *cur = pop(&s);
        if (cur->left)  push(&s, cur->left);
        if (cur->right) push(&s, cur->right);
        free(cur);
    }
}
