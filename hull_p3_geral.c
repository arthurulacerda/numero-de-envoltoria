#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define TRUE 1
#define FALSE 0

int g[MAX][MAX];	// Grafo G de entrada (Lista de Adjacencia).
int nVert;			// Número de vértices do grafo G.
int nAdj[MAX];		// Número de adjacentes de cada vértice do grafo G.

/** Zera todos os elementos, descartando possíveis lixos de memória 
*/
Zera(){
	int i,j;
	nVert = 0;
	for(i = 0; i < MAX; i++){
		nAdj[i] = 0;
		for(j = 0; j < MAX; j++)
			g[i][j] = 0;
		
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

		g[v1][ nAdj[v1] - 1 ] = v2;
		g[v2][ nAdj[v2] - 1 ] = v1;

		i++;
	}
}

/** Imprime as listas de adjacências do grafo g 
*/
ImprimeG(){
	int i,j;
	printf("\nLista de Adjacência de G: \n\n");
	for(i = 0; i < nVert; i++){
		printf("%d->\t",i);
		for(j = 0; j < nAdj[i]; j++){
			printf("%d\t",g[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

int hullnumber(){
	int *combinacao = (int*) malloc (nVert*sizeof(int));
	int tamanho = 2;
	int i;
	int j;

	/* Realiza todas as combinações de determinado tamanho.
	* Ex.: Combinações de A, B e C de tamanho 2:
	* (A,B)
	* (A,C)
	* (B,C)
	*/
	while(tamanho <= nVert){
		/* Seleciona a primeira combinação
		* (0, 1, ..., tamanho-1)
		*/
		for(i = 0; i < tamanho; i++)
			combinacao[i] = i;

		/* Para cada combinação, calcula-se o fecho do conjunto.
		*/
		while(combinacao[0] < nVert - tamanho + 1){
			j = tamanho - 1;
			if(FechoConvexoP3(combinacao, tamanho)){
				// Caso retorne verdadeiro, imprime o hullnumber e o hullset mínimo.
				printf("hullnumber: %d\nhullset: ",tamanho);
				
				for(i = 0; i < tamanho; i++)
					printf("%d ",combinacao[i]);
				printf("\n");
				return;
			}


			while(combinacao[j] + 1 > nVert - tamanho + j && j!=0){
				j--;
			}

			combinacao[j]++;

			while(j < tamanho){
				combinacao[j+1] = combinacao[j] + 1;
				j++;
			}

		}
		tamanho ++;
	}
	free(combinacao);
}

/** Calculo do fecho convexo dado um conjunto S.
* @param S: Ponteiro que aponta para o vetor que contém os elemento do conjunto S.
* @param Ssize: Quantidade de elementos do conjunto S.
* @return:	TRUE	, se o grafo g foi completamente contaminado.
*			FALSE	, caso contrário.
*/
int FechoConvexoP3(int *S, int Ssize){
	
	// G = S
	if(Ssize == nVert)
		return TRUE;

	int i,j;
	
	/* Vetor que determinará quais vértices estão contaminados baseado na quantidade de vizinhos contaminados.
	*  0 para contaminados.
	*  1 ou mais para não contaminados.
	*/
	int Contaminado[MAX];

	// Vetor de vértices que pertencem ao fecho.
	int Fecho[MAX];

	// Inicializa todos os elementos como não contaminados.
	for(i = 0; i < nVert; i++){
		Contaminado[i] = 2;
	}

	// Insere todos elementos de S no Fecho.
	// Marca os elementos do fecho como contaminados.
	for(i = 0; i < Ssize; i++){
		Fecho[i] = S[i];
		Contaminado[Fecho[i]] = 0;
	}
	int Fechosize = Ssize;

	for(i = 0; i < Fechosize; i++){

		// Seleciona-se um vértice 'a' do fecho
		int a = Fecho[i];

		// Percorre a lista de adjacência deste vértice 
		for(j = 0; j < nAdj[a]; j++){

			// Se o vértice adjacente visitado não estiver contaminado, então
			if(Contaminado[ g[a][j] ]){
				Contaminado[g[a][j]] --; // Decrementa-se o valor no vetor 'Contaminado'.
				
				// Se o valor do vetor 'Contaminado' chegou a zero, então ele é incluido no Fecho.
				if(!Contaminado [ g[a][j] ])
				Fecho[Fechosize++] = g[a][j];
			}
		}
	}

	if(Fechosize == nVert)
		return TRUE;
	return FALSE;
}

int main(){
	Zera();
	IniciaG();
	hullnumber();
}