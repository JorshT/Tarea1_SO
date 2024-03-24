#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void copyFile(const char *src, const char *dest) {
  char command[1024];
  snprintf(command, sizeof(command), "cp \"%s\" \"%s\"", src, dest);
  system(command);
}

void f_alfabeto() {
  char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char dirname[3];

  mkdir("Alfabetico",0777);
  chdir("Alfabetico");

  for (int i = 0; i < 26; i++) {
    snprintf(dirname, sizeof(dirname), "%c", alphabet[i]);
    mkdir(dirname,0777);
    // printf("Se ha creado la carpeta %s.\n", dirname);
  }
  chdir("..");
}

void f_generacion() {
  mkdir("Generación",0777);
  chdir("Generación");
  mkdir("I",0777);
  mkdir("II",0777);
  mkdir("III",0777);
  mkdir("IV",0777);
  chdir("..");
}

void writePokemonCountToFile(const char *filename, int *count, const char *labels[], int size, const char *title) {
  FILE *file = fopen(filename, "a"); 
  if (file == NULL) {
    perror("Error al abrir el archivo");
    exit(EXIT_FAILURE);
  }
  fprintf(file, "%s\n", title);
  for (int i = 0; i < size; i++) {
    fprintf(file, "%s - Cantidad%d\n", labels[i], count[i]);
  }

  fclose(file);
}

int main() {
  FILE * file = fopen("RegistroPokemon.txt", "w");
  fclose(file);
  f_alfabeto();
  f_generacion();

  int pokemonCountByLetter[26] = {0};   
  int pokemonCountByGeneration[4] = {0}; 

  DIR *d;
  struct dirent *dir;

  d = opendir("Sprites");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (strstr(dir->d_name, ".png")) {
        char firstLetter = toupper(dir->d_name[0]);
        char destDir[1024];
        char srcPath[1024];
        char destPath[1024];
        char generationDir[20];

        if (isalpha(firstLetter)) {
          pokemonCountByLetter[firstLetter - 'A']++;
          snprintf(destDir, sizeof(destDir), "Alfabetico/%c", firstLetter);
          snprintf(srcPath, sizeof(srcPath), "Sprites/%s", dir->d_name);

          snprintf(destPath, sizeof(destPath), "%s/%s", destDir, dir->d_name);

          copyFile(srcPath, destPath);
        }

        int pokedexNumber;
        sscanf(dir->d_name, "%*[^_ ]_%d", &pokedexNumber);

        if (pokedexNumber >= 1 && pokedexNumber <= 151) {
          strcpy(generationDir, "Generación/I");
          pokemonCountByGeneration[0]++;
        } else if (pokedexNumber >= 152 && pokedexNumber <= 251) {
          strcpy(generationDir, "Generación/II");
          pokemonCountByGeneration[1]++;
        } else if (pokedexNumber >= 252 && pokedexNumber <= 386) {
          strcpy(generationDir, "Generación/III");
          pokemonCountByGeneration[2]++;
        } else if (pokedexNumber >= 387 && pokedexNumber <= 493) {
          strcpy(generationDir, "Generación/IV");
          pokemonCountByGeneration[3]++;
        } else {
          printf("Número de Pokédex fuera de rango: %d\n", pokedexNumber);
          continue;
        }

        snprintf(srcPath, sizeof(srcPath), "Sprites/%s", dir->d_name);

        snprintf(destPath, sizeof(destPath), "%s/%s", generationDir, dir->d_name);

        copyFile(srcPath, destPath);
      }
    }
    closedir(d);
  }

  const char *alphabetLabels[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I",
                                  "J", "K", "L", "M", "N", "O", "P", "Q", "R",
                                  "S", "T", "U", "V", "W", "X", "Y", "Z"};
  const char *generationLabels[] = {"I", "II","III", "IV"};

  writePokemonCountToFile("RegistroPokemon.txt", pokemonCountByGeneration, generationLabels, 4, "Generación");
  
  writePokemonCountToFile("RegistroPokemon.txt", pokemonCountByLetter, alphabetLabels, 26, "Alfabético");
  
  printf("Archivo RegistroPokemon.txt creado exitosamente.\n");

  return 0;
}
