#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 80

int main(){

	char* inFileName = "recordes.txt";
	char* outFileName = "temp.txt";
	FILE* inFile = fopen(inFileName, "r");
	FILE* outFile = fopen(outFileName, "w+");
	char jogador1[MAX],jogador2[MAX],jogador3[MAX],jogador4[MAX],jogador5[MAX],placar1[MAX],placar2[MAX],placar3[MAX],placar4[MAX],placar5[MAX];
	char barraN[5] = "\n";
	char* ptr;
	int temp,pont,ret;
	int pont1 = 0;
	int pont2 = 0;
	int pont3 = 0;
	int pont4 = 0;
	int pont5 = 0;

	if( inFile == NULL )
	{
		printf("Open Error");
	}
	
	printf("Insira sua pontuação\n");
	scanf("%d", &pont);
	
	while( fgets(jogador1, sizeof(jogador1), inFile) != NULL )
	{
		jogador1[strlen(jogador1)-1] = '\0';
		fgets(placar1, sizeof(placar1), inFile);
		fgets(jogador2, sizeof(jogador2), inFile);
		jogador2[strlen(jogador2)-1] = '\0';
		fgets(placar2, sizeof(placar2), inFile);
		fgets(jogador3, sizeof(jogador3), inFile);
		jogador3[strlen(jogador3)-1] = '\0';
		fgets(placar3, sizeof(placar3), inFile);
		fgets(jogador4, sizeof(jogador4), inFile);
		jogador4[strlen(jogador4)-1] = '\0';
		fgets(placar4, sizeof(placar4), inFile);
		fgets(jogador5, sizeof(jogador5), inFile);
		jogador5[strlen(jogador5)-1] = '\0';
		fgets(placar5, sizeof(placar5), inFile);
		
		pont1 = strtol(placar1, &ptr, 10);
		pont2 = strtol(placar2, &ptr, 10);
		pont3 = strtol(placar3, &ptr, 10);
		pont4 = strtol(placar4, &ptr, 10);
		pont5 = strtol(placar5, &ptr, 10);
		
		printf("%d\n", pont1);printf("%d\n", pont2);printf("%d\n", pont3);printf("%d\n", pont4);printf("%d\n", pont5);
		
		if(pont > pont1){
			temp = pont1;
			pont1 = pont;
			pont5 = pont4;
			pont4 = pont3;
			pont3 = pont2;
			pont2 = temp;
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
		
		else if (pont > pont2){
			temp = pont2;
			pont2 = pont;
			pont5 = pont4;
			pont4 = pont3;
			pont3 = temp;	
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
	
		else if (pont > pont3){
			temp = pont3;
			pont3 = pont;
			pont5 = pont4;
			pont4 = temp;
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
	
		else if (pont > pont4){
			temp = pont4;
			pont4 = pont;
			pont5 = temp;
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
	
		else if(pont > pont5){
			pont5 = pont;
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
		else{
			fprintf(outFile, "%s", jogador1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont1);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont2);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont3);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont4);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%s", jogador5);fprintf(outFile, "%s", barraN);
			fprintf(outFile, "%d", pont5);fprintf(outFile, "%s", barraN);
		}
	}

	fclose(inFile);
	fclose(outFile);
	
    ret = remove(inFileName);

    if(ret != 0) 
    {
       printf("Error: unable to delete the file");
    }
	
	ret = rename(outFileName, inFileName);
	
    if(ret != 0) 
    {
       printf("Error: unable to rename the file");
    }
	
	return 0;
}
