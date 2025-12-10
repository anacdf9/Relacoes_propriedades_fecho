/*
 * programa: check-closure.c
 * disciplina: matematica discreta
 * trabalho pratico - Relações, propriedades e fecho
 * autor: Ana Clara Silveira Ferreira
 * matricula: 0117238
 * data: 22/11/2025
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define max_elementos 100

/* estrutura para o req. 02 - estrutura de dados matriz */
typedef struct {
    int **matriz;      // matriz de adjacencia (0 ou 1)
    int n;             // numero de elementos do conjunto
    char **elementos;  // nomes dos elementos
} relacao;

/* req. 01 - entrada de dados */
relacao* ler_entrada(const char *nome_arquivo) {
    // abre o arquivo em modo de leitura
    FILE *arquivo = fopen(nome_arquivo, "r");
    
    // verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("erro: arquivo '%s' não encontrado.\n", nome_arquivo);
        return NULL;
    }

    // aloca memoria para a estrutura da relacao
    relacao *rel = (relacao *)malloc(sizeof(relacao));
    rel->n = 0;
    rel->matriz = NULL;

    char controle;
    int x, y;

    // loop de leitura: le o caractere de controle (n, r, f)
    while (fscanf(arquivo, " %c", &controle) == 1) {
        
        // se for 'n', le o tamanho do conjunto
        if (controle == 'n') {
            fscanf(arquivo, "%d", &rel->n);
            
            // aloca as linhas da matriz
            rel->matriz = (int **)malloc(rel->n * sizeof(int *));
            
            // aloca as colunas e inicializa com 0 (calloc)
            for (int i = 0; i < rel->n; i++) {
                rel->matriz[i] = (int *)calloc(rel->n, sizeof(int));
            }
        }
        // se for 'r', le um par ordenado (relacao)
        else if (controle == 'r') {
            fscanf(arquivo, "%d %d", &x, &y);
            
            // verifica se os indices estao dentro do limite valido (1 ate n)
            if (x >= 1 && x <= rel->n && y >= 1 && y <= rel->n) {
                // marca 1 na matriz (ajustando indice para comecar em 0)
                rel->matriz[x-1][y-1] = 1;
            }
        }
        // se for 'f', indica fim do arquivo
        else if (controle == 'f') {
            break;
        }
    }

    // fecha o arquivo e retorna a estrutura preenchida
    fclose(arquivo);
    return rel;
}

// funcao auxiliar para mostrar a matriz no terminal (debug)
void imprimir_matriz(relacao *rel, const char *titulo) {
    if (rel == NULL || rel->matriz == NULL) return;
    
    printf("\n%s\n", titulo);
    // percorre linhas
    for (int i = 0; i < rel->n; i++) {
        // percorre colunas
        for (int j = 0; j < rel->n; j++) {
            printf("%d ", rel->matriz[i][j]);
        }
        printf("\n");
    }
}

/* req. 03 - check propriedade reflexiva */
int verificar_reflexiva(relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return 0;
    
    // percorre apenas a diagonal principal (i, i)
    for (int i = 0; i < rel->n; i++) {
        // se encontrar algum elemento na diagonal que e 0, falha
        if (rel->matriz[i][i] == 0) {
            return 0; // não é reflexiva
        }
    }
    return 1; // é reflexiva
}

/* req. 04 - check propriedade simétrica */
int verificar_simetrica(relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return 0;
    
    // percorre toda a matriz
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            // verifica se o par (i,j) e diferente do par inverso (j,i)
            if (rel->matriz[i][j] != rel->matriz[j][i]) {
                return 0; // não é simétrica
            }
        }
    }
    return 1; // é simétrica
}

/* req. 05 - check propriedade transitiva */
int verificar_transitiva(relacao *rel) {
    if (rel == NULL || rel->matriz == NULL) return 0;
    
    // percorre triplas de elementos (i, j, k)
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            // se existe relacao de i para j
            if (rel->matriz[i][j] == 1) {
                for (int k = 0; k < rel->n; k++) {
                    // e existe relacao de j para k
                    if (rel->matriz[j][k] == 1) {
                        // deve existir relacao direta de i para k
                        if (rel->matriz[i][k] == 0) {
                            return 0; // falha na transitividade
                        }
                    }
                }
            }
        }
    }
    return 1; // é transitiva
}

