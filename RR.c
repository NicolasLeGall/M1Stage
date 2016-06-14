#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "RR.h"
#include "distribution.h"
#include "initialisation.h"

int RR(Antenne *antenne, int nb_user){
	int i = 0;
	int j = 0;
	int currentUser = 0;
	int debitTotalTrame = 0;
	
/*NB_SUBCARRIERS = 128 NB_TIME_SLOTS = 5 */
	for(i = 0; i < NB_TIME_SLOTS ; i++){
		for(j = 0; j< NB_SUBCARRIERS ; j++){
			//consumeBit renvois des valeurs de 0 à 10. renvois le nombre de bit consumer dans un packet d'un utilisateurs dans un time slot pour une subcarrier
			debitTotalTrame += consumeBit(antenne, currentUser, j);
			//printf("debitTotalTrame=%d\n",consumeBit(antenne, currentUser, j));
			//le modulo permet de revenir à 0 quand on a atteint le nb_user on parcour donc les utilisateur de 0 à nb_user
			currentUser = (currentUser+1) % nb_user;
		}
	}
	return debitTotalTrame;

}
