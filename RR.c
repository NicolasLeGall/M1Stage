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

	for(i = 0; i < NB_TIME_SLOTS ; i++){
		for(j = 0; j< NB_SUBCARRIERS ; j++){

			debitTotalTrame += consumeBit(antenne, currentUser, j);
			
			currentUser = (currentUser+1) % nb_user;
		}
	}
	return debitTotalTrame;

}
