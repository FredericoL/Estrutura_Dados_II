//Aluno: Frederico Luck de Oliveira
//Turma: Estrutura de Dados II, CC6N

// *PS: Não consegui fazer a função de remoção

#include <stdio.h>
#include <stdlib.h>

// A ordem mínima da árvore B (t). Cada nó pode ter no máximo 2*ORDEM-1 dados.
#define ORDEM 3   

struct NoArvoreB {
    int dados[2 * ORDEM - 1];  // Array de dados
    struct NoArvoreB* filhos[2 * ORDEM];  // Ponteiros para os fihlos
    int numDados;  // Número atual de dados
    int folha;  // 1 se for folha, 0 se não for
};

// Função para criar um novo nó da árvore B
struct NoArvoreB* criarNo(int folha) {
    struct NoArvoreB* novoNo = (struct NoArvoreB*)malloc(sizeof(struct NoArvoreB));
    novoNo->folha = folha;
    novoNo->numDados = 0;

    for (int i = 0; i < 2 * ORDEM; i++) {
        novoNo->filhos[i] = NULL;
    }

    return novoNo;
}

// Função para dividir um nó filho cheio
void dividirFilho(struct NoArvoreB* pai, int i, struct NoArvoreB* filho) {
    struct NoArvoreB* novoFilho = criarNo(filho->folha);
    novoFilho->numDados = ORDEM - 1;

    // Copia os últimos dados de filho para novoFilho
    for (int j = 0; j < ORDEM - 1; j++) {
        novoFilho->dados[j] = filho->dados[j + ORDEM];
    }

    // Copia os filhos se não for uma folha
    if (!filho->folha) {
        for (int j = 0; j < ORDEM; j++) {
            novoFilho->filhos[j] = filho->filhos[j + ORDEM];
        }
    }

    filho->numDados = ORDEM - 1;

    for (int j = pai->numDados; j >= i + 1; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[i + 1] = novoFilho;

    for (int j = pai->numDados - 1; j >= i; j--) {
        pai->dados[j + 1] = pai->dados[j];
    }

    pai->dados[i] = filho->dados[ORDEM - 1];
    pai->numDados++;
}

// Função auxiliar para inserir um novo dado no nó que não está cheio
void inserirNaoCheio(struct NoArvoreB* no, int dado) {
    int i = no->numDados - 1;

    if (no->folha) {
        while (i >= 0 && dado < no->dados[i]) {
            no->dados[i + 1] = no->dados[i];
            i--;
        }

        no->dados[i + 1] = dado;
        no->numDados++;
    } else {
        while (i >= 0 && dado < no->dados[i]) {
            i--;
        }
        i++;

        if (no->filhos[i]->numDados == 2 * ORDEM - 1) {
            dividirFilho(no, i, no->filhos[i]);

            if (dado > no->dados[i]) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], dado);
    }
}

// Função para inserir um dado na árvore B
struct NoArvoreB* inserir(struct NoArvoreB* raiz, int dado) {
    if (raiz == NULL) {
        raiz = criarNo(1);
        raiz->dados[0] = dado;
        raiz->numDados = 1;
    } else {
        if (raiz->numDados == 2 * ORDEM - 1) {
            struct NoArvoreB* novaRaiz = criarNo(0);
            novaRaiz->filhos[0] = raiz;
            dividirFilho(novaRaiz, 0, raiz);

            int i = 0;
            if (dado > novaRaiz->dados[0]) {
                i++;
            }
            inserirNaoCheio(novaRaiz->filhos[i], dado);

            raiz = novaRaiz;
        } else {
            inserirNaoCheio(raiz, dado);
        }
    }
    return raiz;
}

// Função para imprimir a árvore B
void imprimirArvore(struct NoArvoreB* no, int nivel) {
    if (no != NULL) {
        printf("Nível %d [", nivel);
        for (int i = 0; i < no->numDados; i++) {
            printf("%d ", no->dados[i]);
        }
        printf("]\n");

        for (int i = 0; i <= no->numDados; i++) {
            imprimirArvore(no->filhos[i], nivel + 1);
        }
    }
}

int main() {
    struct NoArvoreB* raiz = NULL;

    int dados[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(dados) / sizeof(dados[0]);

    for (int i = 0; i < n; i++) {
        raiz = inserir(raiz, dados[i]);
    }

    printf("Impressão da árvore B:\n");
    imprimirArvore(raiz, 0);

    return 0;
}
