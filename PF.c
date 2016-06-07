#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "PF.h"
#include "distribution.h"
#include "initialisation.h"

int PF(Antenne *antenne, int nb_user) {	

	int maxU = 0;
	float ratioActu = 0;
	float ratioMax = 0;
	int i, g, j, debitTotalTrame = 0;
	int count = 0;

	for(g = 0; g < NB_TIME_SLOTS ; g++){// parcours les timeslots, //tant que User.BufferVide > 0 ou que g<5, on transmet au debit actuel a cet user
		for(j = 0; j < NB_SUBCARRIERS ; j++){ //parcourt les subcariers
			maxU = 0;
			ratioMax = 0;

			// si l'User a un meilleur debit par rapport à son débit habituel (on utilise la distance), et que son buffer n'est pas vide: il devient le MaxUser 
			for (i = 0; i < nb_user ; i++){
				ratioActu = (float)antenne->users[i]->SNRActuels[j] / (float)antenne->users[i]->distance;
				if(ratioActu > ratioMax && (antenne->users[i]->bufferVide == 0)){

					ratioMax = ratioActu;
					maxU = i;
					
				}
			}
			/*printf("maxU = %d   ", MaxU);*/
			debitTotalTrame += consumeBit(antenne, maxU, j);

		}

	}
	return debitTotalTrame;
}
