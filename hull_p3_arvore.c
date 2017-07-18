#include <stdio.h>
#define MAX 50
#define TRUE 1
#define FALSE 0

typedef struct Vertice{
	int nAdj;	// Número de vértices adjacentes.
	int rotulo;	// Rótulo do vértice.
	char pertence;	// Verificador se o vértice pertence ou não a G.
} vertice;

int G[MAX][MAX], nVert;	// Matriz de adjacência do grafo G e seu número de vértices respectivamente.
vertice iG[MAX];		// Informações dos vértices do grafo G.

int S[MAX], Ssize;		// Conjunto Solução S.
int St[MAX], Stsize;	// Conjunto auxiliar para solução S'.

/**
* Define o valor inicial das variáveis globais
* para que o algorítmo se inicie corretamente.
*/
Zera(){
	int i,j;
	Ssize = 0;
	Stsize = 0;
	for(i = 0; i < MAX; i++){
		iG[i].nAdj = 0;
		iG[i].rotulo = 0;
		iG[i].pertence = TRUE;
		S[i] = 0;
		St[i] = 0;
		for(j = 0; j < MAX; j++){
			G[i][j] = 0;
		}
	}
}

/**
* Define os vértices e arestas de G, colocando-os em
* uma matriz de adjacência, tal como também identificando
* o número de adjacentes que cada vértice possui.
*/
IniciaG(){
	int i;
	int nArest;
	int v1,v2;

	printf("Número de vértices do grafo G: ");
	scanf("%d",&nVert);

	i = 0;
	printf("Número de arestas do grafo G: ");
	scanf("%d",&nArest);

	printf("Digite cada uma das arestas no formato \"x y\", sendo x e y vértices adjacentes em G:\n"
		"Obs: Os vértices de G são representados pelos números de 0 até %d\n",nVert-1);
	while(i < nArest){
		scanf("%d %d", &v1, &v2);
		if(v1 == v2 || v1 >= nVert || v2 >= nVert || v1 < 0 || v2 < 0 || G[v1][v2] != 0){
			printf("Erro, redigite a aresta\n");
			continue;
		}

		G[v1][v2] ++;
		G[v2][v1] ++;
		iG[v1].nAdj ++;
		iG[v2].nAdj ++;

		i++;
	}
}

/**
* Imprime os vértices que estão presentes em G, seu número de vizinhança
* e quais são seus adjacentes.
*/
printG(){
	int i,j;
	for(i = 0; i < nVert; i++){
		if(iG[i].pertence){
			printf("Vértice %d:"
				"\nVizinhos: %d"
				"\nAdjacentes -> ",i,iG[i].nAdj);
			for(j = 0; j < nVert; j++){
				if(G[i][j] == 1)
					printf("%d ",j);
			}
			printf("\n\n");
		}
	}
}

/**
* Remove um vértice do grafo G, colocando seu estado de permanencia no
* grafo como falso, eliminando todas arestas e diminuindo o número de 
* vizinhança dos vértices que eram adjacentes a ele.
* 	@v: vértice a ser removido de G. 
*/
removeG(int v){
	int i;
	iG[v].pertence = FALSE;
	for(i = 0; i < nVert; i++){
		if(G[v][i] == 1){
			G[v][i] = 0;
			G[i][v] = 0;
			iG[i].nAdj --;
		}
	}
}

/**
* Calcula o menor hull set do grafo G e o armazena no conjunto de
* solução S.
*/
int hull(){
	int sizeG = nVert; // Controla o tamanho do grafo G, conforme os vértices vão sendo removidos.
	int Sti;	// índice do conjunto S'.
	int i,j;	// índices auxiliares.

	// Se V(G) = vazio, o algorítmo se finaliza e a solução estará armazenada no conjunto S.
	while(sizeG > 0){
		
		//Iterativamente, enquanto F possuir alguma folha própria v com rótulo 1, remover v de F;
		for(i = 0; i < nVert; i++)
			if(iG[i].pertence && iG[i].nAdj == 1 && iG[i].rotulo == 1){
				removeG(i);
				sizeG--;
				i = 0;
			}

		// Iterativamente, enquanto F possuir vértice isolado v com (v) ≥ 2, remover v de F;
		for(i = 0; i < nVert; i++)
			if(iG[i].pertence && iG[i].nAdj == 0 && iG[i].rotulo >= 2){
				removeG(i);
				sizeG--;
				i = 0;
			}

		// Escolher um vértice apropriado v ∈ V (F ), tal que v seja isolado ou v seja uma folha com rótulo 0;
		for(i = 0; i < nVert; i++){
			if(iG[i].pertence && (iG[i].nAdj == 0 || (iG[i].nAdj == 1 && iG[i].rotulo == 0)))
				break;
		}

		if(iG[i].pertence){
			// Incluir v em S e em S';
			S[Ssize++] = i;
			St[Stsize++] = i;
			Sti = 0;
			// Iterativamente efetuar as seguintes operações, enquanto S' != vazio:
			while(Sti < Stsize){
				// selecionar v' pertencente a S';
				int vt = St[Sti];
				int w;
				/*para todo w pertencente a NG(v) incrementar rotulo(w) de 1 e se rotulo(w) atingir o valor
				2 então incluir w em S'.*/
				for(w = 0; w < nVert; w++){
					if(G[vt][w] == 1)
						if(++ iG[w].rotulo == 2)
							St[Stsize ++] = w;
				}
				// remover v de S'e de F;
				removeG(vt);
				Sti++;
				sizeG--;
			}
			Stsize = 0;
		}
	}
}

/**
* Imprime o conjunto S, que é o menor conjunto que 
* seu fecho tem como resultado o grafo G completo.
*/
printS(){
	int i;
	printf("\n\nS = ");
	for(i = 0; i < Ssize; i++){
		printf("%d ",S[i]);
	}
	printf("\n\n");
}

int main(){
	Zera();
	IniciaG();
	hull();
	printS();
}