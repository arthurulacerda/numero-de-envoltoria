#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define TRUE 1
#define FALSE 0

int g[MAX][MAX];	// Grafo G de entrada (Lista de Adjacencia).
int nVert;			// Número de vértices do grafo G.
int nAdj[MAX];		// Número de adjacentes de cada vértice do grafo G.

int vCorte[MAX][3];	// Linha 0: Vértices de corte
					// Linha 1: Quantidade de blocos em que o vértice de corte está.
					// Linha 2: Quantidade de vezes que já foi usado como vértice de corte.
int nCorte;			// Número de vértices d ecorte.

int blocoVcorte[MAX][MAX];	// Vértices de corte em cada bloco.
int nVcorteBloco[MAX];		// Número de vértice de corte em cada bloco.

int nBlocos;			// Número de blocos.
int nVertBloco[MAX];	// Número de vértices em cada bloco.
int bloco[MAX][MAX];	// Todos Blocos, com seus respectivos vértices.

int PE[MAX];	// 	Tempo de entrada no DFS.
int PS[MAX];	//	Tempo de saída no DFS.
int t;			//	Tempo do DFS.
int pai[MAX];
int back[MAX];	// 		Seja T uma árvore de profundidade para o grafo G.
				// back[v] é o PE do vértice w mais próximo da raiz de T que
				// pode ser alcançado a partir de v usando 0 ou mais arestas
				// de T para baixo e, a seguir, no máximo uma aresta de
				// retorno.

typedef struct Aresta{
	int v1, v2;
} aresta;

typedef struct Pilha{
	aresta e;
	struct Pilha *abaixo;
} pilha;

pilha *p;		// Pilha de arestas.

/** Inicializa com valor zero ou NULL, todos as variáveis que precisam desse
* comportamento ao inicio do programa.
*/
Zera(){
	int i,j;
	p = NULL;
	t = 0;
	nVert = 0;
	nCorte = 0;
	nBlocos = 0;
	for(i = 0; i < MAX; i++){
		nAdj[i] = 0;
		nVertBloco[i] = 0;
		PE[i] = 0;
		PS[i] = 0;
		back[i] = 0;
		pai[i] = 0;
		nVcorteBloco[i] = 0;
		for(j = 0; j < MAX; j++){
			g[i][j] = 0;
			bloco[i][j] = 0;
		}
		for(j = 0; j < 3 ; j++){
			vCorte[i][j] = 0;
		}
	}
}

/** Monta o grafo G, recebendo de entrada seus vértices e suas arestas.
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

/** Imprime a Lista de Adjacência do grafo G.
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

/** Recebe dois inteiros, x e y, e retorna o menor deles.
*/
int min(int x, int y){
	if(x < y)
		return x;
	return y;
}

/** Verifica se os vértices já estão no bloco.
* Adiciona zero, um ou dois vértices ao bloco que está sendo analizado.
*/
addBloco(){
	int i;
	int flag1 = TRUE;
	int flag2 = TRUE;
	for(i = 0; i < nVertBloco[nBlocos]; i++){
		if(bloco[nBlocos][i] == p->e.v1)
			flag1 = FALSE;
		if(bloco[nBlocos][i] == p->e.v2)
			flag2 = FALSE;
		if(flag1 == FALSE && flag2 == FALSE)
			break;
	}

	if(flag1 == TRUE)
		bloco[nBlocos][nVertBloco[nBlocos]++] = p->e.v1;

	if(flag2 == TRUE)
		bloco[nBlocos][nVertBloco[nBlocos]++] = p->e.v2;
}

/** Empilha a aresta para montar o bloco.
*/
Empilha(aresta vw){

	pilha *temp = (pilha*) malloc (sizeof(pilha));
		temp->e = vw;

	if (p == NULL){
		p = temp;
		p->abaixo = NULL;
		return;
	}

	temp->abaixo = p;
	p = temp;
}

/** Desempilha a pilha, encontrando um novo bloco para o grafo.
*/
Desempilha(aresta vw){
	pilha *temp;
	while(1){
		if(p == NULL)
			break;
		addBloco();
		temp = p;
		p = p->abaixo;
		aresta verif = temp->e;
		free(temp);
		if(verif.v1 == vw.v1 && verif.v2 == vw.v2)
			break;
	}
}

/** Faz uma busca em profundidade e encontra os blocos
* e as articulações do grafo.
*/
P(int v){
	int i;
	int j;
	int filhos = 0;

	PE[v] = ++t;
	back[v] = PE[v];

	for(i = 0; i < nAdj[v]; i++){

		int w = g[v][i];
		aresta vw;
		vw.v1 = v;
		vw.v2 = w;

		if( PE[w] == 0){
			
			Empilha(vw);
			filhos++;
			pai[w] = v;
			P(w);
			if(back[w] >= PE[v]) {
				Desempilha(vw);
				nBlocos++;
				int flag = 1;
				for(j = 0; j < nCorte; j++){
					if(vCorte[j][0] == v){
						flag = 0;
						break;
					}
				}
				if(flag == 1 && (v != 0 || filhos >= 2))
					vCorte[nCorte++][0] = v;
			}
			back[v] = min(back[v],back[w]);

		} else if(PS[w] == 0 && w != pai[v]){
			Empilha(vw);
			back[v] = min(back[v],PE[w]);
		}
	}

	PS[v] = ++t;
}

