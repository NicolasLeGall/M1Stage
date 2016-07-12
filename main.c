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
	
	double bit_par_UR = 0;
	
	int PDOR=0;
	int PDORProche=0;
	int PDORLoin=0;
	
	double res_sommeUR;
	double res_sommeUR_proche;
	double res_sommeUR_loin;
	
	int user_sommeUR=0;
	int user_sommeUR_loin=0;
	int user_sommeUR_proche=0;
	
	int taux_remplissage_buffer=0;
	int taux_remplissage_buffer_proche=0;
	int taux_remplissage_buffer_loin=0;
	
	int nbPaquetsTotalPDORProche=0;
	int nbPaquetsTotalPDORLoin=0;
	int nbPaquetsTotalsommePaquets_conommer=0;
	int nbPaquetsTotalsommePaquets_conommerproche=0;
	int nbPaquetsTotalsommePaquets_conommerloin=0;
	
	double somme_bitsRestants=0;
	double somme_bitsRestants_loin=0;
	double somme_bitsRestants_proche=0;
	
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
	int nbBitsgenere = 0;
	int choixAlgo = 0;
	int total_nbBitsgenere = 0;
	int i;
	/*int jg =0;*/
	int g =0;
	int z= 0;
	Packet *packet;
	Packet *packet_proche;
	Packet *packet_loin;
	/*---INITIALISATIONS---*/
	printf("\nSIMULATION NTR\n\n");
	/*printf("Nombre d'utilisateur pour la simulation: ");
	scanf("%d", &nb_user);*/
	printf("Nombre de tours pour la simulation, chaque tour = 2ms: ");
	scanf("%d", &nb_tours);
	/*printf("Nombre de bits à générer (>100) :");
	scanf("%d", &nbBitsgenere);*/

	printf("Algorithme : 1 pour RR, 2 pour MAXSNR, 3 pour PF :");
	scanf("%d", &choixAlgo);
	/* on dispose d'un antenne avec tout les valeurs a 0 est avec nb_user dessus ici 2*/
	initAntenne(&monAntenne, nb_user);
	fichier = fopen("test.csv", "w+");

	if (fichier != NULL){
		/* on écrit en début de fichier des informations pour savoir ce que vont contenir les colones*/
		fprintf(fichier,"nb_user=%d;nb_tours=%d;choixAlgo=%d;\n",nb_user, nb_tours, choixAlgo);
		/*fprintf(fichier,"nb_user;debit;delais;delai_proche;delai_loin;sommeDelaisProche;sommeDelaisLoin;nbPaquetsEnvoyesProche;nbPaquetsEnvoyesLoin\n");*/
		if(choixAlgo == 1){
			fprintf(fichier,"nb user RR;debit RR;total_nbBitsgenere RR;nb_bit_consommer RR;delais RR;delai_proche RR;delai_loin RR;PDOR RR;PDORProche RR;PDORLoin RR;Bandepassante RR;Bandepassante_proche RR;Bandepassante_loin RR;Bit_par_UR RR;Taux_remplissage_buffer RR;Taux_remplissage_buffer_proche RR;Taux_remplissage_buffer_loin RR;nbPaquetsEnvoyes RR;nbPaquetsEnvoyesProche RR;nbPaquetsEnvoyesLoin RR;\n");
		}else if(choixAlgo == 2){
			fprintf(fichier,"nb_user MaxSNR;debit MaxSNR;total_nbBitsgenere MaxSNR;nb_bit_consommer MaxSNR;delais MaxSNR;delai_proche MaxSNR;delai_loin MaxSNR;PDOR MaxSNR;PDORProche MaxSNR;PDORLoin MaxSNR;Bandepassante MaxSNR;Bandepassante_proche MaxSNR;Bandepassante_loin MaxSNR;Bit_par_UR MaxSNR;Taux_remplissage_buffer MaxSNR;Taux_remplissage_buffer_proche MaxSNR;Taux_remplissage_buffer_loin MaxSNR;nbPaquetsEnvoyes MaxSNR;nbPaquetsEnvoyesProche MaxSNR;nbPaquetsEnvoyesLoin MAXSNR;\n");	
		}else if(choixAlgo == 3){
			fprintf(fichier,"nb_user PF;debit PF;total_nbBitsgenere PF;nb_bit_consommer PF;delais PF;delai_proche PF;delai_loin PF;PDOR PF;PDORProche PF;PDORLoin PF;Bandepassante PF;Bandepassante_proche PF;Bandepassante_loin PF;Bit_par_UR PF;Taux_remplissage_buffer PF;Taux_remplissage_buffer_proche PF;Taux_remplissage_buffer_loin PF;nbPaquetsEnvoyes PF;nbPaquetsEnvoyesProche PF;nbPaquetsEnvoyesLoin PF;\n");	
		}
		fclose(fichier);
	}
	
	
	
	
	/*---BOUCLE PRINCIPALE---*/
	while(nb_user <= 150){
		/*c'est la boucle qui fait toute la simu c'est ici que tout ce passe*/
		for(i = 0; i < nb_tours; i++){
			
			/*Initialisation des paquets utilisateurs*/
			/*Le temps de création d'un packet est donnée a chaque packet avec monAntenne.actualTime */
			
			nbBitsgenere = produceBit(&monAntenne, nb_user);
			total_nbBitsgenere = total_nbBitsgenere + nbBitsgenere;
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
		

			/*Calcul du nombre de bit qui reste dans les paquets non envoyer*/
			for(g = 0; g < (nb_user); g++){
	
		
				packet = monAntenne.users[g]->lePaquet;
				while(packet != NULL){
					somme_bitsRestants = somme_bitsRestants + packet->bitsRestants;
					packet = packet->nextPacket;
				}
				if(monAntenne.users[g]->distance == 6){
					packet_proche = monAntenne.users[g]->lePaquet;
					while(packet_proche != NULL){
						somme_bitsRestants_proche = somme_bitsRestants_proche + packet_proche->bitsRestants;
						packet_proche = packet_proche->nextPacket;
					}
				}else{
					packet_loin = monAntenne.users[g]->lePaquet;
					while(packet_loin != NULL){
						somme_bitsRestants_loin  = somme_bitsRestants_loin + packet_loin->bitsRestants;
						packet_loin = packet_loin->nextPacket;
					}
				}	
			}
		
			
			/*Incrémentation du temps*/
			monAntenne.actualTime += 2;

		}
		
		
		
		
			/*ici c'est la boucle ou on récupére les valeur pour faire les résultat*/
		for(i = 0; i< nb_user; i++){

			/*Si il reste des packet non envoyer --> Récupération des délais et nb de paquets restants dans les paquets non envoyes */
			if(monAntenne.users[i]->lePaquet != NULL){
			/*printf("proche %.2f || loin %.2f --- ", (float)(monAntenne.users[i]->proche)/(128*5*nb_tours) ,(float)(monAntenne.users[i]->loin)/(128*5*nb_tours));*/
		
				tmpPacket = monAntenne.users[i]->lePaquet;
				/*je parcour tous les packet pour un utilisateurs*/
				while(tmpPacket->nextPacket->nextPacket != NULL){
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
			nbPaquetsTotalsommePaquets_conommer+= monAntenne.users[i]->sommePaquets_conommer;
			user_sommeUR = user_sommeUR + monAntenne.users[i]->sommeUR;
			
			if(monAntenne.users[i]->distance == 6){	
				sommeDelaisProche += monAntenne.users[i]->sommeDelais;
				nbPaquetsTotalProche += monAntenne.users[i]->sommePaquets;
				nbPaquetsTotalPDORProche += monAntenne.users[i]->sommeDelaisPDOR;
				nbPaquetsTotalsommePaquets_conommerproche+= monAntenne.users[i]->sommePaquets_conommer;
				user_sommeUR_proche = user_sommeUR_proche + monAntenne.users[i]->sommeUR;
			}else{	
				sommeDelaisLoin += monAntenne.users[i]->sommeDelais;
				nbPaquetsTotalLoin += monAntenne.users[i]->sommePaquets;
				nbPaquetsTotalPDORLoin += monAntenne.users[i]->sommeDelaisPDOR;
				nbPaquetsTotalsommePaquets_conommerloin+= monAntenne.users[i]->sommePaquets_conommer;
				user_sommeUR_loin = user_sommeUR_loin + monAntenne.users[i]->sommeUR;
			}
			/*récupération du nombre d'UR utiliser */
						
		}
		
		res_sommeUR = ((double)(user_sommeUR)/(double)(5*128*nb_tours))*100;	
		res_sommeUR_proche = ((double)(user_sommeUR_proche)/(double)(5*128*nb_tours))*100;	
		res_sommeUR_loin = ((double)(user_sommeUR_loin)/(double)(5*128*nb_tours))*100;	
	
		bit_par_UR = debitTotal/((double)user_sommeUR);
		taux_remplissage_buffer = somme_bitsRestants/monAntenne.actualTime;
		taux_remplissage_buffer_proche = somme_bitsRestants_proche/monAntenne.actualTime;
		taux_remplissage_buffer_loin = somme_bitsRestants_loin/monAntenne.actualTime;
		
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
		printf("	nbPaquetsEnvoyesProche : %d,    nbPaquetsEnvoyesLoin : %d,    nbPaquetsEnvoyes : %d,         sommeEnvoyesLoinProche : %d\n", nbPaquetsTotalsommePaquets_conommerproche,
		nbPaquetsTotalsommePaquets_conommerloin, nbPaquetsTotalsommePaquets_conommer, nbPaquetsTotalsommePaquets_conommerproche+nbPaquetsTotalsommePaquets_conommerloin);
		printf("	DelaisProche : %.1f, delaisLoin : %.1f, sommeDelais : %.1f, delaisLoinProche : %.1f\n", (double)(sommeDelaisProche),(double)(sommeDelaisLoin),(double)(sommeDelais),(double)(sommeDelaisLoin+sommeDelaisProche));
		/*printf("	débit max théorique: %.3f bit/S\n", 128*5*4.5*nb_tours);
		printf("	débit moyen utilisateurs théorique: %.3f bit/S\n", (128*5*4.5*nb_tours)/nb_user);
		printf("	débit moyen utilisateurs théorique pour 2ms: %.3f bit/ms\n", ((128*5*4.5*nb_tours)/nb_user)/500);*/
		printf("	Pourcentage de bande passante utilisé : %.2f proche : %.2f loin : %.2f \n",res_sommeUR,res_sommeUR_proche,res_sommeUR_loin);
		printf("	Bit par Unité de ressource : %.2f \n",bit_par_UR);
		printf("	somme_bitsRestants/le temps : %d somme_bitsRestants_proche/le temps : %d somme_bitsRestants_loin/le temps : %d\n",taux_remplissage_buffer,taux_remplissage_buffer_proche,taux_remplissage_buffer_loin);
		printf("	PDOR : %d || PDORProche : %d  || PDORLoin : %d\n",PDOR,PDORProche,PDORLoin);
		printf("	Nombre total de Bits genere : %d bits || consommer : %.0f bits\n", total_nbBitsgenere, debitTotal);
		/*printf("	Débit par nb_user : %.0f bits pour %d ms\n", debitTotal/nb_user, nb_tours*2);*/
		printf("	Débit total de la simulation: %.3f bits/ms || Somme des delais: %.3f ms\n", debit_total_simu, sommeDelais);
		printf("	Delai moyen : %.3f ms\n", delais_moyen);
		printf("	Delai moyen des utilisateurs proches(6): %.3f ms || eloignes(3): %.3f ms\n", delais_moyen_proche, delais_moyen_loin);
		printf("	nbPaquetsNonEnvoyes : %d || nbPaquetsTotal : %d \n", nbPaquetsNonEnvoyes, nbPaquetsTotal);
		
	    fichier = fopen("test.csv", "a");
		/*On ecrit dans le fichier toute les valeurs que l'on a obtenue*/
	    if (fichier != NULL)
	    {
			/*si les nombre sont écrie avec un point au lieu d'un virgule sa passer pas sur excel */
	        /*fprintf(fichier,"%d;%.0f;%.0f;%.0f;%.0f;%.0f;%.0f;%d;%d\n", nb_user, debit_total_simu, delais_moyen,delais_moyen_proche,delais_moyen_loin, sommeDelaisProche, sommeDelaisLoin, nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche, nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin);*/
	        fprintf(fichier,"%d;%.0f;%d;%.0f;%.0f;%.0f;%.0f;%d;%d;%d;%.2f;%.2f;%.2f;%.2f;%d;%d;%d;%d;%d;%d\n", nb_user, debit_total_simu, total_nbBitsgenere, debitTotal, delais_moyen, delais_moyen_proche, delais_moyen_loin,
			PDOR, PDORProche, PDORLoin, res_sommeUR, res_sommeUR_proche, res_sommeUR_loin, bit_par_UR, taux_remplissage_buffer, taux_remplissage_buffer_proche, taux_remplissage_buffer_loin, nbPaquetsTotal-nbPaquetsNonEnvoyes,
			nbPaquetsTotalProche-nbPaquetsNonEnvoyesProche, nbPaquetsTotalLoin-nbPaquetsNonEnvoyesLoin);
	 
		fclose(fichier);
	    }
		/* on increment le nombre d'utilisateur pour la boucle while*/ 
		nb_user=nb_user+2;
		
		/* on reinitialise toute les valeurs*/	
		debitTotal = 0;
		total_nbBitsgenere = 0;
		
		sommeDelais = 0;
		sommeDelaisProche = 0;
		sommeDelaisLoin = 0;
		
		PDOR =0;
		PDORProche =0;
		PDORLoin =0;
		nbPaquetsTotalPDORProche =0;
		nbPaquetsTotalPDORLoin =0;
		
		bit_par_UR =0;
		
		somme_bitsRestants =0;
		somme_bitsRestants_loin =0;
		somme_bitsRestants_proche =0;
		
		taux_remplissage_buffer =0;
		taux_remplissage_buffer_proche =0;
		taux_remplissage_buffer_loin =0;
		
		res_sommeUR=0;
		res_sommeUR_proche=0;
		res_sommeUR_loin=0;
		user_sommeUR=0;
		user_sommeUR_loin=0;
		user_sommeUR_proche=0;
		
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
		nbPaquetsTotalsommePaquets_conommer=0;
		nbPaquetsTotalsommePaquets_conommerproche=0;
		nbPaquetsTotalsommePaquets_conommerloin=0;
		
		initAntenne(&monAntenne, nb_user);	
	
	
	}



	return 0;
}






