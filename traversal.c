#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 100
#define SPACING 4 /* horizontal spacing between adjacent tree nodes in the ASCII drawing */
#define MAX_ROWS 40
#define MAX_COLS 400

/* ---------- Tree node ---------- */
typedef struct Node
{
    char data;
    struct Node *left;
    struct Node *right;
    int col;   /* used only for the ASCII tree drawing */
    int depth; /* used only for the ASCII tree drawing */
} Node;

Node *createNode(char data)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n)
    {
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

/* ---------- Simple array-based Stack of Node* ---------- */
typedef struct
{
    Node *items[MAX_STACK];
    int top; /* index of the top element; -1 means empty */
} Stack;

void initStack(Stack *s) { s->top = -1; }
int isEmpty(Stack *s) { return s->top == -1; }

void push(Stack *s, Node *node)
{
    if (s->top >= MAX_STACK - 1)
    {
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
    s->items[++(s->top)] = node;
}

Node *pop(Stack *s)
{
    if (isEmpty(s))
    {
        fprintf(stderr, "Stack underflow\n");
        exit(1);
    }
    return s->items[(s->top)--];
}

/* ---------- Parser: parenthesis notation -> linked tree ---------- */
void skipSpaces(const char **s)
{
    while (**s == ' ' || **s == '\t' || **s == '\n')
        (*s)++;
}

Node *parseTree(const char **s)
{
    skipSpaces(s);
    if (**s == '\0' || **s == ',' || **s == ')')
    {
        return NULL;
    }
    char value = **s;
    (*s)++;
    Node *node = createNode(value);

    skipSpaces(s);
    if (**s == '(')
    {
        (*s)++;
        node->left = parseTree(s);

        skipSpaces(s);
        if (**s != ',')
        {
            fprintf(stderr, "Invalid input: expected ',' near \"%s\"\n", *s);
            exit(1);
        }
        (*s)++;
        node->right = parseTree(s);

        skipSpaces(s);
        if (**s != ')')
        {
            fprintf(stderr, "Invalid input: expected ')' near \"%s\"\n", *s);
            exit(1);
        }
        (*s)++;
    }
    return node;
}

Node *buildTreeFromString(const char *input)
{
    const char *p = input;
    Node *root = parseTree(&p);
    skipSpaces(&p);
    if (*p != '\0')
    {
        fprintf(stderr, "Invalid input: unexpected trailing characters \"%s\"\n", p);
        exit(1);
    }
    return root;
}

static int g_col_counter;
static int g_max_depth;

void assignPositions(Node *node, int depth)
{
    if (node == NULL)
        return;
    assignPositions(node->left, depth + 1);
    node->col = g_col_counter++;
    node->depth = depth;
    if (depth > g_max_depth)
        g_max_depth = depth;
    assignPositions(node->right, depth + 1);
}

void fillGrid(Node *node, char grid[MAX_ROWS][MAX_COLS])
{
    if (node == NULL)
        return;
    int r = node->depth * 2;
    int c = node->col * SPACING + SPACING / 2;
    grid[r][c] = node->data;
    if (node->left)
    {
        grid[r + 1][c - 1] = '/';
        fillGrid(node->left, grid);
    }
    if (node->right)
    {
        grid[r + 1][c + 1] = '\\';
        fillGrid(node->right, grid);
    }
}

void drawTreeASCII(Node *root)
{
    static char grid[MAX_ROWS][MAX_COLS];
    g_col_counter = 0;
    g_max_depth = 0;
    assignPositions(root, 0);

    int rows = (g_max_depth + 1) * 2;
    int cols = g_col_counter * SPACING + SPACING;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            grid[i][j] = ' ';

    fillGrid(root, grid);

    for (int i = 0; i < rows; i++)
    {
        int lastNonSpace = -1;
        for (int j = 0; j < cols; j++)
            if (grid[i][j] != ' ')
                lastNonSpace = j;
        for (int j = 0; j <= lastNonSpace; j++)
            putchar(grid[i][j]);
        putchar('\n');
    }
}

/* ================= ITERATIVE TRAVERSALS ================= */

void preorder(Node *root)
{
    if (root == NULL)
        return;
    Stack s;
    initStack(&s);
    push(&s, root);

    printf("Preorder  : ");
    while (!isEmpty(&s))
    {
        Node *cur = pop(&s);
        printf("%c ", cur->data);
        if (cur->right != NULL)
            push(&s, cur->right);
        if (cur->left != NULL)
            push(&s, cur->left);
    }
    printf("\n");
}

void inorder(Node *root)
{
    Stack s;
    initStack(&s);
    Node *cur = root;

    printf("Inorder   : ");
    while (cur != NULL || !isEmpty(&s))
    {
        while (cur != NULL)
        {
            push(&s, cur);
            cur = cur->left;
        }
        cur = pop(&s);
        printf("%c ", cur->data);
        cur = cur->right;
    }
    printf("\n");
}

void postorder(Node *root)
{
    if (root == NULL)
        return;
    Stack s1, s2;
    initStack(&s1);
    initStack(&s2);
    push(&s1, root);

    while (!isEmpty(&s1))
    {
        Node *cur = pop(&s1);
        push(&s2, cur);
        if (cur->left != NULL)
            push(&s1, cur->left);
        if (cur->right != NULL)
            push(&s1, cur->right);
    }

    printf("Postorder : ");
    while (!isEmpty(&s2))
    {
        Node *cur = pop(&s2);
        printf("%c ", cur->data);
    }
    printf("\n");
}

void freeTree(Node *node)
{
    if (node == NULL)
        return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

/* ================= main ================= */
int main(void)
{
    const char *input = "A(B(D(G,H),E),C(I,F(,J)))";

    Node *root = buildTreeFromString(input);

    printf("Input tree (parenthesis notation): %s\n\n", input);

    printf("Tree structure:\n");
    drawTreeASCII(root);
    printf("\n");

    preorder(root);
    inorder(root);
    postorder(root);

    freeTree(root);
    return 0;
}
