#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>
/*
#define MAX_PASSENGER_GETTING_OFF_BUSS 5
#define NOMBRE_MAX_STATIONS 10
#define NOMBRE_STATIONS_DEBUT 3
#define MAX_SIZE_COMMAND_NAME 20
#define MAX_SIZE_COMMANDS 20*/
#define PRICE_BUS 100
#define PRICE_SIZE_BUS 50
#define PRICE_UPGRADE_SB 100
#define PRICE_UPGRADE_SP 200
#define PRICE_UPGRADE_CT 100
#define MAX_INCREASE_CT 5
#define MAX_INCREASE_SP 2
#define MAX_INCREASE_SB 2
#define MAX_NB_BUS 4


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

void add_station(int id, int capacity, int x, int y);
void add_player(int id, int money, int size, int speed, int cost, int end);
void add_bus(int id, int player, int x, int y, int destination, int size);
void add_traveler(int id_traveler, int id_station_pop, int id_station_dest);
void increment_station(int pop_station, int dest_station);
int play(struct state_upgrade* s_upgrade);
void play_first(int station);
int search_station_with_max_traveler();
void buy_bus();
void bye_traveler(int id_traveler);
int is_my_bus(int id_bus);
int disable_traveler(int id_traveler);
int decrement_station(int station_pop, int station_dest);
int go_to_station();
void buy_price();
void buy_speed();
int clear_goto_station();
void disable_station(int station);
int is_available_bus(struct bus* bus);
int get_destination_by_traveler(int id_traveler);
int add_traveler_to_bus(int idt,int idb);
int is_my_traveler(int id_traveler);
void get_first_info();
void get_infos(struct state_upgrade* s_upgrade, int nb_joueur);
void buy_upgrade(struct state_upgrade* s_upgrade);
int deliver_traveler(struct bus* bus);
void deplacement();
int count_dest_by_dest_and_by_bus( int id_bus,int id_destination);
void able_station(int station);
void game_over();
int count_traveler_by_bus( int id_bus );
int search_station_by_location();


struct station* list_station;
struct player* list_player;
struct traveler* list_traveler;
struct bus* list_bus;
struct nb_traveler_by_station* list_nb_station;
struct my_traveler* list_my_traveler;

int main(void){
    int nb_joueur;

    int turn = 0;
    struct state_upgrade* s_upgrade;

    s_upgrade = (struct state_upgrade*)malloc(sizeof(struct state_upgrade));
    
    scanf("%d %d",&nb_joueur, &s_upgrade->my_id);//Premier tour
    get_first_info();

    while(1){
        turn++;
        get_infos(s_upgrade, nb_joueur);
        
        fprintf(stderr,"Mon argent : %d\n",s_upgrade->money);
        if(turn == 1){
            play_first(search_station_by_location());
        }else{
            play(s_upgrade);
        }
        fflush(stdout);
    }
    return 0;
}

int play(struct state_upgrade* s_upgrade){
    buy_upgrade(s_upgrade);
    //go_to_station(s_upgrade->my_id);
    deplacement();
    printf("PASS\n");
    return 0;
}

void deplacement(){
    struct bus* current_bus = list_bus;
    int station_selected;
    while( current_bus != NULL ){
        if( is_available_bus(current_bus) <= 0){
            if( (station_selected = deliver_traveler(current_bus) ) != -1 && count_traveler_by_bus(current_bus->id) > ( (current_bus->size * 5) /2 ) ){
                printf("DESTINATION %d %d;",current_bus->id, station_selected);
                disable_station(station_selected);
                current_bus->busy = 2;
            }else{
                station_selected = search_station_with_max_traveler();
                printf("DESTINATION %d %d;",current_bus->id, station_selected);
                disable_station(station_selected);
                current_bus->busy = 2;
            }
        }
        current_bus = current_bus->next;
    }

}

void buy_price(){
    printf("UPDATECT;");
}

void buy_speed(){
    printf("UPDATESP;");
}

void buy_bus(){
    printf("BUS %d;", search_station_with_max_traveler());
}

void play_first(int station){
    printf("BUS %d;PASS\n", station);
}

