# EDA II – Exposición

Alumnos:
- 322246698 - Nogueira Lopez Erik Yair
- 322166910 - Sánchez de Tagle Caballero Diego Omar
- 425094484 - Hernández Hernandez David

---

## Objetivo

El estudiante implementará y expondrá los temas asignados de Estructuras de Datos y Algoritmos II.
Controlar con precisión qué valor toma una variable al entrar a una región paralela y qué valor queda al salir, según cómo se declare.

---

## Descripción

Durante la práctica se implementará la herramienta estándar para programación en paralelo en sistemas de memoria compartida OpenMP, dentro de la práctica se observará los comportamientos y diferencia entre private, firstprivate y lastprivate (con qué valor entran y salen las variables).

Así como threadprivate y la manera en la que se distingue de las anteriores.

---

## Investigación
# Gestión de Datos en OpenMP

OpenMP (**Open Multi-Processing**) es una API diseñada para la computación paralela en sistemas de memoria compartida. A continuación, se detalla cómo se gestiona el alcance y la visibilidad de las variables entre hilos.

## 1. Tipos de Alcance de Variables (Data Sharing)

El alcance determina qué hilos pueden ver o modificar una variable:

*   **Shared (Compartida):** Todos los hilos acceden a la misma dirección de memoria. Es el valor por defecto para la mayoría de las variables declaradas fuera de una región paralela.
*   **Private (Privada):** Cada hilo tiene su propia copia local de la variable. Los cambios realizados por un hilo no son visibles para los demás.

---

## 2. Funcionamiento de Cláusulas de Privacidad

Para controlar con precisión la inicialización y el retorno de los valores en variables privadas, se utilizan las siguientes cláusulas:


| Cláusula | Valor al entrar (Entrada) | Valor al salir (Salida) |
| :--- | :--- | :--- |
| `private` | No definido (valor residual/basura). | Se pierde el valor local; la variable original no cambia. |
| `firstprivate` | Se inicializa con el valor que tenía antes de la región paralela. | Se pierde el valor local del hilo. |
| `lastprivate` | Igual que `private` (o `firstprivate` si se combinan). | El valor de la **última iteración lógica** (en un bucle) se copia a la variable original. |

---

## 3. Cláusula `threadprivate`

A diferencia de las anteriores, que actúan sobre regiones paralelas específicas, `threadprivate` se aplica a variables **globales o estáticas**.

*   **Persistencia:** El valor de la variable persiste entre diferentes regiones paralelas a lo largo de la vida del hilo.
*   **Uso:** Ideal para mantener estados globales únicos por hilo sin necesidad de pasar la variable como argumento constantemente.

---

## 4. Casos de Uso: ¿Cuándo usar cada una?


| Si necesitas... | Usa... |
| :--- | :--- |
| Una variable temporal o un índice de bucle interno. | `private` |
| Que cada hilo lea una configuración inicial común. | `firstprivate` |
| Recuperar el resultado del último paso de un bucle paralelo. | `lastprivate` |
| Mantener copias locales de variables globales/estáticas. | `threadprivate` |



---

## Implementación
1. Directiva 
#pragma omp parallel for
- La directiva le dice al compilador que el bucle for que sigue debe dividirse entre los hilos disponibles en el sistema. 

2. Cláusula 
lastprivate(last)
- Aislamiento: Crea una copia local de last para cada hilo, esto evita una condición de carrera, donde varios hilos intentarían escribir en la misma memoria al mismo tiempo.
- Sincronización final: A diferencia de una variable private normal (que se destruye al terminar), lastprivate toma el valor del hilo que ejecutó la última iteración lógica y lo copia de vuelta a la variable original del main. 

3. Funciones de la Librería
#include <omp.h>: Necesario para usar las funciones de OpenMP.
- omp_get_thread_num(): Devuelve el identificador único del hilo que está ejecutando ese código en ese momento (ej. Hilo 0, Hilo 1, etc.).

4. Flujo de ejecución
- Fuera del bloque: last vale -1.
- Dentro del bloque: Cada hilo trabaja con su propia versión de last, pero cada uno reporta su propio valor de i.
- Salida del bloque: El sistema identifica qué hilo hizo i = 7 y "rescata" ese valor.
- Final: El printf final muestra 7, confirmando que la comunicación entre el mundo paralelo y el secuencial fue exitosa.

---

## Resultados

---

## Conclusiones

---

## Referencias

OpenMP. (s/f). Hpc-Wiki.Info. Recuperado el 26 de mayo de 2026, de https://hpc-wiki.info/hpc/OpenMP
OpenMP: For. (s/f). Jakascorner.com. Recuperado el 26 de mayo de 2026, de http://jakascorner.com/blog/2016/05/omp-for.html
TylerMSFT. (s/f). 2. Directivas. Microsoft.com. Recuperado el 26 de mayo de 2026, de https://learn.microsoft.com/es-es/cpp/parallel/openmp/2-directives?view=msvc-170
XL Fortran for AIX. (2023, julio 13). Ibm.com. https://www.ibm.com/docs/es/xl-fortran-aix/16.1.0?topic=fortran-routines-openmp

