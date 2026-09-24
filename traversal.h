#ifndef TRAVERSAL_H
#define TRAVERSAL_H

#define MAX_STACK 100
#define SPACING 4
#define MAX_ROWS 40
#define MAX_COLS 400

/* ---------- 트리 노드 ---------- */
typedef struct Node {
    char data;
    struct Node *left;
    struct Node *right;
    int col;    /* ASCII 트리 출력에서만 사용 */
    int depth;  /* ASCII 트리 출력에서만 사용 */
} Node;

Node *createNode(char data);

/* ---------- Stack #1: Node* 전용 스택 ---------- */
typedef struct {
    Node *items[MAX_STACK];
    int top;
} Stack;

void  initStack(Stack *s);
int   isEmpty(Stack *s);
void  push(Stack *s, Node *node);
Node *pop(Stack *s);

/* ---------- Stack #2: (Node*, depth) 쌍 ---------- */
typedef struct { Node *node; int depth; } NodeDepth;
typedef struct { NodeDepth items[MAX_STACK]; int top; } StackND;

void      initStackND(StackND *s);
int       isEmptyND(StackND *s);
void      pushND(StackND *s, Node *node, int depth);
NodeDepth popND(StackND *s);

/* ---------- Stack #3: 파서 프레임 ---------- */
typedef struct { Node *node; int expecting; } Frame;
typedef struct { Frame items[MAX_STACK]; int top; } FrameStack;

void  initFrameStack(FrameStack *s);
int   isEmptyFrameStack(FrameStack *s);
void  pushFrame(FrameStack *s, Node *node, int expecting);
Frame *peekFrame(FrameStack *s);
void  popFrame(FrameStack *s);

/* ---------- 파서 ---------- */
void  skipSpaces(const char **s);
Node *buildTreeFromString(const char *input);

/* ---------- ASCII 트리 출력 ---------- */
void assignPositions(Node *root);
void fillGrid(Node *root, char grid[MAX_ROWS][MAX_COLS]);
void drawTreeASCII(Node *root);

/* ---------- 순회 (채점 대상 함수) ---------- */
void preorder(Node *root);
void inorder(Node *root);
void postorder(Node *root);

/* ---------- 메모리 해제 ---------- */
void freeTree(Node *root);

#endif /* TRAVERSAL_H */
