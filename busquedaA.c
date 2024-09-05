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
void imprimirMatriz(char** matriz, int filas, int columnas){
    for (int i = 0; i < filas; i++){
        for (int j = 0; j < columnas; j++){
            printf("%c ", matriz[i][j]);
        }
        printf("\n");
    }
}

void liberarMatriz(char** matriz, int filas){
    for (int i = 0; i < filas; i++){
        free(matriz[i]);
    }
    free(matriz);
}

// Función para leer archivo *.txt y reconstruir la matriz de nodos
// Función para leer archivo *.txt y reconstruir la matriz de nodos
nodo** crearNodosDesdeMatriz(char** matriz, int filas, int columnas) {
    nodo** nodos = (nodo**)malloc(filas * sizeof(nodo*));
    for (int i = 0; i < filas; i++) {
        nodos[i] = (nodo*)malloc(columnas * sizeof(nodo));
        for (int j = 0; j < columnas; j++) {
            nodos[i][j].x = i+1;
            nodos[i][j].y = j+1;
            nodos[i][j].tipoTerreno = matriz[i][j];
            nodos[i][j].visitado = false;
            nodos[i][j].costo_transito = 0;
            nodos[i][j].costo_heuristica = 0;
            nodos[i][j].costo_total = 0;

            // Inicializar todos los punteros a NULL
            nodos[i][j].arriba = NULL;
            nodos[i][j].abajo = NULL;
            nodos[i][j].izquierda = NULL;
            nodos[i][j].derecha = NULL;
            
            // Enlaces a nodos adyacentes
            if (i > 0) nodos[i][j].arriba = &nodos[i-1][j];
            if (i < filas - 1) nodos[i][j].abajo = &nodos[i][j];
            if (j > 0) nodos[i][j].izquierda = &nodos[i][j];
            if (j < columnas - 1) nodos[i][j].derecha = &nodos[i][j];
            printf("Nodo (%d, %d): Tipo de terreno: %c\n", nodos[i][j].x, nodos[i][j].y, nodos[i][j].tipoTerreno);
        }
    }
    return nodos;
}

// Función para liberar la memoria de los nodos
void liberarNodos(nodo** nodos, int filas) {
    for (int i = 0; i < filas; i++) {
        free(nodos[i]);
    }
    free(nodos);
}

// Función para imprimir los nodos
void imprimirNodos(nodo** nodos, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            nodo actual = nodos[i][j];
            printf("Nodo (%d, %d): Tipo de terreno: %c\n", actual.x, actual.y, actual.tipoTerreno);
            
            if (actual.arriba != NULL)
                printf("\tArriba: (%d, %d)\n", actual.arriba->x, actual.arriba->y);
            if (actual.abajo != NULL)
                printf("\tAbajo: (%d, %d)\n", actual.abajo->x, actual.abajo->y);
            if (actual.izquierda != NULL)
                printf("\tIzquierda: (%d, %d)\n", actual.izquierda->x, actual.izquierda->y);
            if (actual.derecha != NULL)
                printf("\tDerecha: (%d, %d)\n", actual.derecha->x, actual.derecha->y);
        }
    }
}

int main(){
    int filas, columnas;
    char** matriz = leerMatriz("Matriz1.txt", &filas, &columnas);
    imprimirMatriz(matriz, filas, columnas);
    nodo** nodos = crearNodosDesdeMatriz(matriz, filas, columnas);
    imprimirNodos(nodos, filas, columnas);
    liberarNodos(nodos, filas);
    liberarMatriz(matriz, filas);


    return 0;

}
