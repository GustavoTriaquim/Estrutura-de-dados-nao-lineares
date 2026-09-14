#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int destino;
    struct No* proximo;
} No;

typedef struct ListaAdjacencia {
    No* cabeca;
} ListaAdjacencia;

typedef struct Grafo {
    int numVertices;
    ListaAdjacencia* array;
} Grafo;

No* criarNo(int destino) {
    No* novoNo = (No*) malloc(sizeof(No));
    novoNo->destino = destino;
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

void adicionarAresta(Grafo* grafo, int origem, int destino) {
    No* novoNo = criarNo(destino);
    novoNo->proximo = grafo->array[origem].cabeca;
    grafo->array[origem].cabeca = novoNo;
}

void imprimirGrafo(Grafo* grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        No* atual = grafo->array[i].cabeca;
        printf("Vértice %d:", i);
        while (atual != NULL) {
            printf(" -> %d", atual->destino);
            atual = atual->proximo;
        }
        printf("\n");
    }
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

void removerDaLista(ListaAdjacencia* lista, int valor) {
    No* atual = lista->cabeca;
    No* anterior = NULL;

    while (atual != NULL && atual->destino != valor) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        return;
    }
    if (anterior == NULL) {
        lista->cabeca = atual->proximo;
    }
    else {
        anterior->proximo = atual->proximo;
    }
    free(atual);
}

void removerAresta(Grafo* grafo, int origem, int destino) {
    removerDaLista(&(grafo->array[origem]), destino);
}

int main() {
    int V = 5;
    Grafo* grafo = criarGrafo(V);

    adicionarAresta(grafo, 0, 1);
    adicionarAresta(grafo, 0, 4);
    adicionarAresta(grafo, 1, 2);
    adicionarAresta(grafo, 1, 3);
    adicionarAresta(grafo, 1, 4);
    adicionarAresta(grafo, 2, 3);
    adicionarAresta(grafo, 3, 4);

    imprimirGrafo(grafo);

    liberarGrafo(grafo);

    return 0;
}
