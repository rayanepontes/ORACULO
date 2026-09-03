// Menu principal do jogo.
#include <stdio.h>
#include "oraculo_main.h"


void mostrar_instrucoes(void)
{
	printf("\n=== Instrucoes ===\n");
	printf("Use o menu para iniciar uma partida.\n");
	printf("Boa sorte!\n");
	printf("\nPressione ENTER para voltar ao menu...");
	limpar_entrada();
	getchar();
}

int main(void)
{
	int opcao = 0;

	do {
		printf("\n=== ORACULO ===\n");
		printf("1. Iniciar jogo\n");
		printf("2. Instrucoes\n");
		printf("0. Sair\n");
		printf("Escolha uma opcao: ");

		if (scanf("%d", &opcao) != 1) {
			printf("\nOpcao invalida.\n");
			limpar_entrada();
			continue;
		}

		switch (opcao) {
		case 1:
			limpar_entrada();

			char nome[100];

			printf("Digite seu nome: ");
			scanf("%99s", nome);

			while (nome[0] == '\0') {
				printf("Nome invalido. Digite seu nome novamente: ");
				scanf("%99s", nome);
			}

    		printf("Bem-vindo ao Oraculo, %s!\n", nome);

			printf("\nJogo iniciado!\n");
			printf("Boa sorte e cuidado com quem confia. . .\n");

			iniciar_jogo(nome);
			limpar_entrada();

			break;
		case 2:
			mostrar_instrucoes();
			break;
		case 0:
			printf("\nAte logo!\n");
			break;
		default:
			printf("\nOpcao invalida.\n");
			break;
		}
	} while (opcao != 0);

	return 0;
}
