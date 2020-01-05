/*
Dés le lancement de la partie le programme récupère les infos du jeu
A chaque tour le programme récupère et mets à jour les infos du jeu

Au niveau des améliorations : dès que mon IA possède assez d'argent alors
elle achète tous les bus supplémentaires puis dès qu'elle a assez d'argent toutes les 
améliorations pour augmenter la vitesse des bus et enfin toutes les améliorations pour augmenter
le prix des tickets de chaque passagers afin de gagner plus d'argent

En début de partie mon IA cherche la station qui est la plus proche de ses deux autres voisine
Puis à chaque tour l'IA regarde pour tous les bus si il possède des passagers
    si oui alors l'IA va à la station la plus proche de toutes les stations des passagers du bus
    sinon elle envoie le bus à la station la plus remplit.
Arriver sur une station chaque bus reste 2 tours, le premier tour pour déposer les voyageurs et le second pour prendre des voyageurs
Si le bus était vide alors il ne reste qu'un seul tour afin de récupérer les voyageurs seulement
*/
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<unistd.h>


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
//Declaration des fonctions
void get_first_info();
void get_infos(struct state_upgrade* s_upgrade, int nb_joueur);
void deplacement();
void add_station(int id, int capacity, int x, int y);
void add_player(int id, int money, int size, int speed, int cost, int end);
void add_bus(int id, int player, int x, int y, int destination, int size);
void add_traveler(int id_traveler, int id_station_pop, int id_station_dest);
void add_traveler_to_bus(int idt,int idb);
void increment_station(int pop_station, int dest_station);
void decrement_station(int station_pop, int station_dest);
void play(struct state_upgrade* s_upgrade);
void play_first(int station);
void buy_upgrade(struct state_upgrade* s_upgrade);
void buy_bus();
void buy_price();
void buy_speed();
void able_station(int station);
void disable_station(int station);
void disable_traveler(int id_traveler);
void bye_traveler(int id_traveler);
void game_over();

int deliver_traveler(struct bus* bus);
int is_my_bus(int id_bus);
int is_available_bus(struct bus* bus);
int is_my_traveler(int id_traveler);
int get_destination_by_traveler(int id_traveler);
int count_dest_by_dest_and_by_bus( int id_bus,int id_destination);
int count_traveler_by_bus( int id_bus );
int search_station_first_turn();
int search_station_by_location(struct bus* bus);
int search_station_with_max_traveler();


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
    
    scanf("%d %d",&nb_joueur, &s_upgrade->my_id);
    get_first_info();

    while(1){
        turn++;
        get_infos(s_upgrade, nb_joueur);
        if(turn == 1){
            play_first(search_station_first_turn());
        }else{
            play(s_upgrade);
        }
        fflush(stdout);
    }
    return 0;
}

//Fait jouer l'IA à tous les tours sauf le premier
void play(struct state_upgrade* s_upgrade){
    buy_upgrade(s_upgrade);
    deplacement();
    printf("PASS\n");
    return;
}

//Fonction qui permet de déplacer les bus de l'IA
void deplacement(){
    struct bus* current_bus = list_bus;
    int station_selected;
    int debug;
    while( current_bus != NULL ){
        debug = is_available_bus(current_bus);
        if( debug <= 0){
            if( (station_selected = deliver_traveler(current_bus) ) != -1 ){
                printf("DESTINATION %d %d;",current_bus->id, station_selected);
                disable_station(station_selected);
                current_bus->busy = 3;
            }else{
                station_selected = search_station_with_max_traveler();
                if(station_selected != -1){
                    printf("DESTINATION %d %d;",current_bus->id, station_selected);
                    disable_station(station_selected);
                    current_bus->busy = 2;
                }
            }
        }
        current_bus = current_bus->next;
    }
}

//Achète l'amélioration CT
void buy_price(){
    printf("UPDATECT;");
}

//Achète l'amélioration SP
void buy_speed(){
    printf("UPDATESP;");
}

