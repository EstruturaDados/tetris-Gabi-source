#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------------------- CONFIGURAÇÕES --------------------
#define TAMANHO_FILA 5      // Quantidade fixa de peças futuras
#define TAMANHO_PILHA 3     // Capacidade máxima da pilha de reserva

// -------------------- DEFINIÇÃO DA STRUCT --------------------
/*
 * Representa uma peça do jogo Tetris.
 * Atributos:
 *   - nome: tipo da peça ('I', 'O', 'T', 'L')
 *   - id: identificador único crescente
 */
typedef struct {
    char nome;
    int id;
} Peca;

// -------------------- VARIÁVEIS GLOBAIS --------------------
int contadorID = 0;  // Usado para gerar IDs únicos para cada peça criada
char tiposDePecas[] = {'I', 'O', 'T', 'L'}; // Tipos possíveis

// -------------------- DECLARAÇÕES DE FUNÇÕES --------------------
Peca gerarPeca();
void inicializarFila(Peca fila[], int *indiceInicio, int *indiceFim, int *total);
int inserirNaFila(Peca fila[], int *indiceFim, int *total, Peca novaPeca);
int removerDaFila(Peca fila[], int *indiceInicio, int *total, Peca *pecaRemovida);

int empilhar(Peca pilha[], int *topo, Peca novaPeca);
int desempilhar(Peca pilha[], int *topo, Peca *pecaRemovida);

void exibirEstadoAtual(Peca fila[], int indiceInicio, int total, Peca pilha[], int topo);

// ================================================================
//                    FUNÇÃO PRINCIPAL
// ================================================================
int main() {
    srand(time(NULL));

    // Estruturas de armazenamento
    Peca filaDePecas[TAMANHO_FILA];
    Peca pilhaDeReserva[TAMANHO_PILHA];

    // Controle da fila
    int inicioFila = 0;
    int fimFila = 0;
    int totalNaFila = 0;

    // Controle da pilha
    int topoPilha = 0;

    int opcao = 0;

    inicializarFila(filaDePecas, &inicioFila, &fimFila, &totalNaFila);

    do {
        exibirEstadoAtual(filaDePecas, inicioFila, totalNaFila, pilhaDeReserva, topoPilha);

        printf("\n=============== MENU DE AÇÕES ===============\n");
        printf("1 - Jogar peça (remove da fila)\n");
        printf("2 - Reservar peça (fila -> pilha)\n");
        printf("3 - Usar peça reservada (remove pilha)\n");
        printf("0 - Sair do jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("\n");

        if (opcao == 1) {
            // Jogar peça: remove da fila e descarta
            Peca jogada;
            if (removerDaFila(filaDePecas, &inicioFila, &totalNaFila, &jogada)) {
                printf("Você jogou a peça: [%c %d]\n", jogada.nome, jogada.id);
            }
            inserirNaFila(filaDePecas, &fimFila, &totalNaFila, gerarPeca());
        }
        else if (opcao == 2) {
            // Reservar peça: fila -> pilha
            Peca reservada;
            if (removerDaFila(filaDePecas, &inicioFila, &totalNaFila, &reservada)) {
                if (empilhar(pilhaDeReserva, &topoPilha, reservada)) {
                    printf("Peça [%c %d] reservada!\n", reservada.nome, reservada.id);
                } else {
                    printf("⚠ Pilha cheia! Peça perdida...\n");
                }
                inserirNaFila(filaDePecas, &fimFila, &totalNaFila, gerarPeca());
            }
        }
        else if (opcao == 3) {
            // Usar peça da reserva (pilha)
            Peca usada;
            if (desempilhar(pilhaDeReserva, &topoPilha, &usada)) {
                printf("Peça reservada usada: [%c %d]\n", usada.nome, usada.id);
            } else {
                printf("⚠ Não há peças reservadas na pilha!\n");
            }
        }

    } while (opcao != 0);

    printf("Jogo encerrado. Até a próxima!\n");
    return 0;
}

// ================================================================
//                FUNÇÕES DE MANIPULAÇÃO DE PEÇAS
// ================================================================
/*
 * Cria uma nova peça com tipo aleatório e ID único.
 */
Peca gerarPeca() {
    Peca nova;
    nova.nome = tiposDePecas[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

// ================================================================
//                IMPLEMENTAÇÃO DA FILA CIRCULAR
// ================================================================
/*
 * Preenche a fila inicialmente mantendo seu tamanho fixo.
 */
void inicializarFila(Peca fila[], int *indiceInicio, int *indiceFim, int *total) {
    *indiceInicio = 0;
    *indiceFim = 0;
    *total = 0;

    for (int i = 0; i < TAMANHO_FILA; i++) {
        inserirNaFila(fila, indiceFim, total, gerarPeca());
    }
}

/*
 * Inserção na fila circular (enqueue)
 */
int inserirNaFila(Peca fila[], int *indiceFim, int *total, Peca novaPeca) {
    if (*total == TAMANHO_FILA)
        return 0; // fila cheia

    fila[*indiceFim] = novaPeca;
    *indiceFim = (*indiceFim + 1) % TAMANHO_FILA;
    (*total)++;
    return 1;
}

/*
 * Remoção na fila circular (dequeue)
 */
int removerDaFila(Peca fila[], int *indiceInicio, int *total, Peca *pecaRemovida) {
    if (*total == 0)
        return 0; // fila vazia

    *pecaRemovida = fila[*indiceInicio];
    *indiceInicio = (*indiceInicio + 1) % TAMANHO_FILA;
    (*total)--;
    return 1;
}

// ================================================================
//                IMPLEMENTAÇÃO DA PILHA (LIFO)
// ================================================================
/*
 * Empilha peça no topo da reserva.
 */
int empilhar(Peca pilha[], int *topo, Peca novaPeca) {
    if (*topo == TAMANHO_PILHA)
        return 0; // pilha cheia

    pilha[*topo] = novaPeca;
    (*topo)++;
    return 1;
}

/*
 * Desempilha peça do topo.
 */
int desempilhar(Peca pilha[], int *topo, Peca *pecaRemovida) {
    if (*topo == 0)
        return 0; // pilha vazia

    (*topo)--;
    *pecaRemovida = pilha[*topo];
    return 1;
}

// ================================================================
//                     FUNÇÃO DE EXIBIÇÃO
// ================================================================
/*
 * Mostra a fila e a pilha visualmente no terminal.
 */
void exibirEstadoAtual(Peca fila[], int indiceInicio, int total, Peca pilha[], int topo) {
    printf("\n=========== ESTADO ATUAL DO JOGO ===========\n");

    printf("Fila de peças futuras: ");
    int pos = indiceInicio;
    for (int i = 0; i < total; i++) {
        printf("[%c %d] ", fila[pos].nome, fila[pos].id);
        pos = (pos + 1) % TAMANHO_FILA;
    }

    printf("\nPilha de reserva (Topo → Base): ");
    for (int i = topo - 1; i >= 0; i--) {
        printf("[%c %d] ", pilha[i].nome, pilha[i].id);
    }

    printf("\n============================================\n");
}

