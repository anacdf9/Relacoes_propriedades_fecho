# Relações e Propriedades de Fecho

Programa em C que processa relações binárias, verifica propriedades (reflexiva, simétrica, transitiva) e calcula os fechos correspondentes.

## Compilação

```bash
gcc -lm trabalho.c -o check-closure.bin
```

## Uso

```bash
./check-closure.bin <arquivo_entrada> <prefixo_saida>
```

**Exemplo:**
```bash
./check-closure.bin entrada.txt resultado
```

Gera três arquivos DOT: `resultado-ref.dot`, `resultado-sim.dot`, `resultado-tra.dot`

## Formato do Arquivo de Entrada

```
n <número>     # Número de elementos do conjunto A
r <x> <y>      # Aresta de x para y
r <x> <y>      # Adicione quantas arestas precisar
f              # Fim do arquivo
```

### Exemplo (entrada.txt)
```
n 3
r 1 2
r 2 3
r 3 1
f
```

## O Programa Faz

| Requisito | Verifica | Calcula |
|-----------|----------|---------|
| **Reflexiva** | Todo elemento relacionado a si mesmo? | Adiciona pares (x,x) que faltam |
| **Simétrica** | Se x→y existe, y→x também existe? | Adiciona arestas reversas |
| **Transitiva** | Se x→y e y→z existem, x→z existe? | Adiciona atalhos para caminhos |

## Explicação dos Algoritmos

### Requisito 01 - Entrada de Dados
Lê um arquivo especificando uma relação binária:
- Identifica o número de elementos `n`
- Para cada linha `r x y`, registra o par (x,y) na matriz
- Termina ao encontrar `f`

### Requisito 02 - Estrutura de Dados Matriz
Armazena a relação em uma matriz n×n onde:
- `matriz[i][j] = 1` significa o par (i+1, j+1) pertence à relação
- `matriz[i][j] = 0` significa o par (i+1, j+1) não pertence à relação
- Permite verificação O(1) de pertencimento

### Requisito 03 - Check Reflexiva
Verifica se a relação é reflexiva: ∀x ∈ A, (x,x) ∈ R

**Algoritmo:**
```
Para cada elemento i:
    Se matriz[i][i] == 0:
        Retornar "NÃO reflexiva"
Retornar "Reflexiva"
```

**Exemplo:**
```
R = {(1,1), (2,2), (3,3)} → REFLEXIVA ✓
R = {(1,1), (2,3)} → NÃO REFLEXIVA ✗ (faltam (2,2) e (3,3))
```

### Requisito 04 - Check Simétrica
Verifica se a relação é simétrica: (x,y) ∈ R ⟹ (y,x) ∈ R

**Algoritmo:**
```
Para cada par (i,j):
    Se matriz[i][j] ≠ matriz[j][i]:
        Retornar "NÃO simétrica"
Retornar "Simétrica"
```

**Exemplo:**
```
R = {(1,2), (2,1), (2,3), (3,2)} → SIMÉTRICA ✓
R = {(1,2), (2,3)} → NÃO SIMÉTRICA ✗ (faltam (2,1) e (3,2))
```

### Requisito 05 - Check Transitiva
Verifica se a relação é transitiva: (x,y) ∈ R ∧ (y,z) ∈ R ⟹ (x,z) ∈ R

**Algoritmo:**
```
Para cada tripla (i,j,k):
    Se (i,j) ∈ R E (j,k) ∈ R:
        Se (i,k) ∉ R:
            Retornar "NÃO transitiva"
Retornar "Transitiva"
```

**Exemplo:**
```
R = {(1,2), (2,3), (1,3)} → TRANSITIVA ✓
R = {(1,2), (2,3)} → NÃO TRANSITIVA ✗ (falta (1,3))
```

### Requisito 06 - Fecho Reflexivo
Calcula a menor relação que contém R e é reflexiva.

**Algoritmo:**
```
fecho = Copiar(R)
Para cada elemento i:
    fecho[i][i] = 1
Retornar fecho
```

**Exemplo:**
```
R = {(1,1), (1,2)} → Fecho = {(1,1), (1,2), (2,2)}
                      Adicionado: (2,2)
```

### Requisito 07 - Fecho Simétrico
Calcula a menor relação que contém R e é simétrica.

**Algoritmo:**
```
fecho = Copiar(R)
Para cada posição (i,j):
    Se fecho[i][j] == 1:
        fecho[j][i] = 1
Retornar fecho
```

