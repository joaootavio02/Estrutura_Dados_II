#include <stdio.h>
#include <stdlib.h>

typedef struct no {
    int dado;
    struct no *esq;
    struct no *dir;
} No;

// Função para criar um novo nó
No* criarNo(int valor) {
    No* novoNo = (No*) malloc(sizeof(No));
    novoNo->dado = valor;
    novoNo->esq = NULL;
    novoNo->dir = NULL;
    return novoNo;
}

// Função para inserir um elemento na árvore
No* inserir(No* raiz, int valor) {
    if (raiz == NULL) {
        return criarNo(valor);
    } else if (valor < raiz->dado) {
        raiz->esq = inserir(raiz->esq, valor);
    } else {
        raiz->dir = inserir(raiz->dir, valor);
    }
    return raiz;
}

// Função para pesquisar um elemento na árvore
No* pesquisar(No* raiz, int valor) {
    if (raiz == NULL || raiz->dado == valor) {
        return raiz;
    } else if (valor < raiz->dado) {
        return pesquisar(raiz->esq, valor);
    } else {
        return pesquisar(raiz->dir, valor);
    }
}

// Função para encontrar o menor elemento na subárvore direita
No* encontrarMenor(No* no) {
    No* atual = no;
    while (atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

// Função para remover um elemento da árvore
No* remover(No* raiz, int valor) {
    if (raiz == NULL) {
        return raiz;
    }

    if (valor < raiz->dado) {
        raiz->esq = remover(raiz->esq, valor);
    } else if (valor > raiz->dado) {
        raiz->dir = remover(raiz->dir, valor);
    } else {
        // Nó a ser removido encontrado

        // Caso 1: Nó folha
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            return NULL;
        }

        // Caso 2: Nó com apenas um filho
        else if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }

        // Caso 3: Nó com dois filhos
        No* temp = encontrarMenor(raiz->dir);
        raiz->dado = temp->dado;
        raiz->dir = remover(raiz->dir, temp->dado);
    }
    return raiz;
}



int main() {
    No* raiz = NULL;

    raiz = inserir(raiz, 10);
    raiz = inserir(raiz, 5);
    raiz = inserir(raiz, 15);

    // Pesquisar o valor 15
    No* encontrado = pesquisar(raiz, 15);
    if (encontrado != NULL) {
        printf("Valor 15 encontrado!\n");
    } else {
        printf("Valor não encontrado!\n");
    }

    // Remover o valor 10
    raiz = remover(raiz, 10);

    return 0;
}