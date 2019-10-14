#include<stdlib.h>
#include<stdio.h>
#include<math.h>

struct nb_traveler_by_station
{
    int id_station;
    int nb;
    struct station* next_station;
};

struct traveler
{
    int id_traveler;
    int available;
    int id_station_dest;
    int id_station_pop;
    struct traveler* next_traveler;
};


struct station
{
    int id;
    int capacity;
    int x;
    int y;
    struct station* next;
};

struct player
{
    int id;
    int money;
    int size_bus;
    int speed_bus;
    int cost_bus;
    int end;
    struct player* next;
};

struct bus
{
    int id;
    int player;
    int x;
    int y;
    int destination;
    int size;
    struct bus* next;
};

//Ecrire des fonctions pour la récup de toutes les données ?
/*Il existera une fonction pour l'IA joue son tour.
Cette fonction appellera des fonctions pour écrire les actions à faire 
puis la première fonction enverra le signal '\n' pour terminer son tour.*/

/*Pour les informations des joueurs, faire deux pointeurs (style tableau à double dimension tab[i][j])
avec i le numéro du joueur et j la donnée intéressante 0 pour l'argent 1 pour sb, etc 
ou sinon tab[i] pointe sur une structure*/

int add_station(int id, int capacity, int x, int y);
int add_player(int id, int money, int size, int speed, int cost, int end);
int add_bus(int id, int player, int x, int y, int destination, int size);
int add_traveler(int id_traveler, int id_station_pop, int id_station_dest);
int play(int my_money);
int play_first(int station);
int search_station(int turn);
int buy_bus();



struct station list_station;
struct player list_player;
struct traveler list_traveler;

int main(void){
    int nb_joueur, my_numero, my_money=0;
    int ID, X , Y , K;
    int j, m, usb, usp, uct, end;
    int ns;
    int nbus;
    int A,S;
    int nt,bt,dt;
    int idt, ids1, ids2;
    int idb;

    int num_station;
    int* my_nb_bus;

    list_station.id=-1;
    list_player.id = -1;
    list.traveler.id=-1;

    my_nb_bus = (int*)malloc(sizeof(int));

    //Premier tour 
    scanf("%d %d",&nb_joueur, &my_numero);
    //Récupération des stations
    for(int i = 0 ; i < 3 ; i++){
        scanf("%d %d %d %d",&ID,&X,&Y,&K);
        if( add_station(ID, K, X, Y) != 0){
            fprintf(stderr,"Error add station...");
            return 2;
        }
    }
   
    //Lancement du premier tour
    //Il faudrait mettre le bus sur une station proche d'une/des autres station
    num_station= search_station(0);
    if(num_station == -1){
        fprintf(stderr,"Erreur lors de la recherche de station...");
        return 1;
    }
    //fprintf(stderr,"J'ai choisi la station %d\n",num_station);
    if(play_first(num_station) == 1){
        fprintf(stderr,"Erreur lors du premier tour...\n");
        return 2;
    }

    while(1){
        
        //Récupération à chaque tour !
        for(int i = 0 ; i < nb_joueur ; i++){
            //Informations sur tous les joueurs ! argent, amélioration de sb, sp et ct
            scanf("%d %d %d %d %d %d", &j, &m, &usb, &usp, &uct, &end);
            if( add_player(j,m,usb, usp, uct, end) == -1){
                fprintf(stderr,"Error add player...\n");
                return 1;
            }
            //fprintf(stderr, "J %d m %d  Usb %d usp %d uct %d end %d\n",j,m,usb,usp, uct,end );
            if(j == my_numero)my_money=m;
        }
        /*
        *@var ns variable qui sert à savoir si une station a été créée
        * 
        * */
        scanf("%d",&ns);
        
        if(ns == 1){
            //Création d'une nouvelle station
            scanf("%d %d %d %d",&ID,&X,&Y,&K);
            if( add_station(ID, K, X, Y) != 0){
                fprintf(stderr,"Error modification station...");
                return 2;
            }
            //fprintf(stderr,"Creation station : %d %d %d %d\n",ID,X,Y,K);
        }

        //La variable nbus permet de savoir combien de bus sont actuellement en partie
        scanf("%d",&nbus);

        //Cette boucle for permet de récupérer toutes les données des bus actuellement en partie
        for(int i = 0 ; i < nbus ; i++){
            //Récupération de toutes les données de bus
            scanf("%d %d %d %d %d %d",&ID, &j, &X, &Y, &A, &S);

            if( add_bus(ID,j,X,Y,A,S) != 0){
                fprintf(stderr,"Error modification bus...");
                return 3;
            }
            //Il faut créer/mettre à jour les bus
        }

        //Informations sur les new voyageurs, ceux qui sont montés au tour précédent et ceux qui sont descendu
        scanf("%d %d %d",&nt, &bt, &dt);


        //Informations sur les new voyageurs
        for(int i=0 ; i < nt ; i++){
            scanf("%d %d %d",&idt, &ids1, &ids2);
            if( add_traveler(idt,ids1, ids2) != 0){
                fprintf(stderr,"Error add traveler...");
                return 4;
            }
        }
        //fprintf(stderr, "idt%d ids1 %d ids2 %d\n",idt,ids1,ids2);
        //Informations des voyageurs montés dans un bus
        for(int i=0; i < bt ; i++){
            scanf("%d %d",&idt, &idb);
            //Vérifier si le bus est le notre
            //Changer le flag available du voyageur et diminuer de 1 le nombre de personne de la station
        }
        //fprintf(stderr, "idt %d idb %d\n",idt, idb);
        //Information des voyageurs étant descendu d'un bus
        for(int i=0; i < dt ; i++){
            scanf("%d",&idt);
            //Supprimer le voyageur de la liste chainée et faire un free
        }
        //fprintf(stderr, "idt%d\n",idt);
        //printf("PASS\n");
        fflush(stdout);
        fflush(stdin);
        play(my_money, my_nb_bus);
    }
    return 0;
}

