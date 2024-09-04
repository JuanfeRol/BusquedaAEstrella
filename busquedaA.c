#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//Función para leer archivo *.txt y reconstruir la matriz
char ** leerMatriz(const char* nombreArcivo, int* filas, int* columnas){
    FILE* archivo = fopen(nombreArcivo, "r");
    if(archivo == NULL){
        printf("Error al abrir el archivo\n");
        exit(1);
    }

    //Lectura de las dimensiones de la matriz
    fscanf(archivo, "%d %d", filas, columnas);

    //Se reserva memoria para la matriz
    char** matriz = (char**)malloc(*filas*sizeof(char*));
    for(int i = 0; i < *filas; i++){
        matriz[i] = (char*)malloc(*columnas*sizeof(char));
    }
    
    //Se llena la matriz con las letras del archivo
    for (int i = 0; i < *filas; i++){
        for (int j = 0; j < *columnas; j++){
            fscanf(archivo, " %c", &matriz[i][j]);
        }
    }

    fclose(archivo);
    return matriz;
}

//Estructura de los nodos
typedef struct nodo{

//Posición del nodo
int x, y;

//Costos
int costo_transito; //g(n)
int costo_heuristica; //h(n)
int costo_total; //f(n)

struct nodo* arriba;
struct nodo* abajo;
struct nodo* izquierda;
struct nodo* derecha;

//Variable booleana para saber si el nodo ya fue visitado
bool visitado;

char tipoTerreno;//(I (inicio), F(Final), P (Plano), M(Montaña), A(Pantano))
} nodo;


//Función para imprimir la matriz (Prueba)
//void imprimirMatriz(char** matriz, int filas, int columnas){
//    for (int i = 0; i < filas; i++){
//        for (int j = 0; j < columnas; j++){
//            printf("%c ", matriz[i][j]);
//        }
//        printf("\n");
//    }
//}



int main(){
    int filas, columnas;
    char** matriz = leerMatriz("Matriz1.txt", &filas, &columnas);
    imprimirMatriz(matriz, filas, columnas);
    return 0;

}