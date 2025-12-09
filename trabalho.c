#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ======================================================================
 * PROGRAMA: Relações e Propriedades de Fecho
 * 
 * OBJETIVO: Implementar um software capaz de processar um arquivo de
 * entrada que especifica uma relação R sobre o produto cartesiano A×A,
 * e calcular as propriedades reflexiva, simétrica e transitiva, bem como
 * os fechos correspondentes.
 * 
 * REQUISITOS IMPLEMENTADOS:
 * 1. Entrada de Dados (Req. 01)
 * 2. Estrutura de Dados Matriz (Req. 02)
 * 3. Check Propriedade Reflexiva (Req. 03)
 * 4. Check Propriedade Simétrica (Req. 04)
 * 5. Check Propriedade Transitiva (Req. 05)
 * 6. Fecho Propriedade Reflexiva (Req. 06)
 * 7. Fecho Propriedade Simétrica (Req. 07)
 * 8. Fecho Propriedade Transitiva (Req. 08)
 * 9. Saída de Dados (Req. 09)
 * 
 * AUTOR: [Ana Clara Ferreira]
 * DATA: Dezembro 2025
 * ====================================================================== */

/* ======================================================================
 * DEFINIÇÕES E CONSTANTES
 * ====================================================================== */
#define MAX_ELEMENTOS 100

typedef struct {
    int **matriz;           /* Matriz para representar a relação */
    int n;                  /* Número de elementos do conjunto A */
    char **elementos;       /* Nomes dos elementos (para debug) */
} Relacao;

/* ======================================================================
 * REQ. 01 - ENTRADA DE DADOS
 * 
 * DOCUMENTAÇÃO:
 * Este requisito implementa a leitura de um arquivo de entrada que 
 * contém a especificação de uma relação R sobre um conjunto A.
 * 
 * Formato do arquivo:
 * - Primeira linha: 'n' (número de elementos)
 * - Próximas linhas: 'r x y' (onde x,y indicam um par na relação)
 * - Última linha: 'f' (fim do arquivo)
 * 
 * ALGORITMO:
 * 1. Abrir o arquivo de entrada
 * 2. Ler o primeiro caractere de controle
 * 3. Se for 'n', ler o número de elementos
 * 4. Se for 'r', ler os pares (x,y) e registrar a relação
 * 5. Se for 'f', terminar a leitura
 * 6. Retornar a estrutura com a relação preenchida
 * ====================================================================== */
Relacao* lerEntrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Arquivo '%s' não encontrado.\n", nomeArquivo);
        return NULL;
    }

    Relacao *rel = (Relacao *)malloc(sizeof(Relacao));
    rel->n = 0;
    rel->matriz = NULL;

    char controle;
    int x, y;

    /* Leitura do primeiro controle */
    while (fscanf(arquivo, " %c", &controle) == 1) {
        if (controle == 'n') {
            /* Ler o número de elementos do conjunto A */
            fscanf(arquivo, "%d", &rel->n);
            
            /* Alocar e inicializar a matriz com zeros */
            rel->matriz = (int **)malloc(rel->n * sizeof(int *));
            for (int i = 0; i < rel->n; i++) {
                rel->matriz[i] = (int *)calloc(rel->n, sizeof(int));
            }
        }
        else if (controle == 'r') {
            /* Ler um par da relação e marcar na matriz */
            fscanf(arquivo, "%d %d", &x, &y);
            if (x >= 1 && x <= rel->n && y >= 1 && y <= rel->n) {
                rel->matriz[x-1][y-1] = 1;  /* Convertendo para índice 0 */
            }
        }
        else if (controle == 'f') {
            /* Fim da leitura */
            break;
        }
    }

    fclose(arquivo);
    return rel;
}

/* ======================================================================
 * REQ. 02 - ESTRUTURA DE DADOS MATRIZ
 * 
 * DOCUMENTAÇÃO:
 * Este requisito define como os dados da relação são armazenados em
 * memória utilizando uma estrutura matricial booleana.
 * 
 * A matriz é uma representação compacta onde:
 * - matriz[i][j] = 1 indica que o par (i+1, j+1) pertence à relação
 * - matriz[i][j] = 0 indica que o par (i+1, j+1) não pertence à relação
 * - Linhas representam o primeiro elemento do par
 * - Colunas representam o segundo elemento do par
 * 
 * Exemplo: Para a relação R = {(1,1), (3,2), (2,2)}, a matriz seria:
 *   1 0 0
 *   0 1 0
 *   0 1 0
 * 
 * ALGORITMO:
 * 1. Alocar espaço para uma matriz n×n
 * 2. Inicializar todos os elementos com 0 (falso)
 * 3. Para cada par (x,y) da relação, definir matriz[x-1][y-1] = 1
 * 4. A estrutura permite operações rápidas de verificação O(1)
 * ====================================================================== */

