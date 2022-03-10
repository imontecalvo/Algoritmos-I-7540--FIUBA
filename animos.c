#include "animos.h"

const int DIA_DEL_MES_MIN = 1, DIA_DEL_MES_MAX = 30;

const int VIENTO_ALTO = 75, VIENTO_MEDIO = 50, VIENTO_BAJO = 25;
const int DIA_MAX_DE_VIENTO_ALTO = 10, DIA_MAX_DE_VIENTO_MEDIO = 20;

const char MANIANA = 'M', TARDE = 'T', NOCHE = 'N';

const int HUMEDAD_ALTA = 75, HUMEDAD_MEDIA = 50, HUMEDAD_BAJA = 25;

const char PIE_DERECHO = 'D', PIE_IZQUIERDO = 'I';
const int PUNTOS_PIE_DERECHO = 10, PUNTOS_PIE_IZQUIERDO = 0;

const char ENSALADA = 'E', HAMBURGUESA = 'H', PIZZA = 'P', GUISO = 'G';
const int PUNTOS_ENSALADA = 20, PUNTOS_HAMBURGUESA = 15, PUNTOS_PIZZA = 10, PUNTOS_GUISO = 5;

const int HORAS_DE_SUENIO_MIN = 0, POCAS_HORAS_DE_SUENIO_MAX = 4, MEDIANAS_HORAS_DE_SUENIO_MAX= 8, HORAS_DE_SUENIO_MAX = 12;
const int PUNTOS_POCAS_HORAS_DE_SUENIO = 0, PUNTOS_MEDIANAS_HORAS_DE_SUENIO = 10, PUNTOS_MUCHAS_HORAS_DE_SUENIO = 20;

const char ANIMO_BUENO = 'B', ANIMO_REGULAR = 'R', ANIMO_MALO = 'M';
const int PUNTOS_MAX_DE_ANIMO_MALO = 20, PUNTOS_MAX_DE_ANIMO_REGULAR = 35;



void mostrar_bienvenida (){
	printf("\n╔════════════════════════════════════════════════════════════════════════╗\n");
	printf("║··························INICIALIZAR···ANIMOS··························║ ");
	printf("\n╚════════════════════════════════════════════════════════════════════════╝\n");
}


/*Pre: Debe existir un día del mes a verificar.
*Post: Devuelve true si el dia está en el rango de dias válidos (entre DIA_DEL_MES_MIN y DIA_DEL_MES_MAX), sino false.
*/
bool es_valido_dia_del_mes (int dia_del_mes){
	return ( (dia_del_mes >= DIA_DEL_MES_MIN) && (dia_del_mes <= DIA_DEL_MES_MAX) );
}
	
/*Pre: ---
*Post: El dia es válido, está en el rango de DIA_DEL_MES_MIN y DIA_DEL_MES_MAX.
*/
void averiguar_dia_del_mes (int* dia_del_mes){
	printf("Introduzca el dia del mes (entre %i y %i): ", DIA_DEL_MES_MIN, DIA_DEL_MES_MAX);
	scanf("%i", dia_del_mes);

	while (!es_valido_dia_del_mes(*dia_del_mes)){
		printf("Dia no válido :( Volvé a ingresarlo, acordate que tiene que estar entre %i y %i: ", DIA_DEL_MES_MIN, DIA_DEL_MES_MAX);	
		scanf("%i", dia_del_mes);	
	}
}


/*Pre: El dia debe ser válido, debe estar en el rango de DIA_DEL_MES_MIN y DIA_DEL_MES_MAX.
*Post: Devuelve true si está en epoca de viento alto: entre DIA_DEL_MES_MIN y DIA_MAX_DE_VIENTO_ALTO, sino devuelve false.
*/
bool es_epoca_de_viento_alto (int dia_del_mes){
	return ( (dia_del_mes <= DIA_MAX_DE_VIENTO_ALTO) );
}

