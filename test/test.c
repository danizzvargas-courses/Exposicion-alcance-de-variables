#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>

/*---------------------------------------------------------*/
static void test_compile_lastprivate(void) {
    int status = system("gcc -fopenmp src/LastPrivate.c -o LastPrivate_test 2> /tmp/LastPrivate_build.log");
    assert(status == 0 && "Compilación de LastPrivate.c falló");
    printf("test_compile_lastprivate: OK\n");
}

/*---------------------------------------------------------*/
static void test_run_lastprivate(void) {
    FILE *pipe = popen("./LastPrivate_test 2>&1", "r");
    assert(pipe != NULL && "No se pudo ejecutar LastPrivate_test");

    char line[256];
    int found_initial = 0;
    int found_final = 0;

    while (fgets(line, sizeof(line), pipe)) {
        if (strstr(line, "Valor inicial fuera de la región paralela: last = -1"))
            found_initial = 1;
        if (strstr(line, "Después de la región paralela: last = 7"))
            found_final = 1;
        printf("%s", line);
    }

    int status = pclose(pipe);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == 0 && "LastPrivate_test terminó con error");
    assert(found_initial && "No se encontró la línea inicial esperada");
    assert(found_final && "No se encontró la línea final esperada");
    printf("test_run_lastprivate: OK\n");
}

/*---------------------------------------------------------*/
static void test_compile_firstprivate(void) {
    int status = system("gcc -fopenmp src/FirstPrivate.c -o FirstPrivate_test 2> /tmp/FirstPrivate_build.log");
    assert(status == 0 && "Compilación de FirstPrivate.c falló");
    printf("test_compile_firstprivate: OK\n");
}

/*---------------------------------------------------------*/
static void test_run_firstprivate(void) {
    FILE *pipe = popen("./FirstPrivate_test 2>&1", "r");
    assert(pipe != NULL && "No se pudo ejecutar FirstPrivate_test");

    char line[256];
    int found_initial = 0;
    int found_final = 0;

    while (fgets(line, sizeof(line), pipe)) {
        if (strstr(line, "Valor inicial fuera de la región paralela: first = 100"))
            found_initial = 1;
        if (strstr(line, "Después de la región paralela: first = 100"))
            found_final = 1;
        printf("%s", line);
    }

    int status = pclose(pipe);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == 0 && "FirstPrivate_test terminó con error");
    assert(found_initial && "No se encontró la línea inicial esperada en FirstPrivate_test");
    assert(found_final && "No se encontró la línea final esperada en FirstPrivate_test");
    printf("test_run_firstprivate: OK\n");
}

/*---------------------------------------------------------*/
static void test_compile_threadprivate(void) {
    int status = system("gcc -fopenmp src/ThreadPrivate.c -o ThreadPrivate_test 2> /tmp/ThreadPrivate_build.log");
    assert(status == 0 && "Compilación de ThreadPrivate.c falló");
    printf("test_compile_threadprivate: OK\n");
}

/*---------------------------------------------------------*/
static void test_run_threadprivate(void) {
    FILE *pipe = popen("./ThreadPrivate_test 2>&1", "r");
    assert(pipe != NULL && "No se pudo ejecutar ThreadPrivate_test");

    char line[256];
    int found_maestro   = 0;
    int found_region1   = 0;
    int found_region2   = 0;

    while (fgets(line, sizeof(line), pipe)) {
        if (strstr(line, "Hilo maestro: contador = 99"))
            found_maestro = 1;
        if (strstr(line, "REGIÓN PARALELA 1"))
            found_region1 = 1;
        /* La región 2 sin copyin demuestra que cada hilo persiste su valor */
        if (strstr(line, "REGIÓN PARALELA 2"))
            found_region2 = 1;
        printf("%s", line);
    }

    int status = pclose(pipe);
    assert(WIFEXITED(status) && WEXITSTATUS(status) == 0 && "ThreadPrivate_test terminó con error");
    assert(found_maestro && "No se encontró el valor inicial del hilo maestro");
    assert(found_region1 && "No se encontró la región paralela 1");
    assert(found_region2 && "No se encontró la región paralela 2 (persistencia no verificada)");
    printf("test_run_threadprivate: OK\n");
}

/*---------------------------------------------------------*/
static void test_cleanup(void) {
    remove("LastPrivate_test");
    remove("FirstPrivate_test");
    remove("ThreadPrivate_test");
    printf("test_cleanup: OK\n");
}

int main(void) {
    test_compile_lastprivate();
    test_run_lastprivate();
    test_compile_firstprivate();
    test_run_firstprivate();
    test_compile_threadprivate();
    test_run_threadprivate();
    test_cleanup();
    printf("Todos los tests pasaron.\n");
    return 0;
}
