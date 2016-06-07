#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "distribution.h"
#include "RR.h"
#include "maxSNR.h"
#include "PF.h"
#include "initialisation.h"


int main(){
	FILE* fichier = NULL;

	double debitTotal = 0;

	double sommeDelais = 0;
	double sommeDelaisProche = 0;
	double sommeDelaisLoin = 0;

	int nbPaquetsTotal = 0;
	int nbPaquetsNonEnvoyes = 0;
	int nbPaquetsTotalProche = 0;
	int nbPaquetsTotalLoin = 0;
	Antenne monAntenne;
	Packet *tmpPacket = NULL;

	int nb_user = 0;
	int nb_tours = 10;
	int nbBitsgenere = 250;
	int choixAlgo = 0;

	int i;
	
	
	

	/*---INITIALISATIONS---*/
	printf("\nSIMULATION NTR\n\n");
	printf("Nombre d'utilisateur pour la simulation: ");
	scanf("%d", &nb_user);
	printf("Nombre de tours pour la simulation: ");
	scanf("%d", &nb_tours);
	printf("Nombre de bits à générer (>100) :");
	scanf("%d", &nbBitsgenere);
	printf("Algorithme : 1 pour RR, 2 pour MAXSNR, 3 pour PF :");
	scanf("%d", &choixAlgo);

	initAntenne(&monAntenne, nb_user);
	
	fichier = fopen("test.csv", "w+");

	if (fichier != NULL)
	{
	fprintf(fichier,"nb_user=%d;nb_tours=%d;nbBitsgenere=%d;choixAlgo=%d;\n",nb_user, nb_tours, nbBitsgenere, choixAlgo);
	fprintf(fichier,"nb_user;débit;délais;delai_proche;delai_loin\n");

	fclose(fichier);
	}

	while(nb_user <= 150){
		/*---BOUCLE PRINCIPALE---*/
		for(i = 0; i < nb_tours; i++){
			
			/*Initialisation des paquets utilisateurs*/
			produceBit(&monAntenne, nbBitsgenere, nb_user);
			/*Initilisations des débits des utilisateurs*/
			initMatriceDebits(&monAntenne, nb_user);		
		
			/*Application de l'algorithme et ôtage des bits envoyés avec maxSNR*/
			if(choixAlgo == 1){
				debitTotal += (double)RR(&monAntenne, nb_user);
			}
			else if(choixAlgo == 2){
				debitTotal += (double)maxSNR(&monAntenne, nb_user);
			}
			else if(choixAlgo == 3){
				debitTotal += (double)PF(&monAntenne, nb_user);
			}
			else{
				printf("choix de l'algorithme mauvais. Arret. \n");
			}
		

			/*ENVOI DE LA TRAME */

			/*Mise à jours des délais*/


			/*Incrémentation du temps*/
			monAntenne.actualTime += 2;

		}


		for(i = 0; i< nb_user; i++){

			/* Récupération des délais et nb de paquets restants dans les paquets non envoyes */
			if(monAntenne.users[i]->lePaquet != NULL){

				tmpPacket = monAntenne.users[i]->lePaquet;
				while(tmpPacket->nextPacket != NULL){
					/* Stats globales */
					sommeDelais += (monAntenne.actualTime - tmpPacket->dateCreation);
					nbPaquetsNonEnvoyes++;
					nbPaquetsTotal++;

					/* Stats par distance */
					if(monAntenne.users[i]->distance == 3){	
						sommeDelaisProche += (monAntenne.actualTime - tmpPacket->dateCreation);
						nbPaquetsTotalProche ++;
					}
					else{
						sommeDelaisProche += (monAntenne.actualTime - tmpPacket->dateCreation);
						nbPaquetsTotalLoin ++;
					}
					tmpPacket = tmpPacket->nextPacket;
					/*printf("PAQUETS RESTANTS ! valeur de bufferVide: %d\n ", monAntenne.users[i]->bufferVide);
				*/
				}

			}

			/* Récupération des delais et paquets enregistrés */
			sommeDelais += monAntenne.users[i]->sommeDelais;
			nbPaquetsTotal += monAntenne.users[i]->sommePaquets;
			if(monAntenne.users[i]->distance == 6){	
				sommeDelaisProche += monAntenne.users[i]->sommeDelais;
				nbPaquetsTotalProche += monAntenne.users[i]->sommePaquets;

			}
			else{	
				sommeDelaisLoin += monAntenne.users[i]->sommeDelais;
				nbPaquetsTotalLoin += monAntenne.users[i]->sommePaquets;
			}

		}



		printf("--------------------------------------------------------------\n");
		printf("Statistiques pour %d utilisateurs: \n", nb_user);
		printf("	Débit total : %.0f bits\n", debitTotal);
		printf("	Somme des delais: : %.3f ms\n", sommeDelais);
		printf("	Débit total de la simulation: %.3f bits/ms\n", (double)(debitTotal/monAntenne.actualTime));
		printf("	Delai moyen : %.3f ms\n", (double)(sommeDelais/nbPaquetsTotal));
		printf("	Delai moyen des utilisateurs proches: %.3f ms\n", (double)(sommeDelaisProche/nbPaquetsTotalProche));
		printf("	Delai moyen des utilisateurs eloignes: %.3f ms\n", (double)(sommeDelaisLoin/nbPaquetsTotalLoin));
		printf("	nbPaquetsNonEnvoyes : %d || nbPaquetsTotal : %d \n", nbPaquetsNonEnvoyes, nbPaquetsTotal);
		
		


	    
 
	    fichier = fopen("test.csv", "a");
	 
	    if (fichier != NULL)
	    {
	 
	        fprintf(fichier,"%d;%.0f;%.0f;%.0f;%.0f\n", nb_user, debitTotal/monAntenne.actualTime, sommeDelais/nbPaquetsTotal,sommeDelaisProche/nbPaquetsTotalProche,sommeDelaisLoin/nbPaquetsTotalLoin);
	 
		fclose(fichier);
	    }

		nb_user=nb_user+4;
			
		debitTotal = 0;
		
		sommeDelais = 0;
		sommeDelaisProche = 0;
		sommeDelaisLoin = 0;

		nbPaquetsTotal = 0;
		nbPaquetsNonEnvoyes = 0;
		nbPaquetsTotalProche = 0;
		nbPaquetsTotalLoin = 0;

		initAntenne(&monAntenne, nb_user);	
	
	
	}



	return 0;
}


















