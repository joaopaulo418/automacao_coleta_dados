#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include <time.h>

#define TamanhoVetor 1024

int main(){
    int qtdCount = 0, index=0, i=0, vetor[1024];
    wiringPiSetup();  
    pinMode(2, INPUT); //porta 13 do GPIO que sai do discriminador
    pinMode(1, INPUT); //porta 12 do GPIO que vem do labview para mudar o valor do vetor
    pinMode(0, INPUT); //Sintoniza os sistemas
    FILE *arq;

	while(1){
		if(digitalRead(0) == HIGH){
			while(digitalRead(1) == HIGH){//Mudar posição do vetor
				if(digitalRead(2) == HIGH){//Realizar a contagem dos counts
					//printf("ALTO\n");
					qtdCount++;
					//printf("Entrou: %d\n",qtdCount);
				}
			}
			if(qtdCount!=0){
				vetor[index] = qtdCount;
				//printf("vetor[%d]: %d\n",index, qtdCount);
				//Gravando arquivo
				if(index == 1023){
					arq = fopen("counts.txt","wb");
					if(arq != NULL){
						for(i=0; i<=1023; i++){
							fprintf(arq,"%d - %d\n", i, vetor[i]);	
						}	
					}else{
						printf("Problema ao criar arquivo");
						return 0;	
					}
					fclose(arq);
					index = 0;
				}else{
					index++;
				}
			}
		qtdCount = 0;
		}
	}		
       
    return 0;
}