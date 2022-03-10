#include "defendiendo_torres.h"
#include <stdbool.h>
#include <time.h>


#define TOPE_TERRENO_NIVEL_1_2 15
#define TOPE_TERRENO_NIVEL_3_4 20


const static char ELFO = 'L', ENANO = 'G', ORCO = 'O';

const static int EN_JUEGO = 0, GANADO = 1, PERDIDO = -1;
const static int PRIMER_NIVEL = 1, SEGUNDO_NIVEL = 2, TERCER_NIVEL = 3, CUARTO_NIVEL = 4;
const static int NIVEL_TERMINADO = 1;


const int FUERZA_ATAQUE_ENANO = 60, FUERZA_ATAQUE_ELFO = 30;


const int DANIO_GOLPE_CRITICO = 40;
const int RANGO_ATAQUE_ENANO = 1, RANGO_ATAQUE_ELFO = 3, DISTANCIA_NULA = 0;

const static int SE_PUEDE_AGREGAR_DEFENSOR = 0, NO_SE_PUEDE_AGREGAR_DEFENSOR = -1;

const int TORRE_CAIDA = 0;
const static int VIDA_NULA_ORCO = 0, VIDA_BASE_ORCO = 200;

const int POSICION_INICIAL = 0;

const static int PROBABILIDAD_MAX = 100;

const static int PRIMER_CAMINO = 1, SEGUNDO_CAMINO = 2;
const static char ENTRADA = 'E', TORRE = 'T', CAMINO = 'X', PASTO = '~';


/*
 * PRE: Recibe una configuración ya inicializada.
 *
 * POST: Devuelve un juego correctamente inicializado a partir de la información obtenida de la configuración.
 */
void inicializar_juego(juego_t* juego, configuracion_t configuracion){
	juego->nivel_actual = PRIMER_NIVEL;

	juego->torres.resistencia_torre_1 = configuracion.resistencia_torre_1;
	juego->torres.resistencia_torre_2 = configuracion.resistencia_torre_2;
	
	juego->torres.elfos_extra = configuracion.info_elfos.cantidad_extra;
	juego->torres.enanos_extra = configuracion.info_enanos.cantidad_extra;

	juego->critico_legolas = configuracion.info_elfos.critico;
	juego->critico_gimli = configuracion.info_enanos.critico;
	
	juego->fallo_legolas = configuracion.info_elfos.fallo;
	juego->fallo_gimli = configuracion.info_enanos.fallo;
}


/*
 * PRE: Recibe un vector de orcos válido y la máxima cantidad de orcos de ese nivel.
 *	 	Cada elemento del vector (cada orco) contará con su correspondiente:
 *	 		> Camino
 *			> Posicion en camino
 *			> Vida
 *
 * POST: Devuelve:
 *			> True si quedan orcos vivos: vida > 0.
 *			> False si todos los orcos están muertos: vida <= 0.
 */
bool hay_orcos_vivos (enemigo_t enemigos[MAX_ENEMIGOS], int max_enemigos_nivel){
	bool se_encontro_orco_vivo = false;
	int i = 0;
	
	while ( (i < max_enemigos_nivel) && (!se_encontro_orco_vivo) ){
		if ( enemigos[i].vida > VIDA_NULA_ORCO ){
			se_encontro_orco_vivo = true;
		}i++;
	}
	
	return se_encontro_orco_vivo;
}


int estado_nivel (nivel_t nivel){
	if ( (!hay_orcos_vivos(nivel.enemigos, nivel.max_enemigos_nivel)) && (nivel.tope_enemigos == nivel.max_enemigos_nivel) ){
		return NIVEL_TERMINADO;
	
	}return EN_JUEGO;
}

int estado_juego (juego_t juego){
	if ( (juego.torres.resistencia_torre_1 <= TORRE_CAIDA) || (juego.torres.resistencia_torre_2 <= TORRE_CAIDA) ){
		return PERDIDO;
	}else if ( (juego.nivel_actual == CUARTO_NIVEL) && (estado_nivel (juego.nivel) == NIVEL_TERMINADO) ){
		return GANADO;
	}return EN_JUEGO;
}


/*
 * PRE: Recibe un tipo de defensor válido:
 *		  > ELFO  (L)
 *	 	  > ENANO (G)
 *
 * POST: Devuelve la fuerza de ataque de dicho defensor de acuerdo a su tipo.
 */
