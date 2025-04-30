#include "projeto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define NOME_ARQUIVO "clientes.txt"
#define MAX_LINE_SIZE 256 // Aumentei o tamanho máximo da linha para acomodar mais dados
#define MAX_VEICULOS 21 // Total de veículos disponíveis

void carregarClientes(ListaClientes *lt, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Arquivo '%s' não encontrado. Um novo será criado ao salvar os dados.\n", nomeArquivo);
        lt->num_clientes = 0;
        return;
    }

    lt->num_clientes = 0;
    char linha[MAX_LINE_SIZE];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Remover a quebra de linha, se existir
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') {
            linha[len - 1] = '\0';
        }

        Cliente cliente;
        char veiculosStr[1024] = "";
        int num_campos_lidos = sscanf(linha, "%99[^|]|%10[^|]|%14[^|]|%14[^|]|%99[^|]|%f|%49[^|]|%14[^|]|%99[^|]|%d|%19[^|]|%[^\n]",
                                      cliente.nome,
                                      cliente.dateNascimento,
                                      cliente.cpf,
                                      cliente.telefone,
                                      cliente.endereco,
                                      &cliente.saldo,
                                      cliente.cidade,
                                      cliente.rg,
                                      cliente.email,
                                      &cliente.numVeiculosComprados,
                                      cliente.senha,
                                      veiculosStr);

        if (num_campos_lidos >= 11) { // Garante que pelo menos os campos básicos foram lidos
            // Processar a string de veículos comprados (separada por vírgulas)
            char *token = strtok(veiculosStr, ",");
            int i = 0;
            while (token != NULL && i < MAX_VEICULOS_COMPRADOS) {
                strncpy(cliente.veiculosComprados[i], token, sizeof(cliente.veiculosComprados[i]) - 1);
                cliente.veiculosComprados[i][sizeof(cliente.veiculosComprados[i]) - 1] = '\0'; // Garante terminação nula
                token = strtok(NULL, ",");
                i++;
            }
            cliente.numVeiculosComprados = i; // Atualiza o número correto de veículos

            if (lt->num_clientes < MAX_CLIENTE) {
                lt->clientes[lt->num_clientes++] = cliente;
            } else {
                printf("Aviso: Limite máximo de clientes (%d) atingido. Novos clientes não serão carregados.\n", MAX_CLIENTE);
                break;
            }
        } else {
            printf("Erro ao analisar a linha do arquivo '%s': %s (Número de campos lidos: %d, esperado: 12)\n", nomeArquivo, linha, num_campos_lidos);
            // Considerar se um erro de leitura deve interromper o carregamento ou apenas ignorar a linha
        }
    }

    fclose(arquivo);
    printf("Dados dos clientes carregados com sucesso.\n");
}

void salvarClientes(ListaClientes *lt, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", nomeArquivo);
        return;
    }
	int i;
    for (i = 0; i < lt->num_clientes; i++) {
        Cliente *c = &lt->clientes[i];

        // Serializa os veículos em uma única string separada por vírgulas
        char veiculosStr[1024] = "";
        int j;
        for (j = 0; j < c->numVeiculosComprados; j++) {
            strcat(veiculosStr, c->veiculosComprados[j]);
            if (j < c->numVeiculosComprados - 1) {
                strcat(veiculosStr, ",");
            }
        }

        // Escreve todos os dados no arquivo com separador "|"
        fprintf(arquivo, "%s|%s|%s|%s|%s|%.2f|%s|%s|%s|%d|%s|%s\n",
                c->nome,
                c->dateNascimento,
                c->cpf,
                c->telefone,
                c->endereco,
                c->saldo,
                c->cidade,
                c->rg,
                c->email,
                c->numVeiculosComprados,
                c->senha,
                veiculosStr);
    }

    fclose(arquivo);
    printf("Dados dos clientes salvos com sucesso.\n");
}

