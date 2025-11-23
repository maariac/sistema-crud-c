#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char nome[50];
    char cpf[12];
    int idade;
    char email[50];
} Pessoa;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void cadastrar();
void listar();
void buscar();
void atualizar();
void remover();
int verificarCPF(char cpf[]);

int main() {
    int opcao = 0;

    do {
        printf("===== MENU =====\n");
        printf("1 - Cadastrar Pessoa\n");
        printf("2 - Listar Pessoas\n");
        printf("3 - Buscar por CPF\n");
        printf("4 - Atualizar Pessoa\n");
        printf("5 - Remover Pessoa\n");
        printf("6 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();
        system("cls");

        switch(opcao) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: buscar(); break;
            case 4: atualizar(); break;
            case 5: remover(); break;
            case 6: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while(opcao != 6);

    return 0;
}

int verificarCPF(char cpf[]) {
    FILE *f = fopen("pessoas.bin", "rb");
    if (f == NULL) return 0;

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void cadastrar() {
    Pessoa p;

    printf("Nome completo: ");
    fgets(p.nome, 50, stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("CPF (11 digitos): ");
    scanf("%s", p.cpf);
    limparBuffer();

    if (verificarCPF(p.cpf)) {
        printf("ERRO: CPF ja existe!\n");
        return;
    }

    printf("Idade: ");
    scanf("%d", &p.idade);
    limparBuffer();

    printf("Email: ");
    fgets(p.email, 50, stdin);
    p.email[strcspn(p.email, "\n")] = '\0';

    FILE *f = fopen("pessoas.bin", "ab");
    fwrite(&p, sizeof(Pessoa), 1, f);
    fclose(f);

    printf("Pessoa cadastrada com sucesso!\n");
    system("pause");
    system("cls");
}

void listar() {
    FILE *f = fopen("pessoas.bin", "rb");
    if (f == NULL) {
        printf("Nenhum registro encontrado.\n");
        system("pause");
        system("cls");
        return;
    }

    Pessoa p;
    printf("===== LISTA =====\n");

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        printf("\nNome: %s\nCPF: %s\nIdade: %d\nEmail: %s\n",
               p.nome, p.cpf, p.idade, p.email);
    }

    fclose(f);
    system("pause");
    system("cls");
}

void buscar() {
    char cpf[12];
    printf("Digite o CPF para busca: ");
    scanf("%s", cpf);
    limparBuffer();

    FILE *f = fopen("pessoas.bin", "rb");
    if (f == NULL) {
        printf("Nenhum arquivo encontrado.\n");
        return;
    }

    Pessoa p;
    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            printf("\nEncontrado!\n");
            printf("Nome: %s\nCPF: %s\nIdade: %d\nEmail: %s\n",
                   p.nome, p.cpf, p.idade, p.email);
            fclose(f);
            system("pause");
            system("cls");
            return;
        }
    }

    printf("CPF nao encontrado.\n");
    fclose(f);
    system("pause");
    system("cls");
}

void atualizar() {
    char cpf[12];
    printf("CPF da pessoa a atualizar: ");
    scanf("%s", cpf);
    limparBuffer();

    FILE *f = fopen("pessoas.bin", "rb");
    FILE *temp = fopen("temp.bin", "wb");

    Pessoa p;
    int achou = 0;

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) {
            achou = 1;

            printf("Novo nome: ");
            fgets(p.nome, 50, stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0';

            printf("Nova idade: ");
            scanf("%d", &p.idade);
            limparBuffer();

            printf("Novo email: ");
            fgets(p.email, 50, stdin);
            p.email[strcspn(p.email, "\n")] = '\0';
        }

        fwrite(&p, sizeof(Pessoa), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove("pessoas.bin");
    rename("temp.bin", "pessoas.bin");

    if (!achou) printf("CPF nao encontrado.\n");
    else printf("Atualizado com sucesso!\n");

    system("pause");
    system("cls");
}

void remover() {
    char cpf[12];
    printf("CPF a remover: ");
    scanf("%s", cpf);
    limparBuffer();

    FILE *f = fopen("pessoas.bin", "rb");
    FILE *temp = fopen("temp.bin", "wb");

    Pessoa p;
    int removido = 0;

    while (fread(&p, sizeof(Pessoa), 1, f)) {
        if (strcmp(p.cpf, cpf) == 0) removido = 1;
        else fwrite(&p, sizeof(Pessoa), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove("pessoas.bin");
    rename("temp.bin", "pessoas.bin");

    if (removido)
        printf("Removido com sucesso!\n");
    else
        printf("CPF nao encontrado!\n");

    system("pause");
    system("cls");
}
