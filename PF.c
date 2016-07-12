#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "PF.h"
#include "distribution.h"
#include "initialisation.h"
#include "MRG32k3a.h"

int PF(Antenne *antenne, int nb_user) {	

	int maxU = 0;
	float ratioActu = 0;
	float ratioMax = 0;
	int i, g, j, debitTotalTrame = 0;
	int count = 0;
	float mkn_moyen_user[nb_user];
	int temp=0;
	int proche =0;
	int loin = 0;
	int new = 0;
	int random_user = 0;
	/*calcul pour chaque utilisateur de sont N k,m moyen */
	for (i = 0; i < nb_user ; i++){
		for(j = 0; j < NB_SUBCARRIERS ; j++){
			 temp += antenne->users[i]->SNRActuels[j];
		}
		mkn_moyen_user[i]=(float)temp/NB_SUBCARRIERS;
		temp=0;
		/*printf("mkn_moyen_user[%d]=%.2f\n",i,mkn_moyen_user[i]);*/
	}
	i=0;
	j=0;
	/*NB_SUBCARRIERS = 128 NB_TIME_SLOTS = 5 */
	for(g = 0; g < NB_TIME_SLOTS ; g++){// parcours les timeslots, //tant que User.BufferVide > 0 ou que g<5, on transmet au debit actuel a cet user
		// proche =0;
			// loin =0;
		for(j = 0; j < NB_SUBCARRIERS ; j++){ //parcourt les subcariers
			maxU = 0;
			ratioMax = 0;
			new = 0;
			random_user=(int)(MRG32k3a()*nb_user);//permet de commencer a parcourir la liste de user par un user choisi aléatoirement
			// si l'User a un meilleur debit par rapport à son débit habituel (on utilise la distance), et que son buffer n'est pas vide: il devient le MaxUser 
			for (i = random_user; i < nb_user ; i++){
				// ratioActu = (float)antenne->users[i]->SNRActuels[j] / (float)mkn_moyen_user[i];
				ratioActu = ((float)antenne->users[i]->SNRActuels[j]) / (mkn_moyen_user[i]);
				//ratioActu = (float)(antenne->users[i]->SNRActuels[j]) / (float)(antenne->users[i]->distance);
				/*if(antenne->users[i]->distance == 6){
					ratioActu = (((float)antenne->users[i]->SNRActuels[j])) / 5.5;
					antenne->users[i]->proche+=mkn_moyen_user[i];
				}else{
					ratioActu = (((float)antenne->users[i]->SNRActuels[j])) / 2.7;
					antenne->users[i]->loin+=mkn_moyen_user[i];
				}*/
				if((ratioActu > ratioMax) && (antenne->users[i]->bufferVide == 0)){

					ratioMax = ratioActu;
					maxU = i;
					new = 1;
					
				}
			}
			for (i = 0; i < random_user ; i++){
				ratioActu = ((float)antenne->users[i]->SNRActuels[j]) / (mkn_moyen_user[i]);

				if((ratioActu > ratioMax) && (antenne->users[i]->bufferVide == 0)){

					ratioMax = ratioActu;
					maxU = i;
					new = 1;
					
				}
			}
			
			/*debitTotalTrame += consumeBit(antenne, maxU, j);*/
			/*printf("maxU = %d   ", MaxU);*/
			if(new == 1){
				debitTotalTrame += consumeBit(antenne, maxU, j);
			}
		}
		//printf("proche %.2f || loin %.2f --- ", (float)(proche)/((nb_user/2)*128) ,(float)(loin)/((nb_user/2)*128));
	}
	// printf("proche %.2f || loin %.2f --- ", (float)(proche)/((nb_user/2)*128*5) ,(float)(loin)/((nb_user/2)*128*5));
	return debitTotalTrame;
}
