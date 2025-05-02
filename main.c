#include "projeto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>

#define NOME_ARQUIVO "clientes.txt"
#define MAX_LINE_SIZE 256 // Tamanho máximo de uma linha no arquivo

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    ListaClientes lista_clientes;
    lista_clientes.num_clientes = 0;

    // Carregar os clientes do arquivo ao iniciar o programa
    carregarClientes(&lista_clientes, NOME_ARQUIVO);

    printf("========================================================\n");
    printf("\tBem-vindo ao Sistema de Compra de Carros\n\n");
    printf("========================================================\n\n");

    int logado = 0;
    while (!logado) {
        printf("Por favor, faça o login para continuar.\n\n");

        int pergunta;
        printf("Você possui um login? [1. Sim e 2.Não] digite um dos números:\n");
        if (scanf("%d", &pergunta) != 1) {
            printf("Entrada inválida.\n");
            return 1;
        }
        while (getchar() != '\n'); // Limpar o buffer do teclado

        switch (pergunta) {
            case 1:
                printf("\tLOGIN:\n");
                if (login(&lista_clientes) == 0) {
                    logado = 1; // Login bem-sucedido, sair do loop de login
                } else {
                    printf("Falha no login.\n\n");
                    printf("Você pode ter esquecido a sua senha.\n");
                    printf("Você deseja editar a sua senha? [1. Sim e 2.Não] digite um dos números:\n");
                    if (scanf("%d", &pergunta) == 1) {
                        printf("\tRedefinindo sua senha.\n");
                        editar_senha(&lista_clientes);
                        printf("\nPor favor, tente fazer o login novamente com a nova senha.\n");
                        // O loop 'while (!logado)' continuará
                    } else if (pergunta == 2) {
                        printf("Você pode tentar novamente mais tarde.\n");
                        // O loop 'while (!logado)' continuará
                    } else {
                        printf("Opção inválida.\n");
                        return 1;
                    }
                }
                break;
            case 2:
                printf("\tCADASTRO:\n");
                criar(&lista_clientes);
                if (login(&lista_clientes) == 0) {
                    logado = 1; // Cadastro e login bem-sucedidos
                } else {
                    printf("Falha no login após o cadastro.\n");
                    return 1;
                }
                break;
            default:
                printf("Opção inválida.\n");
                return 1;
        }
    }

    int escolha;
    do {
        menu();
        printf("Escolha uma opção: ");
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (escolha) {
            case 1:
                criar(&lista_clientes);
                break;
            case 2:
                listar(&lista_clientes);
                break;
            case 3:
                editar(&lista_clientes);
                break;
            case 4:
                deletar(&lista_clientes);
                break;
            case 5:
                depositar(&lista_clientes);
                break;
            case 6:
                printf("Exibindo extrato de movimentações:\n");
                exibirExtrato();
                break;
            case 7:
                compra_veiculo(&lista_clientes);
                break;
            case 8:
                printf("Encerrando o programa e salvando os dados...\n");
                salvarClientes(&lista_clientes, NOME_ARQUIVO);
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (escolha != 8);

    return 0;
}