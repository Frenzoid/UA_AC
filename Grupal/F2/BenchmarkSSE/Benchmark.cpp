// Grupo 5 - Benchmark SSE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <string.h>
#include "malloc.h"

#ifdef _WIN32
#include <windows.h>

double PCFreq = 0.0;
__int64 CounterStart = 0;

// Iniciamos el contador.
void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        printf("QueryPerformanceFrequency failed!\n");

    PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

// Obtenemos el tiempo del contador.
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}
#endif

// Rellena los arrays con valores ascendentes dependiendo del tamaño del propio array.
void rellenar(int* vector1, int* vector2, int size) {
    for (int index = 0; index < size; index++) {
        vector1[index] = (index);
        vector2[index] = (index);
    };
};

void sumaSSE(int* vector1, int* vector2, int* vector3) {
    __asm {

        movups xmm0, [vector1] // load vector1 into xmm0
        movups xmm1, [vector2] //load vector2 into xmm1

        addps xmm0, xmm1 // vector1 + vector2

        movups[vector3], xmm0 // guardamos de vuelta.

    };
}

void multiplicaSSE(int* vector1, int* vector2, int* vector3) {
    __asm {

        movups xmm0, [vector1] // load vector1 into xmm0
        movups xmm1, [vector2] //load vector2 into xmm1

        mulps xmm0, xmm1 // vector1 + vector2

        movups[vector3], xmm0 // guardamos de vuelta.
    };
}


// Programa principal del benchmark
int main() {

    // Archivo de salida de los resultados.
    FILE* fp;
    fopen_s(&fp, "salida_SSE.txt", "w");

    if (fp == 0) {
        printf("Error creando archivo de salida.");
        return 1;
    }

    // Imprimimos los datos del ordenador.
    char command[200];
#ifdef _WIN32
    // Muestra y guarda el tipo de procesador.
    strcpy_s(command, "wmic cpu get caption, deviceid, name, numberofcores, maxclockspeed, status && wmic cpu get caption, deviceid, name, numberofcores, maxclockspeed, status > HWInfo.txt");
    system(command);

    // Muestra y guarda la información del SO
    strcpy_s(command, "systeminfo && systeminfo > SWInfo.txt");
    system(command);
#endif

#ifdef _unix
    // Muestra y guarda el tipo de procesador
    strcpy_s(command, "lshw -short && lshw -short > HWInfo.txt");
    system(command);

    // Muestra y guarda la información del SO
    strcpy_s(command, "name -a > SWInfo.txt");
    system(command);
#endif

    // Declaracion de los vectores.
    int* vector1;
    int* vector2;
    int* vector3;

    // Declaracion de los tiempos.
    double duracion = 0, total = 0;

    // Cantidad de iteraciones por tamaño.
    int loop = 3000000;

    // Tamaño de los vectores.
    int sizeVec = 30;

    printf("Realizando calculos... \n");

    fprintf(fp, "Tamanio\t\tDuracion\n--------------------------- \n");
    printf("Tamanio\t\tDuracion\n--------------------------- \n");


    // Bucle para multiplciar.
    for (int size = 100; size <= sizeVec + 100; size++) {

        vector1 = (int*)malloc(size * sizeof(int));
        vector2 = (int*)malloc(size * sizeof(int));
        vector3 = (int*)malloc(size * sizeof(int));

        if (vector1 == NULL || vector2 == NULL) {
            printf("No se pudo asignar memoria para los vectores.");
            return 1;
        }

        rellenar(vector1, vector2, size);

#ifdef _WIN32
        StartCounter();
#endif
#ifdef _unix
        clock_t begin = clock();
#endif

        // Multiplicamos los vecotres usando SSE
        for (int i = 0; i < loop; i++) {
            multiplicaSSE(vector1, vector2, vector3);
        }
#ifdef _unix
        clock_t end = clock();
#endif

        double time;

#ifdef _WIN32
        time = GetCounter();
#endif
#ifdef _unix
        time = (float)((end - begin) / loop / CLOCKS_PER_SEC) * 1000;
#endif
        // Liberamos los vectores.
        free(vector1);
        free(vector2);
        free(vector3);


        fprintf(fp, "%d\t\t", size);
        fprintf(fp, "%f s\n", time);

        printf("%d\t\t", size);
        printf("%f s\n", time);

        total += time;
    };

    fprintf(fp, "==> Media duracion de multiplicacion SSE \t%f s \n\n", (total / (sizeVec)));
    printf("==> Media duracion de multiplicacion SSE \t%f s \n\n", (total / (sizeVec)));

    // Bucle para sumar.
    duracion = 0;
    total = 0;

    for (int size = 100; size <= sizeVec + 100; size++) {

        vector1 = (int*)malloc(size * sizeof(int));
        vector2 = (int*)malloc(size * sizeof(int));
        vector3 = (int*)malloc(size * sizeof(int));

        if (vector1 == NULL || vector2 == NULL) {
            printf("No se pudo asignar memoria para los vectores.");
            return 1;
        }

        rellenar(vector1, vector2, size);

#ifdef _WIN32
        StartCounter();
#endif
#ifdef _unix
        clock_t begin = clock();
#endif
        // Sumamos los vectores usando SSE.
        for (int i = 0; i < loop; i++) {
            sumaSSE(vector1, vector2, vector3);
        }

#ifdef _unix
        clock_t end = clock();
#endif


        double time;

#ifdef _WIN32
        time = GetCounter();
#endif
#ifdef _unix
        time = (float)((end - begin) / loop / CLOCKS_PER_SEC) * 1000;
#endif

        // Liberamos los vectores.
        free(vector1);
        free(vector2);
        free(vector3);


        fprintf(fp, "%d\t\t", size);
        fprintf(fp, "%f s\n", time);

        printf("%d\t\t", size);
        printf("%f s\n", time);

        total += time;
    };

    fprintf(fp, "==> Duracion media de suma SSE \t%f s \n\n", (total / (sizeVec)));
    printf("==> Duracion media de suma SSE \t%f s \n\n", (total / (sizeVec)));

    printf("Aprieta INTRO para cerrar.\n");
    const char x = getchar();


    return 0;
};