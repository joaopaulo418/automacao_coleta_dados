#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <locale.h>
#include <windows.h>
using namespace std;

static const int kTitleBufSize = 1024;

HWND GetConsoleHwnd() {

  HWND hwndFound;
  TCHAR pszNewWindowTitle[kTitleBufSize];
  TCHAR pszOldWindowTitle[kTitleBufSize];

  GetConsoleTitle(pszOldWindowTitle, kTitleBufSize);

  // Format a "unique" NewWindowTitle.
  wsprintf(pszNewWindowTitle,TEXT("%d/%d"),
              GetTickCount(),
              GetCurrentProcessId());

  SetConsoleTitle(pszNewWindowTitle);

  // Ensure window title has been updated.
  Sleep(40);

  hwndFound=FindWindow(NULL, pszNewWindowTitle);

  // Restore original window title.
  SetConsoleTitle(pszOldWindowTitle);

  return hwndFound;
}

int main(){
	
	HWND wnd;
	printf("nome encontrado");
	wnd = GetConsoleHwnd();
	if( wnd != NULL ){
		printf("janela encontrada");
		ShowWindow(wnd, SW_HIDE);
	}
	
    FILE *arquivo;
    int portaServidor = 65000, i, clientAddrSize, run=0, c, cont=0, a=0, b=0;
    char *msgcliente, *mensagem, *run2 = (char*) malloc(20*(sizeof(char)));
    WSADATA WSAData;
    SOCKET server, client;
    SOCKADDR_IN serverAddr, clientAddr;
	WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    for(i=0; i<20; i++){
    	run2[i] = '\0';
	}
    if (server < 0) {
        printf("Nao foi possivel criar o socket\n");
        return 0;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(portaServidor);
    if(bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr)) < 0) {
        puts("Erro ao fazer bind, tente outra porta\n");
        return 0;
    }
    listen(server, 3);
	printf("Aguardando por conexoes...\n");
	clientAddrSize = sizeof(clientAddr);
	while((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET){
		msgcliente = (char*) malloc(20000*sizeof(char));
		mensagem = (char*) malloc(3*sizeof(char));
        arquivo = fopen("dados/dados.txt","w+");
        if(arquivo == NULL){
            printf("Erro ao criar ou reescrever arquivo!\n");
            return 0;
        }
  		if(recv(client, msgcliente, 20000, 0) > 0)
  			printf("Dados recebidos!\n");
		else{
			printf("Erro ao receber dados!\n");
		}
		if(msgcliente[0] == '0'){
			printf("Run nao iniciado!\n");
		}
		cont = 0;
        for(c=0; c<20000; c++){
            if(msgcliente[c] == '/'){
                cont++;
            }
        }
        printf("Quantidade de dados recebidos: %d\n", cont);
        if(fputs(msgcliente, arquivo) == EOF){
            printf("Erro ao escrever no arquivo!\n");
        }
        if(fclose(arquivo) != 0){
            printf("Erro ao fechar arquivo!\n");
        }
        strcpy(mensagem, "bye");
        if(send(client, mensagem, strlen(mensagem),0) < 0){
            printf("Erro ao enviar mensagem para o cliente!\n");
        }
		free(mensagem);
        memset(msgcliente, 0, sizeof(msgcliente));
        closesocket(client);
        free(msgcliente);
        printf("Aguardando por novas conexoes...\n\n");
    }
    if(client < 0){
        printf("Erro ao aceitar conexoes!\n");
        return 0;
    }
    free(run2);
    return 0;
}