/** Associa os vértices de corte a cada bloco e determina
* a quantidade de blocos que cada articulação está.
*/
AssociaVCorteBlocos(){
	int i;
	int j;
	int k;
	for(i = 0; i < nCorte; i++)
		for(j = 0; j < nBlocos; j++)
			for(k = 0; k < nVertBloco[j]; k++)
				if(vCorte[i][0] == bloco[j][k]){
					blocoVcorte[j][nVcorteBloco[j]++] = vCorte[i][0];
					vCorte[i][1]++;
				}
}

/** Imprime todos os vértices de corte e a quantidade
* de blocos em que está presente.
*/
ImprimeVcorte(){
	int i;
	printf("\n\nvertices de corte\n");
	for(i=0;i<nCorte;i++){
		printf("%d: %d blocos\n",vCorte[i][0],vCorte[i][1]);
	}
	printf("\n\n");
}

/**	Imprime todos os vértices do bloco, e imprime
* separadamente, todos as articulações do bloco.
*/
ImprimeBlocos(){
	int i,j;
	for(i = 0; i < nBlocos; i++){
		printf("Bloco %d: ",i);
		for(j = 0; j < nVertBloco[i]; j++){
			printf("%d ",bloco[i][j]);
		}
		printf("\nvértices de corte no bloco: ");
		for(j = 0; j < nVcorteBloco[i]; j++){
			printf("%d ",blocoVcorte[i][j]);
		}
		printf("\n\n");
	}
}

int S[MAX],Ssize = 0;	// Conjunto Solução.
int B[MAX];	// Mostra os vértices que pertecem a B, TRUE se pertencem, FALSE se não pertencem.

