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
    int nb_joueur, mon_numero, mon_argent;
    int ID, X , Y , K;
    int j, m, usb, usp, uct;
    int ns;
    int nbus;
    int A,S;
    int nt,bt,dt;
    int idt, ids1, ids2;
    int idb;

    //Premier tour 
    scanf("%d",&nb_joueur);
    scanf("%d",&mon_numero);
    scanf("%d",&ID);
    scanf("%d",&X);
    scanf("%d",&Y);
    scanf("%d",&K);
    fprintf(stderr, "Station0 nb_jo %d num_jou %d ID %d X %d Y %d K%d\n",nb_joueur,mon_numero, ID, X, Y, K);
    scanf("%d",&ID);
    scanf("%d",&X);
    scanf("%d",&Y);
    scanf("%d",&K);
    fprintf(stderr, "STATION1 ID %d X %d Y %d K%d\n",ID, X, Y, K);
    scanf("%d",&ID);
    scanf("%d",&X);
    scanf("%d",&Y);
    scanf("%d",&K);
    fprintf(stderr, "STATION2 ID %d X %d Y %d K%d\n",ID, X, Y, K);
    while(1){
        //Récupération à chaque tour !
        for(int i = 0 ; i < nb_joueur ; i++){
            //Informations sur tous les joueurs ! argent, amélioration de sb, sp et ct
            scanf("%d", &j);
            scanf("%d",&m);
            scanf("%d",&usb);
            scanf("%d",&usp);
            scanf("%d",&uct);
            if(j == mon_numero)mon_argent=m;
        }
        scanf("%d",&ns);

        if(ns == 1){
            //Création d'une nouvelle station
            scanf("%d",&ID);
            scanf("%d",&X);
            scanf("%d",&Y);
            scanf("%d",&K);
            //Il faut créer une station
        }

        scanf("%d",&nbus);

        for(int i = 0 ; i < nbus ; i++){
            //Récupération de toutes les données de bus
            scanf("%d",&ID);
            scanf("%d",&j);
            scanf("%d",&X);
            scanf("%d",&Y);
            scanf("%d",&A);
            scanf("%d",&S);
            //Il faut créer/mettre à jour les bus
        }

        //Informations sur les new voyageurs, ceux qui sont montés au tour précédent et ceux qui sont descendu
        scanf("%d",&nt);
        scanf("%d",&bt);
        scanf("%d",&dt);

        //Informations sur les new voyageurs
        for(int i=0 ; i < nt ; i++){
            scanf("%d",&idt);
            scanf("%d",&ids1);
            scanf("%d",&ids2);
        }

        //Informations des voyageurs montés dans un bus
        for(int i=0; i < bt ; i++){
            scanf("%d",&idt);
            scanf("%d",&idb);
        }

        //Information des voyageurs étant descendu d'un bus
        for(int i=0; i < dt ; i++){
            scanf("%d",&idt);
        }


        printf("PASS\n");
    }
    return 0;
}


int jouer(){
    //Appels des fonctions

    printf("PASS\n");
    return 0;
}