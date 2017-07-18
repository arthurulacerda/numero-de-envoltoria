#include <stdio.h>
#define MAX 100
#define TRUE 1;
#define FALSE 0;

typedef int BOOL;

int g[MAX][MAX];	// Grafo G de entrada (Matriz de Adjacencia).
int c[MAX][MAX];    // Matriz complementar.
int nVert;			// Número de vértices do grafo G.
int nAdj[MAX];		// Número de adjacentes de cada vértice do grafo G.

/** Zera todos os elementos, descartando possíveis lixos de memória 
*/
Zera(){
	int i,j;
	nVert = 0;
	for(i = 0; i < MAX; i++){
		nAdj[i] = 0;
		for(j = 0; j < MAX; j++){
			g[i][j] = 0;
			if(i!=j)
				c[i][j] = 1;
			else
				c[i][j] = 0;
		}
	}
}

/** Inicializa os valores de G 
*/
IniciaG(){
	int i = 0;
	int nArest;
	int v1,v2;

	printf("Número de vértices do grafo G: ");
	scanf("%d",&nVert);

	printf("Número de arestas do grafo G: ");
	scanf("%d",&nArest);

	printf("Digite cada uma das arestas no formato \"x y\", sendo x e y vértices adjacentes em G:\n"
		"Obs: Os vértices de G são representados pelos números de 0 até %d\n",nVert-1);
	while(i < nArest){
		scanf("%d %d", &v1, &v2);
		if(v1 == v2 || v1 >= nVert || v2 >= nVert || v1 < 0 || v2 < 0){
			printf("Erro, redigite a aresta\n");
			continue;
		}

		nAdj[v1]++;
		nAdj[v2]++;

		g[v1][v2] = 1;
		g[v2][v1] = 1;

		i++;
	}
}

/** Imprime a matriz de adjacência do grafo g 
*/
ImprimeG(){
	int i,j;
	printf("\nMatriz de Adjacência de G: \n\n");
	for(i = 0; i < nVert; i++)
		printf("\t%d",i);
	printf("\n");
	for(i = 0; i < nVert; i++){
		prinf("%d\t");
		for(j = 0; j < nVert; j++){
			printf("%d\t",g[i][j]);
		}
		printf("\n");
	}
}

int BFS(int **s, int sSize){
	BOOL *visited = (BOOL*) calloc (sSize,sizeof(BOOL));
	int *queue = (int*) malloc (sSize*sizeof(int));
	int queueSize = 1;
	queue[0] = 0;
	visited[0] = TRUE;
	int i,j;
	int u;

	for (i = 0; i < queueSize; i++){
		u = queue[i];
		for(j = 0; j < sSize; j++){
			if(s[u][j] == 1 && !visited[j]){
				queue[queueSize++] = j;
				visited[j] = TRUE;
			}
		}
	}

	if(queueSize != sSize){
		int newSize = sSize - queueSize;
		int **new = (int**) malloc (newSize * sizeof(int*));
		for(i = 0; i < queueSize ; i++)
			new[i] = (int*) malloc (newSize * sizeof(int));

		int *componente = (int*) malloc (newSize * sizeof(int));

		for(i = 0,j=0; i < sSize; i++)
			if(!visited[i])
				componente[j++] = i;

		for(i = 0; i < newSize; i++){
			for(j = 0; j < newSize; j++){
				new[i][j] = s[componente[i]] [componente[j]];
			}
		}

		free(queue);
		free(visited);

		return 1 + BFS(new, newSize);

		for(i = 0; i < queueSize ; i++)
			free(new[i]);
		free(new);
	}

	free(queue);
	free(visited);

	return 1;
}

hullnumber(){
	int i,j;
	int u; //vértice universal
	int qteu = 0; // quantidade de vértices universais
	for(i = 0; i < nVert; i++)
		if(nAdj[i] == nVert - 1){
			qteu++;
			u = i;
		}
	if(qteu > 1)
		return 2;

	if(BFS(c,nVert) != 2)
		return 2;

	int **new = (int**) malloc ((nVert-1) * sizeof(int*));
		for(i = 0; i < nVert ; i++)
			new[i] = (int*) malloc ((nVert-1) * sizeof(int));

	int *componente = (int*) malloc (nVert-1 * sizeof(int));

	for(i = 0,j = 0; i < nVert; i++){
		if(i != u){
			componente[j] = i;
		}
	}

	for(i = 0; i < nVert-1; i++){
		for(j = 0; j < nVert-1; j++){
			new[i][j] = g[componente[i]] [componente[j]];
		}
	}

	return BFS(new, nVert-1);

}

//Calcula quantidade de componentes conexas em C
//	se for 2
//Calcula a quantidade de componentes em G2

int main(){
	Zera();
	IniciaG();
	ImprimeG();
    return 0;
}