/*Pre: El dia debe ser válido, debe estar en el rango de DIA_DEL_MES_MIN y DIA_DEL_MES_MAX.
*Post: Devuelve true si está en epoca de viento medio: entre DIA_MAX_DE_VIENTO_ALTO y DIA_MAX_DE_VIENTO_MEDIO, sino devuelve false.
*/
bool es_epoca_de_viento_medio (int dia_del_mes){
	return ( (dia_del_mes > DIA_MAX_DE_VIENTO_ALTO) && (dia_del_mes <= DIA_MAX_DE_VIENTO_MEDIO) );
}

/*Pre: El dia es válido, está en el rango de DIA_DEL_MES_MIN y DIA_DEL_MES_MAX, y por lo tanto en época de viento alto, medio o bajo.
*Post: La velocidad del viento es: VIENTO_ALTO, VIENTO_MEDIO o VIENTO_BAJO.
*/
int calculo_del_viento (int dia_del_mes){
	if (es_epoca_de_viento_alto (dia_del_mes)){
		return VIENTO_ALTO;

	}else if (es_epoca_de_viento_medio (dia_del_mes)){
		return VIENTO_MEDIO;
	
	}else{
		return VIENTO_BAJO;
	}
}


/*Pre: Debe existir una hora del dia a verificar.
*Post: Devuelve true si la hora es válida: MANIANA, TARDE o NOCHE, sino devuelve false.
*/
bool es_valida_hora_del_dia (char hora_del_dia){
	return( (hora_del_dia == MANIANA) || (hora_del_dia == TARDE) || (hora_del_dia == NOCHE) );
}

/*Pre: ---
*Post: La hora es válida: es MANIANA, TARDE o NOCHE.
*/
void averiguar_hora_del_dia (char* hora_del_dia){
	printf("\nIntroduzca la hora del dia: MAÑANA (%c) , TARDE (%c) o NOCHE (%c): ", MANIANA, TARDE, NOCHE);
	scanf(" %c", hora_del_dia);

	while (!es_valida_hora_del_dia(*hora_del_dia)){
		printf("Hora no válida :( Volvé a ingresarla, acordate que tiene que ser: %c , %c o %c: ", MANIANA, TARDE, NOCHE);	
		scanf(" %c", hora_del_dia);
	}
}

/*Pre: La hora debe ser válida: debe ser MANIANA, TARDE o NOCHE.
*Post: El porcentaje de la a humedad es: HUMEDAD_ALTA , HUMEDAD_MEDIA o HUMEDAD_BAJA.
*/
int calculo_de_humedad (int hora_del_dia){
	if (hora_del_dia == MANIANA){
		return HUMEDAD_ALTA;
	
	}else if (hora_del_dia == TARDE){
		return HUMEDAD_BAJA;
	
	}else{
		return HUMEDAD_MEDIA;
	}
}



/*Pre: Debe existir un pie a verificar.
*Post: Devuelve true si el pie es válido: PIE_DERECHO o PIE_IZQUIERDO, sino devuelve false.
*/
bool es_valido_pie_al_levantarse (char pie_al_levantarse){
	return ( (pie_al_levantarse == PIE_DERECHO) || (pie_al_levantarse == PIE_IZQUIERDO) );
}

/*Pre: ---
*Post: El pie es válido: PIE_DERECHO o PIE_IZQUIERDO.
*/
void averiguar_pie_al_levantarse (char* pie_al_levantarse){
	printf("- Con que pie se levantó? Derecho (%c) o Izquierdo (%c): ", PIE_DERECHO, PIE_IZQUIERDO);
	scanf(" %c", pie_al_levantarse);

	while (!es_valido_pie_al_levantarse(*pie_al_levantarse)){
		printf("Pie no válido :( Volvé a ingresarlo, acordate que tiene que ser %c o %c: ", PIE_DERECHO, PIE_IZQUIERDO);	
		scanf(" %c", pie_al_levantarse);
	}
}

/*Pre: El pie debe ser válido: PIE_DERECHO o PIE_IZQUIERDO.
*Post: Devuelve los puntos correspondientes al valor del pie con que se levantó (PUNTOS_PIE_DERECHO o PUNTOS_PIE_IZQUIERDO).
*/
int puntaje_pie_al_levantarse (char pie_al_levantarse){
	if(pie_al_levantarse == PIE_DERECHO){
		return (PUNTOS_PIE_DERECHO);

	}else{
		return (PUNTOS_PIE_IZQUIERDO);
	}
}


