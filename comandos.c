#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "comandos.h"
#include "defendiendo_torres.h"
#include "animos.h"
#include "utiles.h"

#define ANIMO_MALO  'M'
#define ANIMO_REGULAR 'R'
#define ANIMO_BUENO 'B'
#define CLAVE_CONFIG  "config"
#define CLAVE_LISTAR  "listar"
#define CLAVE_GRABACION  "grabacion"
#define CLAVE_VELOCIDAD  "velocidad"
#define DEF_ENANOS "enanos"
#define DEF_ELFOS "elfos"
#define RUTA_RANKING_DEFAULT "ranking.csv"
#define MAX_ARGUMENTO 25
#define MAX_TIPO_DEF 5
#define TOPE_TERRENO_NIVEL_1_2 15
#define TOPE_TERRENO_NIVEL_3_4 20
#define MAX_NOMBRE_CAMPO 20
#define MAX_NOMBRE 25
#define VACIO "\0"

typedef struct caminos {
	coordenada_t posicion_torre;
	coordenada_t coordenadas[MAX_LONGITUD_CAMINO];
	int tope_camino;
}camino_t;

typedef struct caminos_por_nivel {
	camino_t camino_1;
	camino_t camino_2;
}caminos_por_nivel_t;

typedef struct jugador{
	char nombre[MAX_NOMBRE];
	int puntaje;
}jugador_t;

const int EN_JUEGO = 0, GANADO = 1;
const int NIVEL_TERMINADO = 1;
const char ELFO = 'L', ENANO = 'G';
const char ESTE = 'E', OESTE = 'O', NORTE = 'N', SUR = 'S';
const char ARRIBA = 'W', IZQUIERDA = 'A', ABAJO = 'S', DERECHA = 'D';
const char ENTRADA = 'E', TORRE = 'T', CAMINO = 'X', PASTO = '~';
const char SEPARADOR = '=', CARACTER_NULO = '\0';
const int PRIMER_NIVEL = 1, SEGUNDO_NIVEL = 2, TERCER_NIVEL = 3, CUARTO_NIVEL = 4;

const int CANTIDAD_TORRES = 2, PRIMERA_TORRE = 1, SEGUNDA_TORRE = 2;
const int PRIMER_CAMINO = 1, SEGUNDO_CAMINO = 2;
const int RESISTENCIA_TORRES_DEFAULT = 600, RESISTENCIA_MIN = 0;

const int ENEMIGOS_INICIALES = 0;
const int MAX_ENEMIGOS_NIVEL_1 = 100, MAX_ENEMIGOS_NIVEL_2 = 200, MAX_ENEMIGOS_NIVEL_3 = 300, MAX_ENEMIGOS_NIVEL_4 = 500;

const int ENANOS_DEFAULT_NIV_1 = 5, ELFOS_DEFAULT_NIV_1 = 0;
const int ENANOS_DEFAULT_NIV_2 = 0, ELFOS_DEFAULT_NIV_2 = 5;
const int ENANOS_DEFAULT_NIV_3 = 3, ELFOS_DEFAULT_NIV_3 = 3;
const int ENANOS_DEFAULT_NIV_4 = 4, ELFOS_DEFAULT_NIV_4 = 4;

const int PROBABILIDAD_MIN_GOLPE_CRITICO = 0, PROBABILIDAD_MEDIA_GOLPE_CRITICO = 10, PROBABILIDAD_MAX_GOLPE_CRITICO = 25;
const int DEFENSORES_EXTRA_DEFAULT = 10; 
const int COSTO_DEFENSOR_EXTRA = 50, CANTIDAD_MIN_DEFENSORES_EXTRA = 1;

const int PROBABILIDAD_MAX = 100;
const int DIVISOR_FALLO = 2;

const static int VIDA_NULA_ORCO = 0;

const float VELOCIDAD_DEFAULT = 1;

const int MAX_RESISTENCIA_TORRE = 2500, MAX_DEFENSORES_INICIALES = 10, MAX_DEF_EXTRA = 7, MAX_VELOCIDAD = 2;

const int NIVEL_FINAL = 4;

const int LIMITE_MIN = 0, POSICION_AFUERA_MAPA = -1;

const int NO_SE_PUEDE_AGREGAR_DEFENSOR = -1;

const int CANT_ORCOS_PARA_DEF_EXTRA_NIVEL_1 = 25, CANT_ORCOS_PARA_DEF_EXTRA_NIVEL_2_3_4 = 50;
const int SI = 1, NO = 0;

const int MULTIPLICADOR_PUNTAJE = 1000;

const size_t TAMANIO_EXTENSION = 4;

const int VALOR_DEFAULT = -1;
const int VALOR_MINIMO = 0, VALOR_INICIAL = 0, SIN_VALOR = -1;
const static int ERROR = -1;



//-------------------------|| FUNCIONES VARIAS ||--------------------------//

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True Si el comando tiene una estructura válida:
 *				   		- Debe contener únicamente un '='
 *						- El '=' no debe encontrarse en la última posición.
 *
 *			> False Si el comando posee una estructura no válida.
 */
bool comando_tiene_esctructura_valida (char argumento[]){
	int i = 0;
	int contador = 0;
	int posicion;
	while (argumento[i] != CARACTER_NULO){
		
		if ( argumento[i] == SEPARADOR ){
			posicion = i;
			contador++;
		}i++;
	
	}return((contador == 1) && (posicion != (i-1)));
}


/*
 * PRE: Recibe un comando con estructura válida:
 *			- Debe contener únicamente un '='
 *			- El '=' no debe encontrarse en la última posición.
 *
 * POST: Divide al comando, devolviendo:
 *			> La palabra clave que se encuentra antes del '='.
 *			> El valor de la palabra clave, que se encuentra luego del '='.
 */
int obtener_clave_y_valor (char argumento[], char clave[], char valor[]){
	int i = 0;
	int j = 0;
	
	while (argumento[i] != SEPARADOR){
		clave[i] = argumento[i];
		i++;

	}clave[i] = CARACTER_NULO;
	i++;
	
	while ( argumento[i] != CARACTER_NULO){
		valor[j] = argumento[i];
		i++;
		j++;
	}valor[j] = CARACTER_NULO;

	return 0;
}

/*
 * PRE: Recibe dos caminos vacíos.
 *
 * POST: Devuelve los dos caminos inicializados:
 *			> Entrada y torre fuera del mapa para que no generen inconvenientes al crear el camino en un futuro.
 *			> Tope reducido a cero.
 */
void inicializar_caminos (camino_t* camino_1, camino_t* camino_2){
	
	camino_1->tope_camino = 0;
	camino_1->posicion_torre.fil = POSICION_AFUERA_MAPA;
	camino_1->posicion_torre.col = POSICION_AFUERA_MAPA;
	camino_1->coordenadas[0].fil = POSICION_AFUERA_MAPA;
	camino_1->coordenadas[0].col = POSICION_AFUERA_MAPA;
	
	camino_2->tope_camino = 0;
	camino_2->posicion_torre.fil = POSICION_AFUERA_MAPA;
	camino_2->posicion_torre.col = POSICION_AFUERA_MAPA;
	camino_2->coordenadas[0].fil = POSICION_AFUERA_MAPA;
	camino_2->coordenadas[0].col = POSICION_AFUERA_MAPA;
}

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True si la ruta posee extensión .txt
 *			> False si no posee dicha extensión.
 */
bool tiene_extension_txt (char ruta[MAX_RUTA]){
	
	char nombre[MAX_RUTA];
	char extension[TAMANIO_EXTENSION];
	sscanf(ruta, "%[^.].%s\n", nombre, extension);

	return (strcmp (extension, "txt") == 0);
}



//--------------------------|| COMANDO RANKING ||--------------------------//


/*
 * PRE: Recibe el nombre del archivo de la configuración a la cual pertenece el ranking que el usuario desea consultar.
 *		Dicho nombre debe contener su extensión correspondiente.
 *
 * POST: Devuelve la ruta/nombre del archivo del ranking en base a la configuración recibida.
 */
void obtener_ruta_archivo_ranking(char ruta_archivo_ranking[], char nombre_config[]){
	
	size_t tope_nombre_config = strlen(nombre_config);
	
	strcpy (ruta_archivo_ranking, "ranking_");
	strncat (ruta_archivo_ranking, nombre_config, (tope_nombre_config - TAMANIO_EXTENSION) );
	strcat (ruta_archivo_ranking, ".csv");
}


/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True Si los comandos del ranking son válidos.
 *				   Respetan estructura: listar=[valor] config=[valor] en cualquier orden y siendo ambos opcionales..
 *
 *			> False Si los comandos del ranking no son válidos. No respetan la estructura propuesta.
 *				   
 *			Además, obtiene correctamente la cantidad de jugadores que se desean listar y el nombre del archivo ranking.
 */
bool son_comandos_validos_ranking (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO], char ruta_archivo_ranking[MAX_RUTA], int* valor_listar){
	char clave_1[MAX_ARGUMENTO], valor_1[MAX_ARGUMENTO];
	char clave_2[MAX_ARGUMENTO], valor_2[MAX_ARGUMENTO];

	if (!argumento_1){
		strcpy (ruta_archivo_ranking, RUTA_RANKING_DEFAULT);
		*valor_listar = SIN_VALOR;
		return true;
	}

	if ( argumento_1 && comando_tiene_esctructura_valida(argumento_1) ){
		obtener_clave_y_valor (argumento_1, clave_1, valor_1);
		
		if (!argumento_2){
			if (strcmp(clave_1, CLAVE_CONFIG) == 0){
				*valor_listar = SIN_VALOR;
				obtener_ruta_archivo_ranking (ruta_archivo_ranking, valor_1);
				return true;
			
			}else if (strcmp(clave_1, CLAVE_LISTAR) == 0){
				*valor_listar = atoi (valor_1);
				strcpy (ruta_archivo_ranking, RUTA_RANKING_DEFAULT);
				return true;
			}
		
		}else if ( comando_tiene_esctructura_valida (argumento_2) ){
			
			obtener_clave_y_valor (argumento_2, clave_2, valor_2);

			if ( (strcmp(clave_1, CLAVE_LISTAR) == 0) && (strcmp(clave_2, CLAVE_CONFIG) == 0) ){
				*valor_listar = atoi (valor_1);
				obtener_ruta_archivo_ranking (ruta_archivo_ranking, valor_2);
				return true;
			
			}else if ( (strcmp(clave_1, CLAVE_CONFIG) == 0) && (strcmp(clave_2, CLAVE_LISTAR) == 0) ){
				*valor_listar = atoi (valor_2);
				obtener_ruta_archivo_ranking (ruta_archivo_ranking, valor_1);
				return true;
			}
		}
	}return false;
}


/*
 * PRE: Recibe la ruta/nombre correcto del archivo del ranking consultado perteneciente a la confi ingresada
 *      y la cantidad (válida) de jugadores que se desean visualizar.
 *
 * POST: Imprime por pantalla el ranking correcto, mostrando la cantidad de jugadores pedidos.
 */
int mostrar_ranking(char ruta_archivo[MAX_RUTA], int valor_listar){
	
	FILE* ranking = fopen(ruta_archivo, "r");
	
	if (!ranking){
		printf("\nERROR: No se pudo acceder al archivo :(\n\n");
		return ERROR;
	}
	
	jugador_t jugador;

	int cant_usuarios_leidos = 0;

	system("clear");
	printf("\n\t╔═════════════════════════════╗\n");
	printf("\t║·····R··A··N··K··I··N··G·····║\n");
	printf("\t╚═════════════════════════════╝\n\n");

	int leidos = fscanf(ranking, "%[^;];%i\n", jugador.nombre, &jugador.puntaje);
	
	while ( (leidos != EOF) && (cant_usuarios_leidos != valor_listar) ){
		
		printf("\t%i| %s: %i puntos.\n\n", (cant_usuarios_leidos + 1), jugador.nombre, jugador.puntaje);
		cant_usuarios_leidos++;
		leidos= fscanf(ranking, "%[^;];%i\n", jugador.nombre, &jugador.puntaje);
	}

	fclose(ranking);

	return 0;
}


/*
 * PRE: ---
 *
 * POST: Imprime por pantalla mensaje de error acerca de comandos de ranking no válidos.
 */
void mostrar_mensaje_comando_incorrecto_ranking (){
	printf("\nERROR: La clave ingresada es incorrecta! La estructura es la siguiente:\n");
	printf("\t> listar=[valor] config=[valor]\n");
	printf("\n\t(Los parámetros 'listar' y 'config' son opcionales)\n\n");
}



int manejar_visualizacion_ranking (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO]){
	char ruta_archivo_ranking[MAX_RUTA];
	int valor_listar;

	if ( son_comandos_validos_ranking (argumento_1, argumento_2, ruta_archivo_ranking, &valor_listar)){
		
		return ( mostrar_ranking(ruta_archivo_ranking, valor_listar) );
	
	}mostrar_mensaje_comando_incorrecto_ranking();
	return ERROR;
}




//-----------------------|| COMANDO CREAR CAMINOS ||-----------------------//


/*
 * PRE: Recibe el nivel correcto al que pertenece el camino cuestión.
 *
 * POST: Devuelve:
 *			> True si la entrada es válida: está dentro de los limites del mapa y coincide con el punto cardinal asignado al nivel.
 *			> False si la entrada no es válida: está fuera de los limites del mapa y/o no esta situada en la fila/columna correspondiente.
 */
bool es_valida_entrada (int nivel, coordenada_t entrada){
	
	if (nivel == PRIMER_NIVEL){
		return (( (entrada.fil >= LIMITE_MIN) && (entrada.fil < TOPE_TERRENO_NIVEL_1_2 ) ) &&
			   	  (entrada.col == TOPE_TERRENO_NIVEL_1_2 - 1 ));

	}else if (nivel == SEGUNDO_NIVEL){
		return (( (entrada.fil >= LIMITE_MIN) && (entrada.fil < TOPE_TERRENO_NIVEL_1_2 ) ) &&
			  	  (entrada.col == LIMITE_MIN));

	}else if (nivel == TERCER_NIVEL){
		return ( (entrada.fil == LIMITE_MIN) &&
			   ( ((entrada.col >= LIMITE_MIN) && (entrada.col < TOPE_TERRENO_NIVEL_3_4)) ));
	
	}else{
		return ( (entrada.fil == TOPE_TERRENO_NIVEL_3_4 - 1) &&
			   ( (entrada.col >= LIMITE_MIN) && (entrada.col < TOPE_TERRENO_NIVEL_3_4) ));
	}
}


