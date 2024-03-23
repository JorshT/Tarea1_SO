#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

int buscar(char cadena[], char simbolo){
    int valor = 0;
    for(size_t i = 0; i < strlen(cadena); i++){
        if(cadena[i] == simbolo){
            valor = i;
            break;
        }
    }
    return valor;
}

int obtenerGeneracion(char cadena[]){

    int primero = buscar(cadena, '_');
    int segundo = buscar(cadena, '.');

    char gen[4];
    int indice = 0;

    for (int i = primero + 1; i < segundo; i++) {
        gen[indice++] = cadena[i];
    }

    gen[indice] = '\0';
    int entero = (int) strtol(gen, NULL, 10);
    return entero;
}

int main(){


    char *generaciones[] = {"I", "II", "III", "IV"};
    char alfabeto[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


    // Crear directorio "Alfabetico" si no existe
    system("mkdir -p Alfabetico");
    system("mkdir -p Generacion");

    // Crear directorios con nombres de carpetas Alfabeto
    for (size_t i = 0; i < sizeof(alfabeto); i++) {
        char mkdirCommand[50];  // Tamaño suficiente para contener el comando
        snprintf(mkdirCommand, sizeof(mkdirCommand), "mkdir -p Alfabetico/%c", alfabeto[i]);
        system(mkdirCommand);
    }

    // Crear directorios basados en las Generaciones
    for (size_t i = 0; i < sizeof(generaciones) / sizeof(char *); i++) {
        char mkdirCommand[50];  // Tamaño suficiente para contener el comando
        snprintf(mkdirCommand, sizeof(mkdirCommand), "mkdir -p Generacion/%s", generaciones[i]);
        system(mkdirCommand);
    }



    /* Lectura de archivos de Pokemones "Sprites" */

    DIR * folder;
    struct dirent * directorio;

    folder = opendir("Sprites");

    if(folder == NULL){
        printf("Error al abrir la carpeta\n");
        return 1;
    }

    while ((directorio = readdir(folder))) {
        char firstChar = toupper(directorio->d_name[0]);
        char cpCommand[512];
        snprintf(cpCommand, sizeof(cpCommand), "cp Sprites/%s Alfabetico/%c/", directorio->d_name, firstChar);
        system(cpCommand);

        char cpCommandGeneration[512];
        int numGeneracion = obtenerGeneracion(directorio->d_name);

        if (numGeneracion <= 151) {
            snprintf(cpCommandGeneration, sizeof(cpCommandGeneration), "cp Sprites/%s Generacion/%s/", directorio->d_name, "I");
        } else if (numGeneracion > 151 && numGeneracion <= 252) {
            snprintf(cpCommandGeneration, sizeof(cpCommandGeneration), "cp Sprites/%s Generacion/%s/", directorio->d_name, "II");
        } else if (numGeneracion > 252 && numGeneracion <= 386) {
            snprintf(cpCommandGeneration, sizeof(cpCommandGeneration), "cp Sprites/%s Generacion/%s/", directorio->d_name, "III");
        } else {
            snprintf(cpCommandGeneration, sizeof(cpCommandGeneration), "cp Sprites/%s Generacion/%s/", directorio->d_name, "IV");
        }

        // Ejecutar el comando
        system(cpCommandGeneration);
    }

    closedir(folder);


    // Contar la cantidad de archivos para cada directorio y escribirlo en el archivo RegistroPokemon.txt
    FILE *escritura = fopen("RegistroPokemon.txt", "w");
    if (escritura == NULL) {
        perror("Error al abrir archivo\n");
    } else {

        //int totalGen = 0;
        //int totalAlfab = 0;
        /* Escritura para la parte de Generaciones */
        fprintf(escritura,"Generacion\n");
        for (size_t i = 0; i < sizeof(generaciones) / sizeof(char *); i++) {
            char ruta[100];
            snprintf(ruta, sizeof(ruta), "Generacion/%s", generaciones[i]);

            folder = opendir(ruta);

            /* Explicacion formal: debido a la inclusión de los directorios . y .., que están presentes en cada directorio en el sistema de archivos de Unix. Estos directorios representan el directorio actual y el directorio padre, respectivamente por lo cual se consideran a la hora de recorrer los directorios*/

            int contador = -2; /* Se setea en -2 dado que que existen dos directorios extra . y .. */
            while ((directorio = readdir(folder))) {
                contador++;
            }
            closedir(folder);

            fprintf(escritura, "%s - %d\n", generaciones[i], contador);
        }


        /* Escritura para la parte de alfabeto */
        fprintf(escritura,"\nAlfabetico\n");
        for (size_t i = 0; i < sizeof(alfabeto); i++) {
            char ruta[100];
            snprintf(ruta, sizeof(ruta), "Alfabetico/%c", alfabeto[i]);

            folder = opendir(ruta);


            /* Explicacion formal: debido a la inclusión de los directorios . y .., que están presentes en cada directorio en el sistema de archivos de Unix. Estos directorios representan el directorio actual y el directorio padre, respectivamente por lo cual se consideran a la hora de recorrer los directorios*/

            int contador = -2; /* Se setea en -2 dado que que existen dos directorios extra . y .. */
            while ((directorio = readdir(folder))) {
                contador++;
            }
            closedir(folder);

            fprintf(escritura, "%c - %d\n", alfabeto[i], contador);
        }

        


        fclose(escritura);
    }
    
    return 0;

    /* Crea el directorio*/
    /* mkdir("temp", 0755); */

    /*Elimina el directorio que se crea*/
    
    /*
    rmdir("temp");

    if(mkdir("Alfabetico", 0755) == 0){
        printf("Carpeta creada con exito\n");
    }else{
        printf("Error al crear la carpeta\n");
    }
    */

    /*system("mkdir Generacion");
    system("mkdir Generacion/1 Generacion/2 Generacion/3 Generacion/4");
    */
    return 0;
}