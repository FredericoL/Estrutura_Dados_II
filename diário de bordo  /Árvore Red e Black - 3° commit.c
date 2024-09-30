#include <stdio.h>
#include <stdlib.h>

// Definindo cores da árvore
#define RED 0
#define BLACK 1

// Estrutura de um nó da árvore Red-Black
typedef struct NoRB {
    int valor;
    int cor;
    struct NoRB *filhoEsq, *filhoDir, *parente;
} NoRB;

// Função para criar um novo nó
NoRB* novoNo(int valor) {
    NoRB* novo = (NoRB*)malloc(sizeof(NoRB));
    novo->valor = valor;
    novo->cor = RED;  // Novos nós são sempre inseridos como vermelhos
    novo->filhoEsq = novo->filhoDir = novo->parente = NULL;
    return novo;
}

// Função para realizar uma rotação à esquerda
void rotacaoEsquerda(NoRB **raiz, NoRB *no) {
    NoRB *direito = no->filhoDir;
    no->filhoDir = direito->filhoEsq;

    if (direito->filhoEsq != NULL)
        direito->filhoEsq->parente = no;

    direito->parente = no->parente;

    if (no->parente == NULL)
        *raiz = direito;
    else if (no == no->parente->filhoEsq)
        no->parente->filhoEsq = direito;
    else
        no->parente->filhoDir = direito;

    direito->filhoEsq = no;
    no->parente = direito;
}

// Função para realizar uma rotação à direita
void rotacaoDireita(NoRB **raiz, NoRB *no) {
    NoRB *esquerdo = no->filhoEsq;
    no->filhoEsq = esquerdo->filhoDir;

    if (esquerdo->filhoDir != NULL)
        esquerdo->filhoDir->parente = no;

    esquerdo->parente = no->parente;

    if (no->parente == NULL)
        *raiz = esquerdo;
    else if (no == no->parente->filhoDir)
        no->parente->filhoDir = esquerdo;
    else
        no->parente->filhoEsq = esquerdo;

    esquerdo->filhoDir = no;
    no->parente = esquerdo;
}

// Função para ajustar as violações da propriedade Red-Black após inserção
void ajustarInsercao(NoRB **raiz, NoRB *no) {
    while (no != *raiz && no->parente->cor == RED) {
        if (no->parente == no->parente->parente->filhoEsq) {
            NoRB *tio = no->parente->parente->filhoDir;
            if (tio != NULL && tio->cor == RED) {
                no->parente->cor = BLACK;
                tio->cor = BLACK;
                no->parente->parente->cor = RED;
                no = no->parente->parente;
            } else {
                if (no == no->parente->filhoDir) {
                    no = no->parente;
                    rotacaoEsquerda(raiz, no);
                }
                no->parente->cor = BLACK;
                no->parente->parente->cor = RED;
                rotacaoDireita(raiz, no->parente->parente);
            }
        } else {
            NoRB *tio = no->parente->parente->filhoEsq;
            if (tio != NULL && tio->cor == RED) {
                no->parente->cor = BLACK;
                tio->cor = BLACK;
                no->parente->parente->cor = RED;
                no = no->parente->parente;
            } else {
                if (no == no->parente->filhoEsq) {
                    no = no->parente;
                    rotacaoDireita(raiz, no);
                }
                no->parente->cor = BLACK;
                no->parente->parente->cor = RED;
                rotacaoEsquerda(raiz, no->parente->parente);
            }
        }
    }
    (*raiz)->cor = BLACK;
}

// Função para inserir um novo nó na árvore
void inserirRB(NoRB **raiz, int valor) {
    NoRB *novo = novoNo(valor);
    NoRB *pai = NULL;
    NoRB *atual = *raiz;

    while (atual != NULL) {
        pai = atual;
        if (novo->valor < atual->valor)
            atual = atual->filhoEsq;
        else
            atual = atual->filhoDir;
    }

    novo->parente = pai;

    if (pai == NULL)
        *raiz = novo;
    else if (novo->valor < pai->valor)
        pai->filhoEsq = novo;
    else
        pai->filhoDir = novo;

    ajustarInsercao(raiz, novo);
}

// Função para imprimir a árvore Red-Black em formato identado
void imprimirArvoreRB(NoRB *raiz, int profundidade) {
    if (raiz != NULL) {
        imprimirArvoreRB(raiz->filhoDir, profundidade + 1);

        for (int i = 0; i < profundidade; i++)
            printf("       "); // Espaçamento para simular profundidade

        if (raiz->cor == RED)
            printf("\033[31m%d\033[0m\n\n", raiz->valor); // Imprime em vermelho
        else
            printf("%d\n\n", raiz->valor); // Imprime em cor padrão (preto)

        imprimirArvoreRB(raiz->filhoEsq, profundidade + 1); // Chama de forma recursiva para
                                                            // percorrer a subárvore à esq
    }
}

// Função para buscar um valor na árvore Red-Black de forma recursiva
NoRB* buscarRB(NoRB *raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz; // Retorna o nó se encontrado ou NULL se a árvore estiver vazia ou não encontrar o valor
    }

    if (valor < raiz->valor) {
        return buscarRB(raiz->filhoEsq, valor); // Busca na subárvore esquerda se o valor for menor
    } else {
        return buscarRB(raiz->filhoDir, valor); // Busca na subárvore direita se o valor for maior
    }
}


// Função principal
int main() {
    NoRB *raiz = NULL;

    // Exemplo de inserção de valores na árvore Red-Black
    int valores[] = {12, 31, 20, 17, 11, 8, 3, 24, 15, 33};
    int tamanho = sizeof(valores) / sizeof(valores[0]);

    for (int i = 0; i < tamanho; i++) {
        inserirRB(&raiz, valores[i]);
    }

    // Imprimindo a árvore Red-Black
    printf("Árvore Red-Black: \n");
    imprimirArvoreRB(raiz, 3);
    printf("\n");

    int valorParaBuscar = 8; // Variável para buscar determinado valor
    NoRB* resultado = buscarRB(raiz, valorParaBuscar);

    if (resultado != NULL) {
        printf("Valor %d encontrado na árvore.\n", valorParaBuscar);
    } else {
        printf("Valor %d não encontrado na árvore.\n", valorParaBuscar);
    }

    return 0;
}