/*
 * PRE: Recibe el nivel correcto al que pertenece el camino cuestión.
 *
 * POST: Devuelve:
 *			> True si la posición de la torre es válida: está dentro de los limites del mapa y coincide con el punto cardinal asignado al nivel.
 *			       Está opuesta a la entrada.
 *			> False si la posición de la torre no es válida: está fuera de los limites del mapa y/o no esta situada en la fila/columna correspondiente.
 */
bool es_valida_torre (int nivel, coordenada_t torre){
	
	if (nivel == PRIMER_NIVEL){
		return (( (torre.fil >= LIMITE_MIN) && (torre.fil < TOPE_TERRENO_NIVEL_1_2) ) &&
			   	  (torre.col == LIMITE_MIN ));

	}else if (nivel == SEGUNDO_NIVEL){
		return (( (torre.fil >= LIMITE_MIN) && (torre.fil < TOPE_TERRENO_NIVEL_1_2) ) &&
			  	  (torre.col == TOPE_TERRENO_NIVEL_1_2 - 1));

	}else if (nivel == TERCER_NIVEL){
		return ( (torre.fil == TOPE_TERRENO_NIVEL_3_4 - 1) &&
			   ( (torre.col >= LIMITE_MIN) && (torre.col < TOPE_TERRENO_NIVEL_3_4) ));
	
	}else{
		return ( (torre.fil == LIMITE_MIN) &&
			   ( (torre.col >= LIMITE_MIN) && (torre.col < TOPE_TERRENO_NIVEL_3_4) ));
	}
}

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True si dos coordenadas se superponen: coinciden tanto fila como columna.
 *			> False si las dos coordenadas NO se superponen.
 */
bool se_superpone (coordenada_t coord_actual, coordenada_t coord_anterior){
	
	return ( (coord_actual.fil == coord_anterior.fil) && (coord_actual.col == coord_anterior.col) );
}

/*
 * PRE: Recibe el camino, cuya creación está en proceso, correctamente inicializado y actualizado.
 *
 * POST: Devuelve:
 *			> True si el camino está completo: la última coordenada coincide con la posición de la torre.
 *			> False si el camino NO está completo: la última coordenada NO coincide con la posición de la torre fijada.
 */
bool esta_camino_completo (camino_t camino){
	
	coordenada_t ultima_posicion = camino.coordenadas[camino.tope_camino - 1];
	return ( (ultima_posicion.fil == camino.posicion_torre.fil) && (ultima_posicion.col == camino.posicion_torre.col) );
}

/*
 * PRE: Recibe de forma correcta y actualizada la posición actual elegida, la última posición del camino y el otro camino correspondiente al mismo nivel.
 *
 * POST: Devuelve:
 *			> True está habilitado el movimiento hacia arriba:
 *					- La coordenada estaría dentro de los limites del mapa.
 *					- La coordenada es distinta a la anteúltima coordenada del camino, de modo que no puede volver sobre sí mismo sin dar una vuelta.
 *					- La coordenada no se superpone a la torre ni entrada perteneciente al otro camino del mismo nivel (Aplica para niveles 3 y 4).
 *					
 *			> False NO está habilitado el movimiento hacia arriba.
 */
bool se_puede_mover_arriba (coordenada_t ultima_posicion, coordenada_t anteultima_posicion, camino_t camino_comparar){
	
	return ( (ultima_posicion.fil > LIMITE_MIN) && (ultima_posicion.fil - 1 != anteultima_posicion.fil) && 
 
		     ((ultima_posicion.fil - 1 != camino_comparar.coordenadas[0].fil) || (ultima_posicion.col != camino_comparar.coordenadas[0].col)) &&

		     ((ultima_posicion.fil - 1 != camino_comparar.posicion_torre.fil) || (ultima_posicion.col != camino_comparar.posicion_torre.col))    );
}

/*
 * PRE: Recibe de forma correcta y actualizada la posición actual elegida, la última posición del camino y el otro camino correspondiente al mismo nivel.
 *
 * POST: Devuelve:
 *			> True está habilitado el movimiento hacia la izquierda:
 *					- La coordenada estaría dentro de los limites del mapa.
 *					- La coordenada es distinta a la anteúltima coordenada del camino, de modo que no puede volver sobre sí mismo sin dar una vuelta.
 *					- La coordenada no se superpone a la torre ni entrada perteneciente al otro camino del mismo nivel (Aplica para niveles 3 y 4).
 *					
 *			> False NO está habilitado el movimiento hacia la izquierda.
 */
bool se_puede_mover_izquierda (coordenada_t ultima_posicion, coordenada_t anteultima_posicion, camino_t camino_comparar){

	return ( (ultima_posicion.col > LIMITE_MIN) && (ultima_posicion.col - 1 != anteultima_posicion.col) && 

		     ((ultima_posicion.col - 1 != camino_comparar.coordenadas[0].col) || (ultima_posicion.fil != camino_comparar.coordenadas[0].fil)) &&

		     ((ultima_posicion.col - 1 != camino_comparar.posicion_torre.col) || (ultima_posicion.fil != camino_comparar.posicion_torre.fil))    );
}

/*
 * PRE: Recibe de forma correcta y actualizada la posición actual elegida, la última posición del camino y el otro camino correspondiente al mismo nivel.
 *
 * POST: Devuelve:
 *			> True está habilitado el movimiento hacia abajo:
 *					- La coordenada estaría dentro de los limites del mapa.
 *					- La coordenada es distinta a la anteúltima coordenada del camino, de modo que no puede volver sobre sí mismo sin dar una vuelta.
 *					- La coordenada no se superpone a la torre ni entrada perteneciente al otro camino del mismo nivel (Aplica para niveles 3 y 4).
 *					
 *			> False NO está habilitado el movimiento hacia abajo.
 */
bool se_puede_mover_abajo (coordenada_t ultima_posicion, coordenada_t anteultima_posicion, camino_t camino_comparar, int limite_max){

	return ( (ultima_posicion.fil < (limite_max- 1 )) && (ultima_posicion.fil + 1 != anteultima_posicion.fil) && 

		     ((ultima_posicion.fil + 1 != camino_comparar.coordenadas[0].fil) || (ultima_posicion.col != camino_comparar.coordenadas[0].col)) &&

		     ((ultima_posicion.fil + 1 != camino_comparar.posicion_torre.fil) || (ultima_posicion.col != camino_comparar.posicion_torre.col))    );
}

/*
 * PRE: Recibe de forma correcta y actualizada la posición actual elegida, la última posición del camino y el otro camino correspondiente al mismo nivel.
 *
 * POST: Devuelve:
 *			> True está habilitado el movimiento hacia la derecha:
 *					- La coordenada estaría dentro de los limites del mapa.
 *					- La coordenada es distinta a la anteúltima coordenada del camino, de modo que no puede volver sobre sí mismo sin dar una vuelta.
 *					- La coordenada no se superpone a la torre ni entrada perteneciente al otro camino del mismo nivel (Aplica para niveles 3 y 4).
 *					
 *			> False NO está habilitado el movimiento hacia la derecha.
 */
bool se_puede_mover_derecha (coordenada_t ultima_posicion, coordenada_t anteultima_posicion, camino_t camino_comparar, int limite_max){

	return ( (ultima_posicion.col < (limite_max - 1)) && (ultima_posicion.col + 1 != anteultima_posicion.col) && 

		     ((ultima_posicion.col + 1 != camino_comparar.coordenadas[0].col) || (ultima_posicion.fil != camino_comparar.coordenadas[0].fil)) &&

		     ((ultima_posicion.col + 1 != camino_comparar.posicion_torre.col) || (ultima_posicion.fil != camino_comparar.posicion_torre.fil))    );
}

/*
 * PRE: Recibe de forma correcta y actualizada el camino actual, el otro camino correspondiente al mismo nivel, el nivel, y una letra ingresada por el usuario.
 *
 * POST: Realiza el movimiento de acuerdo a la letra recibida:
 			> W -> Movimiento superior, reduciendo en 1 la fila.
 			> A -> Movimiento hacia la izquierda, reduciendo en 1 la columna.
 			> S -> Movimiento inferior, aumentando en 1 la fila.
 			> D -> Movimiento hacia la derecha, aumentando en 1 la columna.

 			El movimiento se realizará siempre y cuando esté disponible, y se incrementará el tope del camino.
 */
bool se_puede_realizar_movimiento (char letra, camino_t* camino_nuevo, camino_t camino_comparar, int nivel){

	coordenada_t ultima_posicion = camino_nuevo->coordenadas[camino_nuevo->tope_camino - 1];
	coordenada_t anteultima_posicion = camino_nuevo->coordenadas[camino_nuevo->tope_camino - 2];
	int limite_max;

	if (nivel == PRIMER_NIVEL|| nivel == SEGUNDO_NIVEL){
		limite_max = TOPE_TERRENO_NIVEL_1_2;
	}else{
		limite_max = TOPE_TERRENO_NIVEL_3_4;
	}

	if ( letra == ARRIBA && se_puede_mover_arriba(ultima_posicion, anteultima_posicion, camino_comparar) ){
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].fil = ultima_posicion.fil - 1;
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].col = ultima_posicion.col;
		camino_nuevo->tope_camino ++;
		return true;
	
	}else if ( letra == IZQUIERDA && se_puede_mover_izquierda(ultima_posicion, anteultima_posicion, camino_comparar) ){
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].fil = ultima_posicion.fil;
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].col = ultima_posicion.col - 1;
		camino_nuevo->tope_camino ++;
		return true;
	
	}else if ( letra == ABAJO && se_puede_mover_abajo(ultima_posicion, anteultima_posicion, camino_comparar, limite_max) ){
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].fil = ultima_posicion.fil + 1;
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].col = ultima_posicion.col;
		camino_nuevo->tope_camino ++;
		return true;

	}else if ( letra == DERECHA && se_puede_mover_derecha(ultima_posicion, anteultima_posicion, camino_comparar, limite_max) ){
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].fil = ultima_posicion.fil;
		camino_nuevo->coordenadas[camino_nuevo->tope_camino].col = ultima_posicion.col + 1;
		camino_nuevo->tope_camino ++;
		return true;

	}else{
		return false;
	}
}

/*
 * PRE: Recibe de un terreno previamente cargado y su tope.
 *
 * POST: Imprime por pantalla el terreno que contiene a los caminos.
 */
void mostrar_mapa_caminos (char terreno[MAX_FILAS][MAX_COLUMNAS], int tope_terreno){

	printf("\n\t    ");
	for(int columna = 0; columna < tope_terreno; columna ++){
		if (columna < 10){
			printf("0%i|", columna);
		}else{
			printf("%i|", columna);
		}
	
	}printf("\n");

	for (int i = 0; i < tope_terreno; i++){
		if (i < 10){
			printf("\t0%i|", i);
		}else{
			printf("\t%i|", i);
		}
		for (int j = 0; j < tope_terreno; j++){
			printf(" %2c", terreno[i][j]);
		}printf("\n");
	}printf("\n");
}

/*
 * PRE: Recibe un camino actualizado junto a su tope.
 *
 * POST: El camino se cargará en la matriz que representa al terreno.
 */
void cargar_camino_a_terreno ( camino_t camino, char terreno[MAX_FILAS][MAX_COLUMNAS] ){
	
	for (int i = 0; i < camino.tope_camino; i++){
		if ( i == 0){
			terreno [camino.coordenadas[i].fil] [camino.coordenadas[i].col] = ENTRADA;
		}
		else{
			terreno [camino.coordenadas[i].fil] [camino.coordenadas[i].col] = CAMINO;
		}
	}terreno [camino.posicion_torre.fil] [camino.posicion_torre.col] = TORRE;
}

/*
 * PRE: Recibe el nivel correspondiente al camino que se está creando.
 *
 * POST: Imprime por pantalla las indicaciones respecto al posicionamiento de entradas y torres según nivel.
 */
void mostrar_mensaje_mapa_segun_nivel (int nivel){
	printf("Ingresá las coordenadas de la/s entradas y torre/s :)\n");

	if (nivel == PRIMER_NIVEL){
		printf("Acordate que se accede por el ESTE\n");
		printf("\t>ENTRADA: última columna del terreno y en cualquier fila\n");
		printf("\t>TORRE: primera columna del terreno y en cualquier fila\n");
	
	}else if (nivel == SEGUNDO_NIVEL){
		printf("Acordate que se accede por el OESTE\n");
		printf("\t>ENTRADA: primera columna del terreno y en cualquier fila\n");
		printf("\t>TORRE: última columna del terreno y en cualquier fila\n");
	
	}else if (nivel == TERCER_NIVEL){
		printf("Acordate que se accede por el NORTE\n");
		printf("\t>ENTRADA: primera fila del terreno y en cualquier columna\n");
		printf("\t>TORRE: ultima fila del terreno y en cualquier columna\n");
	
	}else{
		printf("Acordate que se accede por el SUR\n");
		printf("\t>ENTRADA: ultima fila del terreno y en cualquier columna\n");
		printf("\t>TORRE: primera fila del terreno y en cualquier columna\n");
	
	}
}

/*
 * PRE: ---
 *
 * POST: Devuelve una coordenada.
 */
void pedir_coordenadas (coordenada_t* coordenada){
	printf("Introducí las coordenadas!\n");
	printf("FILA: ");
	scanf("%i", &(coordenada->fil));
	printf("COLUMNA: ");
	scanf("%i", &(coordenada->col));
}

