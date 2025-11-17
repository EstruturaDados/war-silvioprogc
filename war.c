#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- CONSTANTES ---
#define NUM_TERRITORIOS 5
#define MAX_NOME_TERRITORIO 30
#define MAX_COR 10
#define MAX_MISSAO_DESC 100

// --- ENUMS E STRUCTS ---

// Tipos de Missão
typedef enum {
    DESTRUIR_COR,
    CONQUISTAR_TERRITORIOS,
    NUM_TIPOS_MISSAO // Usado para contar o número de tipos
} TipoMissao;

// Struct Territorio
typedef struct {
    char nome[MAX_NOME_TERRITORIO];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// Struct Missao
typedef struct {
    TipoMissao tipo;
    char descricao[MAX_MISSAO_DESC];
    char cor_alvo[MAX_COR]; // Usado para DESTRUIR_COR
    int territorios_alvo;   // Usado para CONQUISTAR_TERRITORIOS
} Missao;

// --- VARIÁVEIS GLOBAIS (Ponteiro para o vetor de territórios e a Missão) ---
Territorio *territorios_ptr = NULL;
Missao missao_atual;

// --- FUNÇÕES AUXILIARES DE I/O E MEMÓRIA ---

// Função auxiliar para coletar as cores únicas presentes no mapa
int coletar_cores_unicas(const Territorio *t, char cores_unicas[][MAX_COR]) {
    int count = 0;
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        int existe = 0;
        // Verifica se a cor já foi adicionada
        for (int j = 0; j < count; j++) {
            if (strcmp(t[i].cor, cores_unicas[j]) == 0) {
                existe = 1;
                break;
            }
        }
        // Se não existe, adiciona a cor
        if (!existe) {
            strcpy(cores_unicas[count], t[i].cor);
            count++;
        }
    }
    return count;
}

// Função para remover o caractere de nova linha (\n) lido pelo fgets
void remover_newline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

// Função para ler uma string de forma segura
void ler_string(char *destino, size_t tamanho, const char *prompt) {
    printf("%s", prompt);
    if (fgets(destino, tamanho, stdin) == NULL) {
        strcpy(destino, ""); // Em caso de erro, define como string vazia
        return;
    }
    remover_newline(destino);
}

// Função para ler um inteiro de forma segura
int ler_inteiro(const char *prompt) {
    char buffer[100];
    int valor = -1;
    ler_string(buffer, sizeof(buffer), prompt);
    sscanf(buffer, "%d", &valor);
    return valor;
}

// Função para inicializar a alocação dinâmica de memória
Territorio* inicializar_territorios() {
    Territorio *t = (Territorio *)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (t == NULL) {
        fprintf(stderr, "Erro: Falha na alocação dinâmica de memória.\n");
        exit(EXIT_FAILURE);
    }
    
    srand(time(NULL));
    return t;
}

// Função para liberar a memória alocada
void liberar_memoria(Territorio *t) {
    if (t != NULL) {
        free(t);
        printf("\nMemória alocada para os territórios liberada com sucesso.\n");
    }
}

// --- FUNÇÕES DE CADASTRO E EXIBIÇÃO ---

// Função para cadastrar um único território
void cadastrar_territorio_unitario(Territorio *t, int i) {
    
    printf("Cadastrando Território %d:\n", i + 1);

    // Nome
    ler_string(t->nome, MAX_NOME_TERRITORIO, "  Nome do Território: ");

    // Cor
    ler_string(t->cor, MAX_COR, "  Cor do Exército: ");

    // Tropas
    int tropas = ler_inteiro("  Quantidade de Tropas: ");
    if (tropas < 1) {
        t->tropas = 1;
        fprintf(stderr, "Aviso: Entrada inválida. Definido como 1 tropa.\n");
    } else {
        t->tropas = tropas;
    }
    
    printf("\n");
}

// Função para cadastrar todos os territórios
void cadastrar_todos_territorios(Territorio *t) {
    printf("--- FASE DE CADASTRO ---\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        cadastrar_territorio_unitario(&t[i], i);
    }
}

// Função para exibir o mapa (estado atual)
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

// Função para exibir a missão atual
void exibir_missao(const Missao *m) {
    printf("\n--- SUA MISSAO ---\n");
    printf("%s\n", m->descricao);
    printf("----------------------------------------\n");
}

// --- FUNÇÕES DE MISSÃO ---

// Função para sortear a missão
void sortear_missao(Missao *m, const Territorio *t) { // Adiciona o ponteiro para Territorio
    // Sorteia um tipo de missão
    m->tipo = rand() % NUM_TIPOS_MISSAO;
    
    switch (m->tipo) {
        case DESTRUIR_COR:
            // Coleta as cores únicas do mapa
            char cores_unicas[NUM_TERRITORIOS][MAX_COR];
            int num_cores = coletar_cores_unicas(t, cores_unicas);
            
            // A cor do jogador é a primeira cor (t[0].cor)
            // Se houver mais de uma cor, sorteia entre as cores inimigas (índices 1 até num_cores - 1)
            if (num_cores > 1) {
                // Sorteia um índice a partir de 1 (para evitar a cor do jogador)
                int cor_idx = (rand() % (num_cores - 1)) + 1; 
                strcpy(m->cor_alvo, cores_unicas[cor_idx]);
            } else {
                // Se só houver uma cor (a do jogador), muda para a missão de conquista
                m->tipo = CONQUISTAR_TERRITORIOS;
                // Continua para o case CONQUISTAR_TERRITORIOS
            }
            
            if (m->tipo == DESTRUIR_COR) {
                snprintf(m->descricao, MAX_MISSAO_DESC, "Destruir o Exército %s (eliminar todos os territórios com esta cor).", m->cor_alvo);
                break;
            }
            
        case CONQUISTAR_TERRITORIOS:
            // Missão: Conquistar um número X de territórios
            m->territorios_alvo = 3; // Exemplo: Conquistar 3 territórios
            snprintf(m->descricao, MAX_MISSAO_DESC, "Conquistar %d territórios (além dos iniciais).", m->territorios_alvo);
            break;
            
        default:
            // Missão padrão em caso de erro
            strcpy(m->descricao, "Conquistar 1 território.");
            break;
    }
}

