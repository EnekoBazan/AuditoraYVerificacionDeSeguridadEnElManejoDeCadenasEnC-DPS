/**
 * exampleStrings_fixed.c
 * Código auditado y corregido según SEI CERT C Coding Standard.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char array1[] = "Foo" "bar";
char array2[] = { 'F', 'o', 'o', 'b', 'a', 'r', '\0' };

enum { BUFFER_MAX_SIZE = 1024 };

/* CORRECCIÓN (MSC20-C): Sustitución del literal crudo C++ por un literal C estándar */
const char* s1 = "\nHello\nWorld\n";
const char* s2 = "\nHello\nWorld\n";

/* CORRECCIÓN (MSC37-C): Cambio de la firma a int para permitir retorno de estado */
int gets_example_func(void) {
    char buf[BUFFER_MAX_SIZE];
 
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 1;
    }
    
    /* CORRECCIÓN (ARR30-C): Verificación de longitud para evitar underflow (buf[-1]) */
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
    return 0;
}

/* CORRECCIÓN (EXP05-C): Se retira 'const' porque la función modifica el contenido apuntado */
char *get_dirname(char *pathname) {
    char *slash = strrchr(pathname, '/');
    if (slash) {
        *slash = '\0'; 
    }
    return pathname;
}
 
void get_y_or_n(void) {  
    char response[8];

    printf("Continue? [y] n: ");  
    
    /* CORRECCIÓN (MSC24-C, STR31-C): Reemplazo de gets() por fgets() especificando el límite del buffer */
    if (fgets(response, sizeof(response), stdin) != NULL) {
        if (response[0] == 'n' || response[0] == 'N') {
            exit(0);  
        }
    }
}

int main(int argc, char *argv[])
{
    char key[24];
    char response[8];
    char array3[16];
    char array4[16];
    char array5 [] = "01234567890123456";
    
    /* CORRECCIÓN (STR30-C): Declaración como array en la pila para permitir modificaciones seguras */
    char ptr_char[] = "new string literal";
    
    /* Se añaden (void) para suprimir los warnings de variables no utilizadas sin alterar la estructura original */
    int size_array1 = strlen("аналитик");
    (void)size_array1;
    int size_array2 = 100;
    (void)size_array2;
    char analitic3[100]="аналитик";
    (void)analitic3;

    /* CORRECCIÓN (STR30-C): Copia del macro __FILE__ a un array local antes de pasarlo a get_dirname */
    char file_path[] = __FILE__;
    puts(get_dirname(file_path));
        
    /* CORRECCIÓN (ARR30-C, STR31-C): Validación de argc y uso de snprintf para evitar desbordamientos */
    if (argc >= 3) {
        snprintf(key, sizeof(key), "%s = %s", argv[1], argv[2]);
    } else {
        snprintf(key, sizeof(key), "Argumentos insuficientes");
    }

    if (fgets(response, sizeof(response), stdin) == NULL) {
        /* Manejo silencioso de error de lectura */
    }
    
    get_y_or_n();

    printf("%s\n", array1);
    printf("%s\n", array2);
 
    puts(s1);
    puts(s2);
    
    /* CORRECCIÓN (STR32-C): Inserción manual del terminador nulo INMEDIATAMENTE después de strncpy */
    strncpy(array3, array5, sizeof(array3) - 1);
    array3[sizeof(array3) - 1] = '\0';
    
    strncpy(array4, array3, sizeof(array4) - 1);
    array4[sizeof(array4) - 1] = '\0';
    
    array5[0] = 'M';
    ptr_char[0] = 'N'; 
    /* Supresión explícita del warning */
    (void)ptr_char;
    
    return 0;
}