#include <stdio.h>
#include <string.h>

#define Tamanho_MeM 10000000  

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

static char heap[Tamanho_MeM];
static Block* ListaVazia = NULL;

void init_memory() {
    ListaVazia = (Block*) heap;
    ListaVazia->size = Tamanho_MeM;
    ListaVazia->next = NULL;
}

void* aloca(size_t size) {
    size += sizeof(Block);  
  
    Block* curr = ListaVazia;
    int blockIndex = 0;

    while (curr) {
        if (curr->size >= size) {
            
            Block* used = curr;
            
            if (blockIndex == 0) {
                ListaVazia = used->next;
            } else {
                Block* prev = ListaVazia;
                for (int i = 0; i < blockIndex - 1; i++) {
                    prev = prev->next;
                }
                prev->next = used->next;
            }

            used->size = size;
            return (void*) (used + 1);
        }

        curr = curr->next;
        blockIndex++;
    }

    return NULL;  
}

void libera(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = ((Block*) ptr) - 1;
    block->next = ListaVazia;
    ListaVazia = block;
}

void listar() {
    Block* curr = ListaVazia;
    if (curr == NULL) {
        printf("Não há  memória.\n");
    } else {
        printf("Memória livre:\n");
        size_t totalLivre = 0;
        int blockCount = 0;

        while (curr) {
            printf("Bloco %d: Endereço: %p, tamanho: %zu bytes\n", blockCount + 1, (void*) curr, curr->size);
            totalLivre += curr->size;
            curr = curr->next;
            blockCount++;
        }

        printf("Espaço livre: %zu bytes (%.2f MB)\n", totalLivre, (float)totalLivre / (1024 * 1024));
    }
}

int main() {
    init_memory();

    char action;
    size_t size;

    while (1) {
        printf("\nEscolha uma ação:\n");
        printf("  (1) Alocar memória\n");
        printf("  (2) Liberar memória\n");
        printf("  (3) Listar blocos livres\n");
        printf("  (4) Sair\n");
        printf("Opção: ");
        scanf(" %c", &action);

        switch (action) {
            case '1':
                printf("Tamanho da alocação desejada (em bytes): ");
                scanf("%zu", &size);
                void* mem = aloca(size);
                if (mem) {
                    printf("Memória alocada em: %p\n", mem);
                } else {
                    printf("Não foi possível alocar memória para %zu bytes.\n", size);
                }
                break;

            case '2':
                printf("Endereço de memória para liberar (formato hexadecimal): ");
                void* ptr;
                scanf("%p", &ptr);
                libera(ptr);
                printf("Memória liberada.\n");
                break;

            case '3':
                listar();
                break;

            case '4':
                return 0;

            default:
                printf("Opção inválida.\n");
        }
    }

    return 0;
}