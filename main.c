/*
Numero de grupo: 9

Integrantes:
- Alan Ezequiel Bernal
- Guillermo Estrada Dubor


Resultados de la comparacion de las estructuras:
                |        Esfuerzo Maximo                                        |
                |        Localizacion           |               |               |
                | Exitosa       | Fracaso       | Alta Ex       | Baja Ex       |
LSO:                    60.00           42.00           58.00           47.00
LSOBB:                  5.00            5.00            58.00           47.00
ABB:                    12.00           11.00           1.50            1.50
                |        Esfuerzo Medio                                         |
                |        Localizacion           |               |               |
                | Exitosa       | Fracaso       | Alta Ex       | Baja Ex       |
LSO:                    23.97           16.75           15.07           16.77
LSOBB:                  4.59            4.00            15.07           16.77
ABB:                    5.71            6.27            1.50            0.98

En base a los resultados obtenidos de los esfuerzos de las estructuras, podemos observar
que el Arbol Binario de Busqueda tiene costos casi constantes para las operaciones de
Altas y Bajas. En cuanto a la localizacion, aunque la Lista secuencial ordenada con Busqueda Binaria
tiene los maximos mas bajos, las medias del ABB y la LSOBB son muy proximas.
Tambien se puede observar que el desempeño de la LSO es la menos optima para las operaciones
realizadas sobre las estructuras.

*/


#include <stdio.h>
#include <stdlib.h>

#include "RebalseAL.h"
#include "RebalseAC.h"
#include "RebalseS.h"


// --- CADENAS GLOBALES
#define PANTALLA_BARRA "-----------------------------------------------------------------------------------\n"
#define PANTALLA_PRINCIPAL_OPERACIONES "\
1. Comparacion de Estructuras\n\
2. Mostrar Estructura\n\
3. Salir del programa\n"


// codigos de operaciones
#define CODOP_ALTA 1
#define CODOP_BAJA 2
#define CODOP_EVOCAR 3

// --- DEFINICION DE MACROS
// controla si una variable tipo 'char' es distinta de 's' y 'n' (util para entradas de tipo SI o NO)
#define entradaDistintaSino(X) ((X) != 's' && (X) != 'n')
// impresion de un envio en el formato por defecto (debe recibir un ENVIO por valor, NO puntero)
#define imprimirEnvio(E) printf("Codigo de Envio:\t\t\t%s\n"\
                                 "DNI del Receptor:\t\t\t%u\n"\
                                 "Nombre y Apellido del Receptor:\t\t%s\n"\
                                 "Domicilio del Receptor:\t\t\t%s\n"\
                                 "DNI del Remitente:\t\t\t%u\n"\
                                 "Nombre y Apellido del Remitente:\t%s\n"\
                                 "Fecha de Envio:\t\t\t\t%s\n"\
                                 "Fecha de Recepcion:\t\t\t%s\n",\
                                 (E).codigo_envio,(E).dni_receptor,\
                                 (E).nombre_apellido_receptor,(E).domicilio_receptor,\
                                 (E).dni_remitente,(E).nombre_apellido_remitente,\
                                 (E).fecha_envio,(E).fecha_recepcion);


int Lectura_Operaciones(RebalseAL *ral, RebalseAC *rac,
                        Costos_estructura *ral_costos, Costos_estructura *rac_costos
                        ) {
    FILE *fichero;
    int operacion, auxiliar;

    Envio nuevo_envio; Envio_init(&nuevo_envio); //variable temporal

    fichero = fopen("Operaciones-Envios.txt","r"); //abrir el archivo
    if (fichero == NULL) return 0;

    else {
        while(!feof(fichero)){

            fscanf(fichero, "%d", &operacion);

            fscanf(fichero," %[^\n]",nuevo_envio.codigo_envio);
            strcpy(nuevo_envio.codigo_envio,strupr(nuevo_envio.codigo_envio));

            if(operacion == CODOP_ALTA || operacion == CODOP_BAJA){

                fscanf(fichero,"%u",&nuevo_envio.dni_receptor);

                fscanf(fichero," %[^\n]",nuevo_envio.nombre_apellido_receptor);
                strcpy(nuevo_envio.nombre_apellido_receptor,strupr(nuevo_envio.nombre_apellido_receptor));

                fscanf(fichero," %[^\n]",nuevo_envio.domicilio_receptor);
                strcpy(nuevo_envio.domicilio_receptor,strupr(nuevo_envio.domicilio_receptor));

                fscanf(fichero,"%u",&nuevo_envio.dni_remitente);

                fscanf(fichero," %[^\n]",nuevo_envio.nombre_apellido_remitente);
                strcpy(nuevo_envio.nombre_apellido_remitente,strupr(nuevo_envio.nombre_apellido_remitente));

                fscanf(fichero," %[^\n]",nuevo_envio.fecha_envio);

                fscanf(fichero," %[^\n]",nuevo_envio.fecha_recepcion);

                if(operacion == CODOP_ALTA) {
                    RAL_alta(ral,&nuevo_envio,ral_costos);
                    RAC_alta(rac,&nuevo_envio,rac_costos);
                } else {
                    RAL_baja(ral, &nuevo_envio, ral_costos);
                    RAC_baja(rac, &nuevo_envio, rac_costos);
                }
            } else if (operacion == CODOP_EVOCAR) {
                RAL_evocar(ral,nuevo_envio.codigo_envio,&nuevo_envio,ral_costos);
                RAC_evocar(rac,nuevo_envio.codigo_envio,&nuevo_envio,rac_costos);

            }

        }
    }

    fclose(fichero);

    return 1;
}

