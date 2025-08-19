#include <stdio.h>

// --- Definicoes do Tabuleiro e Habilidades ---
#define TAMANHO_TABULEIRO 10
#define TAMANHO_HABILIDADE 5
#define TAMANHO_NAVIO 3

// --- Funcoes Auxiliares ---

// Funcao para inicializar o tabuleiro com zeros
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = 0; // 0 = agua
        }
    }
}

// Funcao para exibir o tabuleiro com os diferentes elementos
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("--- Tabuleiro de Batalha Naval (Mestre) ---\n");
    printf("   ");
    for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%d ", j);
    }
    printf("\n");
    printf("   -------------------\n");

    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == 0) {
                printf("~ "); // ~ representa agua
            } else if (tabuleiro[i][j] == 3) {
                printf("N "); // N representa navio
            } else if (tabuleiro[i][j] == 5) {
                printf("# "); // # representa area de efeito
            }
        }
        printf("|\n");
    }
    printf("   -------------------\n");
}

// Funcao para validar e posicionar um navio no tabuleiro
// (Mantida do nivel anterior para o exemplo)
int validarPosicao(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int orientacao) {
    // orientacao 0 = horizontal, 1 = vertical
    if (orientacao == 0) {
        if (coluna + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
    } else {
        if (linha + TAMANHO_NAVIO > TAMANHO_TABULEIRO) return 0;
    }

    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        if (orientacao == 0) {
            if (tabuleiro[linha][coluna + i] != 0) return 0;
        } else {
            if (tabuleiro[linha + i][coluna] != 0) return 0;
        }
    }
    return 1;
}

void posicionarNavio(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], int linha, int coluna, int orientacao) {
    for (int i = 0; i < TAMANHO_NAVIO; i++) {
        if (orientacao == 0) {
            tabuleiro[linha][coluna + i] = 3;
        } else {
            tabuleiro[linha + i][coluna] = 3;
        }
    }
}

// --- Funcoes para as Habilidades Especiais ---

// Funcao para criar a matriz da habilidade Cone
void criarMatrizCone(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Logica para criar a forma de cone:
    // O loop externo percorre as linhas (topo para baixo).
    // O loop interno define a largura do cone que aumenta a cada linha.
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // A largura do cone e 'i * 2 + 1', centrada.
            int centro_j = TAMANHO_HABILIDADE / 2;
            if (j >= centro_j - i && j <= centro_j + i) {
                matriz_habilidade[i][j] = 1;
            } else {
                matriz_habilidade[i][j] = 0;
            }
        }
    }
}

// Funcao para criar a matriz da habilidade Cruz
void criarMatrizCruz(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Logica para criar a forma de cruz:
    // O centro e o ponto de origem.
    // O loop aninhado marca a linha e a coluna central com '1'.
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (i == centro || j == centro) {
                matriz_habilidade[i][j] = 1;
            } else {
                matriz_habilidade[i][j] = 0;
            }
        }
    }
}

// Funcao para criar a matriz da habilidade Octaedro (Losango)
void criarMatrizOctaedro(int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    // Logica para criar a forma de losango (vista frontal de um octaedro):
    // A distancia de Manhattan de cada celula ao centro deve ser <= a uma constante.
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            int distancia = abs(i - centro) + abs(j - centro);
            if (distancia <= centro) {
                matriz_habilidade[i][j] = 1;
            } else {
                matriz_habilidade[i][j] = 0;
            }
        }
    }
}

// Funcao para sobrepor a habilidade no tabuleiro principal
void sobreporHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                        int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE], 
                        int linha_origem, int coluna_origem) {
    // Calcula o deslocamento para centrar a matriz de habilidade no ponto de origem
    int offset = TAMANHO_HABILIDADE / 2;
    
    // Loop para percorrer a matriz de habilidade
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Verifica se a posicao da habilidade esta fora dos limites do tabuleiro
            int tabuleiro_i = linha_origem + i - offset;
            int tabuleiro_j = coluna_origem + j - offset;

            if (tabuleiro_i >= 0 && tabuleiro_i < TAMANHO_TABULEIRO &&
                tabuleiro_j >= 0 && tabuleiro_j < TAMANHO_TABULEIRO) {
                
                // Se a celula da habilidade for '1', marca a celula do tabuleiro como '5'
                if (matriz_habilidade[i][j] == 1 && tabuleiro[tabuleiro_i][tabuleiro_j] != 3) {
                    tabuleiro[tabuleiro_i][tabuleiro_j] = 5;
                }
            }
        }
    }
}

// --- Funcao Principal ---
int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    inicializarTabuleiro(tabuleiro);

    // Posicionamento de Navios (exemplo)
    posicionarNavio(tabuleiro, 1, 1, 0); // Horizontal
    posicionarNavio(tabuleiro, 6, 7, 1); // Vertical

    // --- Matrizes de Habilidade ---
    int cone[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int cruz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int octaedro[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];

    // Cria as matrizes de habilidade usando loops e condicionais
    criarMatrizCone(cone);
    criarMatrizCruz(cruz);
    criarMatrizOctaedro(octaedro);

    // --- Aplicacao das Habilidades no Tabuleiro ---
    printf("\nPosicionando Habilidades no Tabuleiro...\n");
    
    // Habilidade de Cone
    int cone_linha_origem = 2, cone_coluna_origem = 5;
    sobreporHabilidade(tabuleiro, cone, cone_linha_origem, cone_coluna_origem);
    
    // Habilidade de Cruz
    int cruz_linha_origem = 8, cruz_coluna_origem = 2;
    sobreporHabilidade(tabuleiro, cruz, cruz_linha_origem, cruz_coluna_origem);

    // Habilidade de Octaedro
    int octaedro_linha_origem = 5, octaedro_coluna_origem = 5;
    sobreporHabilidade(tabuleiro, octaedro, octaedro_linha_origem, octaedro_coluna_origem);

    // Exibicao do tabuleiro com navios e habilidades
    exibirTabuleiro(tabuleiro);

    return 0;
}