/* req. 06 - fecho propriedade reflexiva */
relacao* calcular_fecho_reflexivo(relacao *rel) {
    if (rel == NULL) return NULL;
    
    // cria uma nova estrutura para o fecho
    relacao *fecho = (relacao *)malloc(sizeof(relacao));
    fecho->n = rel->n;
    fecho->matriz = (int **)malloc(rel->n * sizeof(int *));
    
    // copia a matriz original para a nova matriz
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i] = (int *)malloc(rel->n * sizeof(int));
        for (int j = 0; j < rel->n; j++) {
            fecho->matriz[i][j] = rel->matriz[i][j];
        }
    }
    
    // forca a diagonal principal a ser 1 (x, x)
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i][i] = 1;
    }
    
    return fecho;
}

/* req. 07 - fecho propriedade simétrica */
relacao* calcular_fecho_simetrico(relacao *rel) {
    if (rel == NULL) return NULL;
    
    // aloca e copia a estrutura original
    relacao *fecho = (relacao *)malloc(sizeof(relacao));
    fecho->n = rel->n;
    fecho->matriz = (int **)malloc(rel->n * sizeof(int *));
    
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i] = (int *)malloc(rel->n * sizeof(int));
        for (int j = 0; j < rel->n; j++) {
            fecho->matriz[i][j] = rel->matriz[i][j];
        }
    }
    
    // para cada par (i,j) existente, garante que (j,i) tambem exista
    for (int i = 0; i < rel->n; i++) {
        for (int j = 0; j < rel->n; j++) {
            if (fecho->matriz[i][j] == 1) {
                fecho->matriz[j][i] = 1; // adiciona o arco de volta
            }
        }
    }
    
    return fecho;
}

/* req. 08 - fecho propriedade transitiva */
relacao* calcular_fecho_transitivo(relacao *rel) {
    if (rel == NULL) return NULL;
    
    // aloca e copia a estrutura original
    relacao *fecho = (relacao *)malloc(sizeof(relacao));
    fecho->n = rel->n;
    fecho->matriz = (int **)malloc(rel->n * sizeof(int *));
    
    for (int i = 0; i < rel->n; i++) {
        fecho->matriz[i] = (int *)malloc(rel->n * sizeof(int));
        for (int j = 0; j < rel->n; j++) {
            fecho->matriz[i][j] = rel->matriz[i][j];
        }
    }
    
    // repete o processo ate que nenhuma mudanca ocorra
    int mudou = 1;
    while (mudou) {
        mudou = 0;
        // verifica caminhos indiretos (i -> j -> k)
        for (int i = 0; i < rel->n; i++) {
            for (int j = 0; j < rel->n; j++) {
                if (fecho->matriz[i][j] == 1) {
                    for (int k = 0; k < rel->n; k++) {
                        // se tem i->j e j->k, mas nao tem i->k
                        if (fecho->matriz[j][k] == 1 && fecho->matriz[i][k] == 0) {
                            fecho->matriz[i][k] = 1; // cria atalho i->k
                            mudou = 1; // sinaliza que houve alteracao
                        }
                    }
                }
            }
        }
    }
    
    return fecho;
}

/* req. 09 - saída de dados */
void salvar_saida_dot(relacao *original, relacao *fecho, const char *nome_arquivo) {
    if (original == NULL || fecho == NULL) return;
    
    // tenta criar o arquivo de saída
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        printf("erro: nao foi possivel criar '%s'.\n", nome_arquivo);
        return;
    }
    
    // escreve cabecalho do dot
    fprintf(arquivo, "digraph fecho {\n");
    
    // lista todos os nos isolados primeiro
    for (int i = 1; i <= original->n; i++) {
        fprintf(arquivo, "  %d;\n", i);
    }
    
    fprintf(arquivo, "\n");
    
    // escreve as arestas originais (sem cor)
    for (int i = 0; i < original->n; i++) {
        for (int j = 0; j < original->n; j++) {
            if (original->matriz[i][j] == 1) {
                fprintf(arquivo, "  %d -> %d;\n", i+1, j+1);
            }
        }
    }
    
    // escreve apenas as arestas novas do fecho em vermelho
    for (int i = 0; i < original->n; i++) {
        for (int j = 0; j < original->n; j++) {
            // se esta no fecho, mas nao estava no original
            if (fecho->matriz[i][j] == 1 && original->matriz[i][j] == 0) {
                fprintf(arquivo, "  %d -> %d [color=red];\n", i+1, j+1);
            }
        }
    }
    
    // fecha o bloco do grafo e o arquivo
    fprintf(arquivo, "}\n");
    fclose(arquivo);
}

