#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

printg(int **G, int nVert){
	printf("\n");
	int i,j;
	for(i=0;i<nVert;i++){
		for(j=0;j<nVert;j++){
			printf("%d ",G[i][j]);
		}
		printf("\n");
	}
}

int** iniciaGrafo(int *nV){
	int nVert;
	int i,j;
	int nArest;
	int v1,v2;

	printf("Número de vértices do grafo G: ");
	scanf("%d",&nVert);

	int **G = (int**)malloc(nVert*sizeof(int*));
	for(i = 0; i < nVert; i++){
		G[i] = (int*) calloc (nVert,sizeof(int));
	}

	printf("Número de arestas do grafo G: ");
	scanf("%d",&nArest);

	printf("Digite cada uma das arestas no formato \"x y\", sendo x e y vértices adjacentes em G:\n"
		"Obs: Os vértices de G são representados pelos números de 0 até %d\n",nVert-1);

	i = 0;
	while(i < nArest){
		scanf("%d %d", &v1, &v2);
		if(v1 == v2 || v1 >= nVert || v2 >= nVert || v1 < 0 || v2 < 0 || G[v1][v2] == 1){
			printf("Erro, redigite a aresta\n");
			continue;
		}
		G[v1][v2] = 1;
		G[v2][v1] = 1;
		i++;
	}

	*nV = nVert;
	return G;
}

int** BFS(int **C, int *Visitado, int nVert, int *newnVert){
	int i,j;
	for(i = 0; i < nVert; i++)
		if( Visitado[i] == 0 )
			break;

	int filaSize = 0;
	int chamada = 0;
	int *fila = (int*) malloc (nVert*sizeof(int));
	Visitado[i] = 1;

	fila[filaSize++] = i;
	
	while(chamada < filaSize){
		i = fila[chamada++];
		for(j = 0; j < nVert; j++){
			if(i!=j && C[i][j] == 1 && Visitado[j] == 0){
				Visitado[j] = 1;
				fila[filaSize++] = j;
			}
		}
	}
	int **newG = (int**) malloc (filaSize*sizeof(int*));
	for(i = 0; i < filaSize; i++){
		newG[i] = (int*) calloc (filaSize,sizeof(int));
	}

	for(i = 0; i < filaSize; i++){
		for(j = 0; j < filaSize; j++){
			if(C[fila[i]][fila[j]] == 0)
				newG[i][j] = 1;
		}
	}
	*newnVert = filaSize;

	free(fila);

	return newG;

}

int isConnected(int **G, int nVert){

	int *Visitado = (int*) calloc (nVert,sizeof(int));
	int *componente = (int*) calloc (nVert,sizeof(int));
	int compSize = 0;
	int i,j;
	int chamada = 0;
	componente[0] = 0;
	Visitado[compSize++] = 1;

	while(chamada < compSize){
		i = componente[chamada++];
		for(j = 0; j < nVert; j++){
			if(i!=j && G[i][j] == 1 && Visitado[j] == 0){
				Visitado[j] = 1;
				componente[compSize++] = j;
			}
		}
	}

	if(compSize == nVert){
		free(Visitado);
		free(componente);
		return hullNumber(G, nVert);
	}
	else{
		int g2Size = nVert - compSize;

		int **G1 = (int**) malloc (compSize*sizeof(int*));
		for(i = 0; i < compSize; i++)
			G1[i] = (int*) calloc (compSize,sizeof(int*));

		int **G2 = (int**) malloc (g2Size*sizeof(int*));
		for(i = 0; i < g2Size; i++)
			G2[i] = (int*) calloc (g2Size,sizeof(int*));

		for(i = 0; i < compSize; i++){
			for(j = 0; j < compSize; j++){
				G1[i][j] = G[componente[i]][componente[j]];
			}
		}
		int *g2Lista = (int*) calloc (g2Size,sizeof(int*));

		for(i = 0, j = 0; i < nVert; i++){
			if(Visitado[i] == 0)
				g2Lista[j++] = i;
		}
		for(i = 0; i < g2Size; i++){
			for(j = 0; j < g2Size; j++){
				G2[i][j] = G[g2Lista[i]][g2Lista[j]];
			}
			printf("\n");
		}
		return (hullNumber(G1, compSize) + isConnected(G2, g2Size));
	}
}

int hullNumber(int **G, int nVert){
	int *Visitado = (int*) calloc (nVert,sizeof(int));
	int existeK = FALSE;

	int i,j;
	for(i = 0; i < nVert; i++)
		Visitado[i] = 1;

	int **C = (int**)malloc(nVert*sizeof(int*));
	for(i = 0; i < nVert; i++){
		C[i] = (int*) calloc (nVert,sizeof(int));
		for(j= 0; j < nVert; j++)
			if(G[i][j] == 0)
				C[i][j] = 1;
	}
	printg(G,nVert);
	printg(C,nVert);

	for(i = 0; i < nVert; i++){
		for(j = 0; (j < nVert && i < nVert) ; j++){
			if(i != j && C[i][j] == 1){
				Visitado[i] = 0;
				existeK = TRUE;
				i++;
				j = 0;
				continue;
			}
		}
	}

	if(existeK){

		for(i = 0; i < nVert; i++)
			free(G[i]);
		free(G);

		int newnVert;

		G = BFS(C,Visitado,nVert,&newnVert);
		for(i = 0; i < nVert; i++){
			if(Visitado[i] == 0)	// k>2
				return 2;
		}

		free(Visitado);
		for(i = 0; i < nVert; i++)
			free(C[i]);
		free(C);
		return isConnected(G,newnVert);
	}
	else return nVert;
}

int main(){
	int **G;
	int nVert;
	G = iniciaGrafo(&nVert);
	int i,j;
	printf("\n!!%d!!\n",isConnected(G,nVert));

	return 0;
}
	