/*Pre: Debe existir una cena a verificar.
*Post: Devuelve true si la cena es válida: ENSALADA, HAMBURGUESA, PIZZA o GUISO, sino devuelve false.
*/
bool es_valida_cena (char cena){
	return ( (cena == ENSALADA) || (cena == HAMBURGUESA) || (cena == PIZZA) || (cena == GUISO));
}

/*Pre: ---
*Post: La cena es válida: ENSALADA , HAMBURGUESA , PIZZA  o GUISO.
*/
void averiguar_cena (char* cena){
	printf("\n- Qué cenó anoche? Ensalada (%c), Hamburguesa (%c), Pizza (%c) o Guiso (%c): ", ENSALADA, HAMBURGUESA, PIZZA, GUISO);
	scanf(" %c", cena);

	while (!es_valida_cena(*cena)){
		printf("Cena no válida :( Volvé a ingresarla, acordate que tiene que ser %c, %c, %c o %c: ", 
		ENSALADA, HAMBURGUESA, PIZZA, GUISO);
		scanf(" %c", cena);
	}
}

/*Pre: La cena debe ser válida: ENSALADA , HAMBURGUESA , PIZZA  o GUISO.
*Post: Devuelve los puntos correspondientes a la cena introducida (PUNTOS_ENSALADA, PUNTOS_HAMBURGUESA, PUNTOS_PIZZA o PUNTOS_GUISO).
*/
int puntaje_cena (char cena){
	if(cena == ENSALADA){
		return (PUNTOS_ENSALADA);

	}else if(cena == HAMBURGUESA){
		return (PUNTOS_HAMBURGUESA);

	}else if(cena == PIZZA){
		return (PUNTOS_PIZZA);

	}else{
		return (PUNTOS_GUISO);
	}
}


/*Pre: Debe existir una cantidad de horas de sueño a verificar.
*Post: Devuelve true si las horas de sueño son válidas, están en el rango HORAS_DE_SUENIO_MIN y HORAS_DE_SUENIO_MAX, sino false.
*/
bool es_valida_horas_de_suenio (int horas_de_suenio){
	return ( (horas_de_suenio >= HORAS_DE_SUENIO_MIN) && (horas_de_suenio <= HORAS_DE_SUENIO_MAX) );
}

/*Pre: ---
*Post: Las horas de sueño son válidas: están en el rango HORAS_DE_SUENIO_MIN y HORAS_DE_SUENIO_MAX.
*/
void averiguar_horas_de_suenio (int* horas_de_suenio){
	printf("\n- Cuántas horas durmió anoche? Ingresá un valor entre %i y %i (inclusive): ", HORAS_DE_SUENIO_MIN, HORAS_DE_SUENIO_MAX);
	scanf("%i", horas_de_suenio);

	while (!es_valida_horas_de_suenio(*horas_de_suenio)){
		printf("Horas de sueño no válida :( Volvé a ingresarla, acordate que tiene que estar entre %i y %i (inclusive): ", 
			HORAS_DE_SUENIO_MIN, HORAS_DE_SUENIO_MAX);
		scanf("%i", horas_de_suenio);
	}
}

/*Pre: Las horas de sueño deben estar en el rango HORAS_DE_SUENIO_MIN y HORAS_DE_SUENIO_MAX.
*Post: Devuelve los puntos correspondientes a las horas que durmió (PUNTOS_POCAS_HORAS_DE_SUENIO, 
       PUNTOS_MEDIANAS_HORAS__DE_SUEÑO, PUNTOS_MUCHAS_HORAS_DE_SUEÑO).
*/
int puntaje_horas_de_suenio (int horas_de_suenio){
	if ( (horas_de_suenio <= POCAS_HORAS_DE_SUENIO_MAX) ){
		return (PUNTOS_POCAS_HORAS_DE_SUENIO);

	}else if( (horas_de_suenio > POCAS_HORAS_DE_SUENIO_MAX) && (horas_de_suenio <= MEDIANAS_HORAS_DE_SUENIO_MAX) ){
		return (PUNTOS_MEDIANAS_HORAS_DE_SUENIO);

	}else{
		return (PUNTOS_MUCHAS_HORAS_DE_SUENIO);
	}
}