int login(ListaClientes *lt) {
    char cpf_digitado[15];
    char senha_digitada[20];
    int tentativas = 3;

    while (tentativas > 0) {
        printf("Digite seu CPF ou escreva 'sair' para sair:\n");

        if (scanf(" %11s", cpf_digitado) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (strcmp(cpf_digitado, "sair") == 0) {
            return 1; // Encerra o programa se o usuário digitar "sair"
        }

        printf("Digite a senha: ");
        if (scanf(" %19s", senha_digitada) != 1) {
            printf("Entrada inválida.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        int i;
        for (i = 0; i < lt->num_clientes; i++) {
            if (strcmp(lt->clientes[i].cpf, cpf_digitado) == 0 && strcmp(lt->clientes[i].senha, senha_digitada) == 0) {
                printf("Login realizado com sucesso!\n");
                return 0; // Login bem-sucedido
            }
        }

        tentativas--;
        if (tentativas > 0) {
            printf("CPF ou senha incorretos. %d tentativas restantes.\n", tentativas);
        } else {
            printf("Número máximo de tentativas excedido. Acesso negado.\n");
            return 2; // Falha no login após várias tentativas
        }
    }
    return 2; // Falha no login
}

int criar(ListaClientes *lt) {
    if (lt->num_clientes >= MAX_CLIENTE) {
        printf("Limite de clientes atingido!\n");
        return 1;
    }

    Cliente *t = &lt->clientes[lt->num_clientes];

    printf("Digite o nome:\n");
    scanf(" %[^\n]", t->nome);
    getchar();

    printf("Escreva a data de nascimento (DD/MM/AAAA):\n");
    scanf(" %10s", t->dateNascimento);
    getchar();

    printf("Escreva o seu [CPF]:\n");
    scanf(" %14s", t->cpf);
    getchar();

    printf("Escreva o seu número de telefone:\n");
    scanf(" %14s", t->telefone);
    getchar();

    printf("Escreva o endereço em que reside:\n");
    scanf(" %[^\n]", t->endereco);
    getchar();

    printf("Escreva a cidade em que reside:\n");
    scanf(" %[^\n]", t->cidade);
    getchar();

    printf("Escreva o seu [RG]:\n");
    scanf(" %14s", t->rg);
    getchar();

    printf("Escreva o seu [E-mail]:\n");
    scanf(" %[^\n]", t->email);
    getchar();

    printf("Escreva o seu [Veículo], se não possui um, digite= (Nenhum veículo):\n");
	scanf(" %[^\n]", t->veiculosComprados[0]);
	getchar();
	t->numVeiculosComprados = 1;


    printf("Escreva sua senha:\n");
    scanf(" %19s", t->senha);
    getchar();

    lt->num_clientes++;
    salvarClientes(lt, NOME_ARQUIVO);

    printf("Cliente cadastrado com sucesso!\n");
    return 0;
}

int deletar(ListaClientes *lt) {
    if (lt->num_clientes == 0) {
        printf("Nenhum cliente cadastrado para deletar!\n");
        return 1;
    }

    char cpf[15];
    printf("Escreva o CPF do cliente que deseja deletar:\n");
    scanf(" %14s", cpf);
    getchar();

    int pos = -1;
    int i;
    for (i = 0; i < lt->num_clientes; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Cliente não encontrado!\n");
        return 2;
    }

    for (i = pos; i < lt->num_clientes - 1; i++) {
        lt->clientes[i] = lt->clientes[i + 1];
    }

    lt->num_clientes--;
    salvarClientes(lt, NOME_ARQUIVO); // Salvar a lista atualizada

    printf("Cliente deletado com sucesso!\n");
    return 0;
}

void listar(ListaClientes *lt) {
    printf("Lista de clientes cadastrados:\n");
    int i;
    for (i = 0; i < lt->num_clientes; i++) {
        printf("-----------------------------------------\n");
        printf("Nome: %s\n", lt->clientes[i].nome);
        printf("CPF: %s\n", lt->clientes[i].cpf);
        printf("Email: %s\n", lt->clientes[i].email);
        printf("Telefone: %s\n", lt->clientes[i].telefone);
        printf("Endereço: %s\n", lt->clientes[i].endereco);
        printf("Cidade: %s\n", lt->clientes[i].cidade);
        printf("RG: %s\n", lt->clientes[i].rg);
        printf("Saldo: R$ %.2f\n", lt->clientes[i].saldo);

        printf("Veículos comprados:\n");
        if (lt->clientes[i].numVeiculosComprados == 0) {
            printf("  Nenhum veículo comprado.\n");
        } else {
        	int j;
            for (j = 0; j < lt->clientes[i].numVeiculosComprados; j++) {
                printf("  %d. %s\n", j + 1, lt->clientes[i].veiculosComprados[j]);
            }
        }
        printf("-----------------------------------------\n");
    }

    if (lt->num_clientes == 0) {
        printf("Nenhum cliente cadastrado.\n");
    }
}


int editar(ListaClientes *lt) {
    if (lt->num_clientes == 0) {
        printf("Nenhum cliente cadastrado para editar.\n");
        return 1; // Nenhuma tarefa para editar
    }

    char cpf_editar[15];
    printf("Digite o CPF do cliente que deseja editar:\n");
    scanf("%14s", cpf_editar);
    getchar();

    int pos = -1;
    int i;
    for (i = 0; i < lt->num_clientes; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf_editar) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Cliente não encontrado!\n");
        return 2; // Posição inválida
    }

    printf("Novo Telefone:\n");
    scanf("%14s", lt->clientes[pos].telefone);
    getchar();

    printf("Novo Endereço:\n");
    scanf(" %[^\n]", lt->clientes[pos].endereco);
    getchar();

    printf("Nova Cidade:\n");
    scanf(" %[^\n]", lt->clientes[pos].cidade);
    getchar();

    printf("Novo Email:\n");
    scanf(" %[^\n]", lt->clientes[pos].email);
    getchar();

    salvarClientes(lt, NOME_ARQUIVO);
    printf("Dados do cliente atualizados com sucesso!\n");

    return 0;
}

int editar_senha(ListaClientes *lt) {
    if (lt->num_clientes == 0) {
        printf("Nenhum cliente cadastrado para editar a senha.\n");
        return 1; // Nenhuma tarefa para editar
    }

    char cpf_editar[15];
    printf("Digite o CPF do cliente que deseja editar a senha:\n");
    scanf(" %14s", cpf_editar);
    getchar();

    int pos = -1;
    int i;
    for (i = 0; i < lt->num_clientes; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf_editar) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Cliente não encontrado!\n");
        return 2; // Posição inválida
    }

    printf("Nova senha:\n");
    char nova_senha[20];
    scanf(" %19s", nova_senha);
    getchar();
    strcpy(lt->clientes[pos].senha, nova_senha); // Usa strcpy para copiar a nova senha

    salvarClientes(lt, NOME_ARQUIVO);
    printf("Senha atualizada com sucesso!\n");

    return 0;
}


int depositar(ListaClientes *lt) {
    char cpf[15];
    float valor;
    int cliente_encontrado = 0;

    printf("Digite o CPF do cliente para depósito:\n");
    scanf(" %14s", cpf);
    getchar();
    int i;
    for (i = 0; i < lt->num_clientes; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            cliente_encontrado = 1;
            printf("Digite o valor a ser depositado:\n");
            if (scanf("%f", &valor) != 1) {
                printf("Entrada inválida para o valor.\n");
                while (getchar() != '\n');
                return 1;
            }
            getchar();

            if (valor > 0) {
                lt->clientes[i].saldo += valor;
                salvarClientes(lt, NOME_ARQUIVO);
                char descricao[100];
                snprintf(descricao, sizeof(descricao), "Depósito de R$ %.2f realizado.", valor);
                registrarMovimentacao(lt->clientes[i].cpf, descricao);
                printf("Depósito de %.2f realizado com sucesso.\n", valor);
                return 0;
            } else {
                printf("Valor inválido para depósito.\n");
                return 1;
            }
        }
    }

    if (!cliente_encontrado) {
        printf("Cliente não encontrado.\n");
        return 1;
    }
    return 1; // Adicionado para evitar warning de falta de retorno
}

