#include <stdio.h>
#include <string.h>
#include <stdlib.h> 


typedef struct {
    char nome[30]; 
    char cor[10];  
    int tropas;    
} Territorio;


#define NUM_TERRITORIOS 5


void remover_newline(char *str) {
   
    str[strcspn(str, "\n")] = 0;
}

int main() {
    
    Territorio territorios[NUM_TERRITORIOS];
    int i;
    char buffer[100];

    printf("--- Cadastro de Territórios ---\n\n");

        for (i = 0; i < NUM_TERRITORIOS; i++) {
        
        printf("Cadastrando Território %d:\n", i + 1);

        
        printf(" Nome do Território: ");
        if (fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin) == NULL) {
            fprintf(stderr, "Erro ao ler o nome.\n");
            return 1;
        }
        remover_newline(territorios[i].nome);

       
        printf(" Cor do Exército: ");
        if (fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin) == NULL) {
            fprintf(stderr, "Erro ao ler a cor.\n");
            return 1;
        }
        remover_newline(territorios[i].cor);

       
        printf("Quantidade de Tropas: ");
       
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Erro ao ler a quantidade de tropas.\n");
            return 1;
        }
        
        remover_newline(buffer);
        
       
        if (sscanf(buffer, "%d", &territorios[i].tropas) != 1) {
            
            territorios[i].tropas = 0;
            fprintf(stderr, "Aviso: Entrada inválida para tropas. Definido como 0.\n");
        }
        
        printf("\n");
    }

 
    printf("--- Dados dos Territórios Cadastrados ---\n");

    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exército: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("-----------------------------------------\n");
    }

    return 0;
}