#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#include "initialisation.h"
#include "struct.h"
#include "MRG32k3a.h"
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
	/*donne une distance de 1 ou 3 une fois sur 2 */
	if(randDist%2 == 0){
		user->distance=3;
	}
	else{
		user->distance=1;
	}
	user->bufferVide=1;
	user->SNRmoyen=0;
	user->sommeDelais=0;
	user->sommeDelaisPDOR=0;
	user->sommePaquets = 1;
	user->sommePaquets_consommer = 0;
	user->sommeUR = 0;
	user->bit_en_trop = 0;
	
	
	/*initialisation des condition radio de l'utilisateur*/
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

//!!! Amélioration possible en ajoutant un LastPacket 
int produceBit(Antenne *antenne, int nb_user){

	int i = 0;
	int temps = 0;
	int bitsGeneres ;
	int total_bitsGeneres=0;
	int debordement = 0;
	int resteARemplir = 0;
	int continuer = 1;	
	// Création d'un nouveau packet 
	Packet *packet;
	Packet *packet_test;
	for(i = 0; i < (nb_user); i++){
		//antenne->users[i]->bufferVide = 0;
		continuer = 1;
		packet=NULL;
		//bitsGeneres=(int)(MRG32k3a()*300);
		// c'est de la magie noire mais sa génére en moyenne 150.5 bit
		bitsGeneres=(int)((-1 / 0.00666666) *(log( 1 - MRG32k3a())));
		total_bitsGeneres = total_bitsGeneres + bitsGeneres;
		packet = antenne->users[i]->lePaquet;
		packet_test = antenne->users[i]->lePaquet;
		antenne->users[i]->bit_en_trop = (antenne->users[i]->bit_en_trop) + bitsGeneres;
		//recupere le dernier paquet
		/*while(packet->nextPacket != NULL){
            packet = packet->nextPacket;
        }*/
        //Remplissage des paquets 

        while(continuer){

			// si le buffer est vide
        	if(antenne->users[i]->bufferVide == 1){
				
				// si le nombre de bit a géné est plus grand que la taille d'un paquet
				if(antenne->users[i]->bit_en_trop > 100){
					antenne->users[i]->bufferVide = 0;
					packet->dateCreation = antenne->actualTime;
					packet->bitsRestants=100;
					antenne->users[i]->bit_en_trop = antenne->users[i]->bit_en_trop - 100;
					packet->nextPacket = createPacket(0);
					packet = packet->nextPacket;
					antenne->users[i]->sommePaquets++;
				}else{
					continuer = 0;
					/*packet->dateCreation = antenne->actualTime;
					packet->bitsRestants=bitsGeneres;
					bitsGeneres = 0;
					packet->nextPacket = createPacket(0);
					packet = packet->nextPacket;
					antenne->users[i]->sommePaquets++;*/
				}
			}else{//si le buffer contient quelque chose (n'est pas vide)
				// on parcourt les paquet pour arriver au dernier
				while(packet->nextPacket != NULL){
					packet = packet->nextPacket;
				}
				
				if(antenne->users[i]->bit_en_trop > 100){
					packet->dateCreation = antenne->actualTime;
					packet->bitsRestants=100;
					antenne->users[i]->bit_en_trop = antenne->users[i]->bit_en_trop - 100;
					packet->nextPacket =createPacket(0);
					packet = packet->nextPacket;
					antenne->users[i]->sommePaquets++;
				}else{
					continuer = 0;
					/*packet->dateCreation = antenne->actualTime;
					packet->bitsRestants=bitsGeneres;
					bitsGeneres = 0;
					packet->nextPacket = createPacket(0);
					packet = packet->nextPacket;
					antenne->users[i]->sommePaquets++;*/
				}
			}
        }
		/*if(antenne->users[i]->distance == 1){
			printf("User: %d ->",i);
			while(packet_test->nextPacket != NULL){
				//printf("[%d]->",packet_test->bitsRestants);
				temps++;
				
				packet_test = packet_test->nextPacket;
			}
			printf("[%d]->",temps);
			printf("\n");
		}
		
		temps = 0;*/
		
	}
	return total_bitsGeneres;
}

void initMatriceDebits(Antenne *antenne, int nb_user){
	int i = 0;
	int j = 0;
	double mkn = 0;
	int d = 1;
	double alpha = 0;
	double puissance = 222;
	
	for(i = 0; i < nb_user; i++){
		for(j = 0; j<128; j++){
			/*pour chaque utilisateur on lui définit pour les 128 onde différente combien de bit il va pouvoir envoyer */
			if(antenne->users[i]->distance == 1){
				d = 1;
				alpha =((-1 / 1) *(log( 1 - MRG32k3a())));/*alpha = 1 en moyenne*/
				mkn=1+(((puissance)*alpha)/(d*d));
				
				/*-0.5 pour et (int) pour convertir a l'arrondie inférieur*/
				antenne->users[i]->SNRActuels[j] = ((int)((log(mkn)/log(2))-0.5));// sa fait 6 en moyenne
				//antenne->users[i]->SNRActuels[j] = getSNR(6);
			}else{
				d = 3;
				alpha =((-1 / 1) *(log( 1 - MRG32k3a())));/*alpha = 1 en moyenne*/
				mkn=1+(((puissance)*alpha)/(d*d));
				antenne->users[i]->SNRActuels[j] = ((int)((log(mkn)/log(2))-0.5));//sa fait 3 en moyenne
				//antenne->users[i]->SNRActuels[j] = getSNR(3);
			}
			//printf("initMatriceDebits i :%d j :%d bitsRestants = %d \n", i,j,antenne->users[i]->SNRActuels[j]);
		}
	}
}


int consumeBit(Antenne *antenne, int currentUser, int subCarrier){

	int debordement;
	User *theUser = antenne->users[currentUser];
	Packet *tmpPacket;
	int bitConsommes = 0;
	/*Compte le nombre dUR utiliser*/
	theUser->sommeUR = theUser->sommeUR + 1;
/*
	printf("\n bits restants : %d\n", theUser->lePaquet->bitsRestants);
	printf(" SNR actuel: %d\n", theUser->SNRActuels[subCarrier]);*/

	//Si on consomme plus de bits que le paquet en contient
	if(theUser->lePaquet->bitsRestants <= theUser->SNRActuels[subCarrier]){
		//Mise à jour pour les statistiques
		theUser->sommeDelais += (antenne->actualTime - theUser->lePaquet->dateCreation);
		/*si le delais est supérieurs a 80 ms Pour le calcul du PDOR*/
		if((antenne->actualTime - theUser->lePaquet->dateCreation) >= 80){
			theUser->sommeDelaisPDOR++;
		}
		
		
		// si il reste plusieurs packet dans la chaine
		if((theUser->lePaquet->nextPacket->nextPacket != NULL) ){
			theUser->sommePaquets_consommer++;
			//On soustrait au prochain paquet le SNR moins le contenu du paquet actuel 
			bitConsommes = theUser->SNRActuels[subCarrier];
			theUser->lePaquet->nextPacket->bitsRestants = theUser->lePaquet->nextPacket->bitsRestants - (theUser->SNRActuels[subCarrier] - theUser->lePaquet->bitsRestants);
			//Puis on supprime le paquet 
			tmpPacket = theUser->lePaquet;
			theUser->lePaquet = theUser->lePaquet->nextPacket;
			//free(tmpPacket);
		}else{//si il rester qu'un packet
			theUser->sommePaquets_consommer++;
			bitConsommes = theUser->lePaquet->bitsRestants;
			theUser->lePaquet->bitsRestants = 0;

			theUser->bufferVide = 1;

		}

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
