#include<stdlib.h>
#include<stdio.h>

//Ecrire des fonctions pour la récup de toutes les données ?
/*Il existera une fonction pour l'IA joue son tour.
Cette fonction appellera des fonctions pour écrire les actions à faire 
puis la première fonction enverra le signal '\n' pour terminer son tour.*/

/*Pour les informations des joueurs, faire deux pointeurs (style tableau à double dimension tab[i][j])
avec i le numéro du joueur et j la donnée intéressante 0 pour l'argent 1 pour sb, etc 
ou sinon tab[i] pointe sur une structure*/

int jouer();

int main(void){
    int nb_joueur, mon_numero, mon_argent=0;
    int ID, X , Y , K;
    int j, m, usb, usp, uct;
    int ns;
    int nbus;
    int A,S;
    int nt,bt,dt;
    int idt, ids1, ids2;
    int idb;

    //Premier tour 
    scanf("%d %d",&nb_joueur, &mon_numero);
    scanf("%d %d %d %d",&ID,&X,&Y,&K);
    //fprintf(stderr, "Station0 nb_jo %d num_jou %d ID %d X %d Y %d K%d\n",nb_joueur,mon_numero, ID, X, Y, K);
    scanf("%d %d %d %d",&ID,&X,&Y,&K);
    //fprintf(stderr, "STATION1 ID %d X %d Y %d K%d\n",ID, X, Y, K);
    scanf("%d %d %d %d",&ID,&X,&Y,&K);
    //fprintf(stderr, "STATION2 ID %d X %d Y %d K%d\n",ID, X, Y, K);
   
    printf("PASS\n");
     for(int i = 0 ; i < nb_joueur ; i++){
            //Informations sur tous les joueurs ! argent, amélioration de sb, sp et ct
            scanf("%d %d %d %d %d", &j, &m, &usb, &usp, &uct);
            if(j == mon_numero)mon_argent=m;
        }
        scanf("%d",&ns);

        if(ns == 1){
            //Création d'une nouvelle station
            scanf("%d %d %d %d",&ID,&X,&Y,&K);
            //Il faut créer une station
            fprintf(stderr,"%d %d %d %d",ID,X,Y,K);
        }

        scanf("%d",&nbus);

        for(int i = 0 ; i < nbus ; i++){
            //Récupération de toutes les données de bus
            scanf("%d %d %d %d %d %d",&ID, &j, &X, &Y, &A, &S);
            //Il faut créer/mettre à jour les bus
        }

        //Informations sur les new voyageurs, ceux qui sont montés au tour précédent et ceux qui sont descendu
        scanf("%d %d %d",&nt, &bt, &dt);
        //Informations sur les new voyageurs
        for(int i=0 ; i < nt ; i++){
            scanf("%d %d %d",&idt, &ids1, &ids2);
        }
        //fprintf(stderr, "idt%d ids1 %d ids2 %d\n",idt,ids1,ids2);
        //Informations des voyageurs montés dans un bus
        for(int i=0; i < bt ; i++){
            scanf("%d %d",&idt, &idb);
        }
        //fprintf(stderr, "idt %d idb %d\n",idt, idb);
        //Information des voyageurs étant descendu d'un bus
        for(int i=0; i < dt ; i++){
            scanf("%d",&idt);
        }
    while(1){
        printf("PASS\n");
        //Récupération à chaque tour !
        for(int i = 0 ; i < nb_joueur ; i++){
            //Informations sur tous les joueurs ! argent, amélioration de sb, sp et ct
            scanf("%d %d %d %d %d", &j, &m, &usb, &usp, &uct);
            if(j == mon_numero)mon_argent=m;
        }
        scanf("%d",&ns);

        if(ns == 1){
            //Création d'une nouvelle station
            scanf("%d %d %d %d",&ID,&X,&Y,&K);
            //Il faut créer une station
            fprintf(stderr,"%d %d %d %d",ID,X,Y,K);
        }

        scanf("%d",&nbus);

        for(int i = 0 ; i < nbus ; i++){
            //Récupération de toutes les données de bus
            scanf("%d %d %d %d %d %d",&ID, &j, &X, &Y, &A, &S);
            //Il faut créer/mettre à jour les bus
        }

        //Informations sur les new voyageurs, ceux qui sont montés au tour précédent et ceux qui sont descendu
        scanf("%d %d %d",&nt, &bt, &dt);
        //Informations sur les new voyageurs
        for(int i=0 ; i < nt ; i++){
            scanf("%d %d %d",&idt, &ids1, &ids2);
        }
        //fprintf(stderr, "idt%d ids1 %d ids2 %d\n",idt,ids1,ids2);
        //Informations des voyageurs montés dans un bus
        for(int i=0; i < bt ; i++){
            scanf("%d %d",&idt, &idb);
        }
        //fprintf(stderr, "idt %d idb %d\n",idt, idb);
        //Information des voyageurs étant descendu d'un bus
        for(int i=0; i < dt ; i++){
            scanf("%d",&idt);
        }
        //fprintf(stderr, "idt%d\n",idt);
        //printf("PASS\n");
    }
    return 0;
}


int jouer(){
    //Appels des fonctions
    printf("PASS\n");
    return 0;
}