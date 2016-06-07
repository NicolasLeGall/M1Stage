#include <stdio.h>
#include <stdlib.h>

#include "initialisation.h"
#include "struct.h"

int randDist = 0;

Packet* createPacket(time){
	
	Packet *packet = malloc(sizeof(Packet));
	packet->dateCreation=time;
	packet->bitsRestants = 0;
	packet->nextPacket = NULL;
	return packet;
}

User* initUser(){
	int i = 0 ;
	User *user = malloc(sizeof(User));

	if(randDist%2 == 0){
		user->distance=3;
	}
	else{
		user->distance=6;
	}

	user->bufferVide=1;
	user->SNRmoyen=0;
	user->sommeDelais=0;
	user->sommePaquets = 1;
	
	for(i = 0; i<128; i++)
	{
		user->SNRActuels[i]=0;
	}

	
	user->lePaquet = createPacket(0);

	randDist++;
	return user;
}

void initAntenne(Antenne *antenne, int nb_user){
	int i = 0;
	for(i = 0; i < nb_user; i++)
	{	
		antenne->users[i]=initUser();
	}
	antenne->actualTime = 0;

	
}


void initMatriceDebits(Antenne *antenne, int nb_user){
	int i = 0;
	int j = 0;

	for(i = 0; i < nb_user; i++)
	{
		for(j = 0; j<128; j++){

			antenne->users[i]->SNRActuels[j] = getSNR(antenne->users[i]->distance);
			//printf("initMatriceDebits i :%d j :%d bitsRestants = %d \n", i,j,antenne->users[i]->SNRActuels[j]);
		}
	}
}


//!!! Amélioration possible en ajoutant un LastPacket 
void produceBit(Antenne *antenne, int nbBitsgenere, int nb_user){
	int i = 0;
	int bitsGeneres ;
	int debordement = 0;
	int resteARemplir = 0;
	int continuer = 1;


	// Création d'un nouveau packet 
	Packet *packet;
	
	for(i = 0; i < (nb_user); i++){

		antenne->users[i]->bufferVide = 0;

		continuer = 1;
		packet=NULL;
		bitsGeneres=nbBitsgenere;
		packet = antenne->users[i]->lePaquet;
		//recupere le dernier paquet

		while(packet->nextPacket != NULL)
		{
            packet = packet->nextPacket;
        }
        //Remplissage des paquets 
        while(continuer){
        	//Si il reste de quoi remplir le paquet
        	packet->dateCreation = antenne->actualTime;
        	if(bitsGeneres > (100 - packet->bitsRestants)){
        		bitsGeneres -= 100 - packet->bitsRestants;
        		packet->bitsRestants = 100;
        		packet->nextPacket = createPacket(0);


        		antenne->users[i]->sommePaquets++;
        		packet = packet->nextPacket;
        	}
        	else{
        		packet->bitsRestants += bitsGeneres;
        		continuer = 0;
        	}   		     	
        }
		
	}


}


int consumeBit(Antenne *antenne, int currentUser, int subCarrier){

	int debordement;
	User *theUser = antenne->users[currentUser];
	Packet *tmpPacket;
	int bitConsommes = 0;
/*
	printf("\n bits restants : %d\n", theUser->lePaquet->bitsRestants);
	printf(" SNR actuel: %d\n", theUser->SNRActuels[subCarrier]);*/

	//Si on consomme plus de bits que le paquet en contient
	if(theUser->lePaquet->bitsRestants < theUser->SNRActuels[subCarrier]){
		//Mise à jour pour les statistiques
		theUser->sommeDelais += (antenne->actualTime - theUser->lePaquet->dateCreation);
		
		if(theUser->lePaquet->nextPacket != NULL){
	/*		
	*/		
			//On soustrait au prochain paquet le SNR moins le contenu du paquet actuel 
			bitConsommes = theUser->SNRActuels[subCarrier];
			theUser->lePaquet->nextPacket->bitsRestants -= theUser->SNRActuels[subCarrier] - theUser->lePaquet->bitsRestants;
			//Puis on supprime le paquet 
			tmpPacket = theUser->lePaquet;
			theUser->lePaquet = theUser->lePaquet->nextPacket;
		}
		else{
			
			bitConsommes = theUser->lePaquet->bitsRestants;
			theUser->lePaquet->bitsRestants = 0;

			theUser->bufferVide = 1;
			

		}
		// TODO TODO TODO
		//free(tmpPacket);
	}
	//Si il y a assez de bits dans ce paquet
	else{
		theUser->lePaquet->bitsRestants -= theUser->SNRActuels[subCarrier];
		bitConsommes = theUser->SNRActuels[subCarrier];
	}

	//printf(" bits restants Apres: %d\n", theUser->lePaquet->bitsRestants);
	// On retourne le nombre de bits côtés 
	return bitConsommes;
}



int MaxUser (Antenne *antenne, int subCarrier, int nb_user){
	int i = 0;
	int res = 0;

	for (i = 0; i < nb_user ; i++){
		if((antenne->users[i]->SNRActuels[subCarrier] >= antenne->users[res]->SNRActuels[subCarrier]) && (antenne->users[i]->bufferVide == 0)){
			// si l'User a un meilleur debit, et que son buffer n'est pas vide: il devient le MaxUser 
			res = i;
		}
	}

	return res;
}

int empty(Antenne *antenne, int currentUser, int nb_user){
	if(currentUser < nb_user){
		return antenne->users[currentUser]->bufferVide;
	}
}