int fuerza_ataque_defensor (char tipo){
	if (tipo == ELFO){
		return FUERZA_ATAQUE_ELFO;
	
	}return FUERZA_ATAQUE_ENANO;
}


/*
 * PRE: Recibe un nivel previamente inicializado con sus caminos y respectivos topes.
 *
 * POST: Devuelve:
 *			> True si la coordenada en la que se desea colcocar el defensor no se superpone al camino del nivel.
 *			> False si dicha posicion se superpone con el camino.
 */
bool se_superpone_a_camino (coordenada_t posicion_defensor, nivel_t nivel){
	bool se_superpone = false;
	int i = 0;
	
	while ( (i < nivel.tope_camino_1) && (!se_superpone) ){
		if ( (posicion_defensor.fil == nivel.camino_1[i].fil) && (posicion_defensor.col == nivel.camino_1[i].col) ){
			se_superpone = true;
		}i ++;
	}

	i = 0;
	while ( (i < nivel.tope_camino_2) && (!se_superpone) ){
		if ( (posicion_defensor.fil == nivel.camino_2[i].fil) && (posicion_defensor.col == nivel.camino_2[i].col) ){
			se_superpone = true;
		}i++;
	}

	return se_superpone;
}


/*
 * PRE: Recibe el vector de defensores y su tope con la información actual.
 *
 * POST: Devuelve:
 *			> True si la coordenada en la que se desea colcocar el defensor no se superpone la coordenada de otro defensor.
 *			> False si dicha posicion se superpone con la de otro defensor ya fijado.
 */
bool se_superpone_a_defensor (coordenada_t posicion_defensor, defensor_t defensores[MAX_DEFENSORES], int tope_defensores){
	bool se_superpone = false;
	int i = 0;

	while ( (i < tope_defensores) && (!se_superpone) ){
		if ( (posicion_defensor.fil == defensores[i].posicion.fil) && (posicion_defensor.col == defensores[i].posicion.col) ){
			se_superpone = true;
		}i++;
	}

	return se_superpone;
}


int agregar_defensor (nivel_t* nivel, coordenada_t posicion_defensor, char tipo){

	if ( (!se_superpone_a_camino(posicion_defensor, *nivel)) && (!se_superpone_a_defensor(posicion_defensor, nivel->defensores, nivel->tope_defensores)) ){
		nivel->defensores[nivel->tope_defensores].tipo = tipo;
		nivel->defensores[nivel->tope_defensores].fuerza_ataque = fuerza_ataque_defensor (tipo);
		nivel->defensores[nivel->tope_defensores].posicion = posicion_defensor;
		(nivel->tope_defensores) ++;

		return SE_PUEDE_AGREGAR_DEFENSOR;
	}return NO_SE_PUEDE_AGREGAR_DEFENSOR;
}


/*
 * PRE: Recibe el vector de orcos con información válida y actual junto a su tope actualizado.
 *
 * POST: Todos los orcos cuya vida sea mayor a cero (orcos vivos), avanzarán una posición en su camino.
 */
void avanzar_orco (enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos){
	for (int i = 0; i < tope_enemigos; i++){
		if (enemigos[i].vida > VIDA_NULA_ORCO){

			enemigos[i].pos_en_camino ++;
		}
	}
}


/*
 * PRE: Recibe un nivel y torres, ambos con información válida y actual.
 *
 * POST: En caso de que el orco llegue vivo a la posición de la torre, le reducirá tantos puntos de resistencia
 		 como vida tenga el orco en cuestión. Luego, el orco muere, su vida se vuelve nula.
 */
void orco_atacar_torre(nivel_t* nivel, torres_t* torres){
	
	for (int i = 0; i < nivel->tope_enemigos; i++){
		
		if ( (nivel->enemigos[i].camino == PRIMER_CAMINO) && (nivel->enemigos[i].pos_en_camino == (nivel->tope_camino_1 - 1)) &&
			 (nivel->enemigos[i].vida > VIDA_NULA_ORCO) ){

			torres->resistencia_torre_1 -= nivel->enemigos[i].vida;
			nivel->enemigos[i].vida = VIDA_NULA_ORCO;
		
		}else if ( (nivel->enemigos[i].camino == SEGUNDO_CAMINO) && (nivel->enemigos[i].pos_en_camino == (nivel->tope_camino_2 - 1)) &&
			       (nivel->enemigos[i].vida > VIDA_NULA_ORCO) ){

			torres->resistencia_torre_2 -= nivel->enemigos[i].vida;
			nivel->enemigos[i].vida = VIDA_NULA_ORCO;
		}
	}
}


