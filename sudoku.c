#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct {
    int sudo[9][9];
} Node;

Node* createNode() {
    Node* n = (Node*) malloc(sizeof(Node));
    return n;
}

Node* copy(Node* n) {
    Node* new = (Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file(char* file_name) {
    Node* n = createNode();
    FILE* file = fopen(file_name, "r");
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (!fscanf(file, "%d", &n->sudo[i][j]))
                printf("failed to read data!");
        }
    }

    fclose(file);
    return n;
}

void print_node(Node* n) {
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            printf("%d ", n->sudo[i][j]);
        printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n) {
    int rows[9][10] = {0};
    int cols[9][10] = {0};
    int subs[9][10] = {0};

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int num = n->sudo[i][j];
            if (num != 0) {
                if (rows[i][num] || cols[j][num] || subs[(i / 3) * 3 + j / 3][num]) {
                    return 0;
                }
                rows[i][num] = 1;
                cols[j][num] = 1;
                subs[(i / 3) * 3 + j / 3][num] = 1;
            }
        }
    }
    return 1;
}

List* get_adj_nodes(Node* n) {
    List* list = createList();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                for (int k = 1; k <= 9; k++) {
                    Node* newNode = copy(n);
                    newNode->sudo[i][j] = k;
                    pushBack(list, newNode);
                }
                return list;  // Solo necesitas generar nodos adyacentes para la primera casilla vacía encontrada
            }
        }
    }
    return list;  // Si no hay casillas vacías, devuelve una lista vacía
}

int is_final(Node* n) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                return 0;  // Si aún hay una casilla vacía, no es un estado final
            }
        }
    }
    return 1;  // Si no hay casillas vacías, es un estado final
}

Node* DFS(Node* initial, int* cont) {
    Stack* stack = createStack();
    push(stack, initial);

    while (!isEmpty(stack)) {
        Node* current = top(stack);
        pop(stack);
        (*cont)++;

        if (is_final(current)) {
            return current;  // Encontramos una solución
        }

        List* adj_nodes = get_adj_nodes(current);
        ListNode* current_node = adj_nodes->first;
        while (current_node != NULL) {
            Node* adj_node = (Node*)current_node->data;
            if (is_valid(adj_node)) {
                push(stack, adj_node);
            }
            current_node = current_node->next;
        }
        freeList(adj_nodes);
        free(current);
    }

    return NULL;  // No se encontró una solución
}

int main(int argc, char* argv[]) {
    Node* initial = read_file("s12a.txt");

    int cont = 0;
    Node* final = DFS(initial, &cont);
    printf("iterations:%d\n", cont);
    if (final) {
        print_node(final);
    } else {
        printf("No se encontró una solución.\n");
    }

    return 0;
}