hullnumber(){
	/*se G for 2-conexo retornar S = {x, y} tal que a distância entre
	x e y seja menor ou igual a 2;*/
	if(nCorte == 0){
		S[0] = 0;
		S[1] = g[0][0];
		Ssize = 2;
		return;
	}
	/*senão definir o conjunto S := ∅ e os rótulos
	l(v) := 2, para todo v ∈ V (G).*/
	int *l = (int*) malloc (nVert * sizeof(int));
	int i;
	for(i = 0; i < nVert; i++)
		l[i] = 2;

	/*se não houver mais blocos, então terminar o algoritmo e devolver o conjunto S
	que é um conjunto da envoltória P 3 mínimo de G; senão:
	*/
	for(i = 0; i < nBlocos; i++){
		int u;
		int j,k,m;
		int flag;
		// 1. determinar B e u
		for(j = 0; j < nVert; j++)
			B[j] = FALSE;
		for(j = 0; j < nVertBloco[i]; j++)
			B[bloco[i][j]] = TRUE;
		//B já está determinado, determinar u.
		for(j = 0, flag = FALSE; j < nVcorteBloco[i]; j++){
			u = blocoVcorte[i][j];
			int aux;
			for(k = 0; k < nVert; k++){
				if(vCorte[k][0] == u){
					aux = k;
					break;
				}
			}
			if(k != nVert && vCorte[aux][1] > vCorte[aux][2]+1){
				vCorte[aux][2]++;
				flag = TRUE;
				break;
			}
		}
		int nl0 = 0, nl1 = 0;

		// Conta a quantidade de cada rótulo que o bloco possui.
		for(j = 0; j < nVcorteBloco[i]; j++){
			if( l[ blocoVcorte[i][j] ] == 0)
				nl0++;
			if( l[ blocoVcorte[i][j] ] == 1)
				nl1++;
		}

		if(flag){
			//Se existir u.
			//Se existir vértice com rótulo 0
			if(nl0 > 0){
				int uNeighbor = FALSE;	//verifica se u é vizinho de um vértice com l[v]=0
				int uDistance2 = FALSE; //verifica se u está a distância 2 de um vértice com l[v] = 0
				if(nl0 > 1 || nl1 > 0){	//procurar algum vértice com distância menor ou igual a dois com rótulo 0, ou com distância 1 com rótulo 1.
					for(j = 0; j < nVcorteBloco[i]; j++){
						int v = blocoVcorte[i][j];
						if(l[v] == 0){
							for(k = 0; k < nAdj[v]; k++){
								int w = g[v][k];
								if(B[w] == TRUE){
									if(w == u)
										uNeighbor = TRUE;

									if(l[w] == 0 || l[w] == 1){
										// coloca rótulo de u como zero e vai para o próximo bloco.
										l[u] = 0;
										goto continueOuterLoop;
									}

									for(m = 0; m < nAdj[w]; m++){
										if(g[w][m] == u && v != u)
											uDistance2 = TRUE;

										if(g[w][m] != v && B[ g[w][m] ] == TRUE && l[g[w][m]] == 0){
											// coloca rótulo de u como zero e vai para o próximo bloco.
											l[u] = 0;
											goto continueOuterLoop;
										}
									}
								}
							}
						}
					}
				}
				if(uNeighbor == TRUE){
					// coloca o rótulo de u como 1 e vai para o próximo bloco.
					l[u] = 1;
					goto continueOuterLoop;
				}
				if(uDistance2 == TRUE){
					// mantém o rótulo de u e vai para o próximo bloco.
					goto continueOuterLoop;
				}
			}
			//Se existir vértice com rótulo 1
			if(nl1>0){
				int uNeighbor = FALSE;	//verifica se u é vizinho de um vértice com l[v]=0
					for(j = 0; j < nVcorteBloco[i]; j++){
						int v = blocoVcorte[i][j];
						if(l[v] == 1){
							for(k = 0; k < nAdj[v]; k++){
								int w = g[v][k];
								if(B[w] == TRUE && w == u){
									if(w == u){
										// mantém o rótulo de u e vai para o próximo bloco.
										goto continueOuterLoop;
									}
								}
							}
						}
					}
			}
			
			//Marcar um vértice adjacente a 0 ou a 1 se existir.
			if(nl0 > 0 || nl1 > 0){
				for(j = 0; j < nVcorteBloco[i]; j++){
					int v = blocoVcorte[i][j];
					if(l[v] == 1 || l[v] == 0){
						//Adicionar vizinho a S, colocar rótulo de u como zero e ir para próximo bloco.
						for(k = 0; k < nAdj[v]; k++){
							int w = g[v][k];
							if(B[w] == TRUE && w != u){
								S[Ssize++] = w;
								l[u] = 0;
								goto continueOuterLoop;
							}
						}
					}
				}
			}

			//Se existir apenas vértices com rótulo 2, marcar um vértice adjacente a u, colocar o rótulo de u como 1 e ir para o próximo bloco.
			for(j = 0; j < nAdj[u]; j++){
				int v = g[u][j];
				if(B[v] == TRUE){
					S[Ssize++] = v;
					l[u] = 1;
					goto continueOuterLoop;
				}
			}

		}
		else {
			//Bloco final, não possui u (vértice de corte).
			if(nl0 > 0){
				if(nl0 > 1 || nl1 > 0){	//procurar algum vértice com distância menor ou igual a dois com rótulo 0, ou com distância 1 com rótulo 1.
					for(j = 0; j < nVcorteBloco[i]; j++){
						int v = blocoVcorte[i][j];
						if(l[v] == 0){
							for(k = 0; k < nAdj[v]; k++){
								int w = g[v][k];
								if(B[w] == TRUE){
									if(l[w] == 0 || l[w] == 1){
										// não adiciona nada a S, termina o algorítmo.
										return;
									}
									for(m = 0; m < nAdj[w]; m++){
										if(g[w][m] != v && B[ g[w][m] ] == TRUE && l[g[w][m]] == 0){
											// não adiciona nada a S, termina o algorítmo.
											return;
										}
									}
								}
							}
						}
					}
				}
			}

			if(nl0 > 0 || nl1 > 0){	// caso exista algum vértice de rótulo 0 ou de rótulo 1.
				for(j = 0; j < nVcorteBloco[i]; j++){
					int v = blocoVcorte[i][j];
					if(l[v] == 1 || l[v] == 0){
						//Adicionar vizinho de v a S, termina o algorítmo.
						for(k = 0; k < nAdj[v]; k++){
							int w = g[v][k];
							if(B[w] == TRUE){
								S[Ssize++] = w;
								return;
							}
						}
					}
				}
			}

			for(j = 0; j < nAdj[u]; j++){ // Caso existam apenas vértices de rótulo 2.
				int v = g[u][j];
				// Adicionar dois vértices no bloco a S, termina o algorítmo.
				if(B[v] == TRUE){
					S[Ssize++] = v;
					S[Ssize++] = u;
					return;
				}
			}
		}
		continueOuterLoop:
		;

	}

	free(l);
}

printHull(){
	int i;
	printf("\n\nHull Number: ");
	for(i = 0; i < Ssize; i++){
		printf("%d ",S[i]);
	}
	printf("\n\n");
}

int main(){
	Zera();
	IniciaG();
	ImprimeG();
	P(0);
	AssociaVCorteBlocos();
	ImprimeVcorte();
	ImprimeBlocos();
	hullnumber();
	printHull();
}