/*
 * PRE: Recibe toda la información acerca de los orcos y el nivel actual. Dicha información debe ser válida y actual.
 *
 * POST: Se inicializa un nuevo orco en la última posición del vector, con sus caracteristicas iniciales y se les asigna su camino correspondiente.
 *			> En caso de que el nivel actual sea 1: Sólo se inicializa un orco en el camino 1.
 *			> En caso de que el nivel actual sea 2: Sólo se inicializa un orco en el camino 2.
 *			> En caso de que el nivel actual sea 3 o 4: se inicializa un orco en ambos caminos.
 */
void inicializar_orco (enemigo_t enemigos[MAX_ENEMIGOS], int* tope_enemigos, int max_enemigos_nivel, int nivel_actual){
	if ( *tope_enemigos < max_enemigos_nivel){
		
		if( nivel_actual != SEGUNDO_NIVEL){

			enemigos[*tope_enemigos].camino = PRIMER_CAMINO;
			enemigos[*tope_enemigos].pos_en_camino = POSICION_INICIAL;
			enemigos[*tope_enemigos].vida = VIDA_BASE_ORCO + (rand() % PROBABILIDAD_MAX);
			(*tope_enemigos) ++;
		}

		if( nivel_actual != PRIMER_NIVEL){

			enemigos[*tope_enemigos].camino =SEGUNDO_CAMINO;
			enemigos[*tope_enemigos].pos_en_camino = POSICION_INICIAL;
			enemigos[*tope_enemigos].vida = VIDA_BASE_ORCO + (rand() % PROBABILIDAD_MAX);
			(*tope_enemigos) ++;
		}
	}
}


/*
 * PRE: Recibe los porcentajes de fallo de cada tipo de defensor previamente inicializados y un tipo de defensor válido: ELFO o ENANO.
 *
 * POST: Devuelve:
 *			> True si el número aleatorio generado está en el rango de la probabilidad de fallo.
 *			> False si el número aleatorio generado está fuera del rango de la probabilidad de fallo.
 */
bool es_golpe_fallido(int fallo_gimli, int fallo_legolas, char tipo_defensor){
	if (tipo_defensor == ENANO){
		return ( (rand() % PROBABILIDAD_MAX) < (fallo_gimli) );
	
	}else{
		return ( (rand() % PROBABILIDAD_MAX) < (fallo_legolas) );
	}
}


/*
 * PRE: Recibe los porcentajes de golpe crítico de cada tipo de defensor previamente inicializados y un tipo de defensor válido: ELFO o ENANO.
 *
 * POST: Devuelve:
 *			> True si el número aleatorio generado está en el rango de la probabilidad de golpe crítico.
 *			> False si el número aleatorio generado está fuera del rango de la probabilidad de golpe crítico.
 */
bool es_golpe_critico(juego_t juego, char tipo_defensor){
	if (tipo_defensor == ENANO){
		return ( (rand() % PROBABILIDAD_MAX) < (juego.critico_gimli) );
	
	}else{
		return ( (rand() % PROBABILIDAD_MAX) < (juego.critico_legolas) );
	}
}


/*
 * PRE: Recibe información sobre los defensores (tipo y daños), y la posición en el vector del orco que será atacado, válidos.
 *		El juego debe estar previamente inicializado.
 *
 * POST: Se reducirá la vida del orco seleccionado de acuerdo al tipo de golpe efectuado (crítico o básico) y al tipo de defensor que lo efectúe.
 */
void bajar_vida_orco(juego_t* juego, char tipo_defensor, int golpe_base, int golpe_critico, int orco_id){

	if( es_golpe_critico(*juego, tipo_defensor) ){
		juego->nivel.enemigos[orco_id].vida -= golpe_critico;
	}else{
		juego->nivel.enemigos[orco_id].vida -= golpe_base;
	}
}


/*
 * PRE: Recibe el nivel actual correctamente inicializado y una posición válida del enano que atacará.
 *
 * POST: Devuelve:
 *			> True si algún orco se encuentra en el rango de ataque del enano: las ocho celdas de su alrededor.
 *			> False si no hay ningún orco en el rango de ataque del enano.
 */
