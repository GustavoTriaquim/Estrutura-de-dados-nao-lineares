#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct No {
    int destino;
    int peso;
    struct No* proximo;
} No;

typedef struct ListaAdjacencia {
    No* cabeca;
} ListaAdjacencia;

typedef struct Grafo {
    int numVertices;
    ListaAdjacencia* array;
} Grafo;

No* criarNo(int destino, int peso) {
    No* novoNo = (No*) malloc(sizeof(No));
    novoNo->destino = destino;
    novoNo->peso = peso;
    novoNo->proximo = NULL;
    return novoNo;
}

Grafo* criarGrafo(int V) {
    Grafo* grafo = (Grafo*) malloc(sizeof(Grafo));
    grafo->numVertices = V;
    grafo->array = (ListaAdjacencia*) malloc(V * sizeof(ListaAdjacencia));
    for (int i = 0; i < V; i++) {
        grafo->array[i].cabeca = NULL;
    }
    return grafo;
}

void adicionarAresta(Grafo* grafo, int origem, int destino, int peso) {
    No* novoNo = criarNo(destino, peso);
    novoNo->proximo = grafo->array[origem].cabeca;
    grafo->array[origem].cabeca = novoNo;

    novoNo = criarNo(origem, peso);
    novoNo->proximo = grafo->array[destino].cabeca;
    grafo->array[destino].cabeca = novoNo;
}

/* ---------- BFS ---------- */
/* Percorre o grafo em largura e calcula a distância em NÚMERO DE ARESTAS
   (ignora o peso) a partir de um vértice de origem. */
void bfs(Grafo* grafo, int origem) {
    int V = grafo->numVertices;
    int* visitado = (int*) calloc(V, sizeof(int));
    int* distancia = (int*) malloc(V * sizeof(int));
    int* fila = (int*) malloc(V * sizeof(int));
    int inicio = 0, fim = 0;

    for (int i = 0; i < V; i++) distancia[i] = -1;

    visitado[origem] = 1;
    distancia[origem] = 0;
    fila[fim++] = origem;

    printf("\n--- BFS a partir do vértice %d ---\n", origem);
    while (inicio < fim) {
        int atualV = fila[inicio++];
        printf("Visitando %d\n", atualV);

        No* atual = grafo->array[atualV].cabeca;
        while (atual != NULL) {
            if (!visitado[atual->destino]) {
                visitado[atual->destino] = 1;
                distancia[atual->destino] = distancia[atualV] + 1;
                fila[fim++] = atual->destino;
            }
            atual = atual->proximo;
        }
    }

    for (int i = 0; i < V; i++) {
        printf("Distância (em arestas) até %d: %d\n", i, distancia[i]);
    }

    free(visitado);
    free(distancia);
    free(fila);
}

/* ---------- Dijkstra ---------- */
/* Calcula o caminho de menor CUSTO (soma dos pesos) a partir de um vértice.
   Versão O(V^2), sem fila de prioridade, mas fácil de entender. */
int menorDistanciaNaoVisitado(int* dist, int* visitado, int V) {
    int min = INT_MAX, indiceMin = -1;
    for (int v = 0; v < V; v++) {
        if (!visitado[v] && dist[v] <= min) {
            min = dist[v];
            indiceMin = v;
        }
    }
    return indiceMin;
}

void dijkstra(Grafo* grafo, int origem) {
    int V = grafo->numVertices;
    int* dist = (int*) malloc(V * sizeof(int));
    int* visitado = (int*) calloc(V, sizeof(int));

    for (int i = 0; i < V; i++) dist[i] = INT_MAX;
    dist[origem] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = menorDistanciaNaoVisitado(dist, visitado, V);
        if (u == -1) break; /* vértices restantes são inalcançáveis */
        visitado[u] = 1;

        No* atual = grafo->array[u].cabeca;
        while (atual != NULL) {
            int v = atual->destino;
            int peso = atual->peso;
            if (!visitado[v] && dist[u] != INT_MAX && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
            }
            atual = atual->proximo;
        }
    }

    printf("\n--- Dijkstra a partir do vértice %d ---\n", origem);
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX)
            printf("Distância até %d: inalcançável\n", i);
        else
            printf("Distância até %d: %d\n", i, dist[i]);
    }

    free(dist);
    free(visitado);
}

void liberarGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        No* atual = grafo->array[i].cabeca;
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    free(grafo->array);
    free(grafo);
}

int main() {
    int V = 5;
    Grafo* grafo = criarGrafo(V);

    adicionarAresta(grafo, 0, 1, 4);
    adicionarAresta(grafo, 0, 4, 8);
    adicionarAresta(grafo, 1, 2, 2);
    adicionarAresta(grafo, 1, 3, 5);
    adicionarAresta(grafo, 1, 4, 7);
    adicionarAresta(grafo, 2, 3, 3);
    adicionarAresta(grafo, 3, 4, 6);

    bfs(grafo, 0);
    dijkstra(grafo, 0);

    liberarGrafo(grafo);
    return 0;
}
