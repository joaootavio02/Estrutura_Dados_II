#include <stdio.h>
#include <stdlib.h>

// Estrutura de nó da árvore binária
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Função para criar um novo nó
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Erro ao alocar memória!\n");
        return NULL;
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Função para realizar o caminhamento pré-ordem
void preOrder(Node* root) {
    if (root == NULL) return;
    printf("%d ", root->data);
    preOrder(root->left);
    preOrder(root->right);
}

// Função para realizar o caminhamento pós-ordem
void postOrder(Node* root) {
    if (root == NULL) return;
    postOrder(root->left);
    postOrder(root->right);
    printf("%d ", root->data);
}

// Função para realizar o caminhamento central (in-order)
void inOrder(Node* root) {
    if (root == NULL) return;
    inOrder(root->left);
    printf("%d ", root->data);
    inOrder(root->right);
}

// Função para liberar a memória da árvore
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    // Exemplo de uso das funções
    Node* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->left = createNode(6);
    root->right->right = createNode(7);

    printf("Caminhamento pré-ordem: ");
    preOrder(root);
    printf("\n");

    printf("Caminhamento pós-ordem: ");
    postOrder(root);
    printf("\n");

    printf("Caminhamento central (in-order): ");
    inOrder(root);
    printf("\n");

    // Libera a memória alocada para a árvore
    freeTree(root);

    return 0;
}
