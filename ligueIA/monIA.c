#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>


struct my_traveler
{
    int id;
    int id_bus;
    int id_destination;
    struct my_traveler* next_traveler;
};

struct state_upgrade
{
    int sb;
    int sp;
    int ct;
    int nb_bus;
    int money;
    int my_id;
};
struct nb_traveler_by_station
{
    int id_station;
    int nb_waiting_traveler;
    int nb_coming_traveler;
    int my_traveler;
    int bus_go_to;
    struct nb_traveler_by_station* next_station;
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
    int busy;
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
int increment_station(int pop_station, int dest_station);
int play(struct state_upgrade* s_upgrade);
int play_first(int station);
int search_station_for_buy_bus(int turn);
int buy_bus();
int bye_traveler(int id_traveler);
int is_bus(int id_bus,int my_numero);
int disable_traveler(int id_traveler);
int decrement_station(int station_pop, int station_dest);
int go_to_station();
int buy_price();
int buy_speed();
int clear_goto_station();
int disable_station(int station);
int is_available_bus(struct bus* bus);
int get_destination_by_traveler(int id_traveler);
int add_traveler_to_bus(int idt,int idb);
int is_my_traveler(int id_traveler);


struct station list_station;
struct player list_player;
struct traveler list_traveler;
struct bus list_bus;
struct nb_traveler_by_station list_nb_station;
struct my_traveler list_my_traveler;

//current_destination[0]=x et current_destination[1]=y
int current_destination[2];

int main(void){
    int nb_joueur;
    int ID, X , Y , K;
    int j, m, usb, usp, uct, end;
    int ns;
    int nbus;
    int A,S;
    int nt,bt,dt;
    int idt, ids1, ids2;
    int idb;

    int num_station;

    struct state_upgrade* s_upgrade;

    list_station.id=-1;
    list_player.id = -1;
    list_traveler.id_traveler=-1;
    list_bus.id=-1;
    list_nb_station.id_station=-1;
    list_my_traveler.id=-1;
    list_my_traveler.next_traveler = NULL;

    s_upgrade = (struct state_upgrade*)malloc(sizeof(struct state_upgrade));

    //Premier tour 
    scanf("%d %d",&nb_joueur, &s_upgrade->my_id);
    //Récupération des stations
    
        scanf("%d %d %d %d",&ID,&X,&Y,&K);
        fprintf(stderr,"Je créer une nouvelle station ! %d %d %d %d\n", ID, X, Y, K);
        if( add_station(ID, K, X, Y) != 0 ){
            fprintf(stderr,"Error add station...");
            return 2;
        }

    scanf("%d %d %d %d",&ID,&X,&Y,&K);
    fprintf(stderr,"Je créer une nouvelle station ! %d %d %d %d\n", ID, X, Y, K);
    add_station(ID, K, X, Y);

    scanf("%d %d %d %d",&ID,&X,&Y,&K);
    fprintf(stderr,"Je créer une nouvelle station ! %d %d %d %d\n", ID, X, Y, K);
    add_station(ID, K, X, Y);

    //Lancement du premier tour
    //Il faudrait mettre le bus sur une station proche d'une/des autres station
    num_station= search_station_for_buy_bus(0);
    if(num_station == -1){
        fprintf(stderr,"Error search station...");
        return 1;
    }
    if(play_first(num_station) == 1){
        fprintf(stderr,"Error first trurn...\n");
        return 2;
    }
    s_upgrade->nb_bus = 1;
    s_upgrade->money = 50;

    while(1){
        //Récupération à chaque tour !
        for(int i = 0 ; i < nb_joueur ; i++){
            //Informations sur tous les joueurs ! argent, amélioration de sb, sp et ct
            scanf("%d %d %d %d %d %d", &j, &m, &usb, &usp, &uct, &end);
            if(j == s_upgrade->my_id){
                s_upgrade->sb = usb;
                s_upgrade->sp = usp;
                s_upgrade->ct = uct;
                s_upgrade->money = m - 200;
            }else{
                if( add_player(j,m,usb, usp, uct, end) == -1){
                    fprintf(stderr,"Error add player...\n");
                    return 1;
                }
            } 
        }
        /*
        *@var ns variable qui sert à savoir si une station a été créée
        * 
        * */
        scanf("%d",&ns);
        
        if(ns == 1){
            //Création d'une nouvelle station
            scanf("%d %d %d %d",&ID,&X,&Y,&K);
            fprintf(stderr,"Je créer une nouvelle station ! %d %d %d %d\n", ID, X, Y, K);
            if( add_station(ID, K, X, Y) != 0){
                fprintf(stderr,"Error modification station...");
                return 2;
            }
        }
        //La variable nbus permet de savoir combien de bus sont actuellement en partie
        scanf("%d",&nbus);
        //Cette boucle for permet de récupérer toutes les données des bus actuellement en partie
        for(int i = 0 ; i < nbus ; i++){
            //Récupération de toutes les données de bus
            scanf("%d %d %d %d %d %d",&ID, &j, &X, &Y, &A, &S);
            if(j == s_upgrade->my_id){
                if( add_bus(ID,j,X,Y,A,S) != 0){
                    fprintf(stderr,"Error modification bus...");
                    return 3;
                }
            }
            //Il faut créer/mettre à jour les bus
        }
                fprintf(stderr,"NOUUUS\n");

        //Informations sur les new voyageurs, ceux qui sont montés au tour précédent et ceux qui sont descendu
        scanf("%d %d %d",&nt, &bt, &dt);
        //Informations sur les new voyageurs
        for(int i=0 ; i < nt ; i++){
            scanf("%d %d %d",&idt, &ids1, &ids2);
            if( add_traveler(idt,ids1, ids2) != 0){
                fprintf(stderr,"Error add traveler...");
                return 4;
            }
            increment_station(ids1,ids2);
        }
        fprintf(stderr,"JEEE\n");
        //Informations des voyageurs montés dans un bus
        for(int i=0; i < bt ; i++){
            scanf("%d %d",&idt, &idb);
            //Vérifier si le bus est le notre
            disable_traveler(idt);//On met le voyageur comme indisponible et on réduit les informations liés à ce voyageur
            if(is_bus(idb, s_upgrade->my_id) == 1){//Si c'est notre bus alors on va incrémenter le nb de voyageur dans notre bus
                fprintf(stderr,"TUUUU\n");
                add_traveler_to_bus(idt,idb);
                fprintf(stderr,"ILLL\n");
            }
            //Changer le flag available du voyageur et diminuer de 1 le nombre de personne de la station et de la station d'arrivée également
        }
        fprintf(stderr,"JEEE\n");
        //Information des voyageurs étant descendu d'un bus
        for(int i=0; i < dt ; i++){
            scanf("%d",&idt);
            bye_traveler(idt);
            //Supprimer le voyageur de la liste chainée et faire un free
        }
        fprintf(stderr,"JEEE\n");
        play(s_upgrade);
        fprintf(stderr,"JEEE\n");
        fflush(stdout);
    }
    return 0;
}

int play(struct state_upgrade* s_upgrade){
    //Appels des fonctions
    //Pour acheter un bus il faut 100$ et pas plus de 4 bus
    //Si on a acheté un bus alors on ne peut pas agrandir ce bus
    //buy_bus renvoie le numéro du bus acheté
    int num_bus,res;
    if(s_upgrade->nb_bus < 4 && s_upgrade->money >= 100){
        num_bus=buy_bus();
        s_upgrade->nb_bus = s_upgrade->nb_bus + 1;
        s_upgrade->money = s_upgrade->money - 100;
    }
    if(num_bus == 1){
        //Diriger le bus numéro my_nb_bus
    }
    fprintf(stderr,"Money !! %d \n",s_upgrade->money);
    if(s_upgrade->money >= 200 && s_upgrade->sp <= 2){
        res = buy_speed();
        s_upgrade->money = s_upgrade->money - 200;
    }
    if(s_upgrade->money >= 100 && s_upgrade->ct <= 4){
        res = buy_price();
        s_upgrade->money = s_upgrade->money - 100;
    }
    if(res == 0);//ATTTTTTTTTTTEEEEEEEEEEEEEEEEENNNNNNNNNNNNNNNNNTTTTTTTTTTTTTTTIIIIIIIIIIOOOOOOOONNN
    //1 pour un tour qui n'est pas le premier
    go_to_station(s_upgrade->my_id);
    printf("PASS\n");
    return 0;
}

int buy_price(){
    printf("UPDATECT;");
    return 1;
}

int buy_speed(){
    printf("UPDATESP;");
    return 1;
}

int buy_bus(){
    printf("BUS %d;", search_station_for_buy_bus(1));
    return 1;
}

int play_first(int station){
    printf("BUS %d;PASS\n", station);
    return 0;
}

/**
 * Fonction qui cherche la meilleure station pour mettre le BUS que l'on va acheter
 * @var turn : le numéro du tour en cours pour savoir si 
 * l'on va prendre en compte les voyageurs qu'il y a dans les stations ou non
 * */
int search_station_for_buy_bus(int turn){
    //S'il y a une erreure alors renvoyer -1
    //Premier tour
    if(turn == 0){
        int d1_2, d1_0, d2_0;
        //Calcul de la distance entre la station 0 et la station 1
        d1_0 = (list_station.x-list_station.next->x)*(list_station.x-list_station.next->x) + (list_station.y-list_station.next->y)*(list_station.y-list_station.next->y);
        d1_0 = (int) sqrt((int) d1_0);

        d2_0 = (list_station.x-list_station.next->next->x)*(list_station.x-list_station.next->next->x) + (list_station.y-list_station.next->next->y)*(list_station.y-list_station.next->next->y);
        d2_0 = (int)sqrt((int)d2_0);

        d1_2 = (list_station.next->x-list_station.next->next->x)*(list_station.next->x-list_station.next->next->x) + (list_station.next->y-list_station.next->next->y)*(list_station.next->y-list_station.next->next->y);
        d1_2 = (int)sqrt((int)d1_2);

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
        struct nb_traveler_by_station* current_station = &list_nb_station;
        int station_with_better_traveler = current_station->id_station;
        int max_w_traveler = current_station->nb_waiting_traveler;
        while(current_station->next_station != NULL){
            fprintf(stderr,"Bus to go %d \n", current_station->bus_go_to);
            //fprintf(stderr,"passager : %d id %d \n ", current_station->nb_waiting_traveler, current_station->id_station);
            if(max_w_traveler < current_station->nb_waiting_traveler && current_station->bus_go_to == 0){
                fprintf(stderr,"Change car %d < %d donc %d to %d\n", max_w_traveler, current_station->nb_waiting_traveler, station_with_better_traveler, current_station->id_station);
                station_with_better_traveler = current_station->id_station;
                max_w_traveler = current_station->nb_waiting_traveler;
            }else{
            fprintf(stderr,"Non car %d >= %d pas %d to %d\n", max_w_traveler, current_station->nb_waiting_traveler, station_with_better_traveler, current_station->id_station);
            }
            current_station = current_station->next_station;
        }
        fprintf(stderr,"Go %d max %d\n",station_with_better_traveler, max_w_traveler);
        return station_with_better_traveler;
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
    return 0;
}

int add_bus(int id, int player, int x, int y, int destination, int size){
    struct bus* current_bus;
    current_bus = &list_bus;
    while (current_bus->next != NULL && current_bus->id != id)
    {
        current_bus = current_bus->next;
    }
    if(current_bus->id == -1){
        current_bus->id = id;
        current_bus->destination = destination;
        current_bus->player=player;
        current_bus->x=x;
        current_bus->y=y;
        current_bus->size = size;
        current_bus->next=NULL;
        current_bus->busy = 0;
    }else{
        struct bus* new_bus;
        if(id != current_bus->id){
            new_bus = (struct bus*) malloc(sizeof(struct bus));
            current_bus->next=new_bus;
            new_bus->busy = 0;
        }else{
            new_bus=current_bus;
        }
        new_bus->id = id;
        new_bus->destination = destination;
        new_bus->player=player;
        new_bus->x=x;
        new_bus->y=y;
        new_bus->size = size;
    } 
    return 0;
}

int add_traveler(int id_traveler, int id_station_pop, int id_station_dest){
    struct traveler* current_traveler;
    current_traveler = &list_traveler;
    while(current_traveler->next_traveler != NULL && current_traveler->id_traveler != id_traveler){
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler->id_traveler == -1){
        current_traveler->id_traveler = id_traveler;
        current_traveler->id_station_pop = id_station_pop;
        current_traveler->id_station_dest = id_station_dest;
        current_traveler->available = 1;
        current_traveler->next_traveler=NULL;
    }else{
        struct traveler* new_traveler;
        if(id_traveler != current_traveler->id_traveler){
            new_traveler = (struct traveler*) malloc(sizeof(struct traveler));
            current_traveler->next_traveler=new_traveler;
        }else{
            new_traveler=current_traveler;
        }
        new_traveler->id_traveler = id_traveler;
        new_traveler->id_station_pop = id_station_pop;
        new_traveler->id_station_dest = id_station_dest;
        new_traveler->available = 1;
    }
    return 0;
}

int increment_station(int pop_station, int dest_station){
    struct nb_traveler_by_station* current_station;
    current_station = &list_nb_station;
    while(current_station->next_station != NULL && current_station->id_station != pop_station){
        current_station = current_station->next_station;
    }
    //Augmente le nombre de voyageurs qu'il y a dans la station d'apparition
    if(current_station->id_station == -1){
        current_station->id_station = pop_station;
        current_station->nb_waiting_traveler = 1;
        current_station->next_station = NULL;
        current_station->bus_go_to = 0;
    }else{
        struct nb_traveler_by_station* new_station;
        if(pop_station != current_station->id_station){
            new_station = (struct nb_traveler_by_station*)malloc(sizeof(struct nb_traveler_by_station));
            current_station->next_station = new_station;
            new_station->id_station = pop_station;
            new_station->nb_waiting_traveler = 0;
            new_station->bus_go_to = 0;
        }else{
            new_station=current_station;
        }
        new_station->nb_waiting_traveler = new_station->nb_waiting_traveler + 1;
    }
    //Partie presque identique pour les voyageurs qui veulent aller à cette station (peut etre changer pour seulement les voyageurs que nous prenons dans nos bus)
    current_station = &list_nb_station;
    while(current_station->next_station != NULL && current_station->id_station != dest_station){
        current_station = current_station->next_station;
    }
    if(current_station->id_station == -1){
        current_station->id_station = dest_station;
        current_station->nb_coming_traveler = 1;
        current_station->next_station = NULL;
        current_station->bus_go_to = 0;
    }else{
        struct nb_traveler_by_station* new_station;
        if(dest_station != current_station->id_station){
            new_station = (struct nb_traveler_by_station*)malloc(sizeof(struct nb_traveler_by_station));
            current_station->next_station = new_station;
            new_station->id_station = dest_station;
            new_station->nb_coming_traveler = 0;
            new_station->bus_go_to = 0;
        }else{
            new_station=current_station;
        }
        new_station->nb_coming_traveler = new_station->nb_coming_traveler + 1;
    }
    return 0;
}

//Rendre non disponible un voyageur
int disable_traveler(int id_traveler){
    struct traveler* current_traveler;
    current_traveler = &list_traveler;
    while(current_traveler->next_traveler != NULL && current_traveler->id_traveler != id_traveler){
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler->id_traveler == id_traveler){
        current_traveler->available = 0;
        if(decrement_station(current_traveler->id_station_pop, current_traveler->id_station_dest) != 0){
            fprintf(stderr,"Error lors de la decrementation...");
        }      
        return 0;
    }
    return 1;//Aucun voyageur avec cet id a été trouvé
}

//Réduit le nombre de voyageur dans la station de départ et la destination de 1 dans la station de destination
int decrement_station(int station_pop, int station_dest){
    struct nb_traveler_by_station* station;
    station = &list_nb_station;
    while (station->next_station != NULL && station->id_station != station_pop){
        station = station->next_station;
    }
    if(station-> id_station == station_pop){
        station->nb_waiting_traveler--;
    }else{
        return 1;
    }
    station = &list_nb_station;
    while (station->next_station != NULL && station->id_station != station_dest){
        station = station->next_station;
    }
    if(station-> id_station == station_dest){
        station->nb_coming_traveler--;
    }else{
        return 2;
    }
    return 0;
}

int is_bus(int id_bus,int my_numero){
    struct bus* current_bus;
    current_bus=&list_bus;
    while (current_bus->next != NULL && current_bus->id != id_bus){
        current_bus = current_bus->next;
    }
    if(current_bus->player == my_numero){
        return 1;
    }
    return 0;
}

int bye_traveler(int id_traveler){
    if( !is_my_traveler(id_traveler) )return 1;
    struct my_traveler* current_traveler;
    current_traveler = &list_my_traveler;
    struct my_traveler* previous_traveler;
    previous_traveler = current_traveler;
    while (current_traveler->id != id_traveler)
    {
        previous_traveler = current_traveler;
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler->next_traveler == NULL){
        previous_traveler->next_traveler = NULL;
    }else{
        previous_traveler->next_traveler = current_traveler->next_traveler;
    }
    free(current_traveler);
    return 0;
}

int is_my_traveler(int id_traveler){
    struct my_traveler* current_traveler;
    current_traveler = &list_my_traveler;
    while (current_traveler->next_traveler != NULL && current_traveler->id != id_traveler)
    {
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler->id == id_traveler)return 1;
    return 0;
}

int go_to_station(int id_player){
    struct bus* current_bus = &list_bus;
    int station_selected=-1;
    if(current_bus->player == id_player && is_available_bus(current_bus) <= 0){
            station_selected = search_station_for_buy_bus(1);
            disable_station(station_selected);
            current_bus->destination = station_selected;
            printf("DESTINATION %d %d;",current_bus->id, station_selected);
            current_bus->busy = 2;
        }
    while(current_bus->next != NULL){
        if(current_bus->player == id_player && is_available_bus(current_bus) <= 0){
            station_selected = search_station_for_buy_bus(1);
            disable_station(station_selected);
            current_bus->destination = station_selected;
            printf("DESTINATION %d %d;",current_bus->id, station_selected);
            current_bus->busy = 2;
        }
        current_bus = current_bus->next;
    }
    return 0;
}

int disable_station(int station){
    struct nb_traveler_by_station* current_station = &list_nb_station;
    while (current_station->id_station != station)
    {
        current_station = current_station->next_station;
    }
    current_station->bus_go_to = 1;
    return 0;
}

int able_station(int station){
    struct nb_traveler_by_station* current_station = &list_nb_station;
    while (current_station->id_station != station)
    {
        current_station = current_station->next_station;
    }
    current_station->bus_go_to = 0;
    return 0;
}

int clear_goto_station(){
    struct nb_traveler_by_station* current_station = &list_nb_station;
    current_station->bus_go_to = 0;
    while(current_station->next_station != NULL){
        current_station = current_station->next_station;
        current_station->bus_go_to = 0;
    }
    return 0;
}

int is_available_bus(struct bus* bus){
    int bus_destination = bus->destination;
    struct station* station = &list_station;
    while(station->id != bus_destination){
        station = station->next;
    }
    fprintf(stderr,"bus_dest %d busy %d \n",bus_destination,bus->busy);
    if(station->x == bus->x && station->y == bus->y){
        able_station(bus_destination);
        bus->busy = bus->busy - 1;
        fprintf(stderr,"bus %d %d et station %d %d busy %d\n",bus->x,bus->y,station->x,station->y, bus->busy);
    }
    return bus->busy;
}

int add_traveler_to_bus(int idt,int idb){
    struct my_traveler* traveler = &list_my_traveler;
    int idd;
    int i=0;
fprintf(stderr,"VOUUUS\n");
    idd = get_destination_by_traveler(idt);
    while(traveler->next_traveler != NULL){
        traveler = traveler->next_traveler;
        fprintf(stderr,"%d\n", i);
        i++;
    }
    
    if(traveler->id == -1){
        traveler->id = idt;
        traveler->id_bus = idb;
        traveler->id_destination = idd;
    }else{
        struct my_traveler* new_traveler;
        new_traveler = (struct my_traveler*) malloc(sizeof(struct my_traveler));
        traveler->next_traveler=new_traveler;
        new_traveler->id = idt;
        new_traveler->id_bus = idb;
        new_traveler->id_destination = idd;
    }
    return 0;
}

int get_destination_by_traveler(int id_traveler){
    struct traveler* current_traveler;
    while(current_traveler->id_traveler != id_traveler){
        current_traveler=current_traveler->next_traveler;
    }
    return current_traveler->id_station_dest;
}