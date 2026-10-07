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


# Técnicas de verificación utilizadas
Se han aplicado tres técnicas:

1. Tests funcionales con casos límite diseñados manualmente: Inyección de cadenas que superan el tamaño de los buffers (`response[8]`) y ejecución omitiendo argumentos de entrada (`argv`).
2. Inspección del código de salida del proceso en el Sistema Operativo (`$LASTEXITCODE`): Verificación en PowerShell del código devuelto tras la ejecución para detectar violaciones de acceso a memoria (*Access Violation* `-1073741819` / `0xC0000005`) que ocurren al final del programa sin mostrar mensajes tras los `printf`.
3. Tests unitarios y de regresión mediante `assertions`: Verificación aislada con `<assert.h>` y `memchr` para comprobar la terminación nula de los arrays tras `strncpy`.





# Evidencias de los Probleos Identificados

 1.  ARR30-C — Acceso a punteros argv sin validar argc
  * Test o entrada: Ejecución sin argumentos por línea de comandos (.\exe).
  * Técnica: Test funcional de caso límite por omisión de parámetros.
  * Resultado en código original (original_test.exe): Fallo crítico (Crash / Violación de acceso). El programa imprime el nombre del archivo y finaliza inmediatamente al ejecutar strcpy(key, argv[1]) sobre un puntero nulo (NULL), sin llegar a solicitar entrada por teclado.   
  * Resultado en código corregido (fixed.exe): Ejecución controlada. La guarda if (argc >= 3) detecta la ausencia de argumentos, asigna "Argumentos insuficientes" al buffer de forma segura mediante snprintf y permite que el flujo continúe hasta terminar con código de salida 0.   
  
2.  MSC24-C / STR31-C — Desbordamiento de pila en get_y_or_n mediante gets(response)
    * Test o entrada: Ejecución estándar (.\exe a b) e introducción de una cadena de más de 80 caracteres 'A' en la pregunta Continue? [y] n:.   
    * Técnica: Test funcional con caso límite de desbordamiento de entrada (Buffer Overflow).
    * Resultado en código original (original_test.exe): Fallo crítico (Stack Smashing). La función gets escribe fuera de los 8 bytes del array response y sobrescribe la dirección de retorno en la pila. El sistema operativo aborta el proceso de inmediato al pulsar Enter, impidiendo que se impriman las cadenas posteriores (Foobar).   
    * Resultado en código corregido (fixed.exe): Ejecución segura. La función fgets(response, sizeof(response), stdin) trunca la entrada a un máximo de 7 caracteres legibles y añade el terminador nulo \0. La pila permanece íntegra, el programa imprime Foobar y finaliza correctamente.   

3. STR30-C — Modificación de literal de cadena en .rodata (ptr_char[0] = 'N')
    * Test o entrada: Ejecución normal respondiendo y (.\exe a b) y lectura inmediata de la variable de entorno $LASTEXITCODE en PowerShell.   
    * Técnica: Análisis de código de retorno del sistema operativo tras ejecución funcional.
    * Resultado en código original (original_test.exe): Fallo por violación de acceso. Aunque los mensajes printf se muestran en pantalla, la instrucción ptr_char[0] = 'N' intenta escribir en la sección de memoria de solo lectura (.rodata). $LASTEXITCODE devuelve -1073741819 (0xC0000005, Access Violation).   
    * Resultado en código corregido (fixed.exe): Ejecución limpia. La variable se declara como un array en la pila (char ptr_char[]), permitiendo la modificación de sus elementos sin invadir memoria protegida. $LASTEXITCODE devuelve 0.   
    
4. STR32-C — Ausencia de terminador nulo tras strncpy hacia array3[16]
    * Test o entrada: Ejecución del binario unitario .\tests\test_str32.exe.
    * Técnica: Test unitario y de regresión automatizado mediante aserciones (<assert.h>) e inspección de memoria con memchr.
    * Resultado en código original: Vulnerabilidad confirmada. La función memchr(array3_orig, '\0', 16) devuelve NULL, demostrando que al truncar una cadena de 17 caracteres sobre un buffer de 16, strncpy no inserta el carácter nulo. La posterior llamada a strlen(array3) provoca una lectura fuera de límites (Out-of-bounds read).   
    * Resultado en código corregido: Verificación por aserción superada. Se copian sizeof - 1 bytes y se fuerza array3[15] = '\0'. memchr localiza el terminador y assert(strlen(array3_fixed) == 15) valida que la lectura se mantiene dentro de los límites seguros de memoria.   