void inicializarMatriz(Relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return;
    
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            rel->matriz[i][j] = 0;
        }
    }
}

void imprimirMatriz(Relacao *rel, const char *titulo) {
    if (rel == NULL || rel->matriz == NULL) return;
    
    printf("\n%s\n", titulo);
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            printf("%d ", rel->matriz[i][j]);
        }
        printf("\n");
    }
}

/* ======================================================================
 * REQ. 03 - CHECK PROPRIEDADE REFLEXIVA
 * 
 * DOCUMENTAÇÃO:
 * Uma relação R em um conjunto A é REFLEXIVA se para todo elemento
 * x ∈ A, o par (x, x) pertence a R.
 * 
 * Em termos simples: Todo elemento está relacionado a si mesmo.
 * Exemplo reflexiva: R = {(1,1), (2,2), (3,3)} ✓
 * Exemplo NÃO reflexiva: R = {(1,1), (2,3)} ✗ (falta (2,2) e (3,3))
 * 
 * ALGORITMO:
 * 1. Percorrer a diagonal da matriz (i == j)
 * 2. Se todos os elementos diagonais forem 1, a relação é reflexiva
 * 3. Se algum elemento diagonal for 0, a relação NÃO é reflexiva
 * 4. Complexidade: O(n)
 * ====================================================================== */
int verificarReflexiva(Relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return 0;
    
    /* Verifica se todos os pares (i, i) estão na relação */
    for (int i = 0; i < rel->n; i++) {
        if (rel->matriz[i][i] == 0) {
            return 0;  /* Não reflexiva */
        }
    }
    return 1;  /* Reflexiva */
}

/* ======================================================================
 * REQ. 04 - CHECK PROPRIEDADE SIMÉTRICA
 * 
 * DOCUMENTAÇÃO:
 * Uma relação R em um conjunto A é SIMÉTRICA se para todos os pares
 * (x, y) ∈ R, também temos (y, x) ∈ R.
 * 
 * Em termos simples: Se x está relacionado a y, então y também está
 * relacionado a x.
 * Exemplo simétrica: R = {(1,2), (2,1), (2,3), (3,2)} ✓
 * Exemplo NÃO simétrica: R = {(1,2), (2,3)} ✗ (falta (2,1) e (3,2))
 * 
 * ALGORITMO:
 * 1. Percorrer a matriz triangularmente (i=0 a n, j=i a n)
 * 2. Para cada posição (i,j), verificar se (j,i) tem o mesmo valor
 * 3. Se matriz[i][j] ≠ matriz[j][i] em algum ponto, não é simétrica
 * 4. Se todos os pares opostos forem iguais, é simétrica
 * 5. Complexidade: O(n²)
 * ====================================================================== */
int verificarSimetrica(Relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return 0;
    
    /* Verifica se matriz[i][j] == matriz[j][i] para todos i,j */
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            if (rel->matriz[i][j] != rel->matriz[j][i]) {
                return 0;  /* Não simétrica */
            }
        }
    }
    return 1;  /* Simétrica */
}

/* ======================================================================
 * REQ. 05 - CHECK PROPRIEDADE TRANSITIVA
 * 
 * DOCUMENTAÇÃO:
 * Uma relação R em um conjunto A é TRANSITIVA se para todos os pares
 * (x, y) ∈ R e (y, z) ∈ R, também temos (x, z) ∈ R.
 * 
 * Em termos simples: Se x relacionado a y, e y relacionado a z,
 * então x deve estar relacionado a z.
 * Exemplo: Se (1→2) e (2→3), então deve ter (1→3)
 * 
 * ALGORITMO:
 * 1. Para cada elemento i do conjunto
 * 2. Para cada elemento j do conjunto
 * 3. Se (i,j) ∈ R, então para todo k
 * 4.     Se (j,k) ∈ R, verificar se (i,k) também está em R
 * 5. Se alguma tripla (i,j,k) violar a propriedade, não é transitiva
 * 6. Complexidade: O(n³)
 * ====================================================================== */
int verificarTransitiva(Relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return 0;
    
    /* Verifica se para todo (i,j) e (j,k) em R, (i,k) também está em R */
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            if (rel->matriz[i][j] == 1) {
                for (int k = 0; k < rel->n; k++) {
                    if (rel->matriz[j][k] == 1) {
                        if (rel->matriz[i][k] == 0) {
                            return 0;  /* Não transitiva */
                        }
                    }
                }
            }
        }
    }
    return 1;  /* Transitiva */
}

