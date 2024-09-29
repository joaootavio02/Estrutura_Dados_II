/* Kevin Monteiro do Nascimento Ponciano */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo um tipo para a estrutura do nó da árvore AVL
typedef struct NO* ArvoreAVL;

// Estrutura do nó da árvore AVL, que contém a informação, altura, ponteiros para os nós da esquerda e direita
struct NO {
    int valor; // Valor armazenado no nó
    int altura; // Altura do nó
    struct NO *esquerda; // Ponteiro para o filho da esquerda
    struct NO *direita; // Ponteiro para o filho da direita
};

ArvoreAVL* criar_ArvoreAVL(){
    // Alocando memória para a árvore AVL (raiz)
    ArvoreAVL* raiz = (ArvoreAVL*) malloc(sizeof(ArvoreAVL));
    if(raiz != NULL)
        *raiz = NULL; // Inicializa a raiz como NULL
    return raiz;
}

void liberar_No(struct NO* no){
    if(no == NULL)
        return; // Caso base: se o nó for NULL, retorna
    liberar_No(no->esquerda); // Libera os nós da subárvore esquerda
    liberar_No(no->direita); // Libera os nós da subárvore direita
    free(no); // Libera o nó atual
    no = NULL; // Marca o nó como NULL após a liberação
}

void liberar_ArvoreAVL(ArvoreAVL* raiz){
    if(raiz == NULL)
        return; // Se a raiz já é NULL, não precisa liberar
    liberar_No(*raiz); // Libera todos os nós da árvore
    free(raiz); // Libera a memória alocada para a raiz
}

int altura_DoNo(struct NO* no){
    if(no == NULL)
        return -1; // Altura de um nó inexistente é -1
    else
        return no->altura; // Retorna a altura do nó
}

int fatorDeBalanceamento_No(struct NO* no){
    // Calcula o fator de balanceamento de um nó (diferença entre alturas das subárvores esquerda e direita)
    return labs(altura_DoNo(no->esquerda) - altura_DoNo(no->direita));
}

int maior(int valor1, int valor2){
    // Retorna o maior valor entre dois inteiros
    return (valor1 > valor2) ? valor1 : valor2;
}

int arvoreEstaVazia(ArvoreAVL *raiz){
    if(raiz == NULL || *raiz == NULL)
        return 1; // Retorna 1 se a árvore está vazia
    return 0; // Caso contrário, a árvore não está vazia
}

int contarNos_ArvoreAVL(ArvoreAVL *raiz){
    if (raiz == NULL || *raiz == NULL)
        return 0; // Se a árvore está vazia, tem 0 nós
    int totalEsquerda = contarNos_ArvoreAVL(&((*raiz)->esquerda)); // Conta nós à esquerda
    int totalDireita = contarNos_ArvoreAVL(&((*raiz)->direita)); // Conta nós à direita
    return (totalEsquerda + totalDireita + 1); // Retorna o total de nós (inclui o nó atual)
}

int altura_ArvoreAVL(ArvoreAVL *raiz){
    if (raiz == NULL || *raiz == NULL)
        return 0; // Se a árvore está vazia, a altura é 0
    int alturaEsquerda = altura_ArvoreAVL(&((*raiz)->esquerda)); // Calcula altura da subárvore esquerda
    int alturaDireita = altura_ArvoreAVL(&((*raiz)->direita)); // Calcula altura da subárvore direita
    return maior(alturaEsquerda, alturaDireita) + 1; // A altura da árvore é o máximo entre as subárvores mais 1
}

void percorrerPreOrdem(ArvoreAVL *raiz){
    if(raiz == NULL)
        return; // Caso base: se a árvore está vazia, retorna
    if(*raiz != NULL){
        printf("Nó %d: altura %d\n", (*raiz)->valor, altura_DoNo(*raiz)); // Exibe valor e altura do nó
        percorrerPreOrdem(&((*raiz)->esquerda)); // Percorre a subárvore esquerda em pré-ordem
        percorrerPreOrdem(&((*raiz)->direita)); // Percorre a subárvore direita em pré-ordem
    }
}

void percorrerEmOrdem(ArvoreAVL *raiz){
    if(raiz == NULL)
        return; // Caso base: se a árvore está vazia, retorna
    if(*raiz != NULL){
        percorrerEmOrdem(&((*raiz)->esquerda)); // Percorre a subárvore esquerda em ordem
        printf("Nó %d: altura %d, fator de balanceamento %d\n", (*raiz)->valor, altura_DoNo(*raiz), fatorDeBalanceamento_No(*raiz)); // Exibe valor, altura e fator de balanceamento do nó
        percorrerEmOrdem(&((*raiz)->direita)); // Percorre a subárvore direita em ordem
    }
}

void percorrerPosOrdem(ArvoreAVL *raiz){
    if(raiz == NULL)
        return; // Caso base: se a árvore está vazia, retorna
    if(*raiz != NULL){
        percorrerPosOrdem(&((*raiz)->esquerda)); // Percorre a subárvore esquerda em pós-ordem
        percorrerPosOrdem(&((*raiz)->direita)); // Percorre a subárvore direita em pós-ordem
        printf("%d\n", (*raiz)->valor); // Exibe o valor do nó
    }
}

int buscarNaArvoreAVL(ArvoreAVL *raiz, int valorProcurado){
    if(raiz == NULL)
        return 0; // Se a árvore está vazia, o valor não foi encontrado
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(valorProcurado == atual->valor)
            return 1; // Se o valor é encontrado, retorna 1
        if(valorProcurado > atual->valor)
            atual = atual->direita; // Se o valor é maior, busca na subárvore direita
        else
            atual = atual->esquerda; // Se o valor é menor, busca na subárvore esquerda
    }
    return 0; // Se não encontrar, retorna 0
}

