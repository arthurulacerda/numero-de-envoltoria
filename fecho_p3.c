FechoConvexoP3(){
	int i,j;

	for(i = 0; i < nVert; i++){
		Infected[i] = 2;
	}

	for(i = 0; i < Ssize; i++){
		Fecho[i] = S[i];
		Infected[Fecho[i]] = 0;
	}
	Fechosize = Ssize;

	for(i = 0; i < Fechosize; i++){
		int a = Fecho[i];
		for(j = 0; j < nAdj[a]; j++){
			if(! Infected[ g[a][j] ]){
				Fecho[Fechosize++] = g[a][j];
				Infected[g[a][j]] --;
			}
		}
	}

	for(i = 0; i < Fechosize; i++){
		printf("%d ",Fecho[i]);
	}
	printf("\n");
}