int main()
{
    // Declaracion e inicializacion de las estructuras
    RebalseAL envios_ral; RAL_init(&envios_ral);
    RebalseAC envios_rac; RAC_init(&envios_rac);

    // Costos de cada estructura
    Costos_estructura envios_ral_costos, envios_rac_costos;
    Costos_estructura_init(&envios_ral_costos), Costos_estructura_init(&envios_rac_costos);

    int hubo_memorizacion = 0;

    // INICIO DEL PROGRAMA
    char seleccion_usuario_menu_principal = ' '; // variable para guardar la opcion elegida por usuario
    do {
        system("cls");  // limpa la pantalla

        printf(PANTALLA_BARRA
               "EL REVOLEO\tAdministracion de envios\n"
               PANTALLA_BARRA
               "MENU PRINCIPAL\nElija una operacion:\n\n"
               PANTALLA_PRINCIPAL_OPERACIONES
               "\n"
               ">> ");

        // Se captura la opcion ingresada por el usuario
        fflush(stdin); seleccion_usuario_menu_principal = getchar();

        // SELECCION DE OPERACION
        // ----------------------------------------------------------
        switch (seleccion_usuario_menu_principal) {

            // Comparar estructuras
            case '1': {
                if (!hubo_memorizacion) {
                    Lectura_Operaciones(&envios_ral, &envios_rac,
                                        &envios_ral_costos, &envios_rac_costos);

                    // Calculo de costos medios
                    Costos_estructura_calculoMedias(&envios_ral_costos);
                    Costos_estructura_calculoMedias(&envios_rac_costos);

                    hubo_memorizacion = 1;
                }
                // Impresion por pantalla
                system("cls");
                printf(PANTALLA_BARRA
                       "Comparacion de esfuerzos de estructuras\n"
                       PANTALLA_BARRA
                       "\n'N' es el tamaño del vector de costo correspondiente.\n\n"
                       "\t\t| N = %u\t| N = %u\t|\n"
                       "\n\t\t|\t Esfuerzo Maximo\t|\n"
                       "\t\t|\t Evocacion\t\t|\n"
                       "\t\t| Exitosa\t| Fracaso\t|\n"
                       "RAL:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "RAC:\t\t\t%.2f\t\t%.2f\t\t\n"

                       "\n\t\t|\t Esfuerzo Medio\t\t|\n"
                       "\t\t|\t Evocacion\t\t|\n"
                       "\t\t| Exitosa\t| Fracaso\t|\n"
                       "RAL:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "RAC:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "\n\n",
                       envios_ral_costos.Evocacion_exitosa.cantidad,
                       envios_ral_costos.Evocacion_fallida.cantidad,

                       envios_ral_costos.Evocacion_exitosa.maximo,
                       envios_ral_costos.Evocacion_fallida.maximo,

                       envios_rac_costos.Evocacion_exitosa.maximo,
                       envios_rac_costos.Evocacion_fallida.maximo,

                       envios_ral_costos.Evocacion_exitosa.media,
                       envios_ral_costos.Evocacion_fallida.media,

                       envios_rac_costos.Evocacion_exitosa.media,
                       envios_rac_costos.Evocacion_fallida.media

                       );
                system("pause");
                break; // termina el switch
            }
            // Mostrar estructuras
            case '2': {
                // si no hubo memorizacion realizada
                if (hubo_memorizacion) {

                    // variable para respuesta de usuario
                    char seleccion_usuario_estructura;

                    do {
                        system("cls");
                        printf(PANTALLA_BARRA
                               "Mostrar estructuras\n"
                               PANTALLA_BARRA
                               "\nSeleccione la estructura que desea ver:\n"
                               "1. RAL\n"
                               "2. RAC\n"
                               "3. RS\n"
                               "4. Volver al menu principal\n\n"
                               "> ");


                        // Captura de respuesta del usuario
                        fflush(stdin); seleccion_usuario_estructura = getchar();

                        system("cls");
                        switch (seleccion_usuario_estructura) {
                            case '1': {
                                RAL_mostrarLista(&envios_ral);
                                printf("\n\nSe han impreso %d Envios.\n", envios_ral.cantidad);
                                system("pause");
                                break;
                            };
                            case '2': {
                                RAC_mostrarLista(&envios_rac);
                                printf("\n\nSe han impreso %d Envios.\n", envios_rac.cantidad);
                                system("pause");
                                break;
                            };
                            case '3': {
                                break;
                            }
                        }

                    } while (seleccion_usuario_estructura == '4'); break; // termina el switch
                } else {
                    system("cls");
                    printf(PANTALLA_BARRA
                           "Mostrar estructuras\n"
                           PANTALLA_BARRA
                           "\nDebe realizar la memorizacion de las estructuras antes de continuar!\n"
                           "Ejecute 'Comparar estructuras' en el menu principal\n\n"
                           );
                    system("pause");
                }
            }
        }

    } while (seleccion_usuario_menu_principal != '3');


    return 0;
}