int search_station_by_location(){
    int d1_2, d1_0, d2_0;
    //Calcul de la distance entre la station 0 et la station 1
    d1_0 = (list_station->x-list_station->next->x)*(list_station->x-list_station->next->x) + (list_station->y-list_station->next->y)*(list_station->y-list_station->next->y);
    d1_0 = (int) sqrt((int) d1_0);

    d2_0 = (list_station->x-list_station->next->next->x)*(list_station->x-list_station->next->next->x) + (list_station->y-list_station->next->next->y)*(list_station->y-list_station->next->next->y);
    d2_0 = (int)sqrt((int)d2_0);

    d1_2 = (list_station->next->x-list_station->next->next->x)*(list_station->next->x-list_station->next->next->x) + (list_station->next->y-list_station->next->next->y)*(list_station->next->y-list_station->next->next->y);
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
}

/**
 * Fonction qui cherche la meilleure station pour mettre le BUS que l'on va acheter
 * @var turn : le numéro du tour en cours pour savoir si 
 * l'on va prendre en compte les voyageurs qu'il y a dans les stations ou non
 * */
int search_station_with_max_traveler(){
    //Les autres tours
    struct nb_traveler_by_station* current_station = list_nb_station;
    if(current_station == NULL){
        return 0;
    }
    int station_with_better_traveler = current_station->id_station;
    int max_w_traveler = current_station->nb_waiting_traveler;
    while(current_station->next_station != NULL){
        if(max_w_traveler < current_station->nb_waiting_traveler && current_station->bus_go_to == 0){
            station_with_better_traveler = current_station->id_station;
            max_w_traveler = current_station->nb_waiting_traveler;
        }else{
        }
        current_station = current_station->next_station;
    }
    if(station_with_better_traveler == -1)return 0; //Car il n'y a pas encore de voyageur dans les stations
    return station_with_better_traveler; 
}

void add_station(int id, int capacity, int x, int y){
    struct station* new_station;
    struct station* current_station = list_station;

    //Recherche de la dernière station du maillon
    while(current_station != NULL && current_station->next != NULL){
        current_station = current_station->next;
    }

    //Si id = -1 alors c'est la première du maillon
    if( current_station == NULL ){
        new_station = (struct station*) malloc(sizeof(struct station));
        new_station->id = id;
        new_station->capacity = capacity;
        new_station->x = x;
        new_station->y = y;
        new_station->next = NULL;
        list_station = new_station;
    }else{
        new_station = (struct station*) malloc(sizeof(struct station));
        new_station->id = id;
        new_station->capacity = capacity;
        new_station->x = x;
        new_station->y = y;
        current_station->next = new_station;
    }
    return;
}