/*
 * PRE: Recibe el nivel correspondiente al camino que se está creando, y sus correspondientes dos caminos.
 *
 * POST: Imprime por pantalla el terreno con los dos caminos junto con un mensaje de presentación.
 */
void mostrar_interfaz_creacion_caminos(int nivel, camino_t camino_1, camino_t camino_2){

	printf("\t╔═════════════════════════════╗\n");
	printf("\t║···CREAR··CAMINO··NIVEL··%i···║\n", nivel);
	printf("\t╚═════════════════════════════╝\n");

	char terreno[MAX_FILAS][MAX_COLUMNAS];
	int tope_terreno;



	if (nivel == PRIMER_NIVEL || nivel == SEGUNDO_NIVEL){
		tope_terreno = TOPE_TERRENO_NIVEL_1_2;
	
	}else{
		tope_terreno = TOPE_TERRENO_NIVEL_3_4;
	}

	for (int i = 0; i <= tope_terreno; i++){
		for (int j = 0; j <= tope_terreno; j++){
			terreno[i][j] = PASTO;
		}
	}

	cargar_camino_a_terreno (camino_1, terreno);
	cargar_camino_a_terreno (camino_2, terreno);
	mostrar_mapa_caminos (terreno, tope_terreno);
}

/*
 * PRE: Recibe el nivel correspondiente al camino que se está creando, el camino en creación y el otro camino del mismo nivel.
 *
 * POST: Asigna una coordenada de entrada y torre válidas al camino en creación, previamente solicitadas al usuario, e incrementa el tope de dicho camino.
 */
void solicitar_entrada_y_torre (int nivel, camino_t* camino_nuevo, camino_t camino_comparar){
	
	coordenada_t entrada, torre;

	mostrar_mensaje_mapa_segun_nivel(nivel);
	
	printf("\n-ENTRADA-\n");
	
	pedir_coordenadas (&entrada);
	system("clear");
	mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);
	
	while ( !es_valida_entrada(nivel, entrada) || se_superpone(entrada, camino_comparar.coordenadas[0]) ){
		printf("\nLa entrada no es válida, volvé a intentarlo :(\n");
		pedir_coordenadas(&entrada);
		system("clear");
		mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);
	
	}camino_nuevo->coordenadas[0] = entrada;
	camino_nuevo->tope_camino ++;
	
	printf("\n-TORRE-\n");
	
	pedir_coordenadas (&torre);
	system("clear");
	mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);

	while ( !es_valida_torre(nivel, torre) || se_superpone(torre, camino_comparar.posicion_torre) ){
		printf("La posicion de torre no es válida, volvé a intentarlo :(\n");
		pedir_coordenadas(&torre);
		system("clear");
		mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);
	
	}camino_nuevo->posicion_torre = torre;
}

/*
 * PRE: Recibe el nivel correspondiente al camino que se está creando, el camino en creación y el otro camino del mismo nivel.
 *
 * POST: Imprime por pantalla las indicaciones respecto al movimiento a efectuar, lo solicita al usuario, y efectúa un movimiento válido.
 */
void solicitar_movimiento(int nivel, camino_t* camino_nuevo, camino_t camino_comparar){

	char letra;
	
	printf("Ingresá un caracter:\n");
	printf("\tW -> Moverse hacia arriba.\n");
	printf("\tA -> Moverse hacia la izquierda.\n");
	printf("\tS -> Moverse hacia abajo.\n");
	printf("\tD -> Moverse hacia la derecha.\n");

	scanf(" %c", &letra);
	while (!se_puede_realizar_movimiento (letra, camino_nuevo, camino_comparar, nivel)){
		printf("No se pudo realizar el movimiento :( Volvé a ingresar un caracter!\n");
		scanf(" %c", &letra);
	}
}

/*
 * PRE: Recibe el vector que contiene todos los caminos correctamente creados y actualizados pertenecientes a todos los niveles.
 *		También, recibe una ruta de un archivo válida.
 *
 * POST: Copia los caminos a un archivo de texto con la ruta ingresada por el usuario, identificando el nivel al que pertenecen y si se trata del
 *		 primer o segundo camino.
 *		 En caso de no poder acceder al archivo, imprime por pantalla un mensaje de error.
 */
int copiar_camino_en_archivo( char ruta_archivo_camino[MAX_RUTA], caminos_por_nivel_t caminos[MAX_NIVELES]){

	FILE* archivo_caminos = fopen (ruta_archivo_camino, "w");

	if(!ruta_archivo_camino){
		printf("\nERROR: No se pudo acceder al archivo :(\n");
		return ERROR;
	}

	for (int nivel = PRIMER_NIVEL; nivel <= MAX_NIVELES; nivel++){
		fprintf(archivo_caminos, "NIVEL=%i\n", nivel );
		
		if ( nivel != SEGUNDO_NIVEL){
			fprintf (archivo_caminos, "CAMINO=1\n");
			for (int j = 0; j < caminos[nivel-1].camino_1.tope_camino; j++){
				fprintf ( archivo_caminos, "%i;%i\n", caminos[nivel-1].camino_1.coordenadas[j].fil,caminos[nivel-1].camino_1.coordenadas[j].col );
			}
		
		}if ( nivel != PRIMER_NIVEL){
			fprintf (archivo_caminos, "CAMINO=2\n");
			for ( int j = 0; j < caminos[nivel-1].camino_2.tope_camino; j++){
				fprintf ( archivo_caminos, "%i;%i\n", caminos[nivel-1].camino_2.coordenadas[j].fil,caminos[nivel-1].camino_2.coordenadas[j].col );
			}
		}
	}

	fclose(archivo_caminos);
	return 0;
}

/*
 * PRE: Recibe el nivel correspondiente al camino que se está creando, el camino en creación y el otro camino del mismo nivel.
 *
 * POST: Devuelve un camino válido completamente creado de forma correcta:
 *			> El camino está completo.
 *			> El camino respeta los limites del mapa según el nivel.
 *			> El camino no excede la máxima longitud (200 coordenadas).
 */
void crear_camino_aux (int nivel, camino_t* camino_nuevo, camino_t camino_comparar){

	camino_nuevo->tope_camino = VALOR_INICIAL;

	system("clear");
	mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);
	solicitar_entrada_y_torre(nivel, camino_nuevo, camino_comparar);
	
	while ( !esta_camino_completo(*camino_nuevo) && (camino_nuevo->tope_camino < MAX_LONGITUD_CAMINO) ){
		system("clear");
		mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);
		solicitar_movimiento(nivel, camino_nuevo, camino_comparar);
	
	}mostrar_interfaz_creacion_caminos(nivel, *camino_nuevo, camino_comparar);
	
	if ( camino_nuevo->tope_camino == MAX_LONGITUD_CAMINO ){
		crear_camino_aux(nivel, camino_nuevo, camino_comparar);
	}
}

/*
 * PRE: Recibe una ruta del archivo de camino existente (no vacía) y válida (extensión .txt).
 *
 * POST: Devuelve un vector de caminos que contiene todos los caminos válidos creados completamente para cada nivel, 
 *		 los cuales luego son copiados y guardados en un archivo de texto.
 */
int crear_caminos (char ruta_archivo_camino[MAX_RUTA]){

	caminos_por_nivel_t caminos[MAX_NIVELES];

	for (int nivel = PRIMER_NIVEL; nivel <= MAX_NIVELES; nivel++){
		
		inicializar_caminos ( &(caminos[nivel-1].camino_1), &(caminos[nivel-1].camino_2) );
		
		if( nivel != SEGUNDO_NIVEL){
			crear_camino_aux (nivel, &(caminos[nivel-1].camino_1), caminos[nivel-1].camino_2);
		
		}if (nivel != PRIMER_NIVEL){
			crear_camino_aux (nivel, &(caminos[nivel-1].camino_2), caminos[nivel-1].camino_1);
		}

	}return ( copiar_camino_en_archivo(ruta_archivo_camino, caminos) );

}

/*
 * PRE: ---
 *
 * POST: Imprime por pantalla un mensaje de error alertando al usuario sobre la ausencia de una ruta de archivo donde se guardarán los caminos.
 */
void mostrar_mensaje_comando_incorrecto_caminos (){
	printf("\nERROR: Te olvidaste de ingresar el nombre del archivo donde se guardará el camino o el mismo tiene extensión incorrecta :)\n");
	printf("\tIngresalo de esta forma: 'nombre_del_archivo'.txt\n\n");
}



int manejar_creacion_de_caminos (char ruta_archivo_camino[MAX_RUTA]){
	if ( !ruta_archivo_camino || !tiene_extension_txt(ruta_archivo_camino) ){
		mostrar_mensaje_comando_incorrecto_caminos();
		return ERROR;
	}

	return ( crear_caminos (ruta_archivo_camino));
}



//--------------------|| COMANDO CREAR CONFIGURACION ||--------------------//


/*
 * PRE: Recibe de forma correcta cuál es la torre ( torre 1 o torre 2).
 *
 * POST: Se obtiene mediante el usuario una resistencia de torre válida:
 *			> La resistencia es estrictamente mayor a cero y es menor o igual a 2500 (limite máximo).
 *			> O la resistencia es igual a -1 (valor por defecto). 
 */
void obtener_resistencia_torres (int* resistencia_torre, int nro_de_torre){
	
	printf("\n■ Introducí la resistencia de la torre %i (Mayor a cero y menor o igual a %i): ", nro_de_torre, MAX_RESISTENCIA_TORRE);
	scanf("%i", resistencia_torre);
	
	while( ( (*resistencia_torre <= VALOR_MINIMO) || (*resistencia_torre > MAX_RESISTENCIA_TORRE) ) && (*resistencia_torre != VALOR_DEFAULT) ){
		printf("Resistencia no válida :( Introducí la resistencia de la torre %i: ", nro_de_torre);
		scanf("%i", resistencia_torre);
	}
}

/*
 * PRE: Recibe el tipo de defensor del cual se quiere inicializar las cantidades iniciales.
 *
 * POST: Se obtiene un vector con las cantidades iniciales válidas de dicho tipo de defensor para cada nivel:
 *			> La cantidad de defensores de ese tipo es mayor o igual a cero y es menor o igual a 10 (limite máximo).
 *			> O la cantidad de defensores de ese tipo es igual a -1 (valor por defecto). 
 */
void obtener_cant_inicial_defensores (int cantidad_defensores[MAX_NIVELES], char defensor[MAX_TIPO_DEF]){

	for (int i = 0; i < MAX_NIVELES; i++){
		printf("\n■ Introducí la cantidad inicial de %s para el nivel %i (Mayor o igual a cero y menor o igual a %i): ", defensor, i+1, MAX_DEFENSORES_INICIALES);
		scanf("%i", &cantidad_defensores[i]);
		
		while ( ((cantidad_defensores[i] < VALOR_MINIMO) || (cantidad_defensores[i] > MAX_DEFENSORES_INICIALES)) && (cantidad_defensores[i] != VALOR_DEFAULT )){
			printf("Cantidad no válida! Introducí la cantidad inicial de %s para el nivel %i: ", defensor, i+1);
			scanf("%i", &cantidad_defensores[i]);
		}
	}
}

/*
 * PRE: Recibe el tipo de defensor del cual se quiere inicializar las cantidades extra.
 *
 * POST: Se obtiene la cantidad extra limite de defensores de ese tipo que se podrán añadir durante la partida.
 *			> La cantidad de defensores extra de ese tipo es mayor o igual a cero y es menor o igual a 7 (limite máximo).
 *			> O la cantidad de defensores extra de ese tipo es igual a -1 (valor por defecto). 
 */
void obtener_cant_defensores_extra (info_defensor_t* info_defensor, char defensor[MAX_TIPO_DEF]){

	printf("\n■ Introducí la cantidad de %s extras disponibles (Mayor o igual a cero y menor o igual a %i): ", defensor, MAX_DEF_EXTRA);
	scanf("%i", &(info_defensor->cantidad_extra)) ;

	while ( ((info_defensor->cantidad_extra < VALOR_MINIMO) || (info_defensor->cantidad_extra > MAX_DEF_EXTRA)) && (info_defensor->cantidad_extra != VALOR_DEFAULT) ){
		printf("Cantidad de %s extras no válida :( Volvé a ingresarla: ", defensor);
		scanf("%i", &(info_defensor->cantidad_extra));
	}
}

/*
 * PRE: Recibe el tipo de defensor del cual se quiere inicializar el fallo y el crítico.
 *
 * POST: Se obtiene un porcentaje de fallo y de golpe crítico válidos (entre cero y 100).
 *		 También, puede ser -1 (valor por defecto).
 *
 */
void obtener_animo (info_defensor_t* info_defensor, char defensor[MAX_TIPO_DEF]){

	printf("\n■ Introducí el fallo de los %s (Entre 0 y 100): ", defensor);
	scanf("%i", &(info_defensor->fallo));

	while ( (info_defensor->fallo < VALOR_MINIMO || info_defensor->fallo > PROBABILIDAD_MAX ) && (info_defensor->fallo != VALOR_DEFAULT) ){
		printf("Fallo de %s no válido :( Volvé a ingresarlo: ", defensor);
		scanf("%i", &(info_defensor->fallo));
	}

	printf("\n■ Introducí el critico de los %s (Entre 0 y 100): ", defensor);
	scanf("%i", &(info_defensor->critico));

	while ( (info_defensor->critico < VALOR_MINIMO || info_defensor->critico > PROBABILIDAD_MAX ) && (info_defensor->critico != VALOR_DEFAULT) ){
		printf("Crítico de %s no válido :( Volvé a ingresarlo: ", defensor);
		scanf("%i", &(info_defensor->critico));
	}
}

/*
 * PRE: ---
 *
 * POST: Se obtiene la velocidad con la que se visualizará el juego, siendo mayor a cero y menor a 2.
 *		 También, puede ser -1 (valor por defecto).
 */