bool hay_orco_en_rango_de_enano ( nivel_t nivel, coordenada_t posicion_defensor, int* orco_id ){
	bool orco_encontrado = false;
	coordenada_t posicion_orco;
	int i = 0;
	
	while ( (i < nivel.tope_enemigos) && !orco_encontrado ){
		*orco_id = i;

		if (nivel.enemigos[i].camino == PRIMER_CAMINO){
			posicion_orco = nivel.camino_1[nivel.enemigos[i].pos_en_camino];
		}else{
			posicion_orco = nivel.camino_2[nivel.enemigos[i].pos_en_camino];
		}
		
		if (  ( ((abs(posicion_defensor.fil - posicion_orco.fil)) == RANGO_ATAQUE_ENANO) && ((abs(posicion_defensor.col - posicion_orco.col)) == RANGO_ATAQUE_ENANO) ) ||
			  ( ((abs(posicion_defensor.fil - posicion_orco.fil)) == RANGO_ATAQUE_ENANO) && ((abs(posicion_defensor.col - posicion_orco.col)) == DISTANCIA_NULA) )     ||
			  ( ((abs(posicion_defensor.fil - posicion_orco.fil)) == DISTANCIA_NULA)     && ((abs(posicion_defensor.col - posicion_orco.col)) == RANGO_ATAQUE_ENANO) )  ){
				
				if ( nivel.enemigos[i].vida > VIDA_NULA_ORCO ){
					orco_encontrado = true;
				}
		}
		i ++;
	}

	return orco_encontrado;
}


/*
 * PRE: Recibe el nivel actual correctamente inicializado, una posición válida del elfo que atacará y la posición en su vector (orco_id)
 *		del orco que se desea chequear.
 *
 * POST: Devuelve:
 *			> True si el orco se encuentra en el rango de ataque del elfo: a una distancia "Manhattan" igual o menor a tres celdas.
 *			> False si el orco no está dentro del rango de ataque del elfo.
 */
bool hay_orco_en_rango_de_elfo ( nivel_t nivel, coordenada_t posicion_defensor, int orco_id ){
	coordenada_t posicion_orco;

	if (nivel.enemigos[orco_id].camino == PRIMER_CAMINO){
		posicion_orco = nivel.camino_1[nivel.enemigos[orco_id].pos_en_camino];
	}else{
		posicion_orco = nivel.camino_2[nivel.enemigos[orco_id].pos_en_camino];
	}

	if ( ( (abs(posicion_defensor.fil - posicion_orco.fil) + abs(posicion_defensor.col - posicion_orco.col)) <= RANGO_ATAQUE_ELFO ) &&
		 ( nivel.enemigos[orco_id].vida > VIDA_NULA_ORCO ) ){
		
		return true;
	
	}return false;
}


/*
 * PRE: Recibe el juego con las fuerzas de ataque y probabilidad de crítico previamente inicializadas, y el nivel actual con
 *		información correcta y actualizada acerca de los orcos y los defensores.
 *
 * POST: En caso de que no sea un golpe fallido, el defensor atacará a un orco o a varios propiciándoles un golpe básico o crítico (+40).
 *		 En caso de un golpe fallido o la ausencia de orcos en rango de defensores, ningún orco saldrá herido.
 */	
void defensor_atacar_orco (juego_t* juego){

	int golpe_base, golpe_critico;
	int orco_id;
	coordenada_t posicion_defensor;

	for (int i = 0; i < juego->nivel.tope_defensores; i++){
		
		posicion_defensor = juego->nivel.defensores[i].posicion;
		golpe_base = juego->nivel.defensores[i].fuerza_ataque;
		golpe_critico = golpe_base + DANIO_GOLPE_CRITICO;
		
		if (juego->nivel.defensores[i].tipo == ENANO){
			
			if( ( hay_orco_en_rango_de_enano( juego->nivel, posicion_defensor, &orco_id)) && 
			    (!es_golpe_fallido(juego->fallo_gimli, juego->fallo_legolas, ENANO)) ){

				bajar_vida_orco( juego, ENANO, golpe_base, golpe_critico, orco_id );
			}

		}else{

			for ( orco_id = 0; orco_id < juego->nivel.tope_enemigos; orco_id++){
				
				if( ( hay_orco_en_rango_de_elfo( juego->nivel, posicion_defensor, orco_id )) && 
					(!es_golpe_fallido(juego->fallo_gimli, juego->fallo_legolas, ELFO)) ){

					bajar_vida_orco( juego, ELFO, golpe_base, golpe_critico, orco_id );
				}
			}
		}
	}
}