**Exemplo:**
```
R = {(1,2), (2,3)} → Fecho = {(1,2), (2,1), (2,3), (3,2)}
                      Adicionados: (2,1), (3,2)
```

### Requisito 08 - Fecho Transitivo
Calcula a menor relação que contém R e é transitiva.

**Algoritmo:**
```
fecho = Copiar(R)
Repetir enquanto houver mudanças:
    Para cada tripla (i,j,k):
        Se fecho[i][j] == 1 E fecho[j][k] == 1:
            Se fecho[i][k] == 0:
                fecho[i][k] = 1
Retornar fecho
```

**Exemplo:**
```
R = {(1,2), (2,3)}

Iteração 1:
- (1,2) e (2,3) existem → adiciona (1,3)

Fecho = {(1,2), (2,3), (1,3)} → TRANSITIVA ✓
         Adicionado: (1,3)
```

### Requisito 09 - Saída de Dados
Salva os resultados em formato DOT (Graphviz).

**Formato:**
```
digraph fecho {
    1;
    2;
    3;
    
    1 -> 2;           # Aresta original
    2 -> 3;           # Aresta original
    1 -> 3 [color=red];  # Aresta adicionada
}
```

**Visualizar:**
```bash
# Instalar graphviz (se necessário)
sudo apt-get install graphviz

# Converter para PNG
dot -Tpng resultado-ref.dot -o resultado-ref.png

# Visualizar
xdg-open resultado-ref.png
```

## Exemplo Prático

### 1. Criar arquivo entrada.txt
```
n 3
r 1 2
r 2 3
f
```

### 2. Compilar
```bash
gcc -lm trabalho.c -o check-closure.bin
```

### 3. Executar
```bash
./check-closure.bin entrada.txt saida
```

### 4. Saída do terminal
```
Lendo arquivo de entrada 'entrada.txt'...
Conjunto A tem 3 elementos.

=== MATRIZ ORIGINAL ===
0 1 0 
0 0 1 
0 0 0 

--- PROPRIEDADE REFLEXIVA ---
A relação é reflexiva? NÃO

--- PROPRIEDADE SIMÉTRICA ---
A relação é simétrica? NÃO

--- PROPRIEDADE TRANSITIVA ---
A relação é transitiva? NÃO

--- FECHO REFLEXIVO ---
Matriz do Fecho Reflexivo:
1 1 0 
0 1 1 
0 0 1 
Saída salva em: saida-ref.dot

--- FECHO SIMÉTRICO ---
Matriz do Fecho Simétrico:
0 1 0 
1 0 1 
0 1 0 
Saída salva em: saida-sim.dot

--- FECHO TRANSITIVO ---
Matriz do Fecho Transitivo:
1 1 1 
0 1 1 
0 0 1 
Saída salva em: saida-tra.dot

=== PROGRAMA FINALIZADO COM SUCESSO ===
```

### 5. Ver arquivos DOT gerados
```bash
cat saida-ref.dot
```

## Requisitos

- **C99** (gcc ou compatível)
- **Linux/Unix** ou similar
- **Graphviz** (opcional, apenas para visualizar os grafos)

## Estrutura do Código

- **Funções principales:**
  - `lerEntrada()` - Req. 01
  - `inicializarMatriz()` - Req. 02
  - `verificarReflexiva()` - Req. 03
  - `verificarSimetrica()` - Req. 04
  - `verificarTransitiva()` - Req. 05
  - `calcularFechoReflexivo()` - Req. 06
  - `calcularFechoSimetrico()` - Req. 07
  - `calcularFechoTransitivo()` - Req. 08
  - `salvarSaidaDOT()` - Req. 09
  - `main()` - Orquestra execução

## Notas Importantes

- Os elementos do conjunto A começam em 1 (conforme especificação)
- A matriz usa índices 0-based internamente (conversão automática)
- Arestas adicionadas pelos fechos aparecem em vermelho nos arquivos DOT
- Toda memória alocada é liberada ao final da execução
- O programa trata erros de arquivo e validação de entrada

## Características

✅ **Código comentado** - Fácil de entender e modificar  
✅ **Sem dependências** - Apenas stdlib, stdio, string  
✅ **Dinâmico** - Funciona com qualquer tamanho de conjunto  
✅ **Robusto** - Validação e tratamento de erros  
✅ **Documentado** - README explica cada algoritmo  