void obtener_velocidad_juego (float* velocidad_juego){

	printf("\n■ Introducí la velocidad del juego! (Mayor a cero y menor o igual a %i): ", MAX_VELOCIDAD);
	scanf("%f", velocidad_juego);

	while ( ((*velocidad_juego < VALOR_MINIMO) || (*velocidad_juego > MAX_VELOCIDAD)) && (*velocidad_juego != VALOR_DEFAULT) ){
		printf("Velocidad no válida :( Volvé a ingresarla: ");
		scanf("%f", velocidad_juego);
	}
}


/*
 * PRE: ---
 *
 * POST: Se obtiene la ruta donde se encuentra del archivo de texto .txt que contiene a los caminos a utilizar.
 *		 También, puede contener -1 (caminos por defecto).
 */
void obtener_caminos (char ruta_archivo_camino[MAX_RUTA]){
	
	printf("\n■ Introducí la ruta del archivo que contiene los caminos:\n ");
	scanf("%s", ruta_archivo_camino);
	while ( !tiene_extension_txt(ruta_archivo_camino) ){
		printf("\nLa ruta introducida no tiene extensión .txt!:\n ");
		scanf("%s", ruta_archivo_camino);
	}
}

/*
 * PRE: ---
 *
 * POST: Devuelve una configuración con todos los campos inicializados correctamente de acuerdo a los valores proporcionados por el usuario.
 */
void crear_configuracion(configuracion_t* configuracion){

	system("clear");
	printf("\t╔═════════════════════════════╗\n");
	printf("\t║····CREAR···CONFIGURACIÓN····║\n");
	printf("\t╚═════════════════════════════╝\n");
	printf("IMPORTANTE: Para mantener el valor por defecto, ingresá '-1'\n");

	obtener_resistencia_torres( &(configuracion->resistencia_torre_1), PRIMERA_TORRE);
	obtener_resistencia_torres( &(configuracion->resistencia_torre_2), SEGUNDA_TORRE);

	obtener_cant_inicial_defensores( configuracion->info_enanos.cantidad_inicial, DEF_ENANOS );
	obtener_cant_inicial_defensores( configuracion->info_elfos.cantidad_inicial, DEF_ELFOS );
	obtener_cant_defensores_extra( &(configuracion->info_enanos), DEF_ENANOS );
	obtener_cant_defensores_extra( &(configuracion->info_elfos), DEF_ELFOS );
	obtener_animo( &(configuracion->info_enanos), DEF_ENANOS );
	obtener_animo( &(configuracion->info_elfos), DEF_ELFOS );
	
	obtener_velocidad_juego( &(configuracion->velocidad_juego) );
	obtener_caminos( configuracion->ruta_archivo_camino );

}

/*
 * PRE: Recibe una ruta de archivo donde se guardará la configuración. Debe tener extensión .txt
 *
 * POST: Devuelve: 
 *			> 0 Si copió exitosamente los valores de la configuración ya inicializada en el archivo de texto en cuestión.
 *			> Error (-1) Si no se pudo acceder al archivo en el que se debe guardar la configuración.
 */
int copiar_configuracion_en_archivo (configuracion_t* configuracion, char ruta_archivo_configuracion[MAX_RUTA]){

	FILE* archivo_configuracion = fopen (ruta_archivo_configuracion, "w");

	if(!archivo_configuracion){
		printf("ERROR: No se pudo acceder al archivo de configuración\n");
		return ERROR;
	}

	fprintf (archivo_configuracion, "RESISTENCIA_TORRES=<%i>,<%i>\n", configuracion->resistencia_torre_1, configuracion->resistencia_torre_2 );
	
	fprintf (archivo_configuracion, "ENANOS_INICIO=<%i>,<%i>,<%i>,<%i>\n", configuracion->info_enanos.cantidad_inicial[0], 
																		   configuracion->info_enanos.cantidad_inicial[1],
																		   configuracion->info_enanos.cantidad_inicial[2],
																		   configuracion->info_enanos.cantidad_inicial[3] );
	
	fprintf (archivo_configuracion, "ELFOS_INICIO=<%i>,<%i>,<%i>,<%i>\n", configuracion->info_elfos.cantidad_inicial[0], 
																		  configuracion->info_elfos.cantidad_inicial[1],
																		  configuracion->info_elfos.cantidad_inicial[2],
																		  configuracion->info_elfos.cantidad_inicial[3] );
	
	fprintf (archivo_configuracion, "ENANOS_EXTRA=<%i>\n", configuracion->info_enanos.cantidad_extra );

	fprintf (archivo_configuracion, "ELFOS_EXTRA=<%i>\n", configuracion->info_elfos.cantidad_extra );

	fprintf (archivo_configuracion, "ENANOS_ANIMO=<%i>,<%i>\n", configuracion->info_enanos.fallo, configuracion->info_enanos.critico );
	
	fprintf (archivo_configuracion, "ELFOS_ANIMO=<%i>,<%i>\n", configuracion->info_elfos.fallo, configuracion->info_elfos.critico );
	
	fprintf (archivo_configuracion, "VELOCIDAD=<%f>\n", configuracion->velocidad_juego);
	
	fprintf (archivo_configuracion, "CAMINOS=<%s>\n", configuracion->ruta_archivo_camino );
	

	fclose(archivo_configuracion);

	return 0;
}

/*
 * PRE: ---
 *
 * POST: Imprime por pantalla un mensaje de error alertando al usuario sobre la ausencia de una ruta de archivo de texto 
 *		 donde se guardará la configuración.
 */
void mostrar_mensaje_comando_incorrecto_config(){
	printf("\nERROR: Te olvidaste de ingresar la ruta del archivo donde se guardará la configuración o tiene extensión no válida!\n");
	printf("\tIngresalo de esta forma: 'nombre_del_archivo'.txt\n\n");
}


int manejar_creacion_configuracion(char ruta_archivo_configuracion[MAX_RUTA]){
	
	if ( !ruta_archivo_configuracion || !tiene_extension_txt(ruta_archivo_configuracion) ){
		mostrar_mensaje_comando_incorrecto_config();
		return ERROR;
	}

	configuracion_t configuracion;

	crear_configuracion(&configuracion);
	
	return (copiar_configuracion_en_archivo(&configuracion, ruta_archivo_configuracion));
}



//----------------------|| COMANDO PONEME LA REPE ||-----------------------//


/*
 * PRE: ---
 *
 * POST: Imprime por pantalla un mensaje a modo de introducción/bienvendia para ver la grabación. 
 */
void mostrar_mensaje_grabacion(){
	system("clear");
	printf("\t╔═════════════════════════════╗\n");
	printf("\t║·······VER···GRABACIÓN·······║\n");
	printf("\t╚═════════════════════════════╝\n");

	printf("\n\tA continuación, observarás la grabación que ingresaste!\n");
	detener_el_tiempo(0.75);
	system("clear");
}

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True Si los comandos de la grabación son válidos.
 *				   Respetan estructura: grabacion=[valor] velocidad=[valor] en cualquier orden y siendo 'velocidad=[valor]' opcional.
 *
 *			> False Si los comandos del ranking no son válidos. No respetan la estructura propuesta.
 *				   
 *			Ademàs, obtiene correctamente la ruta donde se encuentra la grabación y la velocidad a la que se quiere ver.
 */
bool son_comandos_validos_repeticion(char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO], char ruta_archivo_grabacion[MAX_RUTA], float* velocidad_juego){
	
	char clave_1[MAX_ARGUMENTO], valor_1[MAX_ARGUMENTO];
	char clave_2[MAX_ARGUMENTO], valor_2[MAX_ARGUMENTO];

	if ( argumento_1 && comando_tiene_esctructura_valida(argumento_1) ){
		obtener_clave_y_valor (argumento_1, clave_1, valor_1);
		
		if (!argumento_2){
			if (strcmp(clave_1, CLAVE_GRABACION) == 0){
				*velocidad_juego = VELOCIDAD_DEFAULT;
				strcpy(ruta_archivo_grabacion, valor_1);
				return true;
			}
		
		}else if ( comando_tiene_esctructura_valida (argumento_2) ){
			
			obtener_clave_y_valor (argumento_2, clave_2, valor_2);

			if ( (strcmp(clave_1, CLAVE_VELOCIDAD) == 0) && (strcmp(clave_2, CLAVE_GRABACION) == 0) ){
				*velocidad_juego = (float) atof (valor_1);
				strcpy(ruta_archivo_grabacion, valor_2);
				return true;
			
			}else if ( (strcmp(clave_1, CLAVE_GRABACION) == 0) && (strcmp(clave_2, CLAVE_VELOCIDAD) == 0) ){
				*velocidad_juego = (float) atof (valor_2);
				strcpy(ruta_archivo_grabacion, valor_1);
				return true;
			}
		}
	}return false;
}

/*
 * PRE: Recibe una ruta de archivo donde se leerá la grabación. Debe tener extensión .dat
 *
 * POST: Devuelve: 
 *			> 0 Si se pudo acceder al archivo y leer la grabación. Además, se muestra la grabación por pantalla.
 *			> Error (-1) Si no se pudo acceder al archivo correspondiente con la ruta ingresada.
 */
int mostrar_grabacion (char ruta_archivo_grabacion[], float velocidad_juego){

	FILE* archivo_grabacion = fopen(ruta_archivo_grabacion, "r");

	if(!archivo_grabacion){
		printf("ERROR: No se pudo acceder a la grabación :( Acordate que la extensión debe ser .dat!\n");
		return ERROR;
	}

	juego_t fotograma_juego;

	while (!feof(archivo_grabacion)){
		system ("clear");
		fread (&fotograma_juego, sizeof(juego_t), 1, archivo_grabacion);
		mostrar_juego (fotograma_juego);
		detener_el_tiempo (velocidad_juego);
	}

	fclose(archivo_grabacion);

	return 0;
}

/*
 * PRE: ---
 *
 * POST: Imprime por pantalla un mensaje de error alertando al usuario acerca del ingreso incorrecto de las instrucciones por comando para ver una grabación.
 */
void mostrar_mensaje_comando_incorrecto_repe(){
	printf("\nERROR: La clave ingresada es incorrecta! La estructura es la siguiente:\n");
	printf("\t> grabacion=[valor] velocidad=[valor]\n");
	printf("\n\t(El parametro 'velocidad' es opcional y la extensión del archivo de la grabación debe ser .dat!)\n\n");
}


int manejar_poneme_la_repe (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO]){
	
	char ruta_archivo_grabacion[MAX_RUTA];
	float velocidad_juego;

	if ( son_comandos_validos_repeticion (argumento_1, argumento_2, ruta_archivo_grabacion, &velocidad_juego) ){
		
		mostrar_mensaje_grabacion();
		return ( mostrar_grabacion(ruta_archivo_grabacion, velocidad_juego) );
	
	}mostrar_mensaje_comando_incorrecto_repe();
	return ERROR;

}


//---------------------------|| COMANDO JUGAR ||---------------------------//

//LECTURA DE CONFIGURACIÓN Y CAMINOS:

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración previamente inicializada.
 *
 * POST: Devuelve el valor de un campo de configuración actualizado:
 *			Si se está trabajando con configuracion por defecto o si contiene un -1 lo actualiza al valor correspondiente de dicho campo.
 *			De lo contrario, el valor se mantiene igual.
 */
void actualizar_valor ( int* campo_config, int valor_por_defecto, bool es_config_default_completa ){
	
	if (*campo_config == VALOR_DEFAULT || es_config_default_completa){
		*campo_config = valor_por_defecto;
	}
}

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración previamente inicializada.
 *
 * POST: Devuelve:
 *			> True si se requiere llamar a animos: se está trabajando con configuración por defecto, o bien, algún porcentaje de fallo o
 *				   golpe crítico fue inicializado en -1.
 *			> False si no se requiere llamar a animos, es decir, no se necesita ningún valor de fallo o crítico por defecto.
 */
bool se_necesita_animos (configuracion_t configuracion, bool es_config_default_completa){
	
	if (es_config_default_completa){
		return true;
	
	}else{
		return (configuracion.info_enanos.fallo  ==  VALOR_DEFAULT || configuracion.info_elfos.fallo   == VALOR_DEFAULT ||
				configuracion.info_enanos.critico == VALOR_DEFAULT || configuracion.info_elfos.critico == VALOR_DEFAULT   );
	}
}

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración previamente inicializada.
 *
 * POST: Devuelve el valor actualizado de las resistencias de las torres:
 *			> Se actualiza a valor por defecto si corresponde.
 *			> De lo contrario queda igual.
 */
void cargar_resistencia_default(configuracion_t* configuracion, bool es_config_default_completa){
	
	actualizar_valor ( &(configuracion->resistencia_torre_1), RESISTENCIA_TORRES_DEFAULT, es_config_default_completa ); 
	actualizar_valor ( &(configuracion->resistencia_torre_2), RESISTENCIA_TORRES_DEFAULT, es_config_default_completa ); 
}

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración previamente inicializada.
 *
 * POST: Devuelve el valor actualizado de las cantidades iniciales de defensores según tipo y nivel:
 *			> Se actualiza a valor por defecto si corresponde.
 *			> De lo contrario queda igual.
 */
void cargar_defensores_iniciales_default(configuracion_t* configuracion, bool es_config_default_completa){

	actualizar_valor ( &(configuracion->info_enanos.cantidad_inicial[0]), ENANOS_DEFAULT_NIV_1, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_enanos.cantidad_inicial[1]), ENANOS_DEFAULT_NIV_2, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_enanos.cantidad_inicial[2]), ENANOS_DEFAULT_NIV_3, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_enanos.cantidad_inicial[3]), ENANOS_DEFAULT_NIV_4, es_config_default_completa );

	actualizar_valor ( &(configuracion->info_elfos.cantidad_inicial[0]), ELFOS_DEFAULT_NIV_1, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_elfos.cantidad_inicial[1]), ELFOS_DEFAULT_NIV_2, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_elfos.cantidad_inicial[2]), ELFOS_DEFAULT_NIV_3, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_elfos.cantidad_inicial[3]), ELFOS_DEFAULT_NIV_4, es_config_default_completa );
}

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración previamente inicializada.
 *
 * POST: Devuelve el valor actualizado de las cantidades extra de defensores según tipo:
 *			> Se actualiza a valor por defecto si corresponde.
 *			> De lo contrario queda igual.
 */
