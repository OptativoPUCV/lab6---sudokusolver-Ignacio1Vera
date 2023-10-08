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
    int i, j, k;
    int row[10] = {0};
    int col[10] = {0};
    int subgrid[10] = {0};

    // Verificar filas y columnas
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (n->sudo[i][j] != 0) {
                if (row[n->sudo[i][j]] == 1)
                    return 0; // Número repetido en la misma fila
                row[n->sudo[i][j]] = 1;

                if (col[n->sudo[i][j]] == 1)
                    return 0; // Número repetido en la misma columna
                col[n->sudo[i][j]] = 1;
            }
        }
        // Reiniciar los arreglos de filas y columnas para la siguiente fila
        for (k = 1; k <= 9; k++) {
            row[k] = 0;
            col[k] = 0;
        }
    }

    // Verificar subgrids de 3x3
    for (i = 0; i < 9; i += 3) {
        for (j = 0; j < 9; j += 3) {
            for (int x = i; x < i + 3; x++) {
                for (int y = j; y < j + 3; y++) {
                    if (n->sudo[x][y] != 0) {
                        if (subgrid[n->sudo[x][y]] == 1)
                            return 0; // Número repetido en el mismo subgrid
                        subgrid[n->sudo[x][y]] = 1;
                    }
                }
            }
            // Reiniciar el arreglo de subgrids para el siguiente subgrid
            for (k = 1; k <= 9; k++) {
                subgrid[k] = 0;
            }
        }
    }

    return 1; // El estado es válido
}

List* get_adj_nodes(Node* n) {
    List* list = createList();
    int i, j;

    // Encuentra la primera casilla vacía
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                break;
            }
        }
        if (j < 9) {
            break;
        }
    }

    // Genera nodos adyacentes cambiando el valor de la primera casilla vacía
    for (int num = 1; num <= 9; num++) {
        Node* newNode = copy(n);
        newNode->sudo[i][j] = num;
        pushBack(list, newNode);
    }

    return list;
}

int is_final(Node* n) {
    // Verifica si todas las casillas están llenas
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                return 0; // Hay al menos una casilla vacía
            }
        }
    }
    return 1; // Todas las casillas están llenas
}

Node* DFS(Node* initial, int* cont) {
    Stack* stack = createStack();
    push(stack, initial);

    while (!isEmpty(stack)) {
        Node* current = top(stack);
        pop(stack);
        (*cont)++; // Incrementa el contador de iteraciones

        if (is_final(current)) {
            // Encontramos una solución
            return current;
        }

        List* adj_nodes = get_adj_nodes(current);
        Node* next;
        while ((next = popFront(adj_nodes)) != NULL) {
            if (is_valid(next)) {
                push(stack, next); // Agrega nodos válidos al stack
            } else {
                free(next); // Libera memoria de nodos no válidos
            }
        }
        freeList(adj_nodes);
    }

    return NULL; // No se encontró solución
}

int main(int argc, char* argv[]) {
    Node* initial = read_file("s12a.txt");

    int cont = 0;
    Node* final = DFS(initial, &cont);
    printf("iterations:%d\n", cont);
    print_node(final);

    free(final); // Libera la memoria del nodo final
    free(initial); // Libera la memoria del nodo inicial

    return 0;
}
