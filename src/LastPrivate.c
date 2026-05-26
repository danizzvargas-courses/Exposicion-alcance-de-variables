#include <stdio.h>
#include <omp.h>

int main(void) {
    int last = -1;

    printf("Valor inicial fuera de la región paralela: last = %d\n", last);

    #pragma omp parallel for lastprivate(last)
    for (int i = 0; i < 8; ++i) {
        /* lastprivate crea una copia privada como private,
           pero al final copia el valor de la última iteración
           del bucle paralelo de vuelta a la variable original. */
        last = i;
        printf("Hilo %d procesando iteración %d, last(private) = %d\n",
               omp_get_thread_num(), i, last);
    }

    printf("Después de la región paralela: last = %d\n", last);
    return 0;
}
