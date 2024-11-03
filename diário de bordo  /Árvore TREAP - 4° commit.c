//Aluno: Frederico Luck de Oliveira
//Turma: Estrutura de Dados II, CC6N

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct TreapNode {
    int dado;
    int prioridade;
    struct TreapNode* esquerda;
    struct TreapNode* direita;
};

// Função para criar um novo nó
struct TreapNode* criarNo(int dado) {
    struct TreapNode* novoNo = (struct TreapNode*)malloc(sizeof(struct TreapNode));
    novoNo->dado = dado;
    novoNo->prioridade = rand() % 100;  // Prioridade aleatória
    novoNo->esquerda = novoNo->direita = NULL;
    return novoNo;
}

// Rotação à direita
struct TreapNode* rotacaoDireita(struct TreapNode* y) {
    struct TreapNode* x = y->esquerda;
    y->esquerda = x->direita;
    x->direita = y;
    return x;
}

// Rotação à esquerda
struct TreapNode* rotacaoEsquerda(struct TreapNode* x) {
    struct TreapNode* y = x->direita;
    x->direita = y->esquerda;
    y->esquerda = x;
    return y;
}

// Inserção em uma árvore TREAP
struct TreapNode* inserir(struct TreapNode* raiz, int dado) {
    if (raiz == NULL) {
        return criarNo(dado);
    }

    if (dado < raiz->dado) {
        raiz->esquerda = inserir(raiz->esquerda, dado);
        if (raiz->esquerda != NULL && raiz->esquerda->prioridade > raiz->prioridade) {
            raiz = rotacaoDireita(raiz);
        }
    } else {
        raiz->direita = inserir(raiz->direita, dado);
        if (raiz->direita != NULL && raiz->direita->prioridade > raiz->prioridade) {
            raiz = rotacaoEsquerda(raiz);
        }
    }

    return raiz;
}

// Função para deletar um nó com o dado fornecida
struct TreapNode* deletar(struct TreapNode* raiz, int dado) {
    if (raiz == NULL) {
        return NULL;
    }

    if (dado < raiz->dado) {
        raiz->esquerda = deletar(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = deletar(raiz->direita, dado);
    } else {
        // Se o nó é encontrado, verificamos os casos de remoção
        if (raiz->esquerda == NULL) {
            struct TreapNode* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            struct TreapNode* temp = raiz->esquerda;
            free(raiz);
            return temp;
        } else {
            // Se o nó tem ambos os filhos, uma rotação é ralizada para manter a propriedade                do heap
            if (raiz->esquerda->prioridade > raiz->direita->prioridade) {
                raiz = rotacaoDireita(raiz);
                raiz->direita = deletar(raiz->direita, dado);
            } else {
                raiz = rotacaoEsquerda(raiz);
                raiz->esquerda = deletar(raiz->esquerda, dado);
            }
        }
    }

    return raiz;
}

// Função para imprimir a árvore de forma visual
void imprimirArvore(struct TreapNode* raiz, int espaco) {
    if (raiz == NULL) {
        return;
    }
    espaco += 10;
    // Imprime a subárvore direita primeiro
    imprimirArvore(raiz->direita, espaco);
    // Imprime o nó atual após o espaço apropriado
    printf("\n");
    for (int i = 10; i < espaco; i++) {
        printf(" ");
    }
    // Código para deixar a prioridade em vermelho na impressão
    printf("%d (\033[31m%d\033[0m)\n", raiz->dado, raiz->prioridade);

    // Imprime a subárvore esquerda
    imprimirArvore(raiz->esquerda, espaco);
}

// Função para liberar a memória
void liberarArvore(struct TreapNode* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    srand(time(NULL));  // Inicializa a semente para números aleatórios

    struct TreapNode* raiz = NULL;
    int dados[] = {20, 10, 30, 5, 15, 25, 35};
    int n = sizeof(dados) / sizeof(dados[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, dados[i]);
    }

    printf("Percurso em ordem da árvore TREAP:\n");
    imprimirArvore(raiz, 0);

    deletar(raiz, 20);

    printf("Percurso em ordem da árvore TREAP após a remoção do número 20"\n);
    imprimirArvore(raiz, 0);

    // Libera a memória
    liberarArvore(raiz);
    return 0;
}
