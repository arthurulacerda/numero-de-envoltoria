#include <stdio.h>
#define MAX 100
#define TRUE 1;
#define FALSE 0;

typedef int BOOL;

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

/** Calcula hullnumber de um grafo split.
* @return: hullnumber do grafo g.
*/
int hullnumber(){
	BOOL existd1 = FALSE; // Verifica a existência de algum vértice de grau 1.

	int d1[MAX]; // Armazena os vértices de grau 1.
	int d1Size = 0;    // Número de vértices de grau 1.

	BOOL f1[MAX];  // 'TRUE' se o vértice possuir apenas um d1 vizinho, 'FALSE' se não.
	BOOL f2[MAX];  // 'TRUE' se o vértice possuir mais de um d1 vizinho, 'FALSE' se não.
	int qtef1 = 0; // Quantidade de vértices com apenas um vizinho d1.
	BOOL existf2 = FALSE; // Veriica a existencia de algum vértice com mais de um vizinho d1.
	int i,j;

	/* Inicializa o vetor f1 */
	for(i = 0; i < nVert; i++){
		f1[i] = FALSE;
		f2[i] = FALSE;
	}

	/* Armazena os vértices de grau 1 em d1 */
	for(i = 0, j = 0; i < nVert; i++){
		if(nAdj[i] == 1){
			d1[d1Size++] = i;
			existd1 = TRUE;
		}
	}
        printf("d1size: %d\n",d1Size);
	/* Se existe vértice de grau 1 */
	if(d1Size != 0){
		for(i = 0; i < d1Size; i++){
			int u = g[ d1[i] ][0]; //vértice que possui vizinho de grau 1.
                        printf("u: %d\tf1[u]: %d\tf2[u]: %d\t",u,f1[u],f2[u]);
			/*Se o vértice 'u' não possui nenhum vizinho d1*/
			if(!f1[u] && !f2[u]){
				f1[u] = TRUE;
				qtef1++;
			}
			/*Se o vértice 'u' já possui um vizinho em d1*/
			else if (f1[u]) {
				f1[u] = FALSE;
				f2[u] = TRUE;
				qtef1--;
				existf2 = TRUE;
			}
			/*Se o vértice 'u' já possui mais de um vizinho em d1, não se faz nada,
			já que o seu estado nem no vetor f2, nem na variável existf2 iria mudar.

			Como a verificação ocorre a cada iteração, não é necessário armazenar a
			quantidade de vértices em f2, pois nunca ocorrerá o caso em que dois vértices
			vão estar em f2, pois antes, um dos dois deverá passar por f1, o que irá
			satisfazer o caso de retorno abaixo.*/
                        printf("i: %d\tqtef1: %d\texistf2: %d\n",d1[i],qtef1,existf2);;
			if(qtef1 > 0 && existf2)
				return d1Size;
		}
		/* Caso a verificação não ocorra, ou existe um ou mais f1, e não existe f2, ou existe um f2 apenas, e nenhum f1. */
		return d1Size + 1;
	}
	/* Se não existe vértice de grau 1*/
	else
		return 2;

}

int main(){
	Zera();
	IniciaG();
	printf("HullNumber: %d\n",hullnumber());
        return 0;
}