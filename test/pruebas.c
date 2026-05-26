#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();
        printf("Hola EDA II desde el hilo %d de %d\n", id, total);
    }
    return 0;
}

 un valor antes de usarla
        mi_variable = id + 100; 
        
        printf("Hilo %d: mi_variable = %d\n", id, mi_variable);
    }

    // La variable original mantiene su valor inicial
    printf("Después de la región: %d\n", mi_variable);

    return 0;
}