//Ajoute au premier tour un joueur et modifie les valeurs à chaque tour
void add_player(int id, int money, int size, int speed, int cost, int end){
    struct player* current_player;
    struct player* new_player;
    current_player = list_player;
    while(current_player != NULL && current_player->next != NULL && current_player->id != id){
        current_player = current_player->next;
    }
    if(current_player == NULL ){
        new_player = (struct player*) malloc(sizeof(struct player));
        new_player->id = id;
        new_player->money = money;
        new_player->cost_bus=cost;
        new_player->size_bus=size;
        new_player->speed_bus=speed;
        new_player->end=end;
        new_player->next=NULL;
        list_player = new_player;
    }else{
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
    return;
}

void add_bus(int id, int player, int x, int y, int destination, int size){
    struct bus* current_bus = list_bus;
     struct bus* new_bus;
    while ( current_bus != NULL && current_bus->next != NULL && current_bus->id != id    )
    {
        current_bus = current_bus->next;
    }
    if(current_bus == NULL){
        new_bus = (struct bus*) malloc(sizeof(struct bus));
        new_bus->id = id;
        new_bus->destination = destination;
        new_bus->player=player;
        new_bus->x=x;
        new_bus->y=y;
        new_bus->size = size;
        new_bus->next=NULL;
        new_bus->busy = 0;
        list_bus = new_bus;
    }else{
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
    return;
}

void add_traveler(int id_traveler, int id_station_pop, int id_station_dest){
    struct traveler* current_traveler;
    struct traveler* new_traveler;
    current_traveler = list_traveler;
    while( current_traveler != NULL && current_traveler->next_traveler != NULL && current_traveler->id_traveler != id_traveler ){
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler == NULL){
        new_traveler = (struct traveler*) malloc(sizeof(struct traveler));
        new_traveler->id_traveler = id_traveler;
        new_traveler->id_station_pop = id_station_pop;
        new_traveler->id_station_dest = id_station_dest;
        new_traveler->available = 1;
        new_traveler->next_traveler=NULL;
        list_traveler = new_traveler;
    }else{
        
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
    return;
}

void increment_station(int pop_station, int dest_station){
    struct nb_traveler_by_station* current_station;
    current_station = list_nb_station;
    while(current_station != NULL && (current_station->next_station != NULL && current_station->id_station != pop_station) ){
        current_station = current_station->next_station;
    }
    //Augmente le nombre de voyageurs qu'il y a dans la station d'apparition
    if(current_station == NULL){
        struct nb_traveler_by_station* new_station = (struct nb_traveler_by_station*)malloc(sizeof(struct nb_traveler_by_station));
        new_station->id_station = pop_station;
        new_station->nb_waiting_traveler = 1;
        new_station->next_station = NULL;
        new_station->bus_go_to = 0;
        list_nb_station = new_station;
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
    current_station = list_nb_station;
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
    return;
}

//Rendre non disponible un voyageur
int disable_traveler(int id_traveler){
    struct traveler* current_traveler;
    current_traveler = list_traveler;
    while( current_traveler != NULL && current_traveler->next_traveler != NULL && current_traveler->id_traveler != id_traveler ){
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler == NULL)return 0;
    if(current_traveler->id_traveler == id_traveler){
        current_traveler->available = 0;
        decrement_station(current_traveler->id_station_pop, current_traveler->id_station_dest);      
        return 0;
    }
    return 1;//Aucun voyageur avec cet id a été trouvé
}

//Réduit le nombre de voyageur dans la station de départ et la destination de 1 dans la station de destination
int decrement_station(int station_pop, int station_dest){
    struct nb_traveler_by_station* station;
    station = list_nb_station;
    while (station->next_station != NULL && station->id_station != station_pop){
        station = station->next_station;
    }
    if(station-> id_station == station_pop){
        station->nb_waiting_traveler--;
    }else{
        return 1;
    }
    station = list_nb_station;
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

int is_my_bus(int id_bus){
    struct bus* current_bus = list_bus;
    while ( current_bus != NULL && current_bus->next != NULL && current_bus->id != id_bus){
        current_bus = current_bus->next;
    }
    if(current_bus->id == id_bus){
        return 1;
    }
    return 0;
}

void print_traveler(){
    struct my_traveler* current_traveler  = list_my_traveler;
    while (current_traveler != NULL){
        fprintf(stderr,"Voyag %d\n",current_traveler->id);
        if(current_traveler->next_traveler != NULL){
            current_traveler = current_traveler->next_traveler;
        }else{
            break;
        }
    }
}

void bye_traveler(int id_traveler){
    if( is_my_traveler(id_traveler) == 0 )return;
    fprintf(stderr,"Je veux supprimer %d\n",id_traveler);
    struct my_traveler* current_traveler  = list_my_traveler;
    struct my_traveler* previous_traveler = list_my_traveler;
    if(current_traveler->id == id_traveler){
        if(current_traveler->next_traveler != NULL){
            list_my_traveler = current_traveler->next_traveler;
        }else{
            list_my_traveler = NULL;
        }
        //free(current_traveler);
        return;
    }
    while (current_traveler->id != id_traveler){
        previous_traveler = current_traveler;
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler->next_traveler == NULL){
        previous_traveler->next_traveler = NULL;
    }else{
        previous_traveler->next_traveler = current_traveler->next_traveler;
    }
    fprintf(stderr,"Suppression du voyageur %d\n",id_traveler);
    //free(current_traveler);
}

int is_my_traveler(int id_traveler){
    struct my_traveler* current_traveler;
    current_traveler = list_my_traveler;
    while ( current_traveler != NULL && current_traveler->next_traveler != NULL && current_traveler->id != id_traveler )
    {
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler == NULL )return 0;
    if(current_traveler->id == id_traveler)return 1;
    return 0;
}



void disable_station(int station){
    struct nb_traveler_by_station* current_station = list_nb_station;
    if(current_station == NULL)return;
    while (current_station->id_station != station){
        current_station = current_station->next_station;
    }
    current_station->bus_go_to = 1;
    return;
}

void able_station(int station){
    struct nb_traveler_by_station* current_station = list_nb_station;
    if(current_station == NULL)return;
    while (current_station->id_station != station)
    {
        current_station = current_station->next_station;
    }
    current_station->bus_go_to = 0;
    return;
}

int clear_goto_station(){
    struct nb_traveler_by_station* current_station = list_nb_station;
    current_station->bus_go_to = 0;
    while(current_station->next_station != NULL){
        current_station = current_station->next_station;
        current_station->bus_go_to = 0;
    }
    return 0;
}

int is_available_bus(struct bus* bus){
    if(bus->busy <= 0)return bus->busy;
    int bus_destination = bus->destination;
    struct station* station = list_station;

    while(station->id != bus_destination){
        station = station->next;
    }
    if(station->x == bus->x && station->y == bus->y){
        bus->busy = bus->busy - 1;
        if(bus->busy <= 0)able_station(bus_destination);
    }
    return bus->busy;
}

int add_traveler_to_bus(int idt,int idb){
    struct my_traveler* traveler = list_my_traveler;
    int idd;
    idd = get_destination_by_traveler(idt);
    
    if(traveler == NULL){
        struct my_traveler* new_traveler;
        new_traveler = (struct my_traveler*) malloc(sizeof(struct my_traveler));
        new_traveler->id = idt;
        new_traveler->id_bus = idb;
        new_traveler->id_destination = idd;
        list_my_traveler = new_traveler;
    }else{
        while(traveler->next_traveler != NULL){
            traveler = traveler->next_traveler;
        }
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
    struct traveler* current_traveler = list_traveler;
    while(current_traveler->id_traveler != id_traveler){
        current_traveler=current_traveler->next_traveler;
    }
    return current_traveler->id_station_dest;
}

void get_first_info(){
    int ID,X,Y,K;
    for(int i = 0 ; i < 3 ; i++ ){
        scanf("%d %d %d %d",&ID,&X,&Y,&K);
        add_station(ID, K, X, Y);
    }
    return;
}

void get_infos(struct state_upgrade* s_upgrade, int nb_joueur){
    int j,m,usb,usp,uct,end;
    int ns,nbus,ID,X,Y,K,A,S;
    int nt, bt, dt, idt, ids1, ids2, idb;
    for(int i = 0 ; i < nb_joueur ; i++){//Informations sur tous les joueurs ! argent, amélioration de sb, sp et ct      
            scanf("%d %d %d %d %d %d", &j, &m, &usb, &usp, &uct, &end);
            if(j == s_upgrade->my_id){
                s_upgrade->sb = usb;
                s_upgrade->sp = usp;
                s_upgrade->ct = uct;
                s_upgrade->money = m;
                if(end == 1){
                    game_over();
                }
            }else{
                add_player(j,m,usb, usp, uct, end);
            } 
        }
  
        scanf("%d",&ns);//ns variable qui sert à savoir si une station a été créée
        if(ns == 1){//Création d'une nouvelle station
            scanf("%d %d %d %d",&ID,&X,&Y,&K);
            add_station(ID, K, X, Y);
        }

        scanf("%d",&nbus);//La variable nbus permet de savoir combien de bus sont actuellement en partie
        //Cette boucle for permet de récupérer toutes les données des bus actuellement en partie
        for(int i = 0 ; i < nbus ; i++){
            scanf("%d %d %d %d %d %d",&ID, &j, &X, &Y, &A, &S);
            if(j == s_upgrade->my_id){
                add_bus(ID,j,X,Y,A,S);
            }
        }

        //Informations sur les new voyageurs, ceux qui sont montés au tour précédent et ceux qui sont descendu
        scanf("%d %d %d",&nt, &bt, &dt);
        for(int i=0 ; i < nt ; i++){
            scanf("%d %d %d",&idt, &ids1, &ids2);
            add_traveler(idt,ids1, ids2);
            increment_station(ids1,ids2);
        }

        //Informations des voyageurs montés dans un bus
        for(int i=0; i < bt ; i++){
            scanf("%d %d",&idt, &idb);
            //Vérifier si le bus est le notre
            disable_traveler(idt);//On met le voyageur comme indisponible et on réduit les informations liés à ce voyageur
            if(is_my_bus(idb) == 1){//Si c'est notre bus alors on va incrémenter le nb de voyageur dans notre bus
                add_traveler_to_bus(idt,idb);
            }
        }

        //Information des voyageurs étant descendu d'un bus
        for(int i=0; i < dt ; i++){
            scanf("%d",&idt);
            bye_traveler(idt);
        }
}

void buy_upgrade(struct state_upgrade* s_upgrade){
    if(s_upgrade->nb_bus < ( MAX_NB_BUS-1 ) && s_upgrade->money >= PRICE_BUS){
        buy_bus();
        s_upgrade->nb_bus = s_upgrade->nb_bus + 1;
        s_upgrade->money = s_upgrade->money - 100;
    }
    if(s_upgrade->money >= PRICE_UPGRADE_CT && s_upgrade->ct < MAX_INCREASE_CT){
        buy_price();
        s_upgrade->money = s_upgrade->money - 100;
    }
    if(s_upgrade->money >= PRICE_UPGRADE_SP && s_upgrade->sp < MAX_INCREASE_SP){
        buy_speed();
        s_upgrade->money = s_upgrade->money - 200;
    }
}

int deliver_traveler(struct bus* bus){
    struct my_traveler* current_traveler = list_my_traveler;
    int best_station = -1;
    int count_dest, count_max=0;
    while(current_traveler != NULL){
        if(current_traveler->id_bus == bus->id){
            count_dest = count_dest_by_dest_and_by_bus(bus->id,current_traveler->id_destination);
            if(count_dest > count_max){
                count_max=count_dest;
                best_station = current_traveler->id_destination;
            }
        }
        current_traveler = current_traveler->next_traveler;
    }
    return best_station;
}

int count_dest_by_dest_and_by_bus( int id_bus,int id_destination){
    struct my_traveler* current_traveler = list_my_traveler;
    int count = 0;
    while(current_traveler != NULL){
        if(current_traveler->id_bus == id_bus && current_traveler->id_destination == id_destination){
            count++;
        }
        current_traveler = current_traveler->next_traveler;
    }
    return count;
}

int count_traveler_by_bus( int id_bus ){
    struct my_traveler* current_traveler = list_my_traveler;
    int count = 0;
    while(current_traveler != NULL){
        if(current_traveler->id_bus == id_bus){
            count++;
        }
        current_traveler = current_traveler->next_traveler;
    }
    return count;
}

void free_traveler(){
    struct traveler* c_traveler = list_traveler;
    struct traveler* p_traveler;
    while(c_traveler != NULL){
        p_traveler = c_traveler;
        c_traveler = c_traveler->next_traveler;
        free(p_traveler);
    }
}
void free_bus(){
    struct bus* c_bus = list_bus;
    struct bus* p_bus;
    while(c_bus != NULL){
        p_bus = c_bus;
        c_bus = c_bus->next;
        free(p_bus);
    }
}
void free_my_traveler(){
    struct my_traveler* c_traveler = list_my_traveler;
    struct my_traveler* p_traveler;
    while(c_traveler != NULL){
        p_traveler = c_traveler;
        c_traveler = c_traveler->next_traveler;
        free(p_traveler);
    }
}
void free_station(){
    struct station* c_station = list_station;
    struct station* p_station;
    while(c_station != NULL){
        p_station = c_station;
        c_station = c_station->next;
        free(p_station);
    }
}
void free_nb_station(){
    struct nb_traveler_by_station* c_station = list_nb_station;
    struct nb_traveler_by_station* p_station;
    while(c_station != NULL){
        p_station = c_station;
        c_station = c_station->next_station;
        free(p_station);
    }
}
void free_player(){
    struct player* c_player = list_player;
    struct player* p_player;
    while(c_player != NULL){
        p_player = c_player;
        c_player = c_player->next;
        free(p_player);
    }
}

void game_over(){
    free_traveler();
    free_bus();
    free_my_traveler();
    free_station();
    free_nb_station();
    free_player();
}