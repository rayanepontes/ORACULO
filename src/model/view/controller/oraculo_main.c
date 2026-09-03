#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include "oraculo_main.h"


void limpar_entrada(void)
{
	int caractere;
	while ((caractere = getchar()) != '\n' && caractere != EOF) {
	}
}

void iniciar_jogo(char nome[])
{

    bool jogo_ativo = true;

    do {
        printf("[SISTEMA DE SEGURANÇA NEXUS LABS]");
        Sleep(500);

        char tecla = _getch();

        if (tecla == 27)
        {
            printf("\nAte logo, %s!\n", nome);
            jogo_ativo = false;
        }


    } while (jogo_ativo);
	
}