//Achète un bus et le dépose sur la station avec le plus de voyageur
void buy_bus(){
    printf("BUS %d;", search_station_with_max_traveler());
}
//Fait jouer l'IA pour le premier tour et achète donc un bus
void play_first(int station){
    printf("BUS %d;PASS\n", station);
}
//Renvoie la station qui est la plus proche des deux autres stations
int search_station_first_turn(){
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
 * Fonction qui cherche la station avec le plus de voyageur
 * */
int search_station_with_max_traveler(){
    //Les autres tours
    struct nb_traveler_by_station* current_station = list_nb_station;
    if(current_station == NULL){
        return -1;
    }
    int station_with_better_traveler = current_station->id_station;
    int max_w_traveler = current_station->nb_waiting_traveler;
    while(current_station->next_station != NULL){
        if(max_w_traveler < current_station->nb_waiting_traveler && current_station->bus_go_to == 0){
            station_with_better_traveler = current_station->id_station;
            max_w_traveler = current_station->nb_waiting_traveler;
        }
        current_station = current_station->next_station;
    }
    return station_with_better_traveler; 
}

//Ajoute une station dans la liste chaînée list_station
void add_station(int id, int capacity, int x, int y){
    struct station* new_station;
    struct station* current_station = list_station;

    //Recherche de la dernière station du maillon
    while(current_station != NULL && current_station->next != NULL){
        current_station = current_station->next;
    }

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

//Renvoie la station correpondant à son id
struct station* get_station_by_id(int id_station){
    struct station* station = list_station;
    while(station != NULL && station->id != id_station){
        station = station->next;
    }
    return station;
}

//Renvoie la distance entre un bus et une station
int distance(struct bus* bus, struct station* station){
    return (int) sqrt(pow(bus->x - station->x, 2) + pow(bus->y - station->y, 2) );
}

//Renvoie la station la plus proche d'un bus
//Cette station possède des voyageurs du bus voulant y aller
int search_station_by_location(struct bus* bus){
    struct station** stations;
    int i=0,best_station,j=0,dist,best_dist;
    struct my_traveler* current_traveler = list_my_traveler;
    stations = (struct station**) malloc( sizeof(struct station**) * bus->size * 5);
    while(current_traveler != NULL){
        if(current_traveler->id_bus == bus->id){
            stations[i] = get_station_by_id(current_traveler->id_destination);
            i++;
        }
        current_traveler = current_traveler->next_traveler;
    }
    if(stations[0] == NULL)return -1;
    best_dist = distance(bus, stations[0]);
    best_station = stations[0]->id;
    while(j < (bus->size * 5) && stations[j] != NULL){
        dist = distance(bus,stations[j]);
        
        if( dist < best_dist ){
            best_station = stations[j]->id;
            best_dist = dist;
        }
        j++;
    }
    return best_station;
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

//Ajoute un bus à la liste chaînée list_bus ou met à jour les informations
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
//Ajoute un voyageur dans la liste chaînée list_traveler ou met à jour les informations
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
//Augmente les nombres de voyageurs voulant arriver à une station et partant d'une station
//La structure connait donc le nombre de personne attendant et voulant venir dans les stations
void increment_station(int pop_station, int dest_station){
    struct nb_traveler_by_station* current_station;
    struct nb_traveler_by_station* new_station;
    current_station = list_nb_station;
    while(current_station != NULL && (current_station->next_station != NULL && current_station->id_station != pop_station) ){
        current_station = current_station->next_station;
    }
    new_station = (struct nb_traveler_by_station*)malloc(sizeof(struct nb_traveler_by_station));
    //Augmente le nombre de voyageurs qu'il y a dans la station d'apparition
    if(current_station == NULL){
        new_station->id_station = pop_station;
        new_station->nb_waiting_traveler = 1;
        new_station->nb_coming_traveler=0;
        new_station->next_station = NULL;
        new_station->bus_go_to = 0;
        list_nb_station = new_station;
    }else{
        if(pop_station != current_station->id_station){
            current_station->next_station = new_station;
            new_station->id_station = pop_station;
            new_station->nb_waiting_traveler = 0;
            new_station->bus_go_to = 0;
            new_station->nb_coming_traveler=0;
            new_station->next_station = NULL;
        }else{
            new_station=current_station;
        }
        new_station->nb_waiting_traveler = new_station->nb_waiting_traveler + 1;
    }
    //Partie presque identique pour les voyageurs qui veulent aller à cette station (peut etre changer pour seulement les voyageurs que nous prenons dans nos bus)
    current_station = list_nb_station;
    while( current_station != NULL && current_station->next_station != NULL && current_station->id_station != dest_station){
        current_station = current_station->next_station;
    }
    new_station = (struct nb_traveler_by_station*)malloc(sizeof(struct nb_traveler_by_station));
    if(current_station == NULL ){
        new_station->id_station = dest_station;
        new_station->nb_coming_traveler = 1;
        new_station->nb_waiting_traveler = 0;
        new_station->next_station = NULL;
        new_station->bus_go_to = 0;
        list_nb_station = new_station;
    }else{
        if(dest_station != current_station->id_station){
            current_station->next_station = new_station;
            new_station->id_station = dest_station;
            new_station->nb_coming_traveler = 0;
            new_station->nb_waiting_traveler = 0;
            new_station->bus_go_to = 0;
            new_station->next_station = NULL;
        }else{
            new_station=current_station;
        }
        new_station->nb_coming_traveler = new_station->nb_coming_traveler + 1;
    }
    return;
}

//Rendre non disponible un voyageur
void disable_traveler(int id_traveler){
    struct traveler* current_traveler;
    current_traveler = list_traveler;
    while( current_traveler != NULL && current_traveler->next_traveler != NULL && current_traveler->id_traveler != id_traveler ){
        current_traveler = current_traveler->next_traveler;
    }
    if(current_traveler == NULL)return;
    if(current_traveler->id_traveler == id_traveler){
        current_traveler->available = 0;
        decrement_station(current_traveler->id_station_pop, current_traveler->id_station_dest);      
        return;
    }
}

//Réduit le nombre de voyageur dans la station de départ et d'arriver
void decrement_station(int station_pop, int station_dest){
    struct nb_traveler_by_station* station;
    station = list_nb_station;
    while (station->next_station != NULL && station->id_station != station_pop){
        station = station->next_station;
    }
    if(station-> id_station == station_pop){
        station->nb_waiting_traveler--;
    }else{
        return;
    }
    station = list_nb_station;
    while (station->next_station != NULL && station->id_station != station_dest){
        station = station->next_station;
    }
    if(station-> id_station == station_dest){
        station->nb_coming_traveler--;
    }else{
        return;
    }
    return;
}
//Renvoie 1 si le bus avec l'id id_bus appartient à l'IA
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
//Retire le voyageur avec l'id id_traveler de toutes les listes chaînées
void bye_traveler(int id_traveler){
    if( is_my_traveler(id_traveler) == 0 )return;
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
    //free(current_traveler);
}
//Renvoie si 1 le voyageur appartient à l'IA et 0 sinon
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
//Met à 1 le flag pour dire qu'un bus va dans cette station
void disable_station(int station){
    struct nb_traveler_by_station* current_station = list_nb_station;
    if(current_station == NULL)return;
    while (current_station->id_station != station){
        current_station = current_station->next_station;
    }
    current_station->bus_go_to = 1;
    return;
}
//Met à 0 le flag pour dire que la station est disponible pour y aller
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
//Renvoie 0 ou - si le bus est disponible (s'il n'est pas en déplacement) et strictement + que 0 sinon 
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
//Ajoute un voyageur à un bus dans la liste list_my_traveler possédant tous les voyageurs de l'IA
void add_traveler_to_bus(int idt,int idb){
    struct my_traveler* traveler = list_my_traveler;
    int idd;
    idd = get_destination_by_traveler(idt);
    struct my_traveler* new_traveler;
    new_traveler = (struct my_traveler*) malloc(sizeof(struct my_traveler));
    new_traveler->id = idt;
    new_traveler->id_bus = idb;
    new_traveler->id_destination = idd;
    if(traveler == NULL){
        list_my_traveler = new_traveler;
    }else{
        while(traveler->next_traveler != NULL){
            traveler = traveler->next_traveler;
        } 
        traveler->next_traveler=new_traveler;
    }
    return;
}
//Renvoie la destination d'un voyageur
int get_destination_by_traveler(int id_traveler){
    struct traveler* current_traveler = list_traveler;
    while(current_traveler->id_traveler != id_traveler){
        current_traveler=current_traveler->next_traveler;
    }
    return current_traveler->id_station_dest;
}
//Récupère les infos du premier tour
void get_first_info(){
    int ID,X,Y,K;
    for(int i = 0 ; i < 3 ; i++ ){
        scanf("%d %d %d %d",&ID,&X,&Y,&K);
        add_station(ID, K, X, Y);
    }
    return;
}
//Récupère les informations lié aux joueurs
void update_player(struct state_upgrade* s_upgrade, int nb_joueur){
    int j,m,usb,usp,uct,end;
    for(int i = 0 ; i < nb_joueur ; i++){//Informations sur tous les joueurs
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
}
//Récupère les informations lié aux stations
void update_station(){
    int ID,X,Y,K,ns;
    scanf("%d",&ns);//ns variable qui sert à savoir si une station a été créée
    if(ns == 1){//Création d'une nouvelle station
        scanf("%d %d %d %d",&ID,&X,&Y,&K);
        add_station(ID, K, X, Y);
    }
}
//Récupère les informations lié aux bus
void update_bus(struct state_upgrade* s_upgrade){
    int nbus,ID,X,Y,A,S,j;
    scanf("%d",&nbus);//La variable nbus permet de savoir combien de bus sont actuellement en partie
    for(int i = 0 ; i < nbus ; i++){
    scanf("%d %d %d %d %d %d",&ID, &j, &X, &Y, &A, &S);
    if(j == s_upgrade->my_id){
        add_bus(ID,j,X,Y,A,S);
        }
    }
}
//Récupère les informations lié aux nouveaux voyageurs
void update_new_traveler(int nt){
    int idt, ids1, ids2;
    for(int i=0 ; i < nt ; i++){
        scanf("%d %d %d",&idt, &ids1, &ids2);
        add_traveler(idt,ids1, ids2);
        increment_station(ids1,ids2);
    }
}
//Récupère les informations lié aux voyageurs montant dans un bus
void update_traveler_go_in_bus(int bt){
    int idt,idb;
    for(int i=0; i < bt ; i++){
        scanf("%d %d",&idt, &idb);
        disable_traveler(idt);//On met le voyageur comme indisponible et on réduit les informations liés à ce voyageur
        if(is_my_bus(idb) == 1){//Si c'est notre bus alors on va incrémenter le nb de voyageur dans notre bus
            add_traveler_to_bus(idt,idb);
        }
    }
}
//Récupère les informations lié aux voyageurs descandant d'un bus
void update_traveler_go_out_bus(int dt){
    int idt;
    for(int i=0; i < dt ; i++){
        scanf("%d",&idt);
        bye_traveler(idt);
    }
}
//Récupère les informations du jeu (les infos de de chaque tour)
void get_infos(struct state_upgrade* s_upgrade, int nb_joueur){
    int nt, bt, dt;
    update_player(s_upgrade,nb_joueur);
    update_station();    
    update_bus(s_upgrade);
    scanf("%d %d %d",&nt, &bt, &dt); 
    update_new_traveler(nt);
    update_traveler_go_in_bus(bt);
    update_traveler_go_out_bus(dt);      
}
//Gère les améliorations de l'IA, si l'IA doit acheter ou non une amélioration
void buy_upgrade(struct state_upgrade* s_upgrade){
    if(s_upgrade->nb_bus < ( MAX_NB_BUS-1 ) && s_upgrade->money >= PRICE_BUS){
        buy_bus();
        s_upgrade->nb_bus = s_upgrade->nb_bus + 1;
        s_upgrade->money = s_upgrade->money - 100;
    }
    if(s_upgrade->money >= PRICE_UPGRADE_CT && s_upgrade->ct < MAX_INCREASE_CT && s_upgrade->sp == MAX_INCREASE_SP){
        buy_price();
        s_upgrade->money = s_upgrade->money - 100;
    }
    if(s_upgrade->money >= PRICE_UPGRADE_SP && s_upgrade->sp < MAX_INCREASE_SP){
        buy_speed();
        s_upgrade->money = s_upgrade->money - 200;
    }
}
//Renvoie la station la plus proche d'un bus s'il contient des voyageurs sinon -1
//La station fait parti des destinations des voyageurs du bus
int deliver_traveler(struct bus* bus){
    int best_station = -1;
    if(list_my_traveler != NULL)best_station = search_station_by_location(bus);
    return best_station;
}
//Renvoie combien de voyageurs d'un bus id_bus vont à une destination id_destination
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
//Renvoie le nombre de voyageurs d'un bus avec l'id id_bus
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
//Libère la mémoire lié à la liste chaînée list_traveler
void free_traveler(){
    struct traveler* c_traveler = list_traveler;
    struct traveler* p_traveler;
    while(c_traveler != NULL){
        p_traveler = c_traveler;
        c_traveler = c_traveler->next_traveler;
        free(p_traveler);
    }
}
//Libère la mémoire lié à la liste chaînée list_bus
void free_bus(){
    struct bus* c_bus = list_bus;
    struct bus* p_bus;
    while(c_bus != NULL){
        p_bus = c_bus;
        c_bus = c_bus->next;
        free(p_bus);
    }
}
//Libère la mémoire lié à la liste chaînée list_my_traveler
void free_my_traveler(){
    struct my_traveler* c_traveler = list_my_traveler;
    struct my_traveler* p_traveler;
    while(c_traveler != NULL){
        p_traveler = c_traveler;
        c_traveler = c_traveler->next_traveler;
        free(p_traveler);
    }
}
//Libère la mémoire lié à la liste chaînée list_station
void free_station(){
    struct station* c_station = list_station;
    struct station* p_station;
    while(c_station != NULL){
        p_station = c_station;
        c_station = c_station->next;
        free(p_station);
    }
}
//Libère la mémoire lié à la liste chaînée list_nb_station
void free_nb_station(){
    struct nb_traveler_by_station* c_station = list_nb_station;
    struct nb_traveler_by_station* p_station;
    while(c_station != NULL){
        p_station = c_station;
        c_station = c_station->next_station;
        free(p_station);
    }
}
//Libère la mémoire lié à la liste chaînée list_player
void free_player(){
    struct player* c_player = list_player;
    struct player* p_player;
    while(c_player != NULL){
        p_player = c_player;
        c_player = c_player->next;
        free(p_player);
    }
}
//Appel toutes les fonctions pour libérer la mémoire en fin de partie
void game_over(){
    free_traveler();
    free_bus();
    free_my_traveler();
    free_station();
    free_nb_station();
    free_player();
}