// Funções de rotação (ajustam o balanceamento da árvore)
void rotacaoLL(ArvoreAVL *raiz){
    struct NO *filhoEsquerda = (*raiz)->esquerda; // Filho à esquerda
    (*raiz)->esquerda = filhoEsquerda->direita; // Ajusta a subárvore esquerda
    filhoEsquerda->direita = *raiz; // Ajusta o filho à direita
    (*raiz)->altura = maior(altura_DoNo((*raiz)->esquerda), altura_DoNo((*raiz)->direita)) + 1; // Recalcula altura da raiz
    filhoEsquerda->altura = maior(altura_DoNo(filhoEsquerda->esquerda), (*raiz)->altura) + 1; // Recalcula altura do filho
    *raiz = filhoEsquerda; // Atualiza a raiz
}

void rotacaoRR(ArvoreAVL *raiz){
    struct NO *filhoDireita = (*raiz)->direita; // Filho à direita
    (*raiz)->direita = filhoDireita->esquerda; // Ajusta a subárvore direita
    filhoDireita->esquerda = *raiz; // Ajusta o filho à esquerda
    (*raiz)->altura = maior(altura_DoNo((*raiz)->esquerda), altura_DoNo((*raiz)->direita)) + 1; // Recalcula altura da raiz
    filhoDireita->altura = maior(altura_DoNo(filhoDireita->direita), (*raiz)->altura) + 1; // Recalcula altura do filho
    *raiz = filhoDireita; // Atualiza a raiz
}

void rotacaoLR(ArvoreAVL *raiz){
    rotacaoRR(&(*raiz)->esquerda); // Primeiro faz uma rotação simples à direita no filho à esquerda
    rotacaoLL(raiz); // Depois faz uma rotação simples à esquerda
}

void rotacaoRL(ArvoreAVL *raiz){
    rotacaoLL(&(*raiz)->direita); // Primeiro faz uma rotação simples à esquerda no filho à direita
    rotacaoRR(raiz); // Depois faz uma rotação simples à direita
}

// Função principal que insere um valor na árvore AVL
int inserirNaArvoreAVL(ArvoreAVL *raiz, int valor){
    if(*raiz == NULL){ // Caso base: árvore vazia ou nó folha
        struct NO *novoNo = (struct NO*) malloc(sizeof(struct NO)); // Cria um novo nó
        if(novoNo == NULL)
            return 0; // Falha ao alocar memória
        novoNo->valor = valor; // Atribui o valor ao novo nó
        novoNo->altura = 0; // A altura inicial é 0
        novoNo->esquerda = NULL; // Não tem filhos inicialmente
        novoNo->direita = NULL;
        *raiz = novoNo; // O novo nó se torna a raiz (ou sub-raiz)
        return 1; // Sucesso ao inserir
    }

    struct NO *atual = *raiz; // Ponto de inserção
    int resultado;
    if(valor < atual->valor){ // Insere na subárvore esquerda
        if((resultado = inserirNaArvoreAVL(&(atual->esquerda), valor)) == 1){ // Recursivamente insere o valor
            if(fatorDeBalanceamento_No(atual) >= 2){ // Verifica se está desbalanceado
                if(valor < (*raiz)->esquerda->valor)
                    rotacaoLL(raiz); // Rotação simples à esquerda (LL)
                else
                    rotacaoLR(raiz); // Rotação dupla esquerda-direita (LR)
            }
        }
    } else if(valor > atual->valor){ // Insere na subárvore direita
        if((resultado = inserirNaArvoreAVL(&(atual->direita), valor)) == 1){
            if(fatorDeBalanceamento_No(atual) >= 2){
                if((*raiz)->direita->valor < valor)
                    rotacaoRR(raiz); // Rotação simples à direita (RR)
                else
                    rotacaoRL(raiz); // Rotação dupla direita-esquerda (RL)
            }
        }
    } else {
        printf("Valor duplicado!\n"); // O valor já existe na árvore
        return 0;
    }

    atual->altura = maior(altura_DoNo(atual->esquerda), altura_DoNo(atual->direita)) + 1; // Recalcula a altura
    return resultado;
}

// Função principal do programa
int main(){
    ArvoreAVL* avl = criar_ArvoreAVL(); // Cria a árvore AVL
    int valores[10] = {1, 2, 3, 10, 4, 5, 9, 7, 8, 6}; // Valores a serem inseridos na árvore

    // Insere todos os valores na árvore
    for(int i = 0; i < 10; i++){
        inserirNaArvoreAVL(avl, valores[i]);
    }

    // Exibe a árvore em ordem
    printf("\nÁrvore AVL em ordem:\n");
    percorrerEmOrdem(avl);

    // Remove alguns valores e exibe a árvore após cada remoção
    remove_ArvoreAVL(avl, 6);
    printf("\nÁrvore AVL após remover 6:\n");
    percorrerEmOrdem(avl);

    remove_ArvoreAVL(avl, 7);
    printf("\nÁrvore AVL após remover 7:\n");
    percorrerEmOrdem(avl);

    remove_ArvoreAVL(avl, 4);
    printf("\nÁrvore AVL após remover 4:\n");
    percorrerEmOrdem(avl);

    liberar_ArvoreAVL(avl); // Libera a memória usada pela árvore

    return 0;
}