// Função para verificar se a missão foi cumprida
int verificar_vitoria(const Territorio *t, const Missao *m) {
    switch (m->tipo) {
        case DESTRUIR_COR: {
            // Verifica se a cor alvo ainda existe no mapa
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(t[i].cor, m->cor_alvo) == 0) {
                    return 0; // Missão não cumprida: a cor alvo ainda existe
                }
            }
            return 1; // Missão cumprida: a cor alvo foi eliminada
        }
            
        case CONQUISTAR_TERRITORIOS: {
            // Conta quantos territórios pertencem ao jogador (assumindo que o jogador é o primeiro exército cadastrado)
            const char *cor_jogador = t[0].cor;
            int contagem_jogador = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(t[i].cor, cor_jogador) == 0) {
                    contagem_jogador++;
                }
            }
            // A missão é conquistar X territórios *além* dos iniciais.
            // Vamos simplificar: se o jogador tiver mais territórios que o alvo, ele vence.
            if (contagem_jogador >= m->territorios_alvo) {
                return 1; // Missão cumprida
            }
            return 0; // Missão não cumprida
        }
            
        default:
            return 0; // Missão desconhecida
    }
}

// --- FUNÇÕES DE BATALHA ---

// 3. Função para simular o lançamento de um dado (1 a 6)
int rolar_dado() {
    return (rand() % 6) + 1;
}

// Função para simular o ataque
void simular_ataque(Territorio *t, int atacante_idx, int defensor_idx) {
    // Validação de índices
    if (atacante_idx < 0 || atacante_idx >= NUM_TERRITORIOS ||
        defensor_idx < 0 || defensor_idx >= NUM_TERRITORIOS ||
        atacante_idx == defensor_idx) {
        printf("Erro: Seleção de território inválida.\n");
        return;
    }

    Territorio *atacante = &t[atacante_idx];
    Territorio *defensor = &t[defensor_idx];

    // Validação de tropas
    if (atacante->tropas < 2) {
        printf("O atacante %s precisa de pelo menos 2 tropas para atacar.\n", atacante->nome);
        return;
    }
    
    // Validação de cores (não pode atacar o próprio território)
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: Não é possível atacar o próprio território.\n");
        return;
    }

    // Lançamento de dados
    int dado_ataque = rolar_dado();
    int dado_defesa = rolar_dado();

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dado_ataque);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dado_defesa);

    // Comparação e atualização de tropas
    if (dado_ataque > dado_defesa) {
        defensor->tropas--;
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
    } else {
        atacante->tropas--;
        printf("VITÓRIA DA DEFESA! O atacante perdeu 1 tropa.\n");
    }

    // Verifica Conquista
    if (defensor->tropas <= 0) {
        // O território é conquistado
        
        // 1. Muda a cor do exército (o dono)
        strcpy(defensor->cor, atacante->cor);
        
        // 2. Move 1 tropa do atacante para o território conquistado
        atacante->tropas--;
        defensor->tropas = 1;
        
        printf("CONQUISTA! O território %s foi dominado pelo Exército %s!\n", 
               defensor->nome, defensor->cor);
    }
}

// --- FUNÇÃO PRINCIPAL ---

int main() {
    int acao;
    int atacante_num, defensor_num;
    
    // Inicializa a alocação de memória e o gerador de números aleatórios
    territorios_ptr = inicializar_territorios();
    
    // 1. Cadastro dos territórios
    cadastrar_todos_territorios(territorios_ptr);
    
    // 2. Sorteio da Missão
    sortear_missao(&missao_atual, territorios_ptr);

    // Laço principal do jogo
    do {
        exibir_mapa(territorios_ptr);
        exibir_missao(&missao_atual);
        
        printf("\n--- MENU DE ACOES ---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        
        acao = ler_inteiro("Escolha sua acao: ");

        switch (acao) {
            case 1: // Atacar
                printf("\n--- FASE DE ATAQUE ---\n");
                atacante_num = ler_inteiro("Escolha o território atacante (1 a 5): ");
                defensor_num = ler_inteiro("Escolha o território defensor (1 a 5): ");
                
                simular_ataque(territorios_ptr, atacante_num - 1, defensor_num - 1);
                break;
                
            case 2: // Verificar Missão
                if (verificar_vitoria(territorios_ptr, &missao_atual)) {
                    printf("\n========================================\n");
                    printf("PARABÉNS! MISSAO CUMPRIDA! VOCÊ VENCEU!\n");
                    printf("========================================\n");
                    acao = 0; // Sai do jogo
                } else {
                    printf("\nVocê ainda não cumpriu sua missão. Continue a lutar!\n");
                }
                break;
                
            case 0: // Sair
                printf("\nEncerrando o jogo.\n");
                break;
                
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }
        
        // Pausa para o usuário ver o resultado
        if (acao != 0) {
            printf("Pressione Enter para continuar...");
            // Limpa o buffer para o próximo getchar
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

    } while (acao != 0);

    // Libera a memória alocada
    liberar_memoria(territorios_ptr);

    return 0;
}