void cargar_defensores_extra_default (configuracion_t* configuracion, bool es_config_default_completa){
	
	actualizar_valor ( &(configuracion->info_enanos.cantidad_extra), DEFENSORES_EXTRA_DEFAULT, es_config_default_completa ); 
	actualizar_valor ( &(configuracion->info_elfos.cantidad_extra) , DEFENSORES_EXTRA_DEFAULT, es_config_default_completa ); 
}

/*
 * PRE: El ánimo del personaje (Legolas o Gimli) debe ser válido:
 *	   > ANIMO_MALO
 *	   > ANIMO_REGULAR
 *	   > ANIMO_BUENO
 *
 * POST: Devuelve el porcentaje de golpe critico/letalidad, el cual es: GOLPE_CRITICO_MIN, GOLPE_CRITICO_MEDIO o GOLPE_CRITICO_MAX
 */
int calculo_golpe_critico (char animo_de_personaje){
	if(animo_de_personaje == ANIMO_MALO){
		return PROBABILIDAD_MIN_GOLPE_CRITICO ;
	
	}else if( animo_de_personaje == ANIMO_REGULAR ){
		return PROBABILIDAD_MEDIA_GOLPE_CRITICO ;
	
	}else{
		return PROBABILIDAD_MAX_GOLPE_CRITICO ;
	}
}

/*
 * PRE: La condición climática debe ser válida:
 *	 viento:					humedad:
 *	  > VIENTO_BAJO				 > HUMEDAD_BAJA
 *	  > VIENTO_MEDIO	  ó      > HUMEDAD_MEDIA
 *	  > VIENTO ALTO				 > HUMEDAD_ALTA
 *
 * POST: Devuelve el porcentaje de fallo.
 */
int calculo_fallo (int condicion_climatica){
	
	return  ( condicion_climatica / DIVISOR_FALLO );
}

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración previamente inicializada.
 *
 * POST: Devuelve el valor actualizado de los porcentajes de fallo y golpe crítico:
 *			> Se actualiza a valor por defecto si corresponde llamando a ANIMOS.
 *			> De lo contrario queda igual.
 */
void cargar_fallo_y_critico_default (configuracion_t* configuracion, int viento ,int humedad, char animo_legolas, char animo_gimli, bool es_config_default_completa){
	
	int fallo_enanos = calculo_fallo (humedad);
	int fallo_elfos  = calculo_fallo (viento);
	int critico_enanos = calculo_golpe_critico (animo_gimli);
	int critico_elfos  = calculo_golpe_critico (animo_legolas);

	actualizar_valor ( &(configuracion->info_enanos.fallo), fallo_enanos, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_elfos.fallo) , fallo_elfos , es_config_default_completa );
	actualizar_valor ( &(configuracion->info_enanos.critico), critico_enanos, es_config_default_completa );
	actualizar_valor ( &(configuracion->info_elfos.critico) , critico_elfos , es_config_default_completa );
}

/*
 * PRE: Recibe si se está utilizando una configuración completamente por defecto o no, y la configuración está previamente inicializada.
 *
 * POST: Devuelve la configuración con todos sus actualizados correctamente, lista para utilizarse, esto es:
 *			> Si se usa config por defecto o el campo contenía un -1, ahora contiene el valor default correspondiente.
 *			> Sino, el campo de la config mantiene el valor que se le asignó.
 */
void cargar_valores_default (configuracion_t* configuracion, bool es_config_default_completa){
	
	cargar_resistencia_default(configuracion, es_config_default_completa);
	
	cargar_defensores_iniciales_default(configuracion, es_config_default_completa);
	
	cargar_defensores_extra_default(configuracion, es_config_default_completa);
	
	if (se_necesita_animos(*configuracion, es_config_default_completa)){
		int viento, humedad;
		char animo_legolas, animo_gimli;
		animos(&viento ,&humedad, &animo_legolas, &animo_gimli);
		cargar_fallo_y_critico_default(configuracion, viento, humedad, animo_legolas, animo_gimli, es_config_default_completa);
	}

	if ( (configuracion->velocidad_juego == VALOR_DEFAULT) || es_config_default_completa ){
		configuracion->velocidad_juego = VELOCIDAD_DEFAULT;
	}

	if ( (strcmp(configuracion->ruta_archivo_camino, "-1") == 0) || es_config_default_completa){
		strcpy(configuracion->ruta_archivo_camino, VACIO);
	}
}

/*
 * PRE: ---
 *
 * POST: Se le asignan los valores a cada campo de la configuración según la lectura que se realiza del archivo recibido.
 */
