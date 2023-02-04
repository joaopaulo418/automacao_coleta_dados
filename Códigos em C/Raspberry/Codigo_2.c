#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>

#define TamanhoVetor 2048

int main(){
	int qtdCount = 0, vetor[TamanhoVetor], index = 0, i, aux = 0,run = 0;
    FILE *arq;
	arq = fopen("counts.txt","wb");
	fprintf(arq,"0");
	fclose(arq);
    //aux -> faz a verificação se a quantidade de counts recebida foi realmente zero
    
    for (i = 0; i < TamanhoVetor; i++){
        vetor[i] = 0;
    }
    
	wiringPiSetup();    
    pinMode(0, INPUT); //porta 11 do GPIO que vem do labview para iniciar o estudo
    pinMode(1, INPUT); //porta 12 do GPIO que vem do labview para mudar o valor do vetor
    pinMode(2, INPUT); //porta 13 do GPIO que sai do discriminador
    pinMode(4, OUTPUT); // porta 16 do GPIO que é acionada ao finalizaro run para indicar pro Envio.c que ele pode enviar
	digitalWrite(4,0);
	
    while(1){
        if(digitalRead(0)){
            while(digitalRead(1) && digitalRead(0)){
                if(digitalRead(2)){
                    qtdCount++;
                }
				aux = 1;
            }
            
			if(index < TamanhoVetor){//caso o index seja menor que 2048 o programa deve adicionar no vetor
				if(aux && !qtdCount){// verifica se o labview acabou de mandar um baixo e não recebeu nenhum count
					vetor[index] += 0;
					printf("Vetor[%d]: %d\n",index,vetor[index]);
					index++;
				}
				else if(qtdCount != 0){//Caso algum count tenha sido contabilizado
					vetor[index] += qtdCount;
					printf("Vetor[%d]: %d\n",index,vetor[index]);
					index++;
				}
			}
			if(index == TamanhoVetor){//caso o index seja igual a 2048 o vetor acabou e o arquivo deve ser salvo
				run++;
				arq = fopen("counts.txt","wb");
				if(arq != NULL){
					for(i=0; i<TamanhoVetor; i++){
						if(i == 0){
						fprintf(arq, "%d/%d/",run,vetor[i]);
						}else{
							fprintf(arq, "%d/", vetor[i]);
						}
					}
					printf("Arquivo Salvo\n");                    
				}
				else{
					printf("Problema ao criar arquivo\n");
				}
				fclose(arq);
				index = 0;
				digitalWrite(4,1);
				digitalWrite(4,0);
			}
			aux = 0;
            qtdCount = 0;
        }
        else{//Estudo ainda nao iniciado ou desligado
            index = 0;
            qtdCount = 0;
            aux = 0;
        }
    }
}