/*Pre: El pie con que se levantó, la cena y las horas de sueño deben ser válidos.
*Post: Devuelve la suma de puntaje_pie_al_levantarse, puntaje_cena y puntaje_horas_de_suenio.
*/
int suma_de_puntos_animo (char pie_al_levantarse, char cena, int horas_de_suenio){

	return( puntaje_pie_al_levantarse(pie_al_levantarse) + puntaje_cena(cena) + puntaje_horas_de_suenio(horas_de_suenio) );
}

/*Pre: El pie con que se levantó, la cena y las horas de sueño deben ser válidos.
*Post: Devuelve el estado de animo: BUENO, REGULAR o MALO según la suma de puntos de animo.
*/
char calculo_del_animo (char pie_al_levantarse, char cena, int horas_de_suenio){
	if (suma_de_puntos_animo (pie_al_levantarse, cena, horas_de_suenio) <= PUNTOS_MAX_DE_ANIMO_MALO){
		return ANIMO_MALO;
	}else if ( (suma_de_puntos_animo(pie_al_levantarse, cena, horas_de_suenio) > PUNTOS_MAX_DE_ANIMO_MALO) && 
		     (suma_de_puntos_animo(pie_al_levantarse, cena, horas_de_suenio) <= PUNTOS_MAX_DE_ANIMO_REGULAR) ){
		return ANIMO_REGULAR;
	}else{
		return ANIMO_BUENO;
	}
}

/*Pre: Se requiere de los 3 parámetros básicos válidos (un pie con que se levantó, cena y horas de sueño).
*Post: Devuelve el estado de ánimo de Legolas: BUENO, REGULAR o MALO.
*/
void determinar_animo_legolas (char* pie_al_levantarse, char* cena, int* horas_de_suenio, char* animo_legolas){
	
	printf("\n\nA continuación requerimos que nos proporciones información de carácter personal sobre Legolas para determinar su ánimo.");
	printf("\nLa información ingresada estará segura y no será compartida con nadie, por favor responder:\n\n");
	
	averiguar_pie_al_levantarse(pie_al_levantarse);
	averiguar_cena(cena);
	averiguar_horas_de_suenio(horas_de_suenio);

	*animo_legolas = calculo_del_animo(*pie_al_levantarse, *cena, *horas_de_suenio);
}

/*Pre: Se requiere de los 3 parámetros básicos válidos (un pie con que se levantó, cena y horas de sueño).
*Post: Devuelve el estado de animo de Gimli: BUENO, REGULAR o MALO.
*/
void determinar_animo_gimli (char* pie_al_levantarse, char* cena, int* horas_de_suenio, char* animo_gimli){
	
	printf("\n\nA continuación requerimos que nos proporciones información de carácter personal sobre Gimli para determinar su ánimo.");
	printf("\nLa información ingresada estará segura y no será compartida con nadie, por favor responder:\n\n");

	averiguar_pie_al_levantarse(pie_al_levantarse);
	averiguar_cena(cena);
	averiguar_horas_de_suenio(horas_de_suenio);

	*animo_gimli = calculo_del_animo(*pie_al_levantarse, *cena, *horas_de_suenio);
}

/*Pre: La velocidad del viento debe ser válida: VIENTO_BAJO, VIENTO_MEDIO o VIENTO_ALTO.
*Post: Se muestra un mensaje dependiendo de la velocidad del viento.
*/
void mostrar_mensaje_viento (int viento){
	if (viento == VIENTO_BAJO){
		printf("\nViento tranqui a unos %i km/h. ", VIENTO_BAJO);

	}else if (viento == VIENTO_MEDIO){
		printf("\nSe presencia un moderado viento con una velocidad de %i km/h. ", VIENTO_MEDIO);

	}else {
		printf("\nViento fuertísimo galopando a unos %i km/h, se observan árboles caídos y casas sin techo... ", VIENTO_ALTO);
	}
}