/* ======================================================================
 * REQ. 06 - FECHO PROPRIEDADE REFLEXIVA
 * 
 * DOCUMENTAÇÃO:
 * O fecho reflexivo de uma relação R é a menor relação que contém R
 * e que possui a propriedade reflexiva.
 * 
 * Significa: Se a relação não é reflexiva, adicionamos os pares (x,x)
 * que faltam para que todo elemento esteja relacionado a si mesmo.
 * 
 * Processo:
 * - Se R já é reflexiva, o fecho é a própria R
 * - Se não é, adicionamos todos os pares da diagonal que faltam
 * 
 * Exemplo: R = {(1,1), (1,2)} → Fecho = {(1,1), (1,2), (2,2)}
 *          Adicionamos (2,2) para garantir reflexividade
 * 
 * ALGORITMO:
 * 1. Copiar a matriz original
 * 2. Para cada elemento i do conjunto
 * 3.     Definir matriz[i][i] = 1
 * 4. Retornar a nova matriz que garante reflexividade
 * 5. Complexidade: O(n)
 * ====================================================================== */
Relacao* calcularFechoReflexivo(Relacao *rel) {
    if (rel == NULL) return NULL;
    
    /* Criar cópia da relação */
    Relacao *fecho = (Relacao *)malloc(sizeof(Relacao));
    fecho->n = rel->n;
    fecho->matriz = (int **)malloc(rel->n * sizeof(int *));
    
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i] = (int *)malloc(rel->n * sizeof(int));
        for (int j = 0; j < rel->n; j++) {
            fecho->matriz[i][j] = rel->matriz[i][j];
        }
    }
    
    /* Adicionar todos os pares reflexivos */
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i][i] = 1;
    }
    
    return fecho;
}

/* ======================================================================
 * REQ. 07 - FECHO PROPRIEDADE SIMÉTRICA
 * 
 * DOCUMENTAÇÃO:
 * O fecho simétrico de uma relação R é a menor relação que contém R
 * e que possui a propriedade simétrica.
 * 
 * Significa: Se (x,y) ∈ R, adicionamos (y,x) para garantir que a
 * relação seja simétrica (se x→y, então y→x).
 * 
 * Processo:
 * - Percorrer todos os pares da relação
 * - Para cada par (x,y) que existe, adicionar seu simétrico (y,x)
 * 
 * Exemplo: R = {(1,2), (2,3)} → Fecho = {(1,2), (2,1), (2,3), (3,2)}
 *          Adicionamos (2,1) simétrico de (1,2)
 *          Adicionamos (3,2) simétrico de (2,3)
 * 
 * ALGORITMO:
 * 1. Copiar a matriz original
 * 2. Para cada posição (i,j) na matriz
 * 3.     Se matriz[i][j] == 1, então definir matriz[j][i] = 1
 * 4. Retornar a nova matriz que garante simetria
 * 5. Complexidade: O(n²)
 * ====================================================================== */
Relacao* calcularFechoSimetrico(Relacao *rel) {
    if (rel == NULL) return NULL;
    
    /* Criar cópia da relação */
    Relacao *fecho = (Relacao *)malloc(sizeof(Relacao));
    fecho->n = rel->n;
    fecho->matriz = (int **)malloc(rel->n * sizeof(int *));
    
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i] = (int *)malloc(rel->n * sizeof(int));
        for (int j = 0; j < rel->n; j++) {
            fecho->matriz[i][j] = rel->matriz[i][j];
        }
    }
    
    /* Adicionar pares simétricos */
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            if (fecho->matriz[i][j] == 1) {
                fecho->matriz[j][i] = 1;
            }
        }
    }
    
    return fecho;
}

