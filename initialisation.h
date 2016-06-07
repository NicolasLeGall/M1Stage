
#include "struct.h"

#ifndef INITIALISATION_H
#define INITIALISATION_H

Packet* createPacket(int time);
User* initUser();
void initAntenne(Antenne *antenne, int nb_user);

void initMatriceDebits(Antenne *antenne, int nb_user);
void produceBit(Antenne *antenne, int nbBitsgenere, int nb_user);
int consumeBit(Antenne *antenne, int currentUser, int subCarrier);
int MaxUser(Antenne *antenne, int subCarrier, int nb_user);
int empty(Antenne *antenne, int currentUser, int nb_user);

#endif 