/*Pre: El porcentaje de la humedad debe ser válido: HUMEDAD_BAJA, HUMEDAD_MEDIA o HUMEDAD_ALTA.
*Post: Se muestra un mensaje dependiendo del porcentaje de la humedad.
*/
void mostrar_mensaje_humedad (int humedad){
	if (humedad == HUMEDAD_BAJA){
		printf("La humedad casi ni se nota... está a un %i por ciento.", HUMEDAD_BAJA);

	}else if (humedad == HUMEDAD_MEDIA){
		printf("En cuanto a la humedad, meh, miti y miti, está a un %i por ciento.", HUMEDAD_MEDIA);

	}else {
		printf("Muuucha humedad en el ambiente, está a un %i por ciento.", HUMEDAD_ALTA);
	}
}

/*Pre: El ánimo de Legolas debe ser válido: ANIMO_MALO, ANIMO_REGULAR o ANIMO_BUENO.
*Post: Se muestra un mensaje dependiendo del ánimo de Legolas.
*/
void mostrar_mensaje_animo_legolas (char animo_legolas){
	if (animo_legolas == ANIMO_MALO){
		printf("\nLegolas no está pasando por su mejor momento anímico (｡•︵•｡) , ");

	}else if (animo_legolas == ANIMO_REGULAR){
		printf("\nLa cara Legolas no denota felicidad ni tristeza, simplemente es un día más de su vida ┐(ﾟ～ﾟ)┌ , ");

	}else {
		printf("\nLegolas está pasando por su mejor momento, está listo para darlo todo ⊂(◉‿◉)つ , ");
	}
}

/*Pre: El animo de Gimli debe ser válido: ANIMO_MALO, ANIMO_REGULAR o ANIMO_BUENO.
*Post: Se muestra un mensaje dependiendo del ánimo de Gimli.
*/
void mostrar_mensaje_animo_gimli (char animo_gimli){

	if (animo_gimli == ANIMO_MALO){
		printf("y Gimli anda un poco bajoneado, pero por cuestiones de privacidad no podemos revelar qué le ocurre (｡•︵•｡) ");

	}else if (animo_gimli == ANIMO_REGULAR){
		printf("y Gimli está maso, no es su mejor día pero tampoco el peor... ┐(ﾟ～ﾟ)┌");

	}else {
		printf("y Gimli tiene el ánimo muy arriba, pocas veces se lo ha visto tan felíz ⊂(◉‿◉)つ");
	}
}

/*Pre: Se requiere de la velocidad del viento, porcentaje de humedad y ánimo de Legolas y Gimli.
*Post: Se muestra un mensaje dependiendo de la velocidad del viento, humedad, y ánimo de Legolas y Gimli, junto con el resumen.
*/
void mostrar_resultado_esperado (int viento, int humedad, char animo_legolas, char animo_gimli){
	
	printf("\nDe acuerdo a la información proporcionada, podemos concluir que:");
	mostrar_mensaje_viento(viento);
	mostrar_mensaje_humedad(humedad);
	mostrar_mensaje_animo_legolas(animo_legolas);
	mostrar_mensaje_animo_gimli(animo_gimli);

	printf("\n\nResumen: %i %i %c %c.\n", viento, humedad, animo_legolas, animo_gimli);
}



void animos(int* viento , int* humedad , char* animo_legolas , char* animo_gimli){
	
	int dia_del_mes; char hora_del_dia;
	char pie_al_levantarse_legolas, cena_legolas; int horas_de_suenio_legolas;
	char pie_al_levantarse_gimli, cena_gimli; int horas_de_suenio_gimli;

	mostrar_bienvenida();

	averiguar_dia_del_mes(&dia_del_mes);
	*viento = calculo_del_viento(dia_del_mes);
	
	averiguar_hora_del_dia(&hora_del_dia);
	*humedad = calculo_de_humedad(hora_del_dia);
	
	determinar_animo_legolas(&pie_al_levantarse_legolas, &cena_legolas, &horas_de_suenio_legolas, animo_legolas);
	determinar_animo_gimli(&pie_al_levantarse_gimli, &cena_gimli, &horas_de_suenio_gimli, animo_gimli);
	
	mostrar_resultado_esperado(*viento, *humedad, *animo_legolas, *animo_gimli);

}