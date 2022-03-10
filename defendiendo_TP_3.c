#include "comandos.h"
#include <string.h>

#define COMANDO_RANKING "ranking"
#define COMANDO_CREAR_CAMINO "crear_camino"
#define COMANDO_CREAR_CONFIG "crear_configuracion"
#define COMANDO_VER_REPETICION "poneme_la_repe"
#define COMANDO_JUGAR "jugar"

const int ERROR = -1;

/*
 * PRE: ---
 *
 * POST: Imprime por pantalla mensaje de error tras recibirse un comando desconocido/incorrecto.
 */
void mostrar_mensaje_error(){
	printf("\nERROR: Ingresá alguno de los siguientes comandos :)\n");
	printf("\t> ranking\n");
	printf("\t> crear_camino\n");
	printf("\t> crear_configuracion\n");
	printf("\t> poneme_la_repe\n");
	printf("\t> jugar\n\n");
}


int main (int argc, char* argv[]){
	
	bool jugar_default = false;

	if (argc > 1){
		
		if ( strcmp (argv[1], COMANDO_RANKING) == 0){
			return ( manejar_visualizacion_ranking(argv[2], argv[3]) );

		}if ( strcmp (argv[1], COMANDO_CREAR_CAMINO) == 0){
			return ( manejar_creacion_de_caminos (argv[2]) );
		
		}if ( strcmp (argv[1], COMANDO_CREAR_CONFIG) == 0){
			return ( manejar_creacion_configuracion (argv[2]) );

		}if ( strcmp (argv[1],COMANDO_VER_REPETICION) == 0){
			return ( manejar_poneme_la_repe (argv[2], argv[3]) );

		}if ( strcmp (argv[1],COMANDO_JUGAR) == 0){
			return ( manejar_jugar_juego(argv[2], argv[3], jugar_default) );
		
		}mostrar_mensaje_error();
		return ERROR;

	}jugar_default = true;
	return ( manejar_jugar_juego(argv[2], argv[3], jugar_default) );
}