/* ======================================================================
 * REQ. 08 - FECHO PROPRIEDADE TRANSITIVA
 * 
 * DOCUMENTAÇÃO:
 * O fecho transitivo de uma relação R é a menor relação que contém R
 * e que possui a propriedade transitiva.
 * 
 * Significa: Se (x,y) ∈ R e (y,z) ∈ R, adicionamos (x,z) para garantir
 * que a relação seja transitiva (se x→y→z, então x→z).
 * 
 * Este é o caminho mais complexo: seguir cadeias de relações.
 * 
 * Processo:
 * - Encontrar todos os "caminhos" na relação
 * - Para cada caminho completo, adicionar a aresta direta
 * 
 * Exemplo: R = {(1,2), (2,3)} → Fecho = {(1,2), (2,3), (1,3)}
 *          Adicionamos (1,3) porque há caminho 1→2→3
 * 
 * ALGORITMO (Método do Fechamento):
 * 1. Copiar a matriz original
 * 2. Repetir até não haver mudanças:
 * 3.     Para cada elemento i, j, k:
 * 4.         Se matriz[i][j] == 1 E matriz[j][k] == 1
 * 5.             Então definir matriz[i][k] = 1
 * 6. Retornar a matriz quando estabilizar
 * 7. Complexidade: O(n⁴) no pior caso, ou O(n³) com Floyd-Warshall
 * ====================================================================== */
Relacao* calcularFechoTransitivo(Relacao *rel) {
    if (rel == NULL) return NULL;
    
    /* Criar cópia da relação */
    Relacao *fecho = (Relacao *)malloc(sizeof(Relacao));
    fecho->n = rel->n;
    fecho->matriz = (int **)malloc(rel->n * sizeof(int *));
    
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i] = (int *)malloc(rel->n * sizeof(int));
        for (int j = 0; j < rel->n; j++) {
            fecho->matriz[i][j] = rel->matriz[i][j];
        }
    }
    
    /* Aplicar fechamento transitivo: enquanto houver mudanças */
    int mudou = 1;
    while (mudou) {
        mudou = 0;
        for (int i = 0; i < rel->n; i++) {
            for (int j = 0; j < rel->n; j++) {
                if (fecho->matriz[i][j] == 1) {
                    for (int k = 0; k < rel->n; k++) {
                        if (fecho->matriz[j][k] == 1 && fecho->matriz[i][k] == 0) {
                            fecho->matriz[i][k] = 1;
                            mudou = 1;
                        }
                    }
                }
            }
        }
    }
    
    return fecho;
}

/* ======================================================================
 * REQ. 09 - SAÍDA DE DADOS
 * 
 * DOCUMENTAÇÃO:
 * Este requisito implementa a escrita dos resultados em arquivo de saída
 * no formato específico (DOT - Graphviz).
 * 
 * Formato DOT:
 * - Descreve um grafo orientado
 * - Nós são elementos do conjunto A
 * - Arestas são pares da relação
 * - Arestas adicionadas pelo fecho são destacadas em vermelho
 * 
 * Processo:
 * - Cabeçalho: "digraph fecho { ... }"
 * - Lista de nós: 1; 2; 3; ...
 * - Arestas originais: 1 -> 2;
 * - Arestas novas (fecho): 1 -> 3 [color=red];
 * - Fechamento do arquivo
 * 
 * Exemplo de saída (fecho reflexivo):
 * 
 * digraph fecho {
 *   1; 2; 3;
 *   1 -> 1; 1 -> 2; [arestas originais]
 *   2 -> 2 [color=red]; [arestas adicionadas]
 * }
 * 
 * ALGORITMO:
 * 1. Abrir arquivo de saída
 * 2. Escrever cabeçalho "digraph fecho {"
 * 3. Listar todos os nós (elementos)
 * 4. Comparar matriz original com a do fecho
 * 5.     Se matriz_original[i][j] == 1: escrever aresta normal
 * 6.     Se matriz_fecho[i][j] == 1 E matriz_original[i][j] == 0:
 * 7.         escrever aresta com [color=red]
 * 8. Fechar com "}"
 * 9. Retornar sucesso ou erro
 * ====================================================================== */
void salvarSaidaDOT(Relacao *original, Relacao *fecho, const char *nomeArquivo) {
    if (original == NULL || fecho == NULL) return;
    
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível criar '%s'.\n", nomeArquivo);
        return;
    }
    
    fprintf(arquivo, "digraph fecho {\n");
    
    /* Listar todos os nós */
    for (int i = 1; i <= original->n; i++) {
        fprintf(arquivo, "  %d;\n", i);
    }
    
    fprintf(arquivo, "\n");
    
    /* Arestas originais (preto/normal) */
    for (int i = 0; i < original->n; i++) {
        for (int j = 0; j < original->n; j++) {
            if (original->matriz[i][j] == 1) {
                fprintf(arquivo, "  %d -> %d;\n", i+1, j+1);
            }
        }
    }
    
    /* Arestas adicionadas pelo fecho (vermelho) */
    for (int i = 0; i < original->n; i++) {
        for (int j = 0; j < original->n; j++) {
            if (fecho->matriz[i][j] == 1 && original->matriz[i][j] == 0) {
                fprintf(arquivo, "  %d -> %d [color=red];\n", i+1, j+1);
            }
        }
    }
    
    fprintf(arquivo, "}\n");
    fclose(arquivo);
}

