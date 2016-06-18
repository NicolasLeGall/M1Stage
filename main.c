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
	
	double delais_moyen = 0;
	double delais_moyen_proche = 0;
	double delais_moyen_loin = 0;
	double debit_total_simu = 0;
	
	int PDOR=0;
	int PDORProche=0;
	int PDORLoin=0;
	
	int nbPaquetsTotalPDORProche=0;
	int nbPaquetsTotalPDORLoin=0;
	
	int nbPaquetsTotal = 0;
	int nbPaquetsTotalPDOR = 0;
	int nbPaquetsNonEnvoyes = 0;
	int nbPaquetsNonEnvoyesLoin = 0;
	int nbPaquetsNonEnvoyesProche = 0;
	int nbPaquetsTotalProche = 0;
	int nbPaquetsTotalLoin = 0;
	Antenne monAntenne;
	Packet *tmpPacket = NULL;

	int nb_user = 2;
	int nb_tours = 10;
	int nbBitsgenere = 150;
	int choixAlgo = 0;

	int i;
	
	
	

	/*---INITIALISATIONS---*/
	printf("\nSIMULATION NTR\n\n");
	/*printf("Nombre d'utilisateur pour la simulation: ");
	scanf("%d", &nb_user);*/
	printf("Nombre de tours pour la simulation, chaque tour = 2ms: ");
	scanf("%d", &nb_tours);
	printf("Nombre de bits à générer (>100) :");
	scanf("%d", &nbBitsgenere);
	printf("Algorithme : 1 pour RR, 2 pour MAXSNR, 3 pour PF :");
	scanf("%d", &choixAlgo);

	/* on dispose d'un antenne avec tout les valeurs a 0 est avec nb_user dessus ici 2*/
	initAntenne(&monAntenne, nb_user);
	
	fichier = fopen("test.csv", "w+");

	if (fichier != NULL){
		/* on écrit en début de fichier des informations pour savoir ce que vont contenir les colones*/
		fprintf(fichier,"nb_user=%d;nb_tours=%d;nbBitsgenere=%d;choixAlgo=%d;\n",nb_user, nb_tours, nbBitsgenere, choixAlgo);
		/*fprintf(fichier,"nb_user;debit;delais;delai_proche;delai_loin;sommeDelaisProche;sommeDelaisLoin;nbPaquetsEnvoyesProche;nbPaquetsEnvoyesLoin\n");*/
		fprintf(fichier,"nb_user;debit;delais;delai_proche;delai_loin;PDOR;PDORProche;PDORLoin;\n");

		fclose(fichier);
	}
	
	/*Packet *packet;
	packet=NULL;
	int jg =0;*/
	
	
	/*---BOUCLE PRINCIPALE---*/
	while(nb_user <= 50){
		/*c'est la boucle qui fait toute la simu c'est ici que tout ce passe*/
		for(i = 0; i < nb_tours; i++){
			
			/*Initialisation des paquets utilisateurs*/
			/*Le temps de création d'un packet est donnée a chaque packet avec monAntenne.actualTime */
			produceBit(&monAntenne, nbBitsgenere, nb_user);
			
			/*
			for(jg = 0; jg < (nb_user); jg++){
				packet = monAntenne.users[jg]->lePaquet;
				while(packet != NULL){
					printf("tour=%d user=%d packet->bitsRestants=%d packet->dateCreation%d\n",i,jg,packet->bitsRestants,packet->dateCreation);
					packet = packet->nextPacket;
				}
			}*/
			
			/*On donne a chaque utilisateur un débit pour les 128 subcarrieur qui varie de 0 à 10 et qui a pour moyenne sa distance de l'antenne*/
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

		/*ici c'est la boucle ou on récupére les valeur pour faire les résultat*/
		for(i = 0; i< nb_user; i++){

			/*Si il reste des packet non envoyer --> Récupération des délais et nb de paquets restants dans les paquets non envoyes */
			if(monAntenne.users[i]->lePaquet != NULL){

				tmpPacket = monAntenne.users[i]->lePaquet;
				/*je parcour tous les packet pour un utilisateurs*/
				while(tmpPacket->nextPacket != NULL){
					/* Stats globales */
					/*sommeDelais += (monAntenne.actualTime - tmpPacket->dateCreation);*/
					nbPaquetsNonEnvoyes++;

					/* Stats par distance */
					if(monAntenne.users[i]->distance == 6){	
						/*sommeDelaisProche += (monAntenne.actualTime - tmpPacket->dateCreation);*/
						nbPaquetsNonEnvoyesProche++;
					}else{
						/*sommeDelaisLoin += (monAntenne.actualTime - tmpPacket->dateCreation);*/
						nbPaquetsNonEnvoyesLoin++;
					}
					tmpPacket = tmpPacket->nextPacket;
					/*printf("PAQUETS RESTANTS ! valeur de bufferVide: %d\n ", monAntenne.users[i]->bufferVide);
				*/
				}

			}

			/* Récupération des delais et paquets enregistrés */
			sommeDelais += monAntenne.users[i]->sommeDelais;
			nbPaquetsTotal += monAntenne.users[i]->sommePaquets;
			nbPaquetsTotalPDOR+= monAntenne.users[i]->sommeDelaisPDOR;
			
			if(monAntenne.users[i]->distance == 6){	
				sommeDelaisProche += monAntenne.users[i]->sommeDelais;
				nbPaquetsTotalProche += monAntenne.users[i]->sommePaquets;
				nbPaquetsTotalPDORProche += monAntenne.users[i]->sommeDelaisPDOR;

			}else{	
				sommeDelaisLoin += monAntenne.users[i]->sommeDelais;
				nbPaquetsTotalLoin += monAntenne.users[i]->sommePaquets;
				nbPaquetsTotalPDORLoin += monAntenne.users[i]->sommeDelaisPDOR;
			}
			
		}

		delais_moyen = sommeDelais/(nbPaquetsTotal-nbPaquetsNonEnvoyes);
		delais_moyen_proche = sommeDelaisProche/(nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche);
		delais_moyen_loin = sommeDelaisLoin/(nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin);
		debit_total_simu = debitTotal/monAntenne.actualTime;
		PDOR=((double)nbPaquetsTotalPDOR/((double)(nbPaquetsTotal-nbPaquetsNonEnvoyes)))*100;
		PDORProche=((double)nbPaquetsTotalPDORProche/((double)(nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche)))*100;
		PDORLoin=((double)nbPaquetsTotalPDORLoin/((double)(nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin)))*100;
		
		printf("--------------------------------------------------------------\n");
		printf("Statistiques pour %d utilisateurs: \n", nb_user);
		printf("	Paquets proche : %d,            Paquets loin : %d,            somme Paquets : %d,            delais loin+proche : %d\n", nbPaquetsTotalProche,nbPaquetsTotalLoin,nbPaquetsTotal,nbPaquetsTotalProche+nbPaquetsTotalLoin);
		printf("	nbPaquetsNonEnvoyesProche : %d, nbPaquetsNonEnvoyesLoin : %d, nbPaquetsNonEnvoyesTotal : %d, sommeDeLoinProche : %d\n", nbPaquetsNonEnvoyesProche, nbPaquetsNonEnvoyesLoin, nbPaquetsNonEnvoyes, nbPaquetsNonEnvoyesProche+nbPaquetsNonEnvoyesLoin);
		printf("	nbPaquetsEnvoyesProche : %d,    nbPaquetsEnvoyesLoin : %d,    nbPaquetsEnvoyes : %d,         sommeEnvoyesLoinProche : %d\n", nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche,
		nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin, nbPaquetsTotal-nbPaquetsNonEnvoyes,nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche+nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin);
		printf("	DelaisProche : %.1f, delaisLoin : %.1f, sommeDelais : %.1f, delaisLoinProche : %.1f\n", (double)(sommeDelaisProche),(double)(sommeDelaisLoin),(double)(sommeDelais),(double)(sommeDelaisLoin+sommeDelaisProche));

		/*printf("	débit max théorique: %.3f bit/S\n", 128*5*4.5*nb_tours);
		printf("	débit moyen utilisateurs théorique: %.3f bit/S\n", (128*5*4.5*nb_tours)/nb_user);
		printf("	débit moyen utilisateurs théorique pour 2ms: %.3f bit/ms\n", ((128*5*4.5*nb_tours)/nb_user)/500);*/
		printf("	PDOR : %d PDORProche : %d PRDORLoin : %d\n",PDOR,PDORProche,PDORLoin);
		
		printf("	Débit total : %.0f bits\n", debitTotal);
		printf("	Débit par nb_user : %.0f bits pour %d ms\n", debitTotal/nb_user, nb_tours*2);
		printf("	Débit total de la simulation: %.3f bits/ms\n", debit_total_simu);
		printf("	Somme des delais: : %.3f ms\n", sommeDelais);
		printf("	Delai moyen : %.3f ms\n", delais_moyen);
		printf("	Delai moyen des utilisateurs proches(6): %.3f ms\n", delais_moyen_proche);
		printf("	Delai moyen des utilisateurs eloignes(3): %.3f ms\n", delais_moyen_loin);

		printf("	nbPaquetsNonEnvoyes : %d || nbPaquetsTotal : %d \n", nbPaquetsNonEnvoyes, nbPaquetsTotal);
		
	    fichier = fopen("test.csv", "a");
		/*On ecrit dans le fichier toute les valeurs que l'on a obtenue*/
	    if (fichier != NULL)
	    {
			/*si les nombre sont écrie avec un point au lieu d'un virgule sa passer pas sur excel */
	        /*fprintf(fichier,"%d;%.0f;%.0f;%.0f;%.0f;%.0f;%.0f;%d;%d\n", nb_user, debit_total_simu, delais_moyen,delais_moyen_proche,delais_moyen_loin, sommeDelaisProche, sommeDelaisLoin, nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche, nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin);*/
	        fprintf(fichier,"%d;%.0f;%.0f;%.0f;%.0f;%d;%d;%d\n", nb_user, debit_total_simu, delais_moyen,delais_moyen_proche,delais_moyen_loin,PDOR,PDORProche,PDORLoin);
	 
		fclose(fichier);
	    }
		/* on increment le nombre d'utilisateur pour la boucle while*/ 
		nb_user=nb_user+2;
		/* on reinitialise toute les valeurs*/	
		debitTotal = 0;
		
		sommeDelais = 0;
		sommeDelaisProche = 0;
		sommeDelaisLoin = 0;
		
		PDOR =0;
		PDORProche =0;
		PDORLoin =0;
		nbPaquetsTotalPDORProche =0;
		nbPaquetsTotalPDORLoin =0;
		
		delais_moyen = 0;
		delais_moyen_proche = 0;
		delais_moyen_loin = 0;
		debit_total_simu = 0;
	
		nbPaquetsTotal = 0;
		nbPaquetsTotalPDOR = 0;
		nbPaquetsNonEnvoyes = 0;
		nbPaquetsNonEnvoyesProche = 0;
		nbPaquetsNonEnvoyesLoin = 0;
		nbPaquetsTotalProche = 0;
		nbPaquetsTotalLoin = 0;

		initAntenne(&monAntenne, nb_user);	
	
	
	}



	return 0;
}






