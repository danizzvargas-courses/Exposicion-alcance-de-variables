#include <stdio.h>
#include <omp.h>

// threadprivate SOLO funciona con variables globales o estáticas
int contador = 0;
#pragma omp threadprivate(contador)

int main() {

    // El hilo maestro (hilo 0) establece el valor inicial
    contador = 99;
    printf("=== ANTES DE CUALQUIER REGIÓN PARALELA ===\n");
    printf("Hilo maestro: contador = %d\n\n", contador);

    // --------------------------------------------------
    // REGIÓN PARALELA 1
    // copyin: copia el valor del maestro (99) a todos los hilos
    // --------------------------------------------------
    printf("=== REGIÓN PARALELA 1 (copyin inicializa todos en 99) ===\n");

    #pragma omp parallel num_threads(3) copyin(contador)
    {
        int hilo = omp_get_thread_num();

        // Todos empiezan en 99 gracias a copyin
        printf("Hilo %d: contador al entrar = %d\n", hilo, contador);

        // Cada hilo modifica SU PROPIA copia
        contador = hilo * 10;

        printf("Hilo %d: contador después de modificar = %d\n", hilo, contador);
    }

    printf("\n=== ENTRE REGIONES (hilo maestro) ===\n");
    printf("Hilo maestro: contador = %d\n", contador);
    printf("(Cada hilo guarda su propio valor en memoria)\n\n");

    // --------------------------------------------------
    // REGIÓN PARALELA 2
    // SIN copyin: cada hilo retoma el valor que dejó en la región 1
    // --------------------------------------------------
    printf("=== REGIÓN PARALELA 2 (sin copyin, cada hilo recuerda el suyo) ===\n");

    #pragma omp parallel num_threads(3)
    {
        int hilo = omp_get_thread_num();

        // Hilo 0 recuerda 0, Hilo 1 recuerda 10, Hilo 2 recuerda 20
        printf("Hilo %d: contador persistido = %d\n", hilo, contador);

        // Cada hilo sigue acumulando sobre su propia copia
        contador += 5;

        printf("Hilo %d: contador después de += 5 → %d\n", hilo, contador);
    }

    printf("\n=== RESULTADO FINAL (hilo maestro) ===\n");
    printf("Hilo maestro: contador = %d\n", contador);

    return 0;
}