/* ======================================================================
 * FUNÇÃO AUXILIAR: Liberar memória
 * ====================================================================== */
void liberarRelacao(Relacao *rel) {
    if (rel == NULL) return;
    
    if (rel->matriz != NULL) {
        for (int i = 0; i < rel->n; i++) {
            free(rel->matriz[i]);
        }
        free(rel->matriz);
    }
    free(rel);
}

/* ======================================================================
 * FUNÇÃO PRINCIPAL
 * ====================================================================== */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Erro de Sintaxe\n");
        printf("Usar: ./check-closure.bin <entrada> <saida>\n");
        return 1;
    }
    
    const char *arquivoEntrada = argv[1];
    const char *preambulo = argv[2];
    
    /* REQ. 01: Ler entrada */
    printf("Lendo arquivo de entrada '%s'...\n", arquivoEntrada);
    Relacao *relacao = lerEntrada(arquivoEntrada);
    
    if (relacao == NULL || relacao->matriz == NULL) {
        printf("Erro ao processar entrada.\n");
        return 1;
    }
    
    printf("Conjunto A tem %d elementos.\n\n", relacao->n);
    
    /* Exibir matriz original */
    imprimirMatriz(relacao, "=== MATRIZ ORIGINAL ===");
    
    /* REQ. 03: Verificar propriedade reflexiva */
    printf("\n--- PROPRIEDADE REFLEXIVA ---\n");
    int ehReflexiva = verificarReflexiva(relacao);
    printf("A relação é reflexiva? %s\n", ehReflexiva ? "SIM" : "NÃO");
    
    /* REQ. 04: Verificar propriedade simétrica */
    printf("\n--- PROPRIEDADE SIMÉTRICA ---\n");
    int ehSimetrica = verificarSimetrica(relacao);
    printf("A relação é simétrica? %s\n", ehSimetrica ? "SIM" : "NÃO");
    
    /* REQ. 05: Verificar propriedade transitiva */
    printf("\n--- PROPRIEDADE TRANSITIVA ---\n");
    int ehTransitiva = verificarTransitiva(relacao);
    printf("A relação é transitiva? %s\n", ehTransitiva ? "SIM" : "NÃO");
    
    /* REQ. 06: Calcular fecho reflexivo */
    printf("\n--- FECHO REFLEXIVO ---\n");
    Relacao *fechoReflexivo = calcularFechoReflexivo(relacao);
    imprimirMatriz(fechoReflexivo, "Matriz do Fecho Reflexivo:");
    char nomeArqReflexivo[256];
    snprintf(nomeArqReflexivo, sizeof(nomeArqReflexivo), "%s-ref.dot", preambulo);
    salvarSaidaDOT(relacao, fechoReflexivo, nomeArqReflexivo);
    printf("Saída salva em: %s\n", nomeArqReflexivo);
    
    /* REQ. 07: Calcular fecho simétrico */
    printf("\n--- FECHO SIMÉTRICO ---\n");
    Relacao *fechoSimetrico = calcularFechoSimetrico(relacao);
    imprimirMatriz(fechoSimetrico, "Matriz do Fecho Simétrico:");
    char nomeArqSimetrico[256];
    snprintf(nomeArqSimetrico, sizeof(nomeArqSimetrico), "%s-sim.dot", preambulo);
    salvarSaidaDOT(relacao, fechoSimetrico, nomeArqSimetrico);
    printf("Saída salva em: %s\n", nomeArqSimetrico);
    
    /* REQ. 08: Calcular fecho transitivo */
    printf("\n--- FECHO TRANSITIVO ---\n");
    Relacao *fechoTransitivo = calcularFechoTransitivo(relacao);
    imprimirMatriz(fechoTransitivo, "Matriz do Fecho Transitivo:");
    char nomeArqTransitivo[256];
    snprintf(nomeArqTransitivo, sizeof(nomeArqTransitivo), "%s-tra.dot", preambulo);
    salvarSaidaDOT(relacao, fechoTransitivo, nomeArqTransitivo);
    printf("Saída salva em: %s\n", nomeArqTransitivo);
    
    /* Liberar memória */
    liberarRelacao(relacao);
    liberarRelacao(fechoReflexivo);
    liberarRelacao(fechoSimetrico);
    liberarRelacao(fechoTransitivo);
    
    printf("\n=== PROGRAMA FINALIZADO COM SUCESSO ===\n");
    return 0;
}
