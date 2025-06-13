#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTOS 100
#define MAX_NOTA 256

typedef struct {
    char nombre[50];
    char apellidos[50];
    char telefono[20];
    char correo[50];
    char nota[MAX_NOTA];
} Contacto;

Contacto agenda[MAX_CONTACTOS];
int total_contactos = 0;
char archivo_actual[100];

void cargarContactos();
void guardarContactos();
void agregarContacto();
void eliminarContacto();
void listarContactos();
void buscarContacto();
void ordenarContactos();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("No hay archivo\n", argv[0]);
        return 1;
    }

    strcpy(archivo_actual, argv[1]);
    cargarContactos();

    int opcion;
    do {
        printf("\n--- AGENDA DE CONTACTOS ---\n");
        printf("1. Agregar contacto\n");
        printf("2. Eliminar contacto\n");
        printf("3. Mostrar contactos\n");
        printf("4. Buscar contacto\n");
        printf("5. Salir\n");
        printf("Opción: ");
        scanf("%d", &opcion);
        getchar(); 

        switch (opcion) {
            case 1: agregarContacto(); break;
            case 2: eliminarContacto(); break;
            case 3: listarContactos(); break;
            case 4: buscarContacto(); break;
            case 5: guardarContactos(); printf("Adiós.\n"); break;
            default: printf("Opción inválida.\n");
        }
    } while (opcion != 5);

    return 0;
}

void cargarContactos() {
    FILE *f = fopen(archivo_actual, "r");
    if (!f) return;

    while (fscanf(f, "%50|%50|%20|%50|%256[^\n]\n",
                  agenda[total_contactos].nombre,
                  agenda[total_contactos].apellidos,
                  agenda[total_contactos].telefono,
                  agenda[total_contactos].correo,
                  agenda[total_contactos].nota) == 5) {
        total_contactos++;
    }

    fclose(f);
}

void guardarContactos() {
    FILE *f = fopen(archivo_actual, "w");
    if (!f) {
        printf("Error al guardar el archivo.\n");
        return;
    }

    for (int i = 0; i < total_contactos; i++) {
        fprintf(f, "%s|%s|%s|%s|%s\n",
                agenda[i].nombre,
                agenda[i].apellidos,
                agenda[i].telefono,
                agenda[i].correo,
                agenda[i].nota);
    }

    fclose(f);
}

void agregarContacto() {
    if (total_contactos >= MAX_CONTACTOS) {
        printf("Agenda llena.\n");
        return;
    }

    printf("Nombre: ");
    fgets(agenda[total_contactos].nombre, sizeof(agenda[total_contactos].nombre), stdin);
    agenda[total_contactos].nombre[strcspn(agenda[total_contactos].nombre, "\n")] = '\0';
    printf("Apellidos: ");
    fgets(agenda[total_contactos].apellidos, sizeof(agenda[total_contactos].apellidos), stdin);
    agenda[total_contactos].apellidos[strcspn(agenda[total_contactos].apellidos, "\n")] ='\0';
    printf("Teléfono: ");
    fgets(agenda[total_contactos].telefono, sizeof(agenda[total_contactos].telefono), stdin);
    agenda[total_contactos].telefono[strcspn(agenda[total_contactos].telefono, "\n")] = '\0';
    printf("Correo: ");
    fgets(agenda[total_contactos].correo, sizeof(agenda[total_contactos].correo), stdin);
    agenda[total_contactos].correo[strcspn(agenda[total_contactos].correo, "\n")] = '\0';
    printf("Nota: ");
    fgets(agenda[total_contactos].nota, sizeof(agenda[total_contactos].nota), stdin);
    agenda[total_contactos].nota[strcspn(agenda[total_contactos].nota, "\n")] = '\0';
    total_contactos++;
    guardarContactos();
    printf("Contacto agregado.\n");
}

void eliminarContacto() {
    char nombre[50];
    printf("Nombre del contacto a eliminar: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    for (int i = 0; i < total_contactos; i++) {
        if (strcmp(agenda[i].nombre, nombre) == 0) {
            for (int j = i; j < total_contactos - 1; j++) {
                agenda[j] = agenda[j + 1];
            }
            total_contactos--;
            guardarContactos();
            printf("Contacto eliminado.\n");
            return;
        }
    }
    printf("Contacto no encontrado.\n");
}

void listarContactos() {
    if (total_contactos == 0) {
        printf("No hay contactos.\n");
        return;
    }

    ordenarContactos();

    for (int i = 0; i < total_contactos; i++) {
        printf("\n[%d] Nombre: %s\nApellidos: %s\nTeléfono: %s\nCorreo: %s\nNota: %s\n",
               i + 1, agenda[i].nombre, agenda[i].apellidos, agenda[i].telefono, agenda[i].correo, agenda[i].nota);
    }
}

void buscarContacto() {
    char criterio[100];
    int encontrado = 0;

    printf("Introduce nombre, apellido, teléfono o correo para buscar: ");
    fgets(criterio, sizeof(criterio), stdin);
    criterio[strcspn(criterio, "\n")] = '\0';

    for (int i = 0; i < total_contactos; i++) {
        if (strstr(agenda[i].nombre, criterio) != NULL ||
            strstr(agenda[i].apellidos, criterio) != NULL ||
            strstr(agenda[i].telefono, criterio) != NULL ||
            strstr(agenda[i].correo, criterio) != NULL) {
            printf("\n[%d] Nombre: %s\nApellidos: %s\nTeléfono: %s\nCorreo: %s\nNota: %s\n",
                   i + 1, agenda[i].nombre, agenda[i].apellidos, agenda[i].telefono, agenda[i].correo, agenda[i].nota);
            encontrado = 1;
        }
    }

    if (!encontrado)
        printf("No se encontró ningún contacto.\n");
}

void ordenarContactos() {
    for (int i = 0; i < total_contactos - 1; i++) {
        for (int j = i + 1; j < total_contactos; j++) {
            if (strcmp(agenda[i].nombre, agenda[j].nombre) > 0) {
                Contacto temp = agenda[i];
                agenda[i] = agenda[j];
                agenda[j] = temp;
            }
        }
    }
}
