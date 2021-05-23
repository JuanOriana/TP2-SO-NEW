# TP2 - OS

Construccion de sistema operativo y estructura y herramientas de manejo de recursos.

## Installation

Ingrese al directorio Toolchain y ejecute el siguiente comando:
```bash
make all
```

Luego regrese al direcotiro principal y ejecute el siguiente comando:
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
  * **Comando1**: llenar con comandos
  
  
 Adicionalmente, la shell permite las siguientes combinaciones especiales
  * **ctrl+s**: guarda los registros
  * **ctrl+l**: hace un clear de la pantalla
  * **ctrl+c**: mata al proceso en FG
  * **ctrl+d**: manda un EOF
  
 

## Licencia
[MIT](https://choosealicense.com/licenses/mit/)
