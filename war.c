#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NUM_TERRITORIOS 5


typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

Territorio *territorios_ptr = NULL;

void remover_newline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

Territorio* inicializar_territorios() {
    
    Territorio *t = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (t == NULL) {
        fprintf(stderr, "Erro: Falha na alocação dinâmica de memória.\n");
        exit(EXIT_FAILURE);
    }
    
        srand(time(NULL));
    
    return t;
}

void liberar_memoria(Territorio *t) {
    if (t != NULL) {
        free(t);
        printf("\nMemória alocada para os territórios liberada com sucesso.\n");
    }
}

// --- FUNÇÕES PRINCIPAIS ---


void cadastrar_territorios(Territorio *t) {
    char buffer[100];
    int i;

    printf("--- FASE DE CADASTRO ---\n");
    for (i = 0; i < NUM_TERRITORIOS; i++) {
        printf("Cadastrando Território %d:\n", i + 1);

                printf("  Nome do Território (máx. 29 caracteres): ");
        if (fgets(t[i].nome, sizeof(t[i].nome), stdin) == NULL) return;
        remover_newline(t[i].nome);

        
        printf("  Cor do Exército (máx. 9 caracteres): ");
        if (fgets(t[i].cor, sizeof(t[i].cor), stdin) == NULL) return;
        remover_newline(t[i].cor);

                printf("  Quantidade de Tropas: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return;
        remover_newline(buffer);
        
        if (sscanf(buffer, "%d", &t[i].tropas) != 1 || t[i].tropas < 1) {
            t[i].tropas = 1; 
            fprintf(stderr, "Aviso: Entrada inválida. Definido como 1 tropa.\n");
        }
        
        printf("\n");
    }
}
void exibir_mapa(const Territorio *t) {
    printf("\n========================================\n");
    printf("MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("========================================\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%d. %s (Exército %s, Tropas: %d)\n", 
               i + 1, t[i].nome, t[i].cor, t[i].tropas);
    }
    printf("----------------------------------------\n");
}

int rolar_dado() {
    return (rand() % 6) + 1;
}

void simular_ataque(Territorio *t, int atacante_idx, int defensor_idx) {
    
    if (atacante_idx < 0 || atacante_idx >= NUM_TERRITORIOS ||
        defensor_idx < 0 || defensor_idx >= NUM_TERRITORIOS ||
        atacante_idx == defensor_idx) {
        printf("Erro: Seleção de território inválida.\n");
        return;
    }

    Territorio *atacante = &t[atacante_idx];
    Territorio *defensor = &t[defensor_idx];

        if (atacante->tropas < 2) {
        printf("O atacante %s precisa de pelo menos 2 tropas para atacar.\n", atacante->nome);
        return;
    }

        int dado_ataque = rolar_dado();
    int dado_defesa = rolar_dado();

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dado_ataque);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dado_defesa);

        if (dado_ataque > dado_defesa) {
                defensor->tropas--;
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
    } else {
        
        atacante->tropas--;
        printf("VITÓRIA DA DEFESA! O atacante perdeu 1 tropa.\n");
    }

        if (defensor->tropas <= 0) {
        
     
        strcpy(defensor->cor, atacante->cor);
        
        atacante->tropas--;
        defensor->tropas = 1;
        
        printf("CONQUISTA! O território %s foi dominado pelo Exército %s!\n", 
               defensor->nome, defensor->cor);
    }
}

int main() {
    int atacante_num, defensor_num;
    char buffer[100];
    
   
    territorios_ptr = inicializar_territorios();
    
    
    cadastrar_territorios(territorios_ptr);

    
    do {
        exibir_mapa(territorios_ptr);
        
        printf("\n--- FASE DE ATAQUE ---\n");
        
       
        printf("Escolha o território atacante (1 a %d, ou 0 para sair): ", NUM_TERRITORIOS);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        remover_newline(buffer);
        if (sscanf(buffer, "%d", &atacante_num) != 1) atacante_num = -1; 
        
        if (atacante_num == 0) {
            break; 
        }
        
        
        printf("Escolha o território defensor (1 a %d): ", NUM_TERRITORIOS);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        remover_newline(buffer);
        if (sscanf(buffer, "%d", &defensor_num) != 1) defensor_num = -1; 

       
        simular_ataque(territorios_ptr, atacante_num - 1, defensor_num - 1);

    } while (1); 

        liberar_memoria(territorios_ptr);

    return 0;
}


