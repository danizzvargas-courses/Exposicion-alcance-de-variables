#include <stdio.h>
#include <omp.h>

int main(void) {
    int last = -1;

    printf("Valor inicial fuera de la región paralela: last = %d\n", last);

    // lastprivate: crea copias locales y exporta el valor de la iteración final (i=7) al terminar
    #pragma omp parallel for lastprivate(last)
    for (int i = 0; i < 8; ++i) {
        // Cada hilo modifica su propia copia privada de 'last'
        last = i; 
        printf("Hilo %d procesando iteración %d, last(private) = %d\n",
               omp_get_thread_num(), i, last);
    }

    // Al salir, 'last' conserva el valor de la iteración lógicamente última, no necesariamente del último hilo en terminar
    printf("Después de la región paralela: last = %d\n", last);
    return 0;
}