# 과제 04 - 이진트리 순회 프로그램 (Binary Tree Traversal)

괄호 표기법으로 입력받은 이진트리를 포인터 기반 연결 자료구조로 구성하고,
전위(Preorder) · 중위(Inorder) · 후위(Postorder) 순회를 **재귀를 사용하지 않고**
Stack을 이용한 반복적(iterative) 방법으로 구현한 프로그램입니다.

## 파일 구성

| 파일 | 설명 |
|---|---|
| `traversal.h` | 트리 노드, Stack 구조체 및 모든 함수의 선언(prototype) |
| `traversal.c` | 파서, ASCII 트리 출력, 순회 함수 등 실제 구현 |
| `main.c` | 프로그램 진입점 (테스트용 트리 구성 및 결과 출력) |

## 컴파일 및 실행 방법

```bash
gcc main.c binary_tree.c -o tree
./tree
```

## 입력 형식 (괄호 표기법)

```
값(왼쪽 서브트리,오른쪽 서브트리)
```

- 자식이 없는 리프 노드는 괄호 없이 값만 작성합니다. 예: `D`
- 한쪽 자식이 없는 경우, 해당 위치를 비워 둡니다. 예: `F(,J)` → F는 왼쪽 자식이 없고 오른쪽 자식은 J
- 예시: `A(B(D,E),C(,F))`

```
        A
       / \
      B   C
     / \   \
    D   E   F
```

`main.c`에서 테스트에 사용한 트리는 다음과 같습니다 (노드 10개):

```
A(B(D(G,H),E),C(I,F(,J)))
```

## 구현 조건 준수 사항

- `preorder()`, `inorder()`, `postorder()`는 물론 파싱(`buildTreeFromString`)과
  메모리 해제(`freeTree`)까지 **프로그램 전체에서 재귀를 사용하지 않습니다.**
- 순회 시 필요한 경우 명시적으로 구현한 Stack(`Stack`, `StackND`, `FrameStack`)을 사용합니다.
- 트리는 포인터를 이용한 연결 자료구조(linked representation)로 구성됩니다.
- 괄호 표기법이 올바르지 않은 경우, 오류 메시지를 출력하고 프로그램을 종료합니다.

## 실행 결과 예시

```
Input tree (parenthesis notation): A(B(D(G,H),E),C(I,F(,J)))

Tree structure:
                      A
                     / \
              B               C
             / \             / \
      D           E       I       F
     / \                           \
  G       H                           J

Preorder  : A B D G H E C I F J
Inorder   : G D H B E A I C F J
Postorder : G H D E B I J F C A
```
