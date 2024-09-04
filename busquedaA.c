#include <stdio.h>
#include <stdlib.h>

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

//Función para imprimir la matriz (Prueba)
void imprimirMatriz(char** matriz, int filas, int columnas){
    for (int i = 0; i < filas; i++){
        for (int j = 0; j < columnas; j++){
            printf("%c ", matriz[i][j]);
        }
        printf("\n");
    }
}


int main(){
    int filas, columnas;
    char** matriz = leerMatriz("Matriz1.txt", &filas, &columnas);
    imprimirMatriz(matriz, filas, columnas);
    return 0;

}