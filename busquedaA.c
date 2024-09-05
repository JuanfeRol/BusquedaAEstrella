
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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


// Punteros para los nodos de inicio y final
nodo* nodoInicio = NULL;
nodo* nodoFinal = NULL;



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
nodo** crearNodosDesdeMatriz(char** matriz, int filas, int columnas) {
    nodo** nodos = (nodo**)malloc(filas * sizeof(nodo*));
    for (int i = 0; i < filas; i++) {
        nodos[i] = (nodo*)malloc(columnas * sizeof(nodo));
        for (int j = 0; j < columnas; j++) {
            nodos[i][j].x = i;
            nodos[i][j].y = j;
            nodos[i][j].tipoTerreno = matriz[i][j];
            nodos[i][j].visitado = false;
            nodos[i][j].costo_transito = 0;
            nodos[i][j].costo_heuristica = 0;
            nodos[i][j].costo_total = 0;

            // Guardar los nodos de inicio y final
            if (nodos[i][j].tipoTerreno == 'I') {
                nodoInicio = &nodos[i][j];
            }
            if (nodos[i][j].tipoTerreno == 'F') {
                nodoFinal = &nodos[i][j];
            }
            if (nodos[i][j].tipoTerreno == 'P') {
                nodos[i][j].costo_transito = 1;
            }
            if (nodos[i][j].tipoTerreno == 'M') {   
                nodos[i][j].costo_transito = 2;
            }
            if (nodos[i][j].tipoTerreno == 'A') {
                nodos[i][j].costo_transito = 3;
            }
            if (nodos[i][j].tipoTerreno == 'O') {
                nodos[i][j].costo_transito = 100;
            }
        }
    }

//Se asginan enlaces a los nodos adyacentes
for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) { 
            nodos[i][j].arriba = NULL;
            nodos[i][j].abajo = NULL;
            nodos[i][j].izquierda = NULL;
            nodos[i][j].derecha = NULL;

            if (i > 0) nodos[i][j].arriba = &nodos[i-1][j];
            if (i < filas - 1) nodos[i][j].abajo = &nodos[i+1][j];
            if (j > 0) nodos[i][j].izquierda = &nodos[i][j-1];
            if (j < columnas - 1) nodos[i][j].derecha = &nodos[i][j+1];
        }
    }

    // Calcular el costo heurístico para cada nodo
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            nodos[i][j].costo_heuristica = costoHeuristico(&nodos[i][j], nodoFinal);
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
            printf("costo heuristico: %d\n", actual.costo_heuristica);  
            printf("costo transito: %d\n", actual.costo_transito);  
        }
    }
}

//Funcion de creacion de costo heuristico basado en la distancia manhataan
//(Proceso)
int costoHeuristico(nodo* nodoActual, nodo* nodoFinal){
    return abs(nodoActual->x - nodoFinal->x) + abs(nodoActual->y - nodoFinal->y);
}

//Funcion para imprimir los nodos como matriz (modificada)
void imprimirNodosAsMatriz(nodo** nodos, int filas, int columnas) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            printf("%c ", nodos[i][j].tipoTerreno);
        }
        printf("\n");
    }
}

//Funcion para la busqueda A*
// Función para realizar la búsqueda A*
void busquedaA(nodo** nodos, int filas, int columnas) {
    nodo* actual = nodoInicio; // Empezar desde el nodo inicial
    actual->visitado = true;   // Marcar el nodo inicial como visitado
    actual->costo_transito = 0;
    actual->costo_total = actual->costo_heuristica;  // f(n) = g(n) + h(n)

    while (actual != nodoFinal) {
        nodo* mejor_siguiente = NULL;
        int mejor_costo_total = 1000000;  // Un número grande como infinito

        // Explorar los vecinos del nodo actual
        nodo* vecinos[] = {actual->arriba, actual->abajo, actual->izquierda, actual->derecha};
        for (int i = 0; i < 4; i++) {
            nodo* vecino = vecinos[i];

            if (vecino != NULL && !vecino->visitado && vecino->tipoTerreno != 'O') {  // Ignorar nodos visitados y obstáculos
                // Calcular el costo de transito acumulativo
                int nuevo_costo_transito = actual->costo_transito + vecino->costo_transito;

                // Calcular el costo total f(n) = g(n) + h(n)
                int nuevo_costo_total = nuevo_costo_transito + vecino->costo_heuristica;

                // Actualizar si se encuentra un mejor nodo
                if (nuevo_costo_total < mejor_costo_total) {
                    mejor_siguiente = vecino;
                    mejor_costo_total = nuevo_costo_total;
                }
            }
        }

        if (mejor_siguiente == NULL) {
            printf("No hay un camino posible.\n");
            return;
        }

        // Avanzar al mejor nodo
        
        actual->tipoTerreno = 'X';  // Marcar el camino
        mejor_siguiente->tipoTerreno = 'X';  // Marcar el camino
        mejor_siguiente->visitado = true;
        mejor_siguiente->costo_transito = actual->costo_transito + mejor_siguiente->costo_transito;
        mejor_siguiente->costo_total = mejor_siguiente->costo_transito + mejor_siguiente->costo_heuristica;
        actual = mejor_siguiente;
    }

    printf("Camino encontrado.\n");
}

int main(){
    int filas, columnas;
    
    //Lectura de la matriz
    char** matriz = leerMatriz("Matriz1.txt", &filas, &columnas);

    printf("Matriz original leida:\n");
    imprimirMatriz(matriz, filas, columnas);
    printf("\n");
    nodo** nodos = crearNodosDesdeMatriz(matriz, filas, columnas);
    //imprimirNodos(nodos, filas, columnas);
    // Medir el tiempo de la búsqueda A*
    clock_t inicio = clock();  // Tiempo de inicio
    busquedaA(nodos, filas, columnas);
    clock_t fin = clock();     // Tiempo de fin
    double tiempo_usado = ((double) (fin - inicio)) / CLOCKS_PER_SEC;  // Cálculo del tiempo en segundos

    printf("Tiempo de búsqueda A*: %f segundos\n", tiempo_usado);    
    
    imprimirNodosAsMatriz(nodos, filas, columnas);
    liberarNodos(nodos, filas);
    liberarMatriz(matriz, filas);
    return 0;

}          