void registrarMovimentacao(const char *cpf, const char *descricao) {
    char nome_arquivo[50];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "extrato_%s.txt", cpf);

    FILE *fp = fopen(nome_arquivo, "a");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo de extrato");
        return;
    }

    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    char horario[26];
    strftime(horario, 26, "%d/%m/%Y %H:%M:%S", tm_info);

    fprintf(fp, "[%s] %s\n", horario, descricao);
    fclose(fp);
}

void exibirExtrato() {
    char cpf[12];
    printf("Digite o CPF do cliente para exibir o extrato:\n");
    scanf(" %11s", cpf);
    getchar();

    char nome_arquivo[50];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "extrato_%s.txt", cpf);

    FILE *fp = fopen(nome_arquivo, "r");
    if (fp == NULL) {
        printf("Nenhuma movimentação encontrada para este cliente.\n");
        return;
    }

    char linha[256];
    printf("\nExtrato de movimentações:\n");
    while (fgets(linha, sizeof(linha), fp)) {
        printf("%s", linha);
    }
    fclose(fp);
}

// Função para exibir o menu de veículos
void exibirMenuVeiculos(Veiculo veiculos[], int numVeiculos) {
    printf("\nMenu de Veículos:\n");
    printf("----------------------------------------------------------\n");
    printf("Categoria | Código | Nome do Veículo         | Preço\n");
    printf("----------------------------------------------------------\n");
    int i;
    for (i = 0; i < numVeiculos; i++) {
        printf("%-10s | %-6d | %-22s | R$ %.2f\n",
               (veiculos[i].categoria == 1) ? "Normal" :
               (veiculos[i].categoria == 2) ? "Rápido" : "Corrida",
               i + 1, veiculos[i].nome, veiculos[i].preco);
    }
}

