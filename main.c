/*
 * main.c
 * ------
 * 프로그램 진입점. 테스트용 트리를 구성하고, 구조를 출력한 뒤
 * 세 가지 순회 결과를 출력한다.
 */

#include <stdio.h>
#include "traversal.h"

int main(void) {
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
