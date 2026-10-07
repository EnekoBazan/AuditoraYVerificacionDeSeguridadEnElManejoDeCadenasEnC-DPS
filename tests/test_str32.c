/**
 * tests/test_str32.c
 * Test unitario con assertions para demostrar STR32-C (ausencia de '\0' tras strncpy)
 */
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(void) {
    char array3_orig[16];
    char array3_fixed[16];
    char array5[] = "01234567890123456"; /* 17 caracteres + '\0' */

    /* Ensuciamos la memoria con 'X' para simular basura en la pila */
    memset(array3_orig, 'X', sizeof(array3_orig));
    memset(array3_fixed, 'X', sizeof(array3_fixed));

    /* 1. Comportamiento del codigo original:
       Copia 16 bytes de array5 en array3_orig, pero NO cabe el '\0' final */
    strncpy(array3_orig, array5, sizeof(array3_orig));
    void *nulo_en_original = memchr(array3_orig, '\0', sizeof(array3_orig));

    printf("[Original] ¿Existe terminador '\\0' dentro de los 16 bytes de array3?: %s\n",
           (nulo_en_original == NULL) ? "NO (VULNERABLE: strlen leera fuera de limites)" : "SI");

    /* 2. Comportamiento del codigo corregido:
       Copia 15 bytes y fuerza manualmente el '\0' en la ultima posicion */
    strncpy(array3_fixed, array5, sizeof(array3_fixed) - 1);
    array3_fixed[sizeof(array3_fixed) - 1] = '\0';
    void *nulo_en_fixed = memchr(array3_fixed, '\0', sizeof(array3_fixed));

    printf("[Corregido] ¿Existe terminador '\\0' dentro de los 16 bytes de array3?: %s (strlen seguro = %zu)\n",
           (nulo_en_fixed != NULL) ? "SI" : "NO", strlen(array3_fixed));

    /* Verificacion formal mediante Assertions */
    assert(nulo_en_original == NULL);  /* Confirma el fallo en el original */
    assert(nulo_en_fixed != NULL);     /* Confirma la correccion */
    assert(strlen(array3_fixed) == 15);

    printf("EXITO: Todas las aserciones (assertions) de verificacion se cumplieron.\n");
    return 0;
}