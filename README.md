# AuditoraYVerificacionDeSeguridadEnElManejoDeCadenasEnC-DPS


# Compilación inicial 

// gcc -std=c11 -Wall -Wextra -Wpedantic exampleStrings.c -o exampleStrings

+ compilador utilizado; Visual Studio Code
+ versión; (MinGW.org GCC-6.3.0-1) 6.3.0
+ estándar de C seleccionado; estándar C11 (ISO/IEC 9899:2011)
+ opciones de compilación; -std=c11 -Wall -Wextra -Wpedantic (Wpedantic exige el cumplimiento del estándar ISO) 
+ errores producidos; 
    * exampleStrings.c:22-25: El compilador no reconoce la letra R y marca que faltan comillas de cierre porque la sintaxis R"foo(...)foo" es un "raw string literal" de C++.   
    * exampleStrings.c:94: Indica que s2 no está declarad, unque lo este. Como la variable s1 ha falaldo al compilarse justo antes, no ha procesado correctamente la declaración de s2.   
+ warnings producidos;
    * exampleStrings.c:32: La función gets_example_func está declarada como void pero intenta devolver un valor numérico (return 1;).   
    * exampleStrings.c:68, 69, 73: Hay variables declaradas que nunca se utilizan en el código (size_array1, size_array2, analitic3).   
    * exampleStrings.c:60: El parámetro argc se recibe en la función main pero no se utiliza dentro de ella.


# Relación con CERT C

+ Variable global s1   

    * Descripción del problema: Uso de inicialización con sintaxis R"foo(...)". Esta construcción pertenece a C++ y no al estándar C11   
    * Regla/Recomendación CERT: MSC20-C (Do not use deprecated or obsolescent features)
    * Consecuencia: Error fatal de compilación, impide la generación del ejecutable
    * Corrección: Reemplazar por un literal de cadena estándar en C ("\nHello\nWorld\n")

+ Función gets_example_func (Línea de retorno)   
    * Descripción del problema: La instrucción return 1; se encuentra dentro de una función con tipo de retorno void   
    * Regla/Recomendación CERT: MSC37-C (Ensure that control never reaches the end of a non-void function)
    * Consecuencia: Warning del compilador y posible comportamiento indefinido al intentar manejar un valor de retorno inexistente
    * Corrección: Eliminar el return 1; o cambiar el void a int
    
+ Función gets_example_func (Tratamiento del buffer)   
    * Descripción del problema: La instrucción buf[strlen(buf) - 1] = '\0' asume que hay caracteres si la entrada es un EOF inmediato o un nulo inicial, strlen devuelve 0, provocando un underflow al restar 1   
    * Regla/Recomendación CERT: ARR30-C (Do not form or use out-of-bounds pointers or array subscripts)
    * Consecuencia: Sobreescritura del byte anterior al inicio del array en la pila 
    * Corrección: Verificar la longitud antes de restar: size_t len = strlen(buf); if (len > 0) buf[len - 1] = '\0';
    
+ Función get_dirname   
    * Descripción del problema: El parámetro se declara como const char *pathname, pero la función modifica su contenido mediante *slash = '\0'  
    * Regla/Recomendación CERT: EXP05-C (Do not cast away a const qualification)
    * Consecuencia: Comportamiento indefinido si el puntero original apunta a memoria de solo lectura
    * Corrección: Eliminar el calificador const del parámetro, exigiendo que la función reciba un buffer explícitamente modificable

+ Función main (Llamada a get_dirname)   
    * Descripción del problema: La instrucción puts(get_dirname(__FILE__)); pasa el macro __FILE__ (literal de cadena) a una función que lo modifica internamente   
    * Regla/Recomendación CERT: STR30-C (Do not attempt to modify string literals)
    * Consecuencia: Segmentation fault al intentar escribir en la sección de memoria de solo lectura
    * Corrección: Pasar una copia del string alojada en un array local o creada dinámicamente con strdup

+ Función get_y_or_n   
    * Descripción del problema: Uso de la función gets(response) para leer datos del usuario en un buffer de tamaño 8  
    * Regla/Recomendación CERT: MSC24-C (Do not use deprecated functions) y STR31-C (Guarantee sufficient space)
    * Consecuencia: Buffer overflow en la pila si el usuario introduce más de 7 caracteres, permitiendo posible ejecución de código malicioso
    * Corrección: Sustituir la función por fgets(response, sizeof(response), stdin);

+ Función main (Acceso a argumentos)
    * Descripción del problema: Uso incondicional de argv[1] y argv[2] sin comprobar previamente el valor de argc   
    * Regla/Recomendación CERT: ARR30-C (Do not form or use out-of-bounds pointers or array subscripts)
    * Consecuencia: Lectura de punteros nulos y Segmentation fault si el programa se ejecuta sin suficientes argumentos
    * Corrección: Añadir una validación condicional if (argc >= 3) antes de acceder a las posiciones del array

+ Función main (Copia de argumentos)   
    * Descripción del problema: Uso de strcpy(key, argv[1]); strcat(...). El array destino key tiene un tamaño fijo de 24 bytes, pero la longitud de los argumentos de entrada es desconocida y no se limita   
    * Regla/Recomendación CERT: STR31-C (Guarantee that storage for strings has sufficient space for character data and the null terminator).
    * Consecuencia: Buffer overflow si la suma de las longitudes de los argumentos supera los 23 caracteres
    * Corrección: Sustituir las funciones de concatenación por snprintf(key, sizeof(key), "%s = %s", argv[1], argv[2]);
    
+ Función main (Copia de array5 a array3)   
    * Descripción del problema: En strncpy(array3, array5, sizeof(array3));, el tamaño de origen array5 (17 chars + nulo) es mayor que el destino array3 (16 bytes)
    * Regla/Recomendación CERT: STR32-C (Do not pass a non-null-terminated character sequence to a library function)
    * Consecuencia: strncpy llenará array3 pero omitirá el terminador nulo \0. Las siguientes funciones de cadena que lo lean fallarán
    * Corrección: Forzar la terminación manual inmediatamente después: array3[sizeof(array3)-1] = '\0';
    
+ Función main (Copia de array3 a array4)   
   * Descripción del problema: En strncpy(array4, array3, strlen(array3));, se llama a strlen sobre array3, el cual carece de terminación   nula por el fallo anterior
    * Regla/Recomendación CERT: STR32-C (Do not pass a non-null-terminated character sequence to a library function)
    * Consecuencia: Lectura fuera de límites (Out-of-bounds read), donde strlen leerá memoria adyacente hasta encontrar un byte nulo al azar
    * Corrección: Garantizar la terminación de array3 resuelve este problema: array4[sizeof(array4) - 1] = '\0';
    
+ Función main (Modificación de puntero a literal)   
    * Descripción del problema: La instrucción ptr_char[0] = 'N'; intenta modificar el primer carácter del puntero, el cual apunta directamente a un literal de cadena 
    * Regla/Recomendación CERT: STR30-C (Do not attempt to modify string literals).
    * Consecuencia: Segmentation fault al intentar sobreescribir memoria protegida de solo lectura
    * Corrección: Declarar la variable original como un array modificable en la pila: char ptr_char[] = "new string literal";


# Declaración de uso de IA

+ herramienta utilizada; Gemini Pro
+ tareas para las que se utilizó; 
+ cómo se verificaron sus respuestas;
+ al menos un ejemplo relevante de utilización.
+ tiempo necesario para hacer este ejercicio.


