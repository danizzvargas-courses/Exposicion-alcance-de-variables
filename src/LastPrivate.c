#include <stdio.h>
#include <omp.h>

int main(void) {
    int last = -1;

    printf("Valor inicial fuera de la región paralela: last = %d\n", last);

    // lastprivate: cada hilo tiene su copia local, pero al final
    // se guarda el valor de la última iteración en la variable original.
    #pragma omp parallel for lastprivate(last)
    for (int i = 0; i < 8; ++i) {
        last = i; // cada hilo actualiza su copia local de last
        printf("Hilo %d procesando iteración %d, last(private) = %d\n",
               omp_get_thread_num(), i, last);
    }

    // El valor original se actualiza con la última iteración después de la región.
    printf("Después de la región paralela: last = %d\n", last);
    return 0;
}
