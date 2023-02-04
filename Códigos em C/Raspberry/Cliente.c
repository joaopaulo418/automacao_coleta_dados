#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include <sys/socket.h> 
#include <sys/types.h>  
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>

#define TAM 20000
#define TamanhoVetor 2048

int Cliente(char *MensagemConcatenada){
	
    int socket_desc;
    int conectou;
    int RespostaServidor;
    int enviou;
    int i, portaServidor = 65000;
    char *resposta_servidor = malloc(3*sizeof(char));
    char *ip = malloc(15*sizeof(char));
    struct sockaddr_in servidor;

    strcpy(ip,"192.168.0.144");//Prestar atenção no penúltimo número desse IP, pois ele é extremamente 
    //importante para conectar na rede

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Nao foi possivel criar socket!\n");
        return 0;
    }

    servidor.sin_addr.s_addr = inet_addr(ip);
    free(ip);
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(portaServidor);
    conectou = connect(socket_desc, (struct sockaddr *) &servidor, sizeof (servidor));

    if (conectou < 0){
        printf("Nao foi possivel conectar!\n");
        return 0;
    }

    enviou = send(socket_desc, MensagemConcatenada, strlen(MensagemConcatenada), 0);
    if (enviou < 0) {
        printf("Erro ao enviar mensagem\n");
        return 0;
    }

    printf("Dados enviados para o servidor com sucesso!!\n");

    memset(MensagemConcatenada, 0, sizeof MensagemConcatenada);

    memset(resposta_servidor, 0, sizeof resposta_servidor);

    RespostaServidor = read(socket_desc, resposta_servidor, 3);
    if(RespostaServidor < 0) {
        printf("Falha ao receber resposta\n");
        return 0;
    }
    
    if(strcmp(resposta_servidor,"bye")==0){
        free(resposta_servidor);
        return 1;
    }
        
    else{
        free(resposta_servidor);
        return 0;
    }
        
}

int main(){
    int qtdCount = 0, vetor[TamanhoVetor], i;

    char *MensagemConcatenada = malloc(TAM * sizeof(char));
    
	wiringPiSetup(); 
    pinMode(3, INPUT); // porta 15 do GPIO que indica que ele pode enviar os dados
	
    FILE *arq;
	arq = fopen("counts.txt","wb");
	fprintf(arq,"0");
	fclose(arq);
    
    while(1){
		if(digitalRead(3)){
			arq = fopen("counts.txt", "r");

			if(arq == NULL){
				printf("Arquivo não pode ser aberto\n");
			}
		
			fread(MensagemConcatenada, TAM, 1, arq);
			
			if(!Cliente(MensagemConcatenada)){
				printf("Servidor NAO CONFIRMOU recebimento da mensagem\n");
			}
			else{
				printf("Servidor CONFIRMOU recebimento da mensagem!\n");
			}
			fclose(arq);
		}
	}
    free(MensagemConcatenada);
}
