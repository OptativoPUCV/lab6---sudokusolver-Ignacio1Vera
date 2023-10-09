#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n = (Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new = (Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file(char* file_name){
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

void print_node(Node* n){
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++)
            printf("%d ", n->sudo[i][j]);
        printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int rows[9][10] = {0};  // Para verificar filas
    int cols[9][10] = {0};  // Para verificar columnas
    int subgrid[3][3][10] = {0};  // Para verificar subgrids

    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            int num = n->sudo[i][j];
            if (num != 0) {
                // Verificar filas
                if (rows[i][num] == 1) return 0;
                rows[i][num] = 1;
                // Verificar columnas
                if (cols[j][num] == 1) return 0;
                cols[j][num] = 1;
                // Verificar subgrids
                if (subgrid[i / 3][j / 3][num] == 1) return 0;
                subgrid[i / 3][j / 3][num] = 1;
            }
        }
    }
    return 1;
}

List* get_adj_nodes(Node* n){
    List* list = createList();
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                int k;
                for (k = 1; k <= 9; k++) {
                    Node* newNode = copy(n);
                    newNode->sudo[i][j] = k;
                    if (is_valid(newNode)) {
                        pushBack(list, newNode);
                    } else {
                        free(newNode);
                    }
                }
                return list;  // Solo necesitamos los nodos adyacentes para la primera celda vacía
            }
        }
    }
    return list;
}

int is_final(Node* n){
    int i, j;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (n->sudo[i][j] == 0) {
                return 0;  // Todavía hay celdas vacías, no es un estado final
            }
        }
    }
    return 1;  // Todas las celdas están llenas, es un estado final
}

Node* DFS(Node* initial, int* cont){
    Stack* stack = createStack();
    push(stack, initial);

    while (!is_empty(stack)) {
        Node* current = (Node*)top(stack);
        pop(stack);

        if (is_final(current)) {
            return current;  // Hemos encontrado una solución
        }

        List* adj_nodes = get_adj_nodes(current);
        Node* adj_node = (Node*)front(adj_nodes);

        while (adj_node != NULL) {
            push(stack, adj_node);
            adj_node = (Node*)next(adj_nodes);
        }

        (*cont)++;
    }

    return NULL;  // No se encontró una solución
}

int main(int argc, char* argv[]) {
    Node* initial = read_file("s12a.txt");
    int cont = 0;
    Node* final = DFS(initial, &cont);
    printf("iterations:%d\n", cont);
    if (final) {
        printf("Solution:\n");
        print_node(final);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