5. STR31-C — Desbordamiento del array key[24] con argumentos extensos
    * Test o entrada: Ejecución pasando dos argumentos de 20 caracteres cada uno: .\exe AAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBB.   
    * Técnica: Test funcional de caso límite sobrepasando el límite de almacenamiento.
    * Resultado en código original (original_test.exe): Corrupción de memoria / Comportamiento indefinido. La concatenación con strcpy y strcat genera una cadena de más de 43 bytes sobre un espacio reservado de 24, invadiendo variables contiguas en la pila.   
    * Resultado en código corregido (fixed.exe): Truncamiento seguro. La función snprintf(key, sizeof(key), ...) limita la salida a 23 caracteres legibles más el byte nulo terminador, garantizando que el array nunca desborde su memoria asignada.




# TECNICAS DE VERIFICACION

# Prueba 1 — ARR30-C (Falta de comprobación de argumentos `argc`):

1. Original 
PS> .\original_test.exe
exampleStrings.c
PS> # El programa sufre un Crash inmediato por desreferencia de puntero NULL en strcpy(key, argv[1])

2. Fixed
PS> .\fixed.exe
exampleStrings_fixed.c

Continue? [y] n: n
PS> # La condición if (argc >= 3) previene el error y permite una ejecución controlada


# Prueba 2 — STR31-C (Desbordamiento de buffer en `key[24]` mediante argumentos largos `argv`):**

1. Original 
PS> .\original_test.exe AAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBB
exampleStrings.c

Continue? [y] n: y
Foobar
Foobar

Hello
World



Hello
World
PS> #Se queda pillado/ Undefined Behavior

2. Fixed
PS> .\fixed.exe AAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBB
exampleStrings_fixed.c

Continue? [y] n: y
Foobar
Foobar

Hello
World


Hello
World


# Prueba 3 — MSC24-C / STR31-C (Desbordamiento de buffer en gets vs fgets)
1. Original 
PS-> .\original_test.exe a b                 
exampleStrings.c

Continue? [y] n: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
PS> # Aborta por corrupcion de pila sin imprimir Foobar

2. Fixed
PS-> .\fixed.exe a b
exampleStrings_fixed.c

Continue? [y] n: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Foobar
Foobar

Hello
World


Hello
World

# Prueba 4 — STR30-C (Violación de acceso al modificar ptr_char verificada con $LASTEXITCODE):

1. Original 
PS> .\original_test.exe a b
exampleStrings.c

Continue? [y] n: y
Foobar
Foobar
...
PS> $LASTEXITCODE
-1073741819

2. Fixed
PS> .\fixed.exe a b
exampleStrings_fixed.c

Continue? [y] n: y
Foobar
Foobar
...
PS> $LASTEXITCODE
0

# Prueba 5 — STR32-C (Test unitario y de regresión en tests/test_str32.c):
PS> gcc -std=c11 -Wall -Wextra -Wpedantic tests/test_str32.c -o tests/test_str32.exe
PS> .\tests\test_str32.exe
[Original] ¿Existe terminador '\0' dentro de los 16 bytes de array3?: NO (VULNERABLE: strlen leera fuera de limites)
[Corregido] ¿Existe terminador '\0' dentro de los 16 bytes de array3?: SI (strlen seguro = 15)
EXITO: Todas las aserciones (assertions) de verificacion se cumplieron.

# Declaración de uso de IA

+ herramienta utilizada; Gemini Pro
+ tareas para las que se utilizó; 
    * Explicación de fallos
    * Consulta, correlación y justificación técnica de las reglas CERT C
    * Interpretación de códigos de error del sistema operativo
    * Estructuración y redacción técnica
+ cómo se verificaron sus respuestas; Buscando en google otras fuentes que verifiquen la respuesta
+ al menos un ejemplo relevante de utilización: Interpretacion del error del caracter R utilizado en C++ y no en C
+ tiempo necesario para hacer este ejercicio: ~4horas


