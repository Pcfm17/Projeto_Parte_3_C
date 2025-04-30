#ifndef PROJETO_H
#define PROJETO_H

#define MAX_CLIENTE 100
#define MAX_VEICULOS_COMPRADOS 10

typedef struct {
    char nome[100];
    char dateNascimento[11];
    char cpf[15];
    char telefone[15];
    char endereco[100];
    float saldo;
    char cidade[50];
    char rg[15];
    char email[100];
    char veiculosComprados[MAX_VEICULOS_COMPRADOS][50];
    int numVeiculosComprados;
    char senha[20];
} Cliente;

typedef struct {
    Cliente clientes[MAX_CLIENTE];
    int num_clientes;
} ListaClientes;

typedef struct {
    char nome[50];
    float preco;
    int categoria; // 1: Normal, 2: Rápido, 3: Corrida
} Veiculo;

// Funções principais
void carregarClientes(ListaClientes *lt, const char *nomeArquivo);
void salvarClientes(ListaClientes *lt, const char *nomeArquivo);
int login(ListaClientes *lt);
int criar(ListaClientes *lt);
int deletar(ListaClientes *lt);
void listar(ListaClientes *lt);
int editar(ListaClientes *lt);
int editar_senha(ListaClientes *lt);
int depositar(ListaClientes *lt);
int compra_veiculo(ListaClientes *lt);
void registrarMovimentacao(const char *cpf, const char *descricao);
void exibirExtrato();
void menu();

#endif