void extraer_datos (configuracion_t* configuracion, FILE* archivo_configuracion){

	char campo_config[MAX_NOMBRE_CAMPO];
	
	int leidos = fscanf (archivo_configuracion, "%[^=]", campo_config);
	
	while (leidos != EOF){
		if ( strcmp(campo_config, "RESISTENCIA_TORRES" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>,<%i>\n", &(configuracion->resistencia_torre_1), &(configuracion->resistencia_torre_2));
		
		}else if ( strcmp(campo_config, "ENANOS_INICIO" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>,<%i>,<%i>,<%i>\n", &(configuracion->info_enanos.cantidad_inicial[0]), &(configuracion->info_enanos.cantidad_inicial[1]),
																	 &(configuracion->info_enanos.cantidad_inicial[2]), &(configuracion->info_enanos.cantidad_inicial[3]) );

		}else if ( strcmp(campo_config, "ELFOS_INICIO" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>,<%i>,<%i>,<%i>\n",&( configuracion->info_elfos.cantidad_inicial[0]), &(configuracion->info_elfos.cantidad_inicial[1]),
																	 &(configuracion->info_elfos.cantidad_inicial[2]), &(configuracion->info_elfos.cantidad_inicial[3]) );

		}else if ( strcmp(campo_config, "ENANOS_EXTRA" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>\n", &(configuracion->info_enanos.cantidad_extra) );

		}else if ( strcmp(campo_config, "ELFOS_EXTRA" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>\n", &(configuracion->info_elfos.cantidad_extra) );

		}else if ( strcmp(campo_config, "ENANOS_ANIMO" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>,<%i>\n", &(configuracion->info_enanos.fallo), &(configuracion->info_enanos.critico) );

		}else if ( strcmp(campo_config, "ELFOS_ANIMO" ) == 0 ){
			fscanf( archivo_configuracion, "=<%i>,<%i>\n", &(configuracion->info_elfos.fallo), &(configuracion->info_elfos.critico) );

		}else if ( strcmp(campo_config, "VELOCIDAD" ) == 0 ){
			fscanf( archivo_configuracion, "=<%f>\n", &(configuracion->velocidad_juego) );

		}else if ( strcmp(campo_config, "CAMINOS" ) == 0 ){
		fscanf( archivo_configuracion, "=<%[^>]>\n", configuracion->ruta_archivo_camino );

		}leidos = fscanf (archivo_configuracion, "%[^=]", campo_config);
	
	}
}

/*
 * PRE: Recibe una ruta de archivo donde se leerá la configuración. Debe tener extensión .txt.
 *		Además, recibe si se está utilizando una configuración completamente por defecto o no.
 *
 * POST: En caso de que no se pueda acceder al archivo y/o que el mismo no exista porque se recibió una ruta vacía, se inicializa una 
 *		 configuración totalmente por defecto.
 *		 Si el archivo existe, inicializar la configuración a partir de los datos leidos y luego actualizar los valores default que correspondan.
 */
void obtener_configuracion (configuracion_t* configuracion, char ruta_archivo_configuracion[MAX_RUTA], bool* es_config_default_completa){

	system("clear");

	FILE* archivo_configuracion = fopen(ruta_archivo_configuracion, "r");

	if (!archivo_configuracion){
		printf("El archivo de configuración no existe! Se utilizará la configuración por defecto :)\n");
		*es_config_default_completa = true;
		cargar_valores_default(configuracion, es_config_default_completa);
		return;
	}

	extraer_datos(configuracion, archivo_configuracion);
	cargar_valores_default(configuracion, *es_config_default_completa);

	fclose(archivo_configuracion);
}

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True Si los comandos de jugar son válidos.
 *				   Respetan estructura: config=[valor] grabacion=[valor] en cualquier orden y siendo ambos opcionales..
 *
 *			> False Si los parámetros no son válidos. No respetan la estructura propuesta.
 *				   
 *			Además, obtiene correctamente la ruta del archivo de donde se leerá la configuración y/o la ruta del archivo donde se grabará la partida.
 *			
 */
bool son_comandos_validos_jugar ( char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO], char ruta_archivo_configuracion[MAX_RUTA], 
								  char ruta_archivo_grabacion[MAX_RUTA], bool jugar_default ){
	

	char clave_1[MAX_ARGUMENTO], valor_1[MAX_ARGUMENTO];
	char clave_2[MAX_ARGUMENTO], valor_2[MAX_ARGUMENTO];

	if (!argumento_1 || jugar_default){
		strcpy ( ruta_archivo_configuracion, VACIO );
		strcpy ( ruta_archivo_grabacion, VACIO );
		return true;
	
	}if ( argumento_1 && comando_tiene_esctructura_valida(argumento_1) ){
		obtener_clave_y_valor (argumento_1, clave_1, valor_1);
		
		if (!argumento_2){
			
			if (strcmp(clave_1, CLAVE_CONFIG) == 0){
				strcpy(ruta_archivo_configuracion, valor_1);
				strcpy ( ruta_archivo_grabacion, VACIO );
				return true;
			
			}else if (strcmp(clave_1, CLAVE_GRABACION) == 0){
				strcpy(ruta_archivo_grabacion, valor_1);
				return true;
			}

		}else if ( comando_tiene_esctructura_valida (argumento_2) ){
			obtener_clave_y_valor (argumento_2, clave_2, valor_2);

			if ( (strcmp(clave_1, CLAVE_CONFIG) == 0) && (strcmp(clave_2, CLAVE_GRABACION) == 0) ){
				strcpy(ruta_archivo_configuracion, valor_1);
				strcpy(ruta_archivo_grabacion, valor_2);
				return true;
			
			}else if ( (strcmp(clave_1, CLAVE_GRABACION) == 0) && (strcmp(clave_2, CLAVE_CONFIG) == 0) ){
				strcpy(ruta_archivo_configuracion, valor_2);
				strcpy(ruta_archivo_grabacion, valor_1);
				return true;
			}
		}

	}return false;
}


void asignar_coordenada (caminos_por_nivel_t caminos_personalizados[], int posicion, int camino, int fila_leida, int columna_leida){
	
	if (camino == PRIMER_CAMINO){
		int tope_camino_1 = caminos_personalizados[posicion].camino_1.tope_camino;

		caminos_personalizados[posicion].camino_1.coordenadas[tope_camino_1].fil = fila_leida;
		caminos_personalizados[posicion].camino_1.coordenadas[tope_camino_1].col = columna_leida;
		caminos_personalizados[posicion].camino_1.tope_camino ++;
	
	}else if (camino == SEGUNDO_CAMINO){
		int tope_camino_2 = caminos_personalizados[posicion].camino_2.tope_camino;

		caminos_personalizados[posicion].camino_2.coordenadas[tope_camino_2].fil = fila_leida;
		caminos_personalizados[posicion].camino_2.coordenadas[tope_camino_2].col = columna_leida;
		caminos_personalizados[posicion].camino_2.tope_camino ++;
	}
}

/*
 * PRE: ---
 *
 * POST: En caso de que no se pueda acceder al archivo y/o que el mismo no exista porque se recibió una ruta vacía o un -1, se vacía la ruta de camino.
 *		 Si el archivo existe, se copian los caminos leidos desde el archivo de caminos, cuya ruta está en la configuración, cargando el
 *		 vector de caminos personalizados que contiene todos los caminos de todos los niveles que se utilizarán.
 */
void cargar_caminos_personalizados (caminos_por_nivel_t caminos_personalizados[MAX_NIVELES], char ruta_archivo_camino[MAX_RUTA]){

	for (int i = 0; i < MAX_NIVELES; i++){
		inicializar_caminos ( &(caminos_personalizados[i].camino_1), &(caminos_personalizados[i].camino_2) );
	}

	FILE* archivo_camino = fopen(ruta_archivo_camino, "r");

	if (!archivo_camino){
		printf("ERROR: No se pudo acceder al archivo de caminos! Se usarán caminos por defecto :)\n");
		strcpy(ruta_archivo_camino, VACIO);
		return;
	}

	int nivel, camino, fil, col;


	int leidos = fscanf(archivo_camino, "NIVEL=%i\n", &nivel);
	while(leidos != EOF){
		
		leidos = fscanf(archivo_camino, "CAMINO=%i\n", &camino);
		while(leidos == 1){
			leidos = fscanf(archivo_camino, "%i;%i\n", &fil, &col);
			
			while (leidos == 2){
				
				asignar_coordenada(caminos_personalizados, nivel-1, camino, fil, col);
			
				leidos = fscanf(archivo_camino, "%i;%i\n", &fil, &col);

			}leidos = fscanf(archivo_camino, "CAMINO=%i\n", &camino);
		}

		leidos = fscanf(archivo_camino, "NIVEL=%i\n", &nivel);
	}

	fclose(archivo_camino);
}


//JUGAR PARTIDA (Antigüo TP2 con cambios necesarios):

/*
 * PRE: ---
 * POST: Devuelve una coordenada de acuerdo a los datos recibidos.
 */
coordenada_t coordenada_de_posicion (int fila, int columna){
	coordenada_t posicion;
	
	posicion.fil = fila;
	posicion.col = columna;

	return posicion;
}


/*
 * PRE: Recibe:
 *			> Un punto cardinal válido: ESTE, OESTE, NORTE o SUR.
 *			> Un booleano previamente inicializado que determina si las posiciones que se pide generar son la segunda o no.
 *			 ( Aplica unicamente a Niveles 1 y 2).
 *
 * POST: Devuelve una coordenada válida para la torre y otra para la entrada:
 *			> La posición de la entrada y de la torre deben corresponderse al punto cardinal recibido, posicionandose en los
 *			  bordes correspondientes.
 *			> La posición de la entrada y de la torre se encuentra dentro de los limites del terreno.
 *			> Para los niveles 3 y 4: 
 *				- Si se trata de la segunda posicion a obtener, dicha coordenada estará contenida en la mitad izquierda
 *			 	  del mapa, tanto la segunda torre como la segunda posición están a la izquierda de la primera.
 *			 	- Ambas torres y ambas entradas no se superpondrán.
 */
void obtener_posicion_entrada_y_torre (coordenada_t* entrada, coordenada_t* torre, char punto_cardinal, bool es_segunda_posicion){
	
	if (punto_cardinal == ESTE){
		*entrada = coordenada_de_posicion ( rand() % TOPE_TERRENO_NIVEL_1_2, (TOPE_TERRENO_NIVEL_1_2 - 1) );
		*torre = coordenada_de_posicion ( rand() % TOPE_TERRENO_NIVEL_1_2, LIMITE_MIN );


	}else if (punto_cardinal == OESTE){
		*entrada = coordenada_de_posicion ( rand() % TOPE_TERRENO_NIVEL_1_2, LIMITE_MIN );
		*torre = coordenada_de_posicion ( rand() % TOPE_TERRENO_NIVEL_1_2 , (TOPE_TERRENO_NIVEL_1_2 - 1) );
	

	}else if (punto_cardinal == NORTE){
		if (es_segunda_posicion){
			*entrada = coordenada_de_posicion ( (TOPE_TERRENO_NIVEL_3_4 - 1), rand() % (TOPE_TERRENO_NIVEL_3_4 / 2));
			*torre = coordenada_de_posicion ( LIMITE_MIN, rand() % (TOPE_TERRENO_NIVEL_3_4 / 2) );
		
		}else{
			*entrada = coordenada_de_posicion ( (TOPE_TERRENO_NIVEL_3_4 - 1), rand() % (TOPE_TERRENO_NIVEL_3_4 / 2) + (TOPE_TERRENO_NIVEL_3_4 / 2));
			*torre = coordenada_de_posicion ( LIMITE_MIN, rand() % (TOPE_TERRENO_NIVEL_3_4 / 2) + (TOPE_TERRENO_NIVEL_3_4 / 2) );
		}


	}else{
		if (es_segunda_posicion){
			*entrada = coordenada_de_posicion ( (LIMITE_MIN ), rand() % (TOPE_TERRENO_NIVEL_3_4 / 2));
			*torre = coordenada_de_posicion ( (TOPE_TERRENO_NIVEL_3_4 - 1), rand() % (TOPE_TERRENO_NIVEL_3_4 / 2) );
		
		}else{
			*entrada = coordenada_de_posicion ( (LIMITE_MIN ), rand() % (TOPE_TERRENO_NIVEL_3_4 / 2) + (TOPE_TERRENO_NIVEL_3_4 / 2));
			*torre = coordenada_de_posicion ( (TOPE_TERRENO_NIVEL_3_4 - 1), rand() % (TOPE_TERRENO_NIVEL_3_4 / 2) + (TOPE_TERRENO_NIVEL_3_4 / 2) );
		}
	}
}


/*
 * PRE: Recibe un tope de defensores válido: se corresponde con la cantidad de defensores que hay en la jugada actual.
 *
 * POST: ---
 */
void pedir_posicion_defensor (coordenada_t* posicion, int tope_defensores){
	printf("INTRODUCÍ LAS COORDENADAS DEL DEFENSOR #%i\n", (tope_defensores + 1));
	
	printf("FILA: \n");
	scanf("%i", &(posicion->fil));
	
	printf("COLUMNA: \n");
	scanf("%i", &(posicion->col));
}

/*
 * PRE: Recibe una posición (fila y columna) y nivel_actual el cual debe ser válido: es 1, 2, 3 o 4.
 *
 * POST: Devuelve:
 *			> True si la posicion es válida: está dentro de los limites del terreno (15x15 o 20x20 segun nivel_actual).
 *			> False si la posicion no es válida: está fuera de los limites del terreno.
 */
bool esta_dentro_de_terreno (coordenada_t posicion, int nivel_actual){
	if ( (nivel_actual == PRIMER_NIVEL ) ||( nivel_actual == SEGUNDO_NIVEL) ){
		return ( ( (posicion.fil >= LIMITE_MIN) && (posicion.fil < TOPE_TERRENO_NIVEL_1_2) ) &&
				 ( (posicion.col >= LIMITE_MIN )&& (posicion.col < TOPE_TERRENO_NIVEL_1_2) ) );
	}else{
		return ( ( (posicion.fil >= LIMITE_MIN) && (posicion.fil < TOPE_TERRENO_NIVEL_3_4) ) &&
				 ( (posicion.col >= LIMITE_MIN) && (posicion.col < TOPE_TERRENO_NIVEL_3_4) ) );
	}
}


/*
 * PRE: Recibe:
 *			> Un juego cargado con información correcta.
 *			> El tipo de defensor que se agregará, debe ser válido: ENANO o ELFO.
 *
 * POST: Muestra por pantalla la interfaz de agregar un defensor, junto con el mapa y la información del juego.
 *		 Devuelve un nuevo defensor válido que se añade al vector de defensores en la última posición, incrementando el tope:
 *			> Tipo válido: ENANO o ELFO
 *			> Posición válida: Su coordenada no se superpone ni al camino ni otro defensor.
 *			> Fuerza de ataque válida: FUERZA_ATAQUE_ENANO (60) o FUERZA_ATAQUE_ELFO (30).
 */
void agregar_defensor_aux (juego_t* juego, char tipo_defensor){
	printf("\n╔═══════════════════════════════════════════════════════════════════════╗\n");
	printf("║*· · · · · · · · · · · · *AGREGAR···DEFENSOR*· · · · · · · · · · · · ·*║");
	printf("\n╚═══════════════════════════════════════════════════════════════════════╝\n");
	
	if (tipo_defensor == ELFO) printf("\n╚ TIPO: Elfo ╝\n");
	if (tipo_defensor == ENANO) printf("\n╚ TIPO: Enano ╝\n");
	
	coordenada_t posicion_defensor;
	pedir_posicion_defensor(&posicion_defensor, juego->nivel.tope_defensores );	
	
	while( (!esta_dentro_de_terreno(posicion_defensor, juego->nivel_actual)) || 
		   (agregar_defensor(&(juego->nivel), posicion_defensor, tipo_defensor) == NO_SE_PUEDE_AGREGAR_DEFENSOR) ){

		printf("\nPosición no válida!\n");
		pedir_posicion_defensor(&posicion_defensor, juego->nivel.tope_defensores);
	}
	mostrar_juego(*juego);
	system("clear");
}

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> True si la ruta está vacía -> "\0"
 *			> False si la ruta NO está vacía.
 */
bool esta_ruta_vacia (char ruta_archivo[MAX_RUTA] ){
	return ( strcmp(ruta_archivo, VACIO) == 0);
}

/*
 * PRE: Recibe el nivel a inicializarse y los caminos personalizados correctamente cargados.
 *
 * POST: Copia el/los camino/s personalizados correspondientes a la estructura del juego (juego.nivel)
 *	
 */
void copiar_camino_personalizado (nivel_t* nivel, caminos_por_nivel_t caminos_personalizados[MAX_RUTA], int nivel_actual){

	int i;
	nivel->tope_camino_1 = caminos_personalizados[nivel_actual-1].camino_1.tope_camino;
	nivel->tope_camino_2 = caminos_personalizados[nivel_actual-1].camino_2.tope_camino;

	for ( i = 0; i < caminos_personalizados[nivel_actual-1].camino_1.tope_camino; i++){
		nivel->camino_1[i] = caminos_personalizados[nivel_actual-1].camino_1.coordenadas[i];
	}

	for ( i = 0; i < caminos_personalizados[nivel_actual-1].camino_2.tope_camino; i++){
		nivel->camino_2[i] = caminos_personalizados[nivel_actual-1].camino_2.coordenadas[i];
	}
}

/*
 * PRE: Recibe un juego correctamente inicializado.
 *
 * POST: Devuelve el nivel uno inicializado correctamente: 
 *	
 */
void inicializar_primer_nivel ( juego_t* juego, bool* es_segunda_posicion, int* elfos_iniciales, int* enanos_iniciales, 
							    configuracion_t configuracion, caminos_por_nivel_t caminos_personalizados[] ){

	juego->nivel.max_enemigos_nivel = MAX_ENEMIGOS_NIVEL_1;
	*enanos_iniciales = configuracion.info_enanos.cantidad_inicial[PRIMER_NIVEL-1];
	*elfos_iniciales =  configuracion.info_elfos.cantidad_inicial[PRIMER_NIVEL-1];

	if ( esta_ruta_vacia( configuracion.ruta_archivo_camino ) ){
		char punto_cardinal = ESTE;
		coordenada_t entrada_1;
		coordenada_t posicion_torre_1;
		*es_segunda_posicion = false;
		obtener_posicion_entrada_y_torre(&entrada_1, &posicion_torre_1, punto_cardinal, *es_segunda_posicion);
		obtener_camino(juego->nivel.camino_1, &(juego->nivel.tope_camino_1), entrada_1, posicion_torre_1);

	}else{
		copiar_camino_personalizado( &(juego->nivel), caminos_personalizados, PRIMER_NIVEL );
	}
}

/*
 * PRE: Recibe un juego correctamente inicializado.
 *
 * POST: Devuelve el dos cuatro inicializado correctamente: 
 *	
 */
void inicializar_segundo_nivel ( juego_t* juego, bool* es_segunda_posicion, int* elfos_iniciales, int* enanos_iniciales, 
							     configuracion_t configuracion, caminos_por_nivel_t caminos_personalizados[] ){

	juego->nivel.max_enemigos_nivel = MAX_ENEMIGOS_NIVEL_2;
	*enanos_iniciales = configuracion.info_enanos.cantidad_inicial[SEGUNDO_NIVEL-1];
	*elfos_iniciales =  configuracion.info_elfos.cantidad_inicial[SEGUNDO_NIVEL-1];

	if ( esta_ruta_vacia( configuracion.ruta_archivo_camino ) ){
		char punto_cardinal = OESTE;
		coordenada_t entrada_2;
		coordenada_t posicion_torre_2;
		*es_segunda_posicion = false;
		obtener_posicion_entrada_y_torre(&entrada_2, &posicion_torre_2, punto_cardinal, *es_segunda_posicion);
		obtener_camino(juego->nivel.camino_2, &(juego->nivel.tope_camino_2), entrada_2, posicion_torre_2);
	
	}else{
		copiar_camino_personalizado( &(juego->nivel), caminos_personalizados, SEGUNDO_NIVEL );
	}
}

/*
 * PRE: Recibe un juego correctamente inicializado.
 *
 * POST: Devuelve el nivel tres inicializado correctamente: 
 *	
 */
void inicializar_tercer_nivel ( juego_t* juego, bool* es_segunda_posicion, int* elfos_iniciales, int* enanos_iniciales, 
							    configuracion_t configuracion, caminos_por_nivel_t caminos_personalizados[] ){
	
	juego->nivel.max_enemigos_nivel = MAX_ENEMIGOS_NIVEL_3;
	*enanos_iniciales = configuracion.info_enanos.cantidad_inicial[TERCER_NIVEL-1];
	*elfos_iniciales =  configuracion.info_elfos.cantidad_inicial[TERCER_NIVEL-1];

	if ( esta_ruta_vacia( configuracion.ruta_archivo_camino ) ){
		char punto_cardinal = NORTE;
		coordenada_t entrada_1, entrada_2;
		coordenada_t posicion_torre_1, posicion_torre_2;

		obtener_posicion_entrada_y_torre(&entrada_1, &posicion_torre_1, punto_cardinal, *es_segunda_posicion);
		*es_segunda_posicion = true;
		obtener_posicion_entrada_y_torre(&entrada_2, &posicion_torre_2, punto_cardinal, *es_segunda_posicion);
		
		obtener_camino(juego->nivel.camino_1, &(juego->nivel.tope_camino_1), entrada_1, posicion_torre_1);
		obtener_camino(juego->nivel.camino_2, &(juego->nivel.tope_camino_2), entrada_2, posicion_torre_2);
	
	}else{
		copiar_camino_personalizado( &(juego->nivel), caminos_personalizados, TERCER_NIVEL );
	}	
}

/*
 * PRE: Recibe un juego correctamente inicializado.
 *
 * POST: Devuelve el nivel cuatro inicializado correctamente: 
 *	
 */
void inicializar_cuarto_nivel ( juego_t* juego, bool* es_segunda_posicion, int* elfos_iniciales, int* enanos_iniciales, 
							    configuracion_t configuracion, caminos_por_nivel_t caminos_personalizados[] ){
	
	juego->nivel.max_enemigos_nivel = MAX_ENEMIGOS_NIVEL_4;
	*enanos_iniciales = configuracion.info_enanos.cantidad_inicial[CUARTO_NIVEL-1];
	*elfos_iniciales =  configuracion.info_elfos.cantidad_inicial[CUARTO_NIVEL-1];

	if ( esta_ruta_vacia( configuracion.ruta_archivo_camino ) ){
		char punto_cardinal = SUR;
		coordenada_t entrada_1, entrada_2;
		coordenada_t posicion_torre_1, posicion_torre_2;

		obtener_posicion_entrada_y_torre(&entrada_1, &posicion_torre_1, punto_cardinal, *es_segunda_posicion);
		*es_segunda_posicion = true;
		obtener_posicion_entrada_y_torre(&entrada_2, &posicion_torre_2, punto_cardinal, *es_segunda_posicion);
	
		obtener_camino(juego->nivel.camino_1, &(juego->nivel.tope_camino_1), entrada_1, posicion_torre_1);
		obtener_camino(juego->nivel.camino_2, &(juego->nivel.tope_camino_2), entrada_2, posicion_torre_2);
	
	}else{
		copiar_camino_personalizado( &(juego->nivel), caminos_personalizados, CUARTO_NIVEL );
	}
}

/*
 * PRE: Recibe un juego correctamente inicializado, el tipo de defensor que se desea añadir y la cantidad.
 *
 * POST: Agrega la cantidad de defensores recibida del tipo recibido mientras muestra lo muestra por pantalla.
 *	
 */
void generar_defensores_iniciales (juego_t* juego, char tipo_defensor, int defensores_iniciales){

	for (int i = 0; i < defensores_iniciales; i++){
		
		mostrar_juego(*juego);
		agregar_defensor_aux(juego, tipo_defensor);
	}
}

/*
 * PRE: Recibe un juego correctamente inicializado y el vector de caminos personalizados correctamente cargado.
 *
 * POST: Devuelve el nivel que se comenzará a jugar inicializado correctamente: 
 *			> Entrada/s y torre/s en posiciones válidas según punto cardinal y limite del terreno.
 *			> Camino/s válidos según las entradas y torres.
 *			> Cantidad de orcos a generar y defensores iniciales inicializados correctamente.
 *			> Topes de orcos, defensores y camino/s correctos.	
 */
void inicializar_nivel ( juego_t* juego, bool* esta_nivel_inicializado, bool* se_pidio_def_por_ultima_vez, 
						 configuracion_t configuracion, caminos_por_nivel_t caminos_personalizados[] ){
	
	bool es_segunda_posicion = false;
	*se_pidio_def_por_ultima_vez = false;

	juego->nivel.tope_enemigos = 0;
	juego->nivel.tope_defensores = 0;
	int enanos_iniciales, elfos_iniciales;

	if(juego->nivel_actual == PRIMER_NIVEL){
	
		inicializar_primer_nivel (juego, &es_segunda_posicion, &elfos_iniciales, &enanos_iniciales, configuracion, caminos_personalizados);
	

	}else if(juego->nivel_actual == SEGUNDO_NIVEL){

		inicializar_segundo_nivel (juego, &es_segunda_posicion, &elfos_iniciales, &enanos_iniciales, configuracion, caminos_personalizados);
	

	}else if(juego->nivel_actual == TERCER_NIVEL){
	
		inicializar_tercer_nivel (juego, &es_segunda_posicion, &elfos_iniciales, &enanos_iniciales, configuracion, caminos_personalizados);


	}else if(juego->nivel_actual == CUARTO_NIVEL){
		
		inicializar_cuarto_nivel (juego, &es_segunda_posicion, &elfos_iniciales, &enanos_iniciales, configuracion, caminos_personalizados);

	}

	generar_defensores_iniciales( juego, ENANO, enanos_iniciales );
	generar_defensores_iniciales( juego, ELFO, elfos_iniciales );

	*esta_nivel_inicializado = true;

}


/*
 * PRE: --- (la respuesta recibida no debe cumplir con ninguna condición previa)
 *
 * POST: Devuelve:
 *			> True si la respuesta recibida es válida: SI (1) o NO (0).
 *			> False si la respuesta no es válida: distinto de SI (1) o NO (0).
 */
bool es_valida_respuesta (int respuesta){
	return (respuesta == SI || respuesta == NO);
}


/*
 * PRE: --- 
 *
 * POST: Devuelve:
 *			> True (1) si el usuario desea agregar un defensor extra.
 *			> False (0) si el usuario no desea agregar un defensor extra.
 */
bool quiere_agregar_defensor_extra(){
	int respuesta;
	printf("\n╔════════════════════════════════════════════════════════════════════════╗\n");
	printf("║··Podés··agregar··un··defensor··extra!··Deseas··hacerlo? 1- SI / 0- NO··║ ");
	printf("\n╚════════════════════════════════════════════════════════════════════════╝\n");

	scanf("%i", &respuesta);
	
	while( !es_valida_respuesta (respuesta) ){
		printf("No es válida :( Acordate que... 1- SI / 2- NO : ");
		scanf("%i", &respuesta);
	}
	return respuesta;
}


/*
 * PRE: Los parametros recibidos deben estar previamente inicializados correctamente
 *
 * POST: Devuelve:
 *			> True si se puede agregar un defensor extra: ya salieron suficientes orcos, aún quedan defensores extras disponibles y 
 * 			  la torre cuenta con suficiente resistencia.
 *			> False si no se puede agregar un defensor extra: no se cumple alguna de las condiciones necesarias.
 *
 * 			> En caso de que sea la última vez que se vaya a solicitar el agregado de un defensor extra en ese nivel, se_pidio_def_por_ultima_vez
 * 			  pasa a ser true.
 */
bool puede_agregar_defensor_extra (juego_t* juego, bool* se_pidio_def_por_ultima_vez){
	
	if ( !(*se_pidio_def_por_ultima_vez) ){
		if ( juego->nivel.tope_enemigos == juego->nivel.max_enemigos_nivel){
			*se_pidio_def_por_ultima_vez = true;
		}
		
		if ( juego->nivel_actual == PRIMER_NIVEL ){
			return (((juego->nivel.tope_enemigos % CANT_ORCOS_PARA_DEF_EXTRA_NIVEL_1) == 0) && 
					 (juego->torres.enanos_extra >= CANTIDAD_MIN_DEFENSORES_EXTRA)          && 
					 (juego->torres.resistencia_torre_1 > COSTO_DEFENSOR_EXTRA) );
		
		}else if ( juego->nivel_actual == SEGUNDO_NIVEL ){
			return (((juego->nivel.tope_enemigos % CANT_ORCOS_PARA_DEF_EXTRA_NIVEL_2_3_4) == 0) && 
					 (juego->torres.elfos_extra >= CANTIDAD_MIN_DEFENSORES_EXTRA)               && 
					 (juego->torres.resistencia_torre_2 > COSTO_DEFENSOR_EXTRA) );
		
		}else{
			return ( ((juego->nivel.tope_enemigos % CANT_ORCOS_PARA_DEF_EXTRA_NIVEL_2_3_4) == 0) && 
				   ( ((juego->torres.enanos_extra >= CANTIDAD_MIN_DEFENSORES_EXTRA) && (juego->torres.resistencia_torre_1 > COSTO_DEFENSOR_EXTRA)) || 
				     ((juego->torres.elfos_extra  >= CANTIDAD_MIN_DEFENSORES_EXTRA) && (juego->torres.resistencia_torre_2 > COSTO_DEFENSOR_EXTRA)) ) );
		}

	}return false;
}


/*
 * PRE: La información de las torres debe estar correctamente inicializada.
 *
 * POST: Devuelve:
 *			> True si se puede agregar el tipo de defensor seleccionado, cumpliéndose especificamente las condiciones necesarias 
 *		   	  para agregar un defensor extra para el tipo de defensor seleccionado.
 *			> False si no se puede agregar el tipo de defensor seleccionado.
 * 			
 *			(Aplica sólo para los niveles 3 y 4 donde hay dos tipos de defensores posibles a agregar).
 */
bool es_valido_defensor_elegido (char tipo_defensor, torres_t torres){
	
	return ( ((tipo_defensor == ENANO) && (torres.enanos_extra >= CANTIDAD_MIN_DEFENSORES_EXTRA) && (torres.resistencia_torre_1 > COSTO_DEFENSOR_EXTRA)) || 
			 ((tipo_defensor == ELFO)  && (torres.elfos_extra  >= CANTIDAD_MIN_DEFENSORES_EXTRA) && (torres.resistencia_torre_2 > COSTO_DEFENSOR_EXTRA)) );
}


/*
 * PRE: La información de las torres debe estar correctamente inicializada.
 *
 * POST: Devuelve el tipo de defensor extra que el usuario desea agregar, el cual es válido: es ELFO o ENANO.
 *		 (Aplica sólo para los niveles 3 y 4 donde hay dos tipos de defensores posibles a agregar).
 */
char seleccion_tipo_defensor (torres_t torres){
	char tipo_defensor;
	printf("Qué tipo de defensor querés agregar? ENANO (G) o ELFO (L): ");
	scanf(" %c", &tipo_defensor);

	while ( !es_valido_defensor_elegido (tipo_defensor, torres) ){
		printf("Defensor no válido :( Acordate que... ENANO (G) o ELFO (L): ");
		scanf(" %c", &tipo_defensor);
	}
	return tipo_defensor;
}


/*
 * PRE: El nivel actual y la información de las torres deben ser correctos.
 *
 * POST: Devuelve el tipo de defensor extra que se agregará de acuerdo al nivel actual.
 *		 El tipo de defensor devuelto será válido: es ELFO o ENANO.
 */
char tipo_defensor_a_agregar (int nivel_actual, torres_t torres){
	if (nivel_actual == PRIMER_NIVEL){
		return ENANO;

	}else if (nivel_actual == SEGUNDO_NIVEL){
		return ELFO;

	}else{
		return seleccion_tipo_defensor(torres);
	}
}


/*
 * PRE: El tipo de defensor debe ser válido: es ELFO o ENANO.
 *
 * POST: Se le reducirá la resistencia a la torre correspondiente (según el tipo de defensor agregado) el costo de defensor extra (50).
 */
void bajar_vida_torre_por_def_extra (torres_t* torres, char tipo_defensor){
	if (tipo_defensor == ENANO){
		torres->resistencia_torre_1 -= COSTO_DEFENSOR_EXTRA;
	
	}else{
		torres->resistencia_torre_2 -= COSTO_DEFENSOR_EXTRA;
	}
}


/*
 * PRE: Los parámetros que recibe deben estar previamente inicializados.
 *
 * POST: > Sòlo en caso de que se cumplan las condiciones necesarias, se agrega un nuevo defensor del tipo correspondiente y se reduce
 *		   la resistencia de la torre.
 *		 > De lo contrario, no se agrega nada.
 */
void agregar_defensor_extra (juego_t* juego, bool* se_pidio_def_por_ultima_vez ){

	if ( puede_agregar_defensor_extra (juego, se_pidio_def_por_ultima_vez) 
		 && quiere_agregar_defensor_extra() ){
		
		char tipo_defensor = tipo_defensor_a_agregar (juego->nivel_actual, juego->torres);
		agregar_defensor_aux(juego, tipo_defensor);
		
		if (tipo_defensor == ENANO){
			juego->torres.enanos_extra --;
		
		}else{
			juego->torres.elfos_extra --;
		}

		bajar_vida_torre_por_def_extra ( &(juego->torres), tipo_defensor );
	}
}


/*
 * PRE: ---
 *
 * POST: Imprime por pantalla la transición al próximo nivel.
 */
void mostrar_transicion_nivel (){

	printf("Demostraste que sos lo suficientemente fuerte como para superar esta prueba, felicidades!.\n");
	printf("Pero no te relajes eh, que la cosa se está complicando cada vez más. Suerte! La vas necesitar...\n");

	detener_el_tiempo(3);
	system("clear");

	printf(" ██████╗\n"); 
	printf(" ╚════██╗\n");		 
	printf("  █████╔╝\n");		  
	printf("  ╚═══██╗\n");		  
	printf(" ██████╔╝\n");		 
	printf(" ╚═════╝\n");

	detener_el_tiempo(0.5);
	system("clear");

	printf(" ██████╗\n"); 
	printf(" ╚════██╗\n");		 
	printf("  █████╔╝\n");		  
	printf(" ██╔═══╝\n");		  
	printf(" ███████╗\n");		 
	printf(" ╚══════╝\n");

	detener_el_tiempo(0.5);
	system("clear");

	printf("  ██╗\n"); 
	printf(" ███║\n");		 
	printf(" ╚██║\n");		  
	printf("  ██║\n");		  
	printf("  ██║\n");		 
	printf("  ╚═╝\n");

	detener_el_tiempo(0.5);
	system("clear");
}

/*
 * PRE: ---
 *
 * POST: Muestra por pantalla la transición al siguiente nivel y se incrementa en 1 el nivel actual. 
 *		 Se inicializa en cero el tope de enemigos.
 */
void pasar_a_nivel_siguiente (juego_t* juego, bool* esta_nivel_inicializado){

	mostrar_transicion_nivel();
	juego->nivel_actual ++;
	juego->nivel.tope_enemigos = 0;
	*esta_nivel_inicializado = false;
}

/*
 * PRE: ---
 *
 * POST: Muestra por pantalla el mensaje de victoria.
 */
void mostrar_mensaje_victoria(){

	printf(" ██████╗  █████╗ ███╗   ██╗ █████╗ ███████╗████████╗███████╗\n");
	printf("██╔════╝ ██╔══██╗████╗  ██║██╔══██╗██╔════╝╚══██╔══╝██╔════╝\n");
	printf("██║  ███╗███████║██╔██╗ ██║███████║███████╗   ██║   █████╗\n");
	printf("██║   ██║██╔══██║██║╚██╗██║██╔══██║╚════██║   ██║   ██╔══╝  \n");
	printf("╚██████╔╝██║  ██║██║ ╚████║██║  ██║███████║   ██║   ███████╗\n");
	printf(" ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝   ╚═╝   ╚══════╝\n");
}

/*
 * PRE: ---
 *
 * POST: Muestra por pantalla el mensaje de derrota.
 */
void mostrar_mensaje_derrota(){

	printf(" ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ \n");
	printf("██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗\n");
	printf("██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝\n");
	printf("██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\n");
	printf("╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\n");
	printf(" ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝\n");

	printf("\n 				Otra vez será, crack...\n");
}

/*
 * PRE: ---
 *
 * POST: Muestra por pantalla un mensaje de bienvenida.
 */
void mostrar_mensaje_bienvenida(){

	system("clear");

	printf("████████▄     ▄████████    ▄████████  ▄█     ▄████████ ███▄▄▄▄   ████████▄     ▄████████       ▄█          ▄████████    ▄████████\n");
	printf("███   ▀███   ███    ███   ███    ███ ███    ███    ███ ███▀▀▀██▄ ███   ▀███   ███    ███      ███         ███    ███   ███    ███\n");
	printf("███    ███   ███    █▀    ███    █▀  ███▌   ███    █▀  ███   ███ ███    ███   ███    █▀       ███         ███    ███   ███    █▀ \n");
	printf("███    ███  ▄███▄▄▄      ▄███▄▄▄     ███▌  ▄███▄▄▄     ███   ███ ███    ███  ▄███▄▄▄          ███         ███    ███   ███       \n");
	printf("███    ███ ▀▀███▀▀▀     ▀▀███▀▀▀     ███▌ ▀▀███▀▀▀     ███   ███ ███    ███ ▀▀███▀▀▀          ███       ▀███████████ ▀███████████\n");
	printf("███    ███   ███    █▄    ███        ███    ███    █▄  ███   ███ ███    ███   ███    █▄       ███         ███    ███          ███\n");
	printf("███   ▄███   ███    ███   ███        ███    ███    ███ ███   ███ ███   ▄███   ███    ███      ███▌    ▄   ███    ███    ▄█    ███\n");
	printf("████████▀    ██████████   ███        █▀     ██████████  ▀█   █▀  ████████▀    ██████████      █████▄▄██   ███    █▀   ▄████████▀ \n");
	printf("\n\n");
	detener_el_tiempo (0.5);
	printf("			    ███      ▄██████▄     ▄████████    ▄████████    ▄████████    ▄████████\n");
	printf("			▀█████████▄ ███    ███   ███    ███   ███    ███   ███    ███   ███    ███\n");
	printf("			   ▀███▀▀██ ███    ███   ███    ███   ███    ███   ███    █▀    ███    █▀ \n");
	printf("			    ███   ▀ ███    ███  ▄███▄▄▄▄██▀  ▄███▄▄▄▄██▀  ▄███▄▄▄       ███       \n");
	printf("			    ███     ███    ███ ▀▀███▀▀▀▀▀   ▀▀███▀▀▀▀▀   ▀▀███▀▀▀     ▀███████████\n");
	printf("			    ███     ███    ███ ▀███████████ ▀███████████   ███    █▄           ███\n");
	printf("			    ███     ███    ███   ███    ███   ███    ███   ███    ███    ▄█    ███\n");
	printf("			   ▄████▀    ▀██████▀    ███    ███   ███    ███   ██████████  ▄████████▀ \n");

	detener_el_tiempo (1.5);
	system("clear");
}

/*
 * PRE: ---
 *
 * POST: Muestra por pantalla un mensaje avisando acerca de la condición de la grabación.
 */
void mostrar_mensaje_grabacion_en_curso (FILE* archivo_grabacion){
	if (archivo_grabacion){
		printf("La partida está siendo grabada :)!\n");
	
	}else{
		printf("La partida no será grabada :)\n");
	}
}

/*
 * PRE: Recibe un juego completamente cargado y actualizado.
 *
 * POST: Si el archivo existe, copia a dicho archivo el estado del juego.
 */
void capturar_fotograma (FILE* archivo_grabacion, juego_t juego){
	if (archivo_grabacion){
		fwrite (&juego, sizeof(juego_t), 1, archivo_grabacion);
	}
}

/*
 * PRE: Recibe un nivel correctamente cargado y actualizado, y el nivel en el que se encuentra la partida.
 *
 * POST: Devuelve la cantidad de orcos que murieron para dicho momento.
 */
int cantidad_orcos_muertos (nivel_t nivel, int nivel_actual){

	int orcos_muertos_ultimo_nivel = VALOR_INICIAL;

	for (int i = 0; i < nivel.tope_enemigos; i++){

		if (nivel.enemigos[i].vida <= VIDA_NULA_ORCO){
			orcos_muertos_ultimo_nivel ++;
		}
	}

	if (nivel_actual == PRIMER_NIVEL){
		return orcos_muertos_ultimo_nivel;
	
	}else if (nivel_actual == SEGUNDO_NIVEL){
		return ( MAX_ENEMIGOS_NIVEL_1 + orcos_muertos_ultimo_nivel );
	
	}else if (nivel_actual == TERCER_NIVEL){
		return ( MAX_ENEMIGOS_NIVEL_1 + MAX_ENEMIGOS_NIVEL_2 + orcos_muertos_ultimo_nivel );
	
	}else{
		return ( MAX_ENEMIGOS_NIVEL_1 + MAX_ENEMIGOS_NIVEL_2 + MAX_ENEMIGOS_NIVEL_3 + orcos_muertos_ultimo_nivel );
	}
}

/*
 * PRE: Recibe un juego y una configuración correctamente inicializada.
 *
 * POST: Calcula el puntaje que obtuvo el jugador actual.
 */
void calcular_puntaje (int* puntaje, juego_t juego, configuracion_t configuracion){
	int total_orcos_muertos = cantidad_orcos_muertos (juego.nivel, juego.nivel_actual);
	int resistencia_torre_1 = juego.torres.resistencia_torre_1;
	int resistencia_torre_2 = juego.torres.resistencia_torre_2;
	int enanos_extra = juego.torres.enanos_extra;
	int elfos_extra = juego.torres.elfos_extra;
	
	int enanos_iniciales = VALOR_INICIAL;
	int elfos_iniciales = VALOR_INICIAL;

	for (int i = 0; i < MAX_NIVELES; i++){
		enanos_iniciales += configuracion.info_enanos.cantidad_inicial[i];
		elfos_iniciales += configuracion.info_elfos.cantidad_inicial[i];
	}

	*puntaje = (total_orcos_muertos * MULTIPLICADOR_PUNTAJE) / (resistencia_torre_1 + resistencia_torre_2 + enanos_iniciales + elfos_iniciales + enanos_extra + elfos_extra );

}

/*
 * PRE: Recibe el puntaje correcto del jugador actual.
 *
 * POST: Obtiene y asigna los datos al jugador: nombre y puntaje.
 */
void obtener_datos_jugador_actual (jugador_t* jugador, int puntaje){
	
	jugador->puntaje = puntaje;

	printf("\t╔═════════════════════════════════════╗\n");
	printf("\t║·······INGRESÁ····TU····NOMBRE·······║\n ");
	printf("\t╚═════════════════════════════════════╝\n");

	printf("Puntaje: %i puntos\n", puntaje);
	printf("Nombre: ");
	scanf("%s", jugador->nombre);
}

/*
 * PRE: Recibe un jugador con sus datos correctos.
 *
 * POST: Crea un archivo de ranking, listando al jugador.
 */
void crear_ranking (char ruta_ranking[MAX_RUTA], jugador_t jugador){

	FILE* ranking =  fopen(ruta_ranking, "w");

	if (!ranking){
		printf("ERROR: No se pudo crear ranking :(\n");
		return;
	}

	fprintf ( ranking, "%s;%i\n", jugador.nombre, jugador.puntaje);

	fclose(ranking);
}

/*
 * PRE: Recibe el puntaje correcto del jugador actual.
 *
 * POST: Añade al jugador en el ranking correspondiente a la configuración con la que estaba jugando.
 *		 En caso de no existir el archivo del ranking, lo crea.
 */
void listar_jugador_en_ranking (char ruta_ranking[MAX_RUTA], int puntaje_actual){
	
	jugador_t jugador_actual;
	jugador_t jugador_anterior;
	
	obtener_datos_jugador_actual (&jugador_actual, puntaje_actual);
	
	FILE* ranking_viejo = fopen(ruta_ranking, "r");

	if (!ranking_viejo){
		crear_ranking (ruta_ranking, jugador_actual);
		return;
	}

	FILE* ranking_nuevo = fopen ("ranking_aux.csv", "w");

	if (!ranking_nuevo){
		printf("ERROR: No se pudo actualizar ranking :(\n");
		fclose(ranking_viejo);
		return;
	}

	bool fue_listado = false;

	int leidos = fscanf ( ranking_viejo, "%[^;];%i\n", jugador_anterior.nombre, &jugador_anterior.puntaje );
	while ( leidos == 2 && !fue_listado){
		
		if ( jugador_actual.puntaje > jugador_anterior.puntaje ){
			fprintf ( ranking_nuevo, "%s;%i\n", jugador_actual.nombre, jugador_actual.puntaje);
			fue_listado = true;

		}else if ( jugador_actual.puntaje < jugador_anterior.puntaje ){
			fprintf ( ranking_nuevo, "%s;%i\n", jugador_anterior.nombre, jugador_anterior.puntaje);
			leidos = fscanf ( ranking_viejo, "%[^;];%i\n", jugador_anterior.nombre, &jugador_anterior.puntaje );

		}else if ( jugador_actual.puntaje == jugador_anterior.puntaje ){
			int comparacion_alfabetica = strcmp ( jugador_actual.nombre, jugador_anterior.nombre);

			if ( comparacion_alfabetica < 0 ){
				fprintf ( ranking_nuevo, "%s;%i\n", jugador_actual.nombre, jugador_actual.puntaje);
				fue_listado = true;
			
			}else {
				fprintf ( ranking_nuevo, "%s;%i\n", jugador_anterior.nombre, jugador_anterior.puntaje);
				leidos = fscanf ( ranking_viejo, "%[^;];%i\n", jugador_anterior.nombre, &jugador_anterior.puntaje );
				
				if (comparacion_alfabetica == 0){
					fue_listado = true;
				}
			}
		}
	
	}if (!fue_listado){
		fprintf ( ranking_nuevo, "%s;%i\n", jugador_actual.nombre, jugador_actual.puntaje);
	
	}while (leidos == 2){
		fprintf ( ranking_nuevo, "%s;%i\n", jugador_anterior.nombre, jugador_anterior.puntaje);
		leidos = fscanf ( ranking_viejo, "%[^;];%i\n", jugador_anterior.nombre, &jugador_anterior.puntaje );
	}

	fclose(ranking_viejo);
	fclose(ranking_nuevo);
	rename("ranking_aux.csv", ruta_ranking);
}

/*
 * PRE: Recibe una configuración previamente inicializada,los caminos personalizados previamente cargados, y la ruta del archivo de ranking de acuerdo a la config.
 *
 * POST: Se lleva a cabo el juego de una partida completa, grabando la misma si corresponde y listando al jugador en el ranking correspondiente.
 */
void jugar_juego ( configuracion_t configuracion, char ruta_archivo_grabacion[MAX_RUTA], caminos_por_nivel_t caminos_personalizados[MAX_NIVELES], 
				   char ruta_archivo_ranking[MAX_RUTA] ){

	juego_t juego;
	int puntaje;
	bool esta_nivel_inicializado = false;
	bool se_pidio_def_por_ultima_vez = false;

	srand ((unsigned)time(NULL));
	mostrar_mensaje_bienvenida();
	system("clear");

	FILE* archivo_grabacion = fopen(ruta_archivo_grabacion, "w");
	mostrar_mensaje_grabacion_en_curso(archivo_grabacion);

	inicializar_juego (&juego, configuracion);

	do{
		if (!esta_nivel_inicializado){
			inicializar_nivel(&juego, &esta_nivel_inicializado , &se_pidio_def_por_ultima_vez, configuracion, caminos_personalizados);
		}
		
		capturar_fotograma (archivo_grabacion, juego);
		jugar_turno (&juego);
		mostrar_juego (juego);
		agregar_defensor_extra (&juego, &se_pidio_def_por_ultima_vez);
		detener_el_tiempo (configuracion.velocidad_juego);
		system ("clear");

		if ( (estado_nivel(juego.nivel) == GANADO) && (juego.nivel_actual != CUARTO_NIVEL) ){
			pasar_a_nivel_siguiente (&juego, &esta_nivel_inicializado);
		}

	}while ( (estado_juego(juego) == EN_JUEGO) );
		capturar_fotograma (archivo_grabacion, juego);

	if (estado_juego(juego) == GANADO){
		mostrar_mensaje_victoria();
	
	}else{
		mostrar_mensaje_derrota();
	}

	calcular_puntaje (&puntaje, juego, configuracion);
	listar_jugador_en_ranking (ruta_archivo_ranking, puntaje);

	if(archivo_grabacion){
		fclose(archivo_grabacion);
	}	
}

/*
 * PRE: ---
 *
 * POST: Imprime por pantalla un mensaje de error alertando al usuario acerca del ingreso incorrecto de las instrucciones por comando para jugar la partida.
 */
void mostrar_mensaje_comando_incorrecto_jugar(){
	printf("\nERROR: La clave ingresada es incorrecta! La estructura es la siguiente:\n");
	printf("\t> config=[valor] grabacion=[valor]\n");
	printf("\n\t(Ambos parámetros son opcionales y la extensión de los archivos debe ser .txt y .dat respectivamente!)\n\n");
}



int manejar_jugar_juego (char argumento_1[MAX_ARGUMENTO], char argumento_2[MAX_ARGUMENTO], bool jugar_default){

	char ruta_archivo_configuracion[MAX_RUTA];
	char ruta_archivo_grabacion[MAX_RUTA];
	char ruta_archivo_ranking[MAX_RUTA];
	
	configuracion_t configuracion;
	bool es_config_default_completa = false;
	
	caminos_por_nivel_t caminos_personalizados[MAX_NIVELES];
	
	if ( son_comandos_validos_jugar(argumento_1, argumento_2, ruta_archivo_configuracion, ruta_archivo_grabacion, jugar_default) ){

		obtener_configuracion( &configuracion, ruta_archivo_configuracion, &es_config_default_completa );
		cargar_caminos_personalizados( caminos_personalizados, configuracion.ruta_archivo_camino );
		
		if (es_config_default_completa){
			strcpy (ruta_archivo_ranking, RUTA_RANKING_DEFAULT);
		
		} else{
			obtener_ruta_archivo_ranking(ruta_archivo_ranking, ruta_archivo_configuracion);
		
		}jugar_juego(configuracion, ruta_archivo_grabacion, caminos_personalizados, ruta_archivo_ranking);

		return 0;
	
	}mostrar_mensaje_comando_incorrecto_jugar();
	
	return ERROR;
}

