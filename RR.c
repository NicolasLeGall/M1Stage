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
	int tamp = 0;
	int temp_user;

	
	for(i = 0; i < NB_TIME_SLOTS ; i++){
		for(j = 0; j< NB_SUBCARRIERS ; j++){
			while(tamp != -1){
				// on regarde si l'utilisateur a quelque chose a consommer
				if(antenne->users[currentUser]->bufferVide == 0){
					//consumeBit renvois des valeurs de 0 à 10. renvois le nombre de bit consumer dans un packet d'un utilisateurs dans un time slot pour une subcarrier
					debitTotalTrame += consumeBit(antenne, currentUser, j);
					
					tamp = -1;
				
				//sinon on passe a l'user d'apres
				}else{
					//permet de parcourir tout les utilisateurs pour voir si chaqun a quelque chose a consommer.
					tamp = tamp + 1;
					if(tamp == nb_user){
						tamp = -1;
					}
				}
				//le modulo permet de revenir à 0 quand on a atteint le nb_user on parcour donc les utilisateur de 0 à nb_user
				currentUser = (currentUser+1) % nb_user;
				
			}
			tamp = 0;
		}
	}

	
	return debitTotalTrame;

}