// funcao para liberar a memoria alocada
void liberar_relacao(relacao *rel) {
    if (rel == NULL) return;
    
    if (rel->matriz != NULL) {
        for (int i = 0; i < rel->n; i++) {
            free(rel->matriz[i]);
        }
        free(rel->matriz);
    }
    free(rel);
}

int main(int argc, char *argv[]) {
    // valida se os argumentos foram passados (req. 01)
    if (argc != 3) {
        printf("erro de sintaxe\n");
        printf("usar: ./check-closure.bin <entrada> <saida>\n");
        return 1;
    }
    
    const char *arquivo_entrada = argv[1];
    const char *preambulo = argv[2];
    
    printf("lendo arquivo de entrada '%s'...\n", arquivo_entrada);
    relacao *rel = ler_entrada(arquivo_entrada);
    
    // se falhou na leitura, encerra
    if (rel == NULL || rel->matriz == NULL) {
        printf("erro ao processar entrada.\n");
        return 1;
    }
    
    printf("conjunto a tem %d elementos.\n\n", rel->n);
    
    imprimir_matriz(rel, " matriz original ");
    
    // --- verificacao e calculo reflexiva ---
    printf("\n  propriedade reflexiva \n");
    int e_reflexiva = verificar_reflexiva(rel);
    printf("a relacao e reflexiva? %s\n", e_reflexiva ? "sim" : "nao");

    // se nao for reflexiva, calcula e salva o fecho (req. 06)
    if (!e_reflexiva) {
        printf(" a relacao nao e reflexiva. calculando fecho...\n");
        relacao *fecho_reflexivo = calcular_fecho_reflexivo(rel);
        
        // monta o nome do arquivo de saida com sufixo correto
        char nome_arq_reflexivo[256];
        snprintf(nome_arq_reflexivo, sizeof(nome_arq_reflexivo), "%s-ref.dot", preambulo);
        
        salvar_saida_dot(rel, fecho_reflexivo, nome_arq_reflexivo);
        printf("saida salva em: %s\n", nome_arq_reflexivo);
        
        liberar_relacao(fecho_reflexivo);
    } else {
        printf(" a relacao ja e reflexiva. nenhum arquivo gerado.\n");
    }
    
    // --- verificacao e calculo simetrica ---
    printf("\n  propriedade simetrica \n");
    int e_simetrica = verificar_simetrica(rel);
    printf("a relacao e simetrica? %s\n", e_simetrica ? "sim" : "nao");

    // se nao for simetrica, calcula e salva o fecho (req. 07)
    if (!e_simetrica) {
        printf(" a relacao nao e simetrica. calculando fecho...\n");
        relacao *fecho_simetrico = calcular_fecho_simetrico(rel);
        
        // monta o nome do arquivo de saida com sufixo correto
        char nome_arq_simetrico[256];
        snprintf(nome_arq_simetrico, sizeof(nome_arq_simetrico), "%s-sim.dot", preambulo);
        
        salvar_saida_dot(rel, fecho_simetrico, nome_arq_simetrico);
        printf("saida salva em: %s\n", nome_arq_simetrico);
        
        liberar_relacao(fecho_simetrico);
    } else {
        printf(" a relacao ja e simetrica. nenhum arquivo gerado.\n");
    }
    
    // --- verificacao e calculo transitiva ---
    printf("\n  propriedade transitiva \n");
    int e_transitiva = verificar_transitiva(rel);
    printf("a relacao e transitiva? %s\n", e_transitiva ? "sim" : "nao");

    // se nao for transitiva, calcula e salva o fecho (req. 08)
    if (!e_transitiva) {
        printf(" a relacao nao e transitiva. calculando fecho...\n");
        relacao *fecho_transitivo = calcular_fecho_transitivo(rel);
        
        // monta o nome do arquivo de saida com sufixo correto
        char nome_arq_transitivo[256];
        snprintf(nome_arq_transitivo, sizeof(nome_arq_transitivo), "%s-tra.dot", preambulo);
        
        salvar_saida_dot(rel, fecho_transitivo, nome_arq_transitivo);
        printf("saida salva em: %s\n", nome_arq_transitivo);
        
        liberar_relacao(fecho_transitivo);
    } else {
        printf(" A relação já é transitiva. Nenhum arquivo gerado.\n");
    }
    
    // libera a memoria da relacao original
    liberar_relacao(rel);
    
    printf("\n programa finalizado  \n");
    return 0;
}