int compra_veiculo(ListaClientes *lt) {
    char cpf[15];
    printf("Digite o CPF do cliente que deseja comprar o veículo:\n");
    scanf(" %14s", cpf);
    getchar();

    int i;
    for (i = 0; i < lt->num_clientes; i++) {
        if (strcmp(lt->clientes[i].cpf, cpf) == 0) {
            Cliente *cliente = &lt->clientes[i];

            if (cliente->numVeiculosComprados >= MAX_VEICULOS_COMPRADOS) {
                printf("Limite de veículos comprados atingido.\n");
                return 1;
            }

            // Definição do array de veículos
            Veiculo veiculos[MAX_VEICULOS] = {
                {"Toyota Corolla", 30000.00, 1},
                {"Honda Civic", 35000.00, 1},
                {"Volkswagen Golf", 40000.00, 1},
                {"Chevrolet Cruze", 45000.00, 1},
                {"Ford Focus", 50000.00, 1},
                {"Hyundai HB20", 55000.00, 1},
                {"Renault Sandero", 60000.00, 1},
                {"BMW Série 3", 70000.00, 2},
                {"Audi A4", 75000.00, 2},
                {"Mercedes-Benz Classe C", 80000.00, 2},
                {"Porsche 911", 85000.00, 2},
                {"Ferrari Roma", 90000.00, 2},
                {"Lamborghini Huracán", 95000.00, 2},
                {"Aston Martin Vantage", 100000.00, 2},
                {"Fórmula 1 SF90", 120000.00, 3},
                {"NASCAR Toyota Camry", 130000.00, 3},
                {"WRC Hyundai i20", 140000.00, 3},
                {"Rally Dakar Peugeot 3008", 150000.00, 3},
                {"DTM BMW M4", 160000.00, 3},
                {"Super GT Honda NSX", 170000.00, 3},
                {"24 Horas de Le Mans Audi R18", 180000.00, 3}
            };
            int numVeiculos = MAX_VEICULOS;

            exibirMenuVeiculos(veiculos, numVeiculos);
            
            int opcaoCompra;
            printf("Deseja comprar o veículo selecionado?\n");
            printf("1 - Sim\n");
            printf("2 - Não\n");
            if (scanf("%d", &opcaoCompra) != 1) {
                printf("Opção inválida.\n");
                while (getchar() != '\n');
                return 1;
            }
            getchar();

            int codigoVeiculo;
            printf("----------------------------------------------------------\n");
            printf("Digite o código do veículo que deseja comprar:\n");
            printf("----------------------------------------------------------\n");
           
            if (scanf("%d", &codigoVeiculo) != 1) {
                printf("Código de veículo inválido.\n");
                while (getchar() != '\n');
                return 1;
            }
            getchar();

            if (codigoVeiculo <= 0 || codigoVeiculo > numVeiculos) {
                printf("Código de veículo inválido.\n");
                return 1;
            }

            if (opcaoCompra == 2) {
                printf("Compra cancelada.\n");
                return 0; // Não comprar o veículo
            } else if (opcaoCompra != 1) {
                printf("Opção inválida.\n");
                return 1;
            }
            // Ajusta o índice para acessar o veículo correto no array
            int indiceVeiculo = codigoVeiculo - 1;
            Veiculo veiculoSelecionado = veiculos[indiceVeiculo];

            // Verifica se o cliente já comprou este veículo
            int j;
            for (j = 0; j < cliente->numVeiculosComprados; j++) {
                if (strcmp(cliente->veiculosComprados[j], veiculoSelecionado.nome) == 0) {
                    printf("Você já comprou este veículo.\n");
                    return 1;
                }
            }

            if (cliente->saldo < veiculoSelecionado.preco) {
                printf("Saldo insuficiente para comprar o veículo.\n");
                return 1;
            }

            cliente->saldo -= veiculoSelecionado.preco;
            strcpy(cliente->veiculosComprados[cliente->numVeiculosComprados], veiculoSelecionado.nome);
            cliente->numVeiculosComprados++;
            salvarClientes(lt, NOME_ARQUIVO);

            char descricao[100];
            snprintf(descricao, sizeof(descricao), "Compra de veículo: %s por R$ %.2f", veiculoSelecionado.nome, veiculoSelecionado.preco);
            registrarMovimentacao(cliente->cpf, descricao);

            printf("Veículo comprado com sucesso!\n");
            return 0;
        }
    }

    printf("Cliente não encontrado.\n");
    return 1;
}

void menu() {
    printf("\nMenu de Opções:\n");
    printf("1. Cadastrar novo cliente\n");
    printf("2. Listar clientes\n");
    printf("3. Editar cliente\n");
    printf("4. Deletar cliente\n");
    printf("5. Depositar saldo\n");
    printf("6. Exibir extrato\n");
    printf("7. Comprar veículo\n");
    printf("8. Sair\n");
}