void jugar_turno(juego_t* juego){
	defensor_atacar_orco(juego);
	avanzar_orco(juego->nivel.enemigos, juego->nivel.tope_enemigos);
	orco_atacar_torre( &(juego->nivel), &(juego->torres) );
	inicializar_orco(juego->nivel.enemigos, &(juego->nivel.tope_enemigos), juego->nivel.max_enemigos_nivel, juego->nivel_actual );

} 


/*
 * PRE: Recibe el vector de enemigos y su tope con información válida y actualizada.
 *
 * POST: Devuelve: la cantidad de orcos vivos que quedan, es decir, con vida mayor a cero.
 */
int orcos_eliminados(enemigo_t enemigos[MAX_ENEMIGOS], int tope_enemigos){
	int cantidad_orcos_eliminados = 0;
	for (int i = 0; i < tope_enemigos; i++){
		if (enemigos[i].vida <= VIDA_NULA_ORCO ){
			cantidad_orcos_eliminados ++;
		}
	}return cantidad_orcos_eliminados;
}


/*
 * PRE: Recibe el vector de defensores y su tope con información válida y actualizada, y un tipo de defensor válido: ELFO o ENANO.
 *
 * POST: Devuelve la cantidad de defensores de ese tipo que posee el usuario actualmente. 
 */
int cantidad_defensores_segun_tipo(defensor_t defensores[MAX_DEFENSORES], int tope_defensores, char tipo){
	int cantidad_defensores = 0;
	for (int i = 0; i < tope_defensores; i++){
		if (defensores[i].tipo == tipo ){
			cantidad_defensores ++;
		}
	}return cantidad_defensores;
} 


/*
 * PRE: Recibe el juego con información válida y actualizada.
 *
 * POST: Muestra por pantalla información relevante para el usuario a la hora de jugar el nivel.
 */
void mostrar_informacion(juego_t juego){
	int cantidad_orcos_eliminados = orcos_eliminados(juego.nivel.enemigos, juego.nivel.tope_enemigos);
	int cantidad_elfos_actuales = cantidad_defensores_segun_tipo(juego.nivel.defensores, juego.nivel.tope_defensores, ELFO);
	int cantidad_enanos_actuales = cantidad_defensores_segun_tipo(juego.nivel.defensores, juego.nivel.tope_defensores, ENANO);

	printf("===================================================================================================================\n");
	printf("					DEFENDIENDO LAS TORRES: NIVEL %i\n", juego.nivel_actual);
	printf("===================================================================================================================\n");

	printf("					Resistencia: TORRE 1  %i♥	TORRE 2  %i♥\n", juego.torres.resistencia_torre_1, juego.torres.resistencia_torre_2);
	printf("	 Orcos eliminados: %i / %i 		||		 Elfos: %i 		Enanos: %i\n", cantidad_orcos_eliminados, juego.nivel.max_enemigos_nivel, 
		                                                                                               cantidad_elfos_actuales, cantidad_enanos_actuales);
	printf("===================================================================================================================\n");
	printf("                               FALLO ELFOS: %i  ||    FALLO ENANOS: %i\n", juego.fallo_legolas, juego.fallo_gimli);
	printf("                             CRITICO ELFOS: %i  ||  CRITICO ENANOS: %i\n", juego.critico_legolas, juego.critico_gimli);
	printf("===================================================================================================================\n");
}


/*
 * PRE: Recibe un camino válido junto con su tope correcto.
 *
 * POST: Se carga el camino al terreno, identificando las entradas y torres.
 */
void cargar_camino ( coordenada_t camino[MAX_LONGITUD_CAMINO], int tope_camino, char terreno[MAX_FILAS][MAX_COLUMNAS] ){
	
	for (int i = 0; i < tope_camino; i++){
		if ( i == 0){
			terreno[camino[i].fil][camino[i].col] = ENTRADA;
		}
		else if (i == tope_camino-1){
			terreno[camino[i].fil][camino[i].col] = TORRE;
		}
		else{
			terreno[camino[i].fil][camino[i].col] = CAMINO;
		}
	}	
}


/*
 * PRE: Se recibe un juego con información válida.
 *
 * POST: Devuelve una matriz con el terreno y el camino cargado.
 */
