#ifndef __COMANDOS_H__
#define __COMANDOS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ARGUMENTO 25
#define MAX_RUTA 25


/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> 0 Si se puede mostrar el ranking: comandos válidos y existe el ranking correspondiente a la configuración ingresada.
 *			> -1 (ERROR) Si no se puede mostrar el ranking: comandos no válidos o no se encontró el archivo del ranking.
 *						 Muestra el correspondiente mensaje de error.
 */
int manejar_visualizacion_ranking (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO]);


/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> 0 si se crearon satisfactoriamente todos los caminos, y luego fueron copiados al correspondiente archivo.
 *			> Error (-1) si no se recibió una ruta de archivo de caminos válida, mostrando previamente un mensaje a modo de alerta.
 *						 También devuelve error si no se pudo acceder al archivo al momento de copiar los caminos en él.
 *		
 */	
int manejar_creacion_de_caminos (char ruta_archivo_camino[MAX_RUTA]);


/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> 0 si se logró inicializar correctamente la configuración y posteriormente se copiaron los campos al archivo de la configuración.
 *			> Error (-1) si no se recibió una ruta de archivo de configuración válida, mostrando previamente un mensaje a modo de alerta.
 *						 También devuelve error si no se pudo acceder al archivo al momento de copiar los campos de la configuración en él.
 *		
 */	
int manejar_creacion_configuracion (char ruta_archivo_configuracion[MAX_RUTA]);


/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> 0 si se logró acceder a la grabación y la misma fue mostrada/impresa por pantalla con éxito.
 *			> Error (-1) si no se recibió una ruta de archivo de grabación, mostrando previamente un mensaje a modo de alerta.
 *						 También devuelve error si no se pudo acceder al archivo, ya sea porque la ruta era incorrecta o por algún otro motivo.
 *		
 */	
int manejar_poneme_la_repe (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO]);


/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> 0 si se logró llevar a cabo la ejecución del juego con éxito, cargando previamente la configuración y los caminos personalizados, y
 *				obteniendo la ruta del archivo de ranking.
 *			> Error (-1) si se recibieron parámetros no válidos.
 *		
 */	
int manejar_jugar_juego (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO], bool jugar_default);


#endif /* __COMANDOS_H__ */