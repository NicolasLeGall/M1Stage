#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "maxSNR.h"
#include "distribution.h"
#include "initialisation.h"
#include "MRG32k3a.h"

int maxSNR(Antenne *antenne, int nb_user) {	
	int MaxU = 0;
	int i, g, j, debitTotalTrame = 0;
	int count = 0;
	int new = 0;
	int random_user = 0;
	
	//faire un tableau qui enregistre si on passer sur un gars ou pas
	
/*NB_SUBCARRIERS = 128 NB_TIME_SLOTS = 5 */
	for(g = 0; g < NB_TIME_SLOTS ; g++){// parcours les timeslots, //tant que User.BufferVide > 0 ou que g<5, on transmet au debit actuel a cet user
		for(j = 0; j < NB_SUBCARRIERS ; j++){ //parcourt les subcariers
			/*pour empécher le cas ou MaxU reste MAxu alors que sont buffer est vide*/
			new = 0;
			for (i = 0; i < nb_user ; i++){
				if(antenne->users[i]->bufferVide == 0){
					MaxU = i;
					break;
				}
			}
			random_user=(int)(MRG32k3a()*nb_user);
			for (i = random_user; i < nb_user ; i++){
				/*si le SNR est mieu que celui a le meilleur SNR jusqu'a present et que buffervide =0 (bufervide est un bolean quand = 0 le buffer n'est pas vide)*/
				if((antenne->users[i]->SNRActuels[j] > antenne->users[MaxU]->SNRActuels[j]) && (antenne->users[i]->bufferVide == 0)){
					// si l'User a un meilleur debit, et que son buffer n'est pas vide: il devient le MaxUser 
					MaxU = i;
					new = 1;
				}
			}
			for (i = 0; i < random_user ; i++){
				/*si le SNR est mieu que celui a le meilleur SNR jusqu'a present et que buffervide =0 (bufervide est un bolean quand = 0 le buffer n'est pas vide)*/
				if((antenne->users[i]->SNRActuels[j] > antenne->users[MaxU]->SNRActuels[j]) && (antenne->users[i]->bufferVide == 0)){
					// si l'User a un meilleur debit, et que son buffer n'est pas vide: il devient le MaxUser 
					MaxU = i;
					new = 1;
				}
			}
			/*printf("maxU = %d   ", MaxU);*/
			if(new == 1){
				debitTotalTrame += consumeBit(antenne, MaxU, j);
			}
			

		}

	}
	return debitTotalTrame;
}