void cargar_terreno ( juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS] ){

	if (juego.nivel_actual == TERCER_NIVEL || juego.nivel_actual == CUARTO_NIVEL){	
		
		for (int i = 0; i <= TOPE_TERRENO_NIVEL_3_4; i++){
			for (int j = 0; j <= TOPE_TERRENO_NIVEL_3_4; j++){
				terreno[i][j] = PASTO;
			}
		}cargar_camino(juego.nivel.camino_1, juego.nivel.tope_camino_1, terreno);
		cargar_camino(juego.nivel.camino_2, juego.nivel.tope_camino_2, terreno);

	}else{

		for (int i = 0; i <= TOPE_TERRENO_NIVEL_1_2; i++){
			for (int j = 0; j <= TOPE_TERRENO_NIVEL_1_2; j++){
				terreno[i][j] = PASTO;
			}
		}

		if ( juego.nivel_actual == PRIMER_NIVEL){
			cargar_camino(juego.nivel.camino_1, juego.nivel.tope_camino_1, terreno);
		
		}else{
			cargar_camino(juego.nivel.camino_2, juego.nivel.tope_camino_2, terreno);
		}
	}
}

/*
 * PRE: Se recibe un nivel con información válida.
 *
 * POST: Se cargan los defensores a la matriz.
 */
void cargar_defensores ( nivel_t nivel, char terreno[MAX_FILAS][MAX_COLUMNAS] ){
	coordenada_t posicion_defensor;
	
	for (int i = 0; i < nivel.tope_defensores; i++){
		posicion_defensor = nivel.defensores[i].posicion;
		
		if(nivel.defensores[i].tipo == ELFO){
			terreno[posicion_defensor.fil][posicion_defensor.col] = ELFO;
		
		}else{
			terreno[posicion_defensor.fil][posicion_defensor.col] = ENANO;
		}
	}
}

/*
 * PRE: Se recibe un nivel con información válida.
 *
 * POST: Se cargan los orcos a la matriz.
 */
void cargar_orcos(nivel_t nivel, char terreno[MAX_FILAS][MAX_COLUMNAS]){
	int posicion_orco;

	for(int i = 0; i < nivel.tope_enemigos; i++){
		posicion_orco = nivel.enemigos[i].pos_en_camino;
		
		if ( (nivel.enemigos[i].camino == PRIMER_CAMINO) && (nivel.enemigos[i].vida > VIDA_NULA_ORCO) ){
			terreno[nivel.camino_1[posicion_orco].fil][nivel.camino_1[posicion_orco].col] = ORCO;
		
		}else if ( (nivel.enemigos[i].camino == SEGUNDO_CAMINO) && (nivel.enemigos[i].vida > VIDA_NULA_ORCO) ) {
			terreno [nivel.camino_2[posicion_orco].fil] [nivel.camino_2[posicion_orco].col] = ORCO;
		}
	}
}

/*
 * PRE: Se recibe un juego con información válida y actualizada.
 *
 * POST: Se muestra por pantalla el mapa del juego con todos sus elementos (camino, defensores, orcos, entradas y torres).
 */
void mostrar_mapa(juego_t juego){
	char terreno[MAX_FILAS][MAX_COLUMNAS];
	cargar_terreno(juego, terreno);
	cargar_defensores(juego.nivel, terreno);
	cargar_orcos(juego.nivel, terreno);
	int tope_terreno;

	if(juego.nivel_actual == PRIMER_NIVEL || juego.nivel_actual == SEGUNDO_NIVEL ){
		tope_terreno = TOPE_TERRENO_NIVEL_1_2;
	
	}else{
		tope_terreno = TOPE_TERRENO_NIVEL_3_4;
	}
	

	printf("    ");
	
	for(int columna = 0; columna < tope_terreno; columna ++){
		if (columna < 10){
			printf("0%i|", columna);
		}else{
			printf("%i|", columna);
		}
	
	}printf("\n");


	for (int i = 0; i < tope_terreno; i++){
		if (i < 10){
			printf("0%i|", i);
		}else{
			printf("%i|", i);
		}
		for (int j = 0; j < tope_terreno; j++){
			printf(" %2c", terreno[i][j]);
		}printf("\n");
	
	}
}


/*
 * PRE: Se recibe un juego con información válida y actualizada.
 *
 * POST: Se muetsra por pantalla tanto la información necesaria que debe conocer el usuario como asi también el mapa con los datos actuales.
 */
void mostrar_juego(juego_t juego){
	mostrar_informacion(juego);
	mostrar_mapa(juego);
}