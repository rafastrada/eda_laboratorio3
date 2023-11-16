/*
Numero de grupo: 9

Integrantes:
- Alan Ezequiel Bernal
- Guillermo Estrada Dubor


Resultados de la comparacion de las estructuras:
                |        Esfuerzo Maximo        |
                |        Evocacion              |
                | Exitosa       | Fracaso       |
RAL:                    6.00            9.00
RAC:                    7.00            11.00
RS:                     5.00            4.00

                |        Esfuerzo Medio         |
                |        Evocacion              |
                | Exitosa       | Fracaso       |
RAL:                    1.68            3.87
RAC:                    1.72            3.56
RS:                     2.29            1.38

En base a los resultados obtenidos de los esfuerzos de las estructuras, podemos observar
que el Rebalse Separado tiene los esfuerzos maximos y medios mas bajos con respecto a las demas
estructuras (salvo en el caso de la media de la evocacion exitosa, pero por poca diferencia),
por lo que ante dicha situacion, la RS es la estructura mas optima.

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


int Lectura_Operaciones(RebalseAL *ral, RebalseAC *rac, RebalseS *rs,
                        Costos_estructura *ral_costos, Costos_estructura *rac_costos, Costos_estructura *rs_costos
                        ) {
    FILE *fichero;
    int operacion, auxiliar;

    Envio nuevo_envio, envio_auxiliar;
    Envio_init(&nuevo_envio); //variable temporal
    Envio_init(&envio_auxiliar);

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
                    RS_alta(rs,&nuevo_envio,rs_costos);
                } else {
                    RAL_baja(ral, &nuevo_envio, ral_costos);
                    RAC_baja(rac, &nuevo_envio, rac_costos);
                    RS_baja(rs, &nuevo_envio, rs_costos);
                }
            } else if (operacion == CODOP_EVOCAR) {
                RAL_evocar(ral,nuevo_envio.codigo_envio,&envio_auxiliar,ral_costos);
                RAC_evocar(rac,nuevo_envio.codigo_envio,&envio_auxiliar,rac_costos);
                RS_evocar(rs,nuevo_envio.codigo_envio,&envio_auxiliar,rs_costos);

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
    RebalseS envios_rs; RS_init(&envios_rs);

    // Costos de cada estructura
    Costos_estructura envios_ral_costos, envios_rac_costos, envios_rs_costos;
    Costos_estructura_init(&envios_ral_costos), Costos_estructura_init(&envios_rac_costos), Costos_estructura_init(&envios_rs_costos);

    int hubo_memorizacion = 0, cantidad_envios = 0;

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
                    Lectura_Operaciones(&envios_ral, &envios_rac, &envios_rs,
                                        &envios_ral_costos, &envios_rac_costos, &envios_rs_costos);

                    // Calculo de costos medios
                    Costos_estructura_calculoMedias(&envios_ral_costos);
                    Costos_estructura_calculoMedias(&envios_rac_costos);
                    Costos_estructura_calculoMedias(&envios_rs_costos);

                    hubo_memorizacion = 1;
                }
                // Impresion por pantalla
                system("cls");
                printf(PANTALLA_BARRA
                       "Comparacion de esfuerzos de estructuras\n"
                       PANTALLA_BARRA
                       "\n\t\t|\t Esfuerzo Maximo\t|\n"
                       "\t\t|\t Evocacion\t\t|\n"
                       "\t\t| Exitosa\t| Fracaso\t|\n"
                       "RAL:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "RAC:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "RS:\t\t\t%.2f\t\t%.2f\t\t\n"

                       "\n\t\t|\t Esfuerzo Medio\t\t|\n"
                       "\t\t|\t Evocacion\t\t|\n"
                       "\t\t| Exitosa\t| Fracaso\t|\n"
                       "RAL:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "RAC:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "RS:\t\t\t%.2f\t\t%.2f\t\t\n"
                       "\n\n",
                       envios_ral_costos.Evocacion_exitosa.maximo,
                       envios_ral_costos.Evocacion_fallida.maximo,

                       envios_rac_costos.Evocacion_exitosa.maximo,
                       envios_rac_costos.Evocacion_fallida.maximo,

                       envios_rs_costos.Evocacion_exitosa.maximo,
                       envios_rs_costos.Evocacion_fallida.maximo,

                       envios_ral_costos.Evocacion_exitosa.media,
                       envios_ral_costos.Evocacion_fallida.media,

                       envios_rac_costos.Evocacion_exitosa.media,
                       envios_rac_costos.Evocacion_fallida.media,

                       envios_rs_costos.Evocacion_exitosa.media,
                       envios_rs_costos.Evocacion_fallida.media

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
                                cantidad_envios = RS_mostrarLista(&envios_rs);
                                printf("\n\nSe han impreso %d Envios.\n", cantidad_envios);
                                system("pause");
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


    // se libera memoria de Rebalse separado
    RS_liberarMemoria(&envios_rs);

    return 0;
}
