#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "distribution.h"
#include "initialisation.h"

double gaussrand()
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
			} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else
		X = V2 * sqrt(-2 * log(S) / S);

	phase = 1 - phase;

	return X; 
}
/*ne marche pas pour une distance supérieur à 10*/
/*Donne une valeur aléatoire entre 0 et 10 mais la moyenne des valeurs donné est égale a distance */
int getSNR(int distance){
	int r = 0;
	double temp = ((gaussrand()*2)+ distance );
	r = (int)temp;
	if(r%2){
		r++;
	}
	if(r > 10 || r < 0){
		return getSNR(distance);
	}
	return r;
}

int getNbBit(){
	int r = 0;
	double temp = ((gaussrand()*50)+ 250 );
	r = (int)temp;
	if(r%2){
		r++;
	}
	if(r > 400 || r < 150){
		return getNbBit();
	}
	return r;
}