int play(int my_money, int* my_nb_bus){
    //Appels des fonctions
    //Pour acheter un bus il faut 100$ et pas plus de 4 bus
    //Si on a acheté un bus alors on ne peut pas agrandir ce bus
    //buy_bus renvoie le numéro du bus acheté
    int num_bus;

    if(*my_nb_bus < 4 && my_money >= 100){
        num_bus=buy_bus();
    }

    printf("PASS\n");
    return 0;
}

int buy_bus(){
    int num_bus = 0;
    return num_bus;
}

int play_first(int station){
    printf("BUS %d ; PASS\n", station);
    return 0;
}

/**
 * Fonction qui cherche la meilleure station pour mettre le BUS que l'on va acheter
 * @var turn : le numéro du tour en cours pour savoir si 
 * l'on va prendre en compte les voyageurs qu'il y a dans les stations ou non
 * */
int search_station(int turn){
    //S'il y a une erreure alors renvoyer -1
    //Premier tour
    if(turn == 0){
        int d1_2, d1_0, d2_0;
        //Calcul de la distance entre la station 0 et la station 1
        d1_0 = (list_station.x-list_station.next->x)*(list_station.x-list_station.next->x) + (list_station.y-list_station.next->y)*(list_station.y-list_station.next->y);
        d1_0 = (int) sqrt((int) d1_0);

        //fprintf(stderr,"d1_0 vaut : %d\n",d1_0);

        d2_0 = (list_station.x-list_station.next->next->x)*(list_station.x-list_station.next->next->x) + (list_station.y-list_station.next->next->y)*(list_station.y-list_station.next->next->y);
        d2_0 = (int)sqrt((int)d2_0);

        //fprintf(stderr,"d2  _0 vaut : %d\n",d2_0);

        d1_2 = (list_station.next->x-list_station.next->next->x)*(list_station.next->x-list_station.next->next->x) + (list_station.next->y-list_station.next->next->y)*(list_station.next->y-list_station.next->next->y);
        d1_2 = (int)sqrt((int)d1_2);

        //fprintf(stderr,"d1_2 vaut : %d\n",d1_2);

        //Choix entre station 1 et 2
        if( d1_2 <= d2_0 ){
            if(d1_0<=d2_0){
                return 1;
            }else{
                return 2;
            }
        }else{
        //Choix entre station 1 et 0
            if(d2_0<=d1_2){
                return 0;
            }else{
                return 1;
            }
        }
    }else{
        //Les autres tours
        return 0;
    }
}

int add_station(int id, int capacity, int x, int y){
    struct station* new_station;
    struct station* current_station = &list_station;
    new_station = (struct station*) malloc(sizeof(struct station));

    new_station->id = id;
    new_station->capacity = capacity;
    new_station->x = x;
    new_station->y = y;

    //Recherche de la dernière station du maillon
    while(current_station->next != NULL){
        current_station = current_station->next;
    }

    //Si id = -1 alors c'est la première du maillon
    if( current_station->id == -1 ){
        current_station->id = id;
        current_station->capacity = capacity;
        current_station->x = x;
        current_station->y = y;
        current_station->next = NULL;
    }else{
        new_station = (struct station*) malloc(sizeof(struct station));
        new_station->id = id;
        new_station->capacity = capacity;
        new_station->x = x;
        new_station->y = y;
        current_station->next = new_station;
    }
    return 0;
}

//Ajoute au premier tour un joueur et modifie les valeurs à chaque tour
int add_player(int id, int money, int size, int speed, int cost, int end){
    struct player* current_player;
    current_player = &list_player;
    while(current_player->next != NULL && current_player->id != id){
        current_player = current_player->next;
    }
    if(current_player->id == -1){
        current_player->id = id;
        current_player->money = money;
        current_player->cost_bus=cost;
        current_player->size_bus=size;
        current_player->speed_bus=speed;
        current_player->end=end;
        current_player->next=NULL;
    }else{
        struct player* new_player;
        if(id != current_player->id){
            new_player = (struct player*) malloc(sizeof(struct player));
            current_player->next=new_player;
        }else{
            new_player=current_player;
        }
        new_player->id = id;
        new_player->money = money;
        new_player->cost_bus=cost;
        new_player->size_bus=size;
        new_player->speed_bus=speed;
        new_player->end=end;
    }
    //fprintf(stderr,"Joueur n°%d créé\n",id);
    return 0;
}

int add_bus(int id, int player, int x, int y, int destination, int size){
    return 0;
}

int add_traveler(int id_traveler, int id_station_pop, int id_station_dest){
    
    return 0;
}