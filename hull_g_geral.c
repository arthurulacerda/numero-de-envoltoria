#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define TRUE 1
#define FALSE 0

int g[MAX][MAX]; // Grafo G de entrada (Lista de Adjacencia).
int nVert; // Número de vértices do grafo G.
int nAdj[MAX]; // Número de adjacentes de cada vértice do grafo G.

ImprimeG() {
    int i, j;
    printf("\nLista de Adjacência de G: \n\n");
    for (i = 0; i < nVert; i++) {
        printf("%d->\t", i);
        for (j = 0; j < nAdj[i]; j++) {
            printf("%d\t", g[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

Zera() {
    int i, j;
    nVert = 0;
    for (i = 0; i < MAX; i++) {
        nAdj[i] = 0;
        for (j = 0; j < MAX; j++)
            g[i][j] = 0;

    }
}

IniciaG() {
    int i = 0;
    int nArest;
    int v1, v2;

    printf("Número de vértices do grafo G: ");
    scanf("%d", &nVert);

    printf("Número de arestas do grafo G: ");
    scanf("%d", &nArest);

    printf("Digite cada uma das arestas no formato \"x y\", sendo x e y vértices adjacentes em G:\n"
            "Obs: Os vértices de G são representados pelos números de 0 até %d\n", nVert - 1);
    while (i < nArest) {
        scanf("%d %d", &v1, &v2);
        if (v1 == v2 || v1 >= nVert || v2 >= nVert || v1 < 0 || v2 < 0) {
            printf("Erro, redigite a aresta\n");
            continue;
        }

        nAdj[v1]++;
        nAdj[v2]++;

        g[v1][ nAdj[v1] - 1 ] = v2;
        g[v2][ nAdj[v2] - 1 ] = v1;

        i++;
    }
}

int* BFS(int *fecho, int fechosize, int u) {
    int *largura = (int*) malloc(nVert * sizeof (int));
    largura[u] = 0;

    int *visited = (int*) calloc(nVert, sizeof (int));
    visited[u] = 1;

    int *fila = (int*) malloc(nVert * sizeof (int));
    int filasize = 1;
    fila[0] = u;

    int i, j;
    int call;
    int callAdj;

    for (i = 0; i < filasize; i++) {
        call = fila[i];
        for (j = 0; j < nAdj[call]; j++) {
            callAdj = g[call][j];
            if (visited[callAdj] == 0) {
                largura[callAdj] = largura[call] + 1;
                visited[callAdj] = 1;
                fila[filasize++] = callAdj;
            }
        }
    }

    free(fila);
    free(visited);

    return largura;
}

int hullnumber() {
    int *combinacao = (int*) malloc(nVert * sizeof (int));
    int tamanho = 2;
    int i;
    int j;

    /* Realiza todas as combinações de determinado tamanho.
     * Ex.: Combinações de A, B e C de tamanho 2:
     * (A,B)
     * (A,C)
     * (B,C)
     */
    while (tamanho <= nVert) {
        /* Seleciona a primeira combinação
         * (0, 1, ..., tamanho-1)
         */
        for (i = 0; i < tamanho; i++)
            combinacao[i] = i;

        /* Para cada combinação, calcula-se o fecho do conjunto.
         */
        while (combinacao[0] < nVert - tamanho + 1) {
            j = tamanho - 1;
            if (FechoConvexoGeodesico(combinacao, tamanho)) {
                // Caso retorne verdadeiro, imprime o hullnumber e o hullset mínimo.
                printf("hullnumber: %d\nhullset: ", tamanho);

                for (i = 0; i < tamanho; i++)
                    printf("%d ", combinacao[i]);
                printf("\n");
                return;
            }


            while (combinacao[j] + 1 > nVert - tamanho + j && j != 0) {
                j--;
            }

            combinacao[j]++;

            while (j < tamanho) {
                combinacao[j + 1] = combinacao[j] + 1;
                j++;
            }

        }
        tamanho++;
    }
    free(combinacao);
}

int FechoConvexoGeodesico(int* S, int Ssize) {
    int *fecho = (int*) calloc(nVert, sizeof (int));
    int fechosize = Ssize;
    int *isFecho = (int*) calloc(nVert, sizeof (int));
    int i, j, k;

    for (i = 0; i < fechosize; i++) {
        fecho[i] = S[i];
    }

    for (i = 0; i < fechosize; i++) {
        isFecho[ fecho[i] ] = 1;
    }

    int *largura;
    for (i = 0; i < fechosize; i++) {
        int *largura = (int*) malloc(nVert * sizeof (int));
        largura = BFS(fecho, fechosize, fecho[i]);
        int aux;

        for (j = 0; j < fechosize; j++) {
            int call = fecho[j];
            if (largura[call] == 0 || largura[call] == 1)
                continue;

            for (k = 0; k < nAdj[call]; k++) {
                int callAdj = g[call][k];

                if (largura[call] == largura[callAdj] + 1 && isFecho[callAdj] == 0) {
                    isFecho[callAdj] = 1;
                    fecho[fechosize++] = callAdj;
                }
            }
        }
        free(largura);
    }

    
    if (fechosize == nVert)
        return TRUE;

    return FALSE;

    /*
    for(i = 0; i < fechosize; i++){
            printf("%d ",fecho[i]);
    }
     */
}

int main() {
    Zera();
    IniciaG();
    ImprimeG();
    hullnumber();


    return 0;
}