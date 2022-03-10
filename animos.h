#ifndef __ANIMOS_H__
#define __ANIMOS_H__
#include <stdio.h>
#include <stdbool.h>

/*
 * PRE: ---
 *
 * POST: Devuelve:
 *			> Viento válido: 75 , 50 o 25.
 *			> Humedad válida: 75 , 50 o 25.
 *			> Ánimos de Legolas y Gimli válidos: Animo Bueno (B) , Regular (R) o Malo (M).
 */
void animos(int* viento , int* humedad , char* animo_legolas , char* animo_gimli);

#endif /* __ANIMOS_H__ */