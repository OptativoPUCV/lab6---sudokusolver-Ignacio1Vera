#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name)
{
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++)
  {
       for(j=0;j<9;j++)
       {
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}


void print_node(Node* n)
{
    int i,j;
    for(i=0;i<9;i++)
    {
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}


int is_valid(Node* n){
    int row[9][10] = {0};  
    int col[9][10] = {0};  
    int subgrid[9][10] = {0};  

    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            int num = n->sudo[i][j];
            if (num != 0) {
                if (row[i][num] == 1) return 0;
                row[i][num] = 1;
                if (col[j][num] == 1) return 0;
                col[j][num] = 1;
                int subgrid_index = (i / 3) * 3 + (j / 3);
                if (subgrid[subgrid_index][num] == 1) return 0;
                subgrid[subgrid_index][num] = 1;
            }
        }
    }
    return 1;
}



List* get_adj_nodes(Node* n)
{
   List* list=createList();
   for (int num = 1; num <= 9; num++) 
   {
       Node* newNode = copy(n);
       for (int i = 0; i < 9; i++) 
       {
           for (int j = 0; j < 9; j++) 
           {
               if (newNode->sudo[i][j] == 0) 
               {
                   newNode->sudo[i][j] = num;
                   if (is_valid(newNode)) 
                       pushBack(list, newNode);
                   break;  
               }
           }
       }
   }
   return list;
}




int is_final(Node* n)
{
    for (int i = 0; i < 9; i++) 
        for (int j = 0; j < 9; j++) 
            if (n->sudo[i][j] == 0) 
                return 0;  
    return 1;  
}




Node* DFS(Node* n, int* cont){
   Stack* S = createStack();
   push(S, n);

   while (!is_empty(S)) {
       Node* current = top(S);
       pop(S);

       (*cont)++;

       if (is_final(current)) {
           return current;
       }

       List* adj_nodes = get_adj_nodes(current);
       Node* next_node = first(adj_nodes);
       while (next_node != NULL) {
           push(S, next_node);
           next_node = next(adj_nodes);
       }
       free(current);
   }

   return NULL;
}





/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/