#include <stdio.h>
#include <stdlib.h>

// Definição de cores usadas na árvore Rubro-Negra
#define RED 1
#define BLACK 0

// Estrutura do nó da árvore Rubro-Negra
struct NO {
    int info;
    struct NO *esq;
    struct NO *dir;
    int cor;
};

typedef struct NO* ArvLLRB; // Definindo o tipo de árvore como ponteiro para struct NO

// =================================
// CRIAÇÃO DA ÁRVORE
// =================================
ArvLLRB* cria_ArvLLRB() {
    ArvLLRB* raiz = (ArvLLRB*) malloc(sizeof(ArvLLRB));
    if (raiz != NULL) {
        *raiz = NULL;
    }
    return raiz;
}

// =================================
// LIBERAÇÃO DA ÁRVORE
// =================================
void libera_NO(struct NO* no) {
    if (no == NULL)
        return;
    libera_NO(no->esq);
    libera_NO(no->dir);
    free(no);
}

void libera_ArvLLRB(ArvLLRB* raiz) {
    if (raiz == NULL)
        return;
    libera_NO(*raiz); // Libera cada nó da árvore
    free(raiz);       // Libera a raiz da árvore
}

// =================================
// CONSULTA DE VALOR NA ÁRVORE
// =================================
int consulta_ArvLLRB(ArvLLRB* raiz, int valor) {
    if (raiz == NULL)
        return 0;
    struct NO* atual = *raiz;
    while (atual != NULL) {
        if (valor == atual->info) {
            return 1; // Valor encontrado
        }
        if (valor > atual->info)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return 0; // Valor não encontrado
}

// =================================
// ROTAÇÕES UTILIZADAS NA ÁRVORE
// =================================
struct NO* rotacionaEsquerda(struct NO* A) {
    struct NO* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

struct NO* rotacionaDireita(struct NO* A) {
    struct NO* B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

// =================================
// PROPRIEDADES DOS NÓS
// =================================
int cor(struct NO* H) {
    if (H == NULL)
        return BLACK;
    else
        return H->cor;
}

void trocaCor(struct NO* H) {
    H->cor = !H->cor;
    if (H->esq != NULL)
        H->esq->cor = !H->esq->cor;
    if (H->dir != NULL)
        H->dir->cor = !H->dir->cor;
}

// =================================
// INSERÇÃO DE NÓS
// =================================
struct NO* insereNO(struct NO* H, int valor, int *resp) {
    if (H == NULL) {
        struct NO* novo = (struct NO*) malloc(sizeof(struct NO));
        if (novo == NULL) {
            *resp = 0;
            return NULL;
        }
        novo->info = valor;
        novo->cor = RED;
        novo->dir = NULL;
        novo->esq = NULL;
        *resp = 1;
        return novo;
    }

    if (valor == H->info)
        *resp = 0; // Valor duplicado
    else {
        if (valor < H->info)
            H->esq = insereNO(H->esq, valor, resp);
        else
            H->dir = insereNO(H->dir, valor, resp);
    }

    // Manter as propriedades da árvore Rubro-Negra
    if (cor(H->dir) == RED && cor(H->esq) == BLACK)
        H = rotacionaEsquerda(H);

    if (cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);

    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);

    return H;
}

int insere_ArvLLRB(ArvLLRB* raiz, int valor) {
    int resp;
    *raiz = insereNO(*raiz, valor, &resp);
    if (*raiz != NULL)
        (*raiz)->cor = BLACK; // Garantir que a raiz sempre seja preta
    return resp;
}

// =================================
// REMOÇÃO DE NÓS
// =================================
struct NO* balancear(struct NO* H) {
    if (cor(H->dir) == RED)
        H = rotacionaEsquerda(H);
    if (H->esq != NULL && cor(H->esq) == RED && cor(H->esq->esq) == RED)
        H = rotacionaDireita(H);
    if (cor(H->esq) == RED && cor(H->dir) == RED)
        trocaCor(H);
    return H;
}

struct NO* move2EsqRED(struct NO* H) {
    trocaCor(H);
    if (cor(H->dir->esq) == RED) {
        H->dir = rotacionaDireita(H->dir);
        H = rotacionaEsquerda(H);
        trocaCor(H);
    }
    return H;
}

struct NO* move2DirRED(struct NO* H) {
    trocaCor(H);
    if (cor(H->esq->esq) == RED) {
        H = rotacionaDireita(H);
        trocaCor(H);
    }
    return H;
}

struct NO* removerMenor(struct NO* H) {
    if (H->esq == NULL) {
        free(H);
        return NULL;
    }
    if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
        H = move2EsqRED(H);
    H->esq = removerMenor(H->esq);
    return balancear(H);
}

struct NO* procuraMenor(struct NO* atual) {
    struct NO* no1 = atual;
    struct NO* no2 = atual->esq;
    while (no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

struct NO* remove_NO(struct NO* H, int valor) {
    if (valor < H->info) {
        if (cor(H->esq) == BLACK && cor(H->esq->esq) == BLACK)
            H = move2EsqRED(H);
        H->esq = remove_NO(H->esq, valor);
    } else {
        if (cor(H->esq) == RED)
            H = rotacionaDireita(H);
        if (valor == H->info && (H->dir == NULL)) {
            free(H);
            return NULL;
        }
        if (cor(H->dir) == BLACK && cor(H->dir->esq) == BLACK)
            H = move2DirRED(H);
        if (valor == H->info) {
            struct NO* x = procuraMenor(H->dir);
            H->info = x->info;
            H->dir = removerMenor(H->dir);
        } else
            H->dir = remove_NO(H->dir, valor);
    }
    return balancear(H);
}

int remove_ArvLLRB(ArvLLRB* raiz, int valor) {
    if (consulta_ArvLLRB(raiz, valor)) {
        struct NO* h = *raiz;
        *raiz = remove_NO(h, valor);
        if (*raiz != NULL)
            (*raiz)->cor = BLACK;
        return 1;
    } else
        return 0;
}

// =================================
// PROPRIEDADES DA ÁRVORE
// =================================
int estaVazia_ArvLLRB(ArvLLRB* raiz) {
    if (raiz == NULL || *raiz == NULL)
        return 1;
    return 0;
}

int totalNO_ArvLLRB(ArvLLRB* raiz) {
    if (raiz == NULL || *raiz == NULL)
        return 0;
    int alt_esq = totalNO_ArvLLRB(&((*raiz)->esq));
    int alt_dir = totalNO_ArvLLRB(&((*raiz)->dir));
    return (alt_esq + alt_dir + 1);
}

int altura_ArvLLRB(ArvLLRB* raiz) {
    if (raiz == NULL || *raiz == NULL)
        return 0;
    int alt_esq = altura_ArvLLRB(&((*raiz)->esq));
    int alt_dir = altura_ArvLLRB(&((*raiz)->dir));
    if (alt_esq > alt_dir)
        return (alt_esq + 1);
    else
        return (alt_dir + 1);
}

// =================================
// PERCURSOS DA ÁRVORE
// =================================
void posOrdem_ArvLLRB(ArvLLRB* raiz, int H) {
    if (raiz == NULL)
        return;
    if (*raiz != NULL) {
        posOrdem_ArvLLRB(&((*raiz)->esq), H + 1);
        posOrdem_ArvLLRB(&((*raiz)->dir), H + 1);
        printf("%d %s: H(%d)\n", (*raiz)->info, (*raiz)->cor == RED ? "Vermelho" : "Preto", H);
    }
}

void emOrdem_ArvLLRB(ArvLLRB* raiz, int H) {
    if (raiz == NULL)
        return;
    if (*raiz != NULL) {
        emOrdem_ArvLLRB(&((*raiz)->esq), H + 1);
        printf("%d%s: H(%d)\n", (*raiz)->info, (*raiz)->cor == RED ? "R" : "B", H);
        emOrdem_ArvLLRB(&((*raiz)->dir), H + 1);
    }
}

void preOrdem_ArvLLRB(ArvLLRB* raiz, int H) {
    if (raiz == NULL)
        return;
    if (*raiz != NULL) {
        printf("%d %s: H(%d)\n", (*raiz)->info, (*raiz)->cor == RED ? "Vermelho" : "Preto", H);
        preOrdem_ArvLLRB(&((*raiz)->esq), H + 1);
        preOrdem_ArvLLRB(&((*raiz)->dir), H + 1);
    }
}

// =================================
// PROGRAMA PRINCIPAL
// =================================
int main() {
    ArvLLRB* raiz = cria_ArvLLRB();
    int i, N = 9, val[9] = {11, 2, 1, 5, 4, 7, 8, 14, 15};

    // Inserindo os valores na árvore
    for (i = 0; i < N; i++) {
        insere_ArvLLRB(raiz, val[i]);
    }

    // Exibindo a árvore em ordem
    emOrdem_ArvLLRB(raiz, 0);

    printf("\n\n==========================\n\n");

    // Removendo nós da árvore
    remove_ArvLLRB(raiz, 4);
    emOrdem_ArvLLRB(raiz, 0);
    printf("\n\n==========================\n\n");
    remove_ArvLLRB(raiz, 1);
    emOrdem_ArvLLRB(raiz, 0);

    return 0;
}
