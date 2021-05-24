# TP2 - OS

Construcción de sistema operativo y estructura y herramientas de manejo de recursos.

## Compilación y ejecución

Ingrese al directorio Toolchain y ejecute el siguiente comando:
```bash
make all
```

Luego regrese al directorio principal y ejecute el siguiente comando:
```bash
make MM=<MMTYPE> all
```
Donde <MMTYPE> puede tomar alguno de los siguientes 2 valores para el manejo de la memoria:
 
* FREE_LIST
* BUDDY
 
 El parametro se puede omitir y se usara FREE_LIST por defecto
## Uso de la Shell
Para usar un comando en la shell, es suficiente con escribir su nombre (con sus parametros de ser necesario) y apretar el enter. 
  Un proceso se puede correr en background poniendo un
  & al final de todo.
```bash
command &
```
  
  Pipe,sem y wc som pipeables mediante el caracter |
  
 ```bash
command1 | command2
```
  Aqui esta la lista de todos los posibles comandos:
  * **help**: muestra la lista de comandos y sus usos.
  * **inforeg**: imprime los valores de todos los registros en pantalla, para actualizar los valores se debe presionar ctrl + s.
  * **printmem**: imprime 32 bytes de memoria a partir de la dirección hexadecimal pasada como parámetro.
  * **time**: imprime el tiempo actual del sistema.
  * **cpuInfo**: imprime el modelo y el vendedor de la CPU.
  * **cpuTemp**: imprime la temperatura actual de la CPU.
  * **changeUsername**: cambia el nombre del usuario de la shell por el nombre pasado como parámetro.
  * **checkZeroException**: causa una excepción de división por cero.
  * **checkInvalidOpcodeException**: causa una excepción de código de operación inválido.
  * **showArgs**: imprime los parámetros pasados al comando.
  * **ps**: imprime el estado de los procesos activos.
  * **mem**: imprime el estado de la memoria.
  * **priotest**: prueba la implementación del manejo de prioridades del scheduler.
  * **schedtest**: prueba la implementación del scheduler del sistema.
  * **memtest**: prueba la implementación del memory manager.
  * **synctest**: prueba la implementación de la sincronización del sistema.
  * **nosynctest**: prueba la implementación de la sincronización del sistema (sin el uso de semáforos).
  * **loop**: imprime el ID del proceso actual cada una determinada cantidad de segundos.
  * **kill**: mata un proceso cuyo pid se pasa como parámetro.
  * **nice**: cambia la prioridad de un proceso cuyo pid se pasa como parámetro.
  * **block**: bloquea a un proceso cuyo pid se pasa como parámetro.
  * **unblock**: desbloquea a un proceso cuyo pid se pasa como parámetro.
  * **sem**: imprime el estado de los semáforos activos.
  * **pipe**: imprime el estado de los pipes activos.
  * **cat**: imprime el stdin tal como lo recipe.
  * **wc**: cuenta la cantidad de líneas del imput.
  * **filter**: filtra las vocales del input.
  * **phylo**: resuelve el problema de los filósofos.
  
  
 Adicionalmente, la shell permite las siguientes combinaciones especiales
  * **ctrl+s**: guarda los registros
  * **ctrl+l**: hace un clear de la pantalla
  * **ctrl+c**: mata al proceso en FG
  * **ctrl+d**: manda un EOF
  
 

## Licencia
[MIT](https://choosealicense.com/licenses/mit/)
