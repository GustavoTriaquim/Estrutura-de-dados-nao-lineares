#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*
 * ALGORITMO DE BELLMAN-FORD (Ford-Moore-Bellman)
 * ------------------------------------------------
 * Descoberto de forma independente por Alfonso Shimbel, Lester Ford Jr.,
 * D. R. Fulkerson e Richard Bellman — por isso também é chamado de
 * Ford-Moore-Bellman.
 *
 * Como funciona:
 * 1) Inicializa a distância da origem como 0 e de todos os outros
 *    vértices como infinito.
 * 2) Repete (V - 1) vezes: para CADA aresta (u, v, peso) do grafo,
 *    se dist[u] + peso < dist[v], atualiza dist[v].
 *    Essa operação é chamada de "relaxamento" da aresta.
 * 3) Faz uma passada extra sobre todas as arestas: se ainda for
 *    possível relaxar alguma, existe um CICLO DE PESO NEGATIVO
 *    alcançável a partir da origem.
 *
 * Diferenças em relação ao Dijkstra:
 * -----------------------------------------------------------------
 * | Aspecto               | Dijkstra                | Bellman-Ford |
 * -----------------------------------------------------------------
 * | Pesos negativos        | Não suporta              | Suporta     |
 * |                        | (resultado incorreto)    |             |
 * | Detecta ciclo negativo | Não                       | Sim         |
 * | Estratégia             | Guloso: sempre expande o | Relaxa TODAS|
 * |                        | vértice de menor         | as arestas, |
 * |                        | distância já conhecido   | repetidas   |
 * |                        |                           | vezes       |
 * | Complexidade           | O(V²) nesta versão simples| O(V * E)   |
 * |                        | ou O((V+E) log V) com     |            |
 * |                        | fila de prioridade        |            |
 * | Quando usar            | Grafos com pesos não      | Grafos que |
 * |                        | negativos, quando         | podem ter  |
 * |                        | performance importa       | pesos      |
 * |                        |                           | negativos, |
 * |                        |                           | ou quando é|
 * |                        |                           | preciso    |
 * |                        |                           | verificar  |
 * |                        |                           | ciclos     |
 * |                        |                           | negativos  |
 * -----------------------------------------------------------------
 */

typedef struct Aresta {
    int origem;
    int destino;
    int peso;
} Aresta;

typedef struct GrafoArestas {
    int numVertices;
    int numArestas;
    Aresta* arestas;
} GrafoArestas;

GrafoArestas* criarGrafoArestas(int V, int E) {
    GrafoArestas* grafo = (GrafoArestas*) malloc(sizeof(GrafoArestas));
    grafo->numVertices = V;
    grafo->numArestas = E;
    grafo->arestas = (Aresta*) malloc(E * sizeof(Aresta));
    return grafo;
}

void bellmanFord(GrafoArestas* grafo, int origem) {
    int V = grafo->numVertices;
    int E = grafo->numArestas;
    int* dist = (int*) malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) dist[i] = INT_MAX;
    dist[origem] = 0;

    /* Passo 2: relaxa todas as arestas V-1 vezes.
       Diferente do Dijkstra (que escolhe gulosamente o vértice não
       visitado de menor distância), aqui percorremos TODAS as
       arestas do grafo a cada iteração, sem nenhuma escolha "gulosa" —
       é isso que permite lidar com pesos negativos corretamente. */
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = grafo->arestas[j].origem;
            int v = grafo->arestas[j].destino;
            int peso = grafo->arestas[j].peso;

            if (dist[u] != INT_MAX && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
            }
        }
    }

    /* Passo 3: verifica ciclo de peso negativo.
       Se após V-1 relaxamentos completos ainda existe uma aresta que
       pode ser relaxada, é porque há um ciclo de peso negativo
       alcançável a partir da origem — o Dijkstra não teria como
       detectar isso, pois nem sequer funciona corretamente com
       pesos negativos. */
    for (int j = 0; j < E; j++) {
        int u = grafo->arestas[j].origem;
        int v = grafo->arestas[j].destino;
        int peso = grafo->arestas[j].peso;

        if (dist[u] != INT_MAX && dist[u] + peso < dist[v]) {
            printf("O grafo contém um ciclo de peso negativo!\n");
            free(dist);
            return;
        }
    }

    printf("\n--- Bellman-Ford a partir do vértice %d ---\n", origem);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX)
            printf("Distância até %d: inalcançável\n", i);
        else
            printf("Distância até %d: %d\n", i, dist[i]);
    }

    free(dist);
}

void liberarGrafoArestas(GrafoArestas* grafo) {
    free(grafo->arestas);
    free(grafo);
}

int main() {
    int V = 5, E = 8;
    GrafoArestas* grafo = criarGrafoArestas(V, E);

    /* Mesmas ligações do grafo da Aula 3, com um peso negativo
       (aresta 2 -> 3) para demonstrar um caso que o Dijkstra não
       conseguiria tratar corretamente, mas o Bellman-Ford sim. */
    grafo->arestas[0] = (Aresta){0, 1, 4};
    grafo->arestas[1] = (Aresta){0, 4, 8};
    grafo->arestas[2] = (Aresta){1, 2, 2};
    grafo->arestas[3] = (Aresta){1, 3, 5};
    grafo->arestas[4] = (Aresta){1, 4, 7};
    grafo->arestas[5] = (Aresta){2, 3, -3};
    grafo->arestas[6] = (Aresta){3, 4, 6};
    grafo->arestas[7] = (Aresta){4, 0, 1};

    bellmanFord(grafo, 0);

    liberarGrafoArestas(grafo);
    return 0;
}
