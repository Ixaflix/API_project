#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define MAX_STR 30
#define HASH_MAG 64 //128
#define HASH_RIC 32
#define ALPHA (sqrt(5) - 1) / 2
#define MAX 0.5

//Struct magazzino

typedef struct lotto_{
    int quantita;
    int scadenza;
    struct lotto_ *next;
}lotto_t;

typedef struct ripiano_{
    char nome[MAX_STR];
    int totale;
    lotto_t* lotto;
}ripiano_t;

typedef struct{
    ripiano_t **key;
    int dimensione;
    int elementi;
}tabellaMag_t;

//Struct ricette

typedef struct ingrediente_{
    ripiano_t *ripiano;
    int quantita;
    struct ingrediente_ *next;
}ingrediente_t;

typedef struct ricetta_{
    char nome[MAX_STR];
    int totale;
    ingrediente_t* ingrediente;
    int tempo;
    int max;
}ricetta_t;

typedef struct{
    ricetta_t **key;
    int dimensione;
    int elementi;
}tabellaRic_t;

//Struct ordini 

typedef struct ordine_{
    int quantita;
    int totale;
    int arrivo;
    ricetta_t *ricetta;
    struct ordine_ *next;
}ordine_t;

void aggiungi_ricetta(tabellaRic_t*, tabellaMag_t*); //controlla se la ricetta è presente e la aggiunge
void rimuovi_ricetta(tabellaRic_t*, char[]); //rimuove la ricetta (controllo su ordini gia effettuato)
void rifornimento(tabellaMag_t*, int); //rifornisce il magazzino
ordine_t* crea_ordine(int, ricetta_t*); //crea un ordine collegando la ricetta
int cucino(ordine_t*, int); //elimina dal magazzino gli ingredienti necessari
int ordine_sospeso(ordine_t*, ordine_t*, char[]); //verifica tra gli attesi se c'è la ricetta che voglio eliminare
void controlla_ordini(ordine_t**, ordine_t**, ordine_t**, ordine_t**, int);
ordine_t* controlla_corriere(ordine_t*, ordine_t**, int); //controlla tra i completi e spedisce

int funzioneHash(char[], int dim);

//* GESTIONE TABELLA HASH PER MAGAZZINO
void creaMag(int dim, tabellaMag_t*);
void raddoppiaMag(tabellaMag_t*);
void inserisciMag(tabellaMag_t*, ripiano_t*);
int trovaMag(tabellaMag_t*, char[]);

//* GESTIONE TABELLA HASH PER RICETTARIO
void creaRic(int dim, tabellaRic_t*);
void raddoppiaRic(tabellaRic_t*);
void inserisciRic(tabellaRic_t*, ricetta_t*);
int trovaRic(tabellaRic_t*, char[]);


int main(int argc, char *argv[]){
	tabellaMag_t *magazzino = NULL;
    tabellaRic_t *ricettario = NULL;
    ordine_t *completi = NULL, *attesi = NULL, *ordine = NULL;
    ordine_t *fine_completi = NULL, *fine_attesi = NULL;
	char comando[MAX_STR], nome[MAX_STR];
    int tempo, dim, periodo, indice;

    magazzino = (tabellaMag_t*)malloc(sizeof(tabellaMag_t));
    if(magazzino){
        creaMag(HASH_MAG, magazzino);
    } else
        printf("errore allocazione tabella\n");
    
    ricettario = (tabellaRic_t*)malloc(sizeof(tabellaRic_t));
    if(ricettario){
        creaRic(HASH_RIC, ricettario);
    } else
        printf("errore allocazione tabella\n");
	

    if (scanf("%d %d", &periodo, &dim) != 2) {
        printf("Errore nella lettura di periodo e dim.\n");
        return 1;
    }

    tempo=0;
    while(scanf("%s", comando)){

        if(!(tempo%periodo) && tempo!=0){
            completi = controlla_corriere(completi, &fine_completi, dim);	
        }	

        if(comando[0]=='a'){
            aggiungi_ricetta(ricettario, magazzino);
        } 
        else if(comando[2]=='m'){
            if(scanf("%s", nome)){
                if(ordine_sospeso(attesi, completi, nome)){
                    printf("ordini in sospeso\n");
                }else{
                    rimuovi_ricetta(ricettario, nome);
                }
            }
        }
        else if(comando[2]=='f'){
            rifornimento(magazzino, tempo);
            controlla_ordini(&attesi, &completi, &fine_attesi, &fine_completi, tempo);
        }
        else if(comando[0]=='o'){
            if(scanf("%s", nome)){
                indice = trovaRic(ricettario, nome);
                if(indice != -1){
                    ordine = crea_ordine(tempo, ricettario->key[indice]);
                    if(ordine){
                        printf("accettato\n");
                        if(cucino(ordine, tempo)){
                            if(completi){
                                fine_completi->next = ordine;
                                fine_completi = fine_completi->next;
                            }else{
                                completi = ordine;
                                fine_completi = ordine;
                            }  
                        }else{
                            if(attesi){
                                fine_attesi->next = ordine;
                                fine_attesi = fine_attesi->next;
                            }else{
                                attesi = ordine;
                                fine_attesi = ordine;
                            }  
                        }
                    }else{
                        printf("errore creazione ordine\n");
                    }
                    ordine=NULL;
                }else{
                    if(scanf("%d", &indice)){
                        printf("rifiutato\n");
                    }
                }
            }                        
        } 
        else {
            return 0;
        }
        tempo++;
        strcpy(comando, "  ");
    }
    return 0;
}

//FUNZIONI PRINCIPALI

void aggiungi_ricetta(tabellaRic_t *ricettario, tabellaMag_t *magazzino){
    ricetta_t *tmp = NULL;
    ingrediente_t *ingrediente = NULL;
    ripiano_t *ripiano = NULL;
	char car, nome[MAX_STR];
	int num, indice;

	if(scanf("%s", nome)){
        indice = trovaRic(ricettario, nome);
        if(indice != -1){
            //caso in cui la ricetta non sia gia presente
            printf("ignorato\n");
            num=0;
            while(scanf("%c", &car) && car != '\n'){
                num++;
            }
			return;
        }else{
            //inserisci la nuova ricetta e gli ingredienti
            tmp = (ricetta_t*)malloc(sizeof(ricetta_t));
            if(tmp){
                strcpy(tmp->nome, nome);
                tmp->ingrediente = NULL;
                tmp->totale = 0;
                tmp->tempo = 0;
                tmp->max = 0;
                
                while(scanf("%c", &car) && car != '\n'){
                    ingrediente = (ingrediente_t*)malloc(sizeof(ingrediente_t));

                    if(ingrediente && scanf("%s", nome) && scanf("%d", &ingrediente->quantita)){
                        indice = trovaMag(magazzino, nome);
                        if(indice != -1){
                            //se l'ingrediente esiste
                            ingrediente->ripiano = magazzino->key[indice];
                        }else{
                            //se devo creare il ripiano
                            ripiano = (ripiano_t*)malloc(sizeof(ripiano_t));
                            if(ripiano){
                                strcpy(ripiano->nome, nome);
                                ripiano->totale = 0;
                                ripiano->lotto = NULL;
                                inserisciMag(magazzino, ripiano);
                                ingrediente->ripiano = ripiano;
                            }else{
                                printf("Errore allocazione ripiano\n");
                            }
                        }
                        ingrediente->next = tmp->ingrediente;
                        tmp->ingrediente = ingrediente;
                        tmp->totale += ingrediente->quantita;
                    }else{
                        printf("Errore di allocazione o lettura ingrediente ricettario\n");
                        return;
                    }
                }
                inserisciRic(ricettario, tmp);
            } else
                printf("Errore di allocazione ricetta\n");
        }
		printf("aggiunta\n");
	}

	return;
}

void rimuovi_ricetta(tabellaRic_t *ricettario, char nome[]){
    ingrediente_t *ingrediente=NULL;
    int indice;

    indice = trovaRic(ricettario, nome);    
    if(indice == -1){
        //Se non la trovo
        printf("non presente\n");
        return;
    } else{
        while(ricettario->key[indice]->ingrediente){
            ingrediente = ricettario->key[indice]->ingrediente;
            ricettario->key[indice]->ingrediente = ricettario->key[indice]->ingrediente->next;
            free(ingrediente);
        }
        ricettario->key[indice]->totale = 0;
        ricettario->key[indice]->nome[0] = '\0';
        printf("rimossa\n");
    }
    return;
}

void rifornimento(tabellaMag_t *magazzino, int tempo){
    lotto_t *lotto = NULL, *ltmp = NULL, *prec = NULL;
    ripiano_t *tmp = NULL;
    char car, nome[MAX_STR];
	int trovato, indice;

    while(scanf("%c", &car) && car != '\n'){
        //creo un lotto
        lotto = (lotto_t*)malloc(sizeof(lotto_t));

        if(lotto != NULL && scanf("%s", nome) && scanf("%d", &lotto->quantita) && scanf("%d", &lotto->scadenza)){
            lotto->next = NULL;

            if(lotto->scadenza > tempo){
                indice = trovaMag(magazzino, nome);
                if(indice != -1){
                    //caso in cui il lotto sia gia presente
                    trovato = 0;
                    magazzino->key[indice]->totale += lotto->quantita;

                    if(magazzino->key[indice]->lotto == NULL){
                        //caso in cui il ripiano sia vuoto
                        magazzino->key[indice]->lotto = lotto;
                    }else{
                        //se il ripiano non è vuoto
                        ltmp = magazzino->key[indice]->lotto;
                        for(prec = NULL; ltmp && !trovato; prec = ltmp, ltmp = ltmp->next){
                            if(ltmp->scadenza == lotto->scadenza){
                                ltmp->quantita += lotto->quantita;
                                trovato = 1;
                                free(lotto);
                            } else if(ltmp->scadenza > lotto->scadenza){
                                if(!prec){
                                    //Inserisco in testa
                                    if(magazzino->key[indice]->lotto != ltmp)
                                        printf("qualcosa non quadra\n");
                                    lotto->next = magazzino->key[indice]->lotto;
                                    magazzino->key[indice]->lotto = lotto;
                                }else{
                                    //Inserisco nel mezzo
                                    lotto->next = ltmp;
                                    prec->next = lotto;
                                }
                                trovato = 1;
                            }
                        }
                        if(!trovato){
                            //Inserisco in coda
                            lotto->next = NULL;
                            if(!prec){
                                //Inserisco in testa
                                magazzino->key[indice]->lotto = lotto;
                            }else{
                                //Inserisco nel mezzo
                                prec->next = lotto;
                            }
                        }
                    }
                    
                }else{
                    //inserisci il nuovo ripiano e il lotto
                    tmp = (ripiano_t*)malloc(sizeof(ripiano_t));
                    if(tmp != NULL){
                        strcpy(tmp->nome, nome);
                        tmp->totale = lotto->quantita;
                        tmp->lotto = lotto;
                        inserisciMag(magazzino, tmp);
                    } else
                        printf("Errore di allocazione ripiano\n");
                }
            } else {
                free(lotto);
            }

        } else
            printf("Errore di allocazione o lettura lotto rif\n");
    }
	printf("rifornito\n");
	return;
}

int ordine_sospeso(ordine_t* attesi, ordine_t* completi, char nome[]){
    int sospendi=0;
    //verifico se esiste un ordine con quel nome
    for(; attesi && !sospendi; attesi=attesi->next){
        if(!strcmp(attesi->ricetta->nome, nome)){
            sospendi=1;
        }
    }
    for(; completi && !sospendi; completi=completi->next){
        if(!strcmp(completi->ricetta->nome, nome)){
            sospendi=1;
        }
    }
    return sospendi;
}

ordine_t* crea_ordine(int tempo, ricetta_t* ricetta){
    ordine_t* ordine = NULL;

    ordine = (ordine_t*)malloc(sizeof(ordine_t));
    if(ordine){
        if(scanf("%d", &ordine->quantita)){
            ordine->arrivo = tempo;
            ordine->totale = (ordine->quantita) * ricetta->totale;
            ordine->ricetta = ricetta;
            ordine->next = NULL;
        }
    } else
        printf("Errore di allocazione\n");
    
    return ordine;
}

int cucino(ordine_t* ordine, int tempo){
    int conto;
    ingrediente_t* ing=NULL;
    lotto_t *lot=NULL;

    //verifico ci siano i materiali e che gli indici siano giusti
    for(ing=ordine->ricetta->ingrediente; ing; ing=ing->next){
        //controllo le scadenze
        // do per scontato che ogni ingrediente abbia ripiano != NULL
        while(ing->ripiano->lotto && ing->ripiano->lotto->scadenza <= tempo){
            lot = ing->ripiano->lotto;
            ing->ripiano->lotto = ing->ripiano->lotto->next;
            ing->ripiano->totale -= lot->quantita;
            free(lot);
        }

        //ingrediente non sufficiente
        if(ing->ripiano->totale < (ing->quantita * ordine->quantita)){
            ordine->ricetta->tempo = tempo;
            ordine->ricetta->max = ordine->quantita;
            return 0;
        }
    }
    

    //elimino i materiali
    for(ing=ordine->ricetta->ingrediente; ing; ing=ing->next){
        for(lot=ing->ripiano->lotto, conto=(ing->quantita * ordine->quantita); lot && conto>0; ){
            if(conto>=lot->quantita){
                //se devo eliminare il ingrediente
                conto -= lot->quantita;
                ing->ripiano->lotto = lot->next;
                ing->ripiano->totale -= lot->quantita;
                free(lot);
                lot = ing->ripiano->lotto;    
            }else{
                //se devo togliere la quantità
                ing->ripiano->totale -= conto;
                lot->quantita -= conto;
                conto = 0;
            }
        }
    }
    return 1;
}

//per ogni atteso verifica se puo essere cucinato e si occupa anche di spostarlo di lista;
void controlla_ordini(ordine_t** attesi, ordine_t** completi, ordine_t** fine_attesi, ordine_t** fine_completi, int tempo){
    ordine_t* tmp=NULL, *prec=NULL, *cmp=NULL, *prec2=NULL;
    ricetta_t *ric = NULL;

    for(tmp=*attesi; tmp;){
        ric = tmp->ricetta;
        if(ric->max <= tmp->quantita && ric->tempo == tempo){
            //se so che a questo tempo una ricetta con quantita minore non è stata prodotta
            prec = tmp;
            tmp = tmp->next;
        }else{
            //se ho cucinato sposto l'ordine in ordine di arrivo 
            if(cucino(tmp, tempo)){
                if(tmp==*attesi){
                    *attesi = tmp->next;
                }else{
                    prec->next = tmp->next;
                }

                if(tmp->next == NULL){
                    *fine_attesi = prec;
                }

                //caso in cui inserisco in testa a completi
                if(*completi==NULL || (*completi)->arrivo > tmp->arrivo){
                    tmp->next = *completi;
                    *completi = tmp;
                    if(tmp->next == NULL)
                        *fine_completi = tmp;
                } else {
                    // Inserimento ordinato nella lista completi
                    prec2 = *completi;
                    cmp = (*completi)->next;
                    while (cmp && cmp->arrivo <= tmp->arrivo) {
                        prec2 = cmp;
                        cmp = cmp->next;
                    }

                    if(cmp == NULL){
                        *fine_completi = tmp;
                    } 
                    prec2->next = tmp;
                    tmp->next = cmp;
                }

                // Aggiorna tmp per continuare la scansione della lista attesi
                tmp = (prec == NULL) ? *attesi : prec->next;
            } else {
                // Se non è stato cucinato, passa al prossimo ordine
                prec = tmp;
                tmp = tmp->next;
            }
        }
    }
} 

ordine_t* controlla_corriere(ordine_t* completi, ordine_t **fine_completi, int dim){
    ordine_t* tmp=NULL, *ordinati=NULL, *prec=NULL;
    int max;

    //sposto completi fino alla prima che eccede il totale
    ordinati=completi;
    if(ordinati && ordinati->totale > dim){
        printf("Carico troppo pesante\n");
        return completi;
    }else if(ordinati && ordinati->totale == dim){
        completi=completi->next;
        ordinati->next=NULL;
    }else if(ordinati){
        dim -= ordinati->totale;
        for(prec=ordinati, tmp=ordinati->next; tmp && tmp->totale<=dim; prec=tmp, tmp=tmp->next){
            dim -= tmp->totale;
        }
        completi=prec->next;
        if(completi==NULL){
            *fine_completi=NULL;
        }
        prec->next=NULL;
    }else{
        printf("camioncino vuoto\n");
        return completi;
    }
    
    while(ordinati){
        max=ordinati->totale;
        for(tmp=ordinati; tmp; tmp=tmp->next){
            if(tmp->totale>max)
                max=tmp->totale;
        }

        tmp=ordinati;
        if(tmp && tmp->totale==max){
            printf("%d %s %d\n", tmp->arrivo, tmp->ricetta->nome, tmp->quantita);
            ordinati=tmp->next;
            free(tmp);
        }else{
            for(prec=tmp, tmp=tmp->next; tmp; prec=tmp, tmp=tmp->next){
                if(tmp->totale==max){
                    prec->next = tmp->next;
                    printf("%d %s %d\n", tmp->arrivo, tmp->ricetta->nome, tmp->quantita);
                    free(tmp);
                    break;
                }
            }
        }
    }
    
    return completi;
} 

//FUNZIONI AUSILIARE

int funzioneHash(char key[], int dim){
    int sum, i;
    for(i=0, sum=0; key[i]!='\0'; i++){
        sum += key[i];
    }
    sum = ((int)(dim * ((float)(ALPHA * sum) - (int)(ALPHA * sum)))) % dim;
    return sum;
}

//! GESTIONE TABELLA HASH PER MAGAZZINO

void creaMag(int dim, tabellaMag_t *tabella){
    int i;

    tabella->dimensione = dim;
    tabella->elementi = 0;
    tabella->key = (ripiano_t**)malloc(dim * sizeof(ripiano_t*));
    if(tabella->key){
        for(i=0; i<dim; i++)
            tabella->key[i] = NULL;
    }else  
        printf("errore allocazione tabella\n");
}

void raddoppiaMag(tabellaMag_t *tabella){
    int dim, i, indice;
    ripiano_t **prec = tabella->key;

    dim = tabella->dimensione;
    creaMag(dim*2, tabella);
    if(tabella->key){
        for(i=0; i<dim; i++){
            if(prec[i]){
                indice = funzioneHash(prec[i]->nome, tabella->dimensione);
                while(tabella->key[indice] != NULL)
                    indice = (indice + 1) % tabella->dimensione;
                tabella->key[indice] = prec[i];
            }
        }
        free(prec);
    }else  
        printf("errore allocazione tabella\n");
}

void inserisciMag(tabellaMag_t *tabella, ripiano_t *ripiano){
    int indice;

    if(tabella->elementi >= tabella->dimensione*MAX){
        raddoppiaMag(tabella);
    }

    indice = funzioneHash(ripiano->nome, tabella->dimensione);

    while(tabella->key[indice] != NULL)
        indice = (indice + 1) % tabella->dimensione;
    tabella->key[indice] = ripiano;
    tabella->elementi++;
}

int trovaMag(tabellaMag_t *tabella, char key[]){
    int indice;

    indice = funzioneHash(key, tabella->dimensione);
    while(tabella->key[indice] != NULL && strcmp(tabella->key[indice]->nome, key))
        indice = (indice + 1) % tabella->dimensione;
    if(tabella->key[indice] != NULL)
        return indice;
    return -1;
}

//! GESTIONE TABELLA HASH PER RICETTARIO

void creaRic(int dim, tabellaRic_t *tabella){
    int i;

    tabella->dimensione = dim;
    tabella->elementi = 0;
    tabella->key = (ricetta_t**)malloc(dim * sizeof(ricetta_t*));
    if(tabella->key){
        for(i=0; i<dim; i++)
            tabella->key[i] = NULL;
    }else  
        printf("errore allocazione tabella\n");
}

void raddoppiaRic(tabellaRic_t *tabella){
    int dim, i, indice;
    ricetta_t **prec = tabella->key;

    dim = tabella->dimensione;
    creaRic(dim*2, tabella);
    if(tabella->key){
        for(i=0; i<dim; i++){
            if(prec[i]){
                indice = funzioneHash(prec[i]->nome, tabella->dimensione);
                while(tabella->key[indice] != NULL)
                    indice = (indice + 1) % tabella->dimensione;
                tabella->key[indice] = prec[i];
            }
        }
        free(prec);
    }else  
        printf("errore allocazione tabella\n");
}

void inserisciRic(tabellaRic_t *tabella, ricetta_t *ricetta){
    int indice;

    if(tabella->elementi >= tabella->dimensione*MAX){
        raddoppiaRic(tabella);
    }

    indice = funzioneHash(ricetta->nome, tabella->dimensione);

    while(tabella->key[indice] != NULL || (tabella->key[indice] != NULL && tabella->key[indice]->totale == 0))
        indice = (indice + 1) % tabella->dimensione;
    if(tabella->key[indice] != NULL && tabella->key[indice]->totale == 0)
        free(tabella->key[indice]);
    tabella->key[indice] = ricetta;
    tabella->elementi++;
}

//forse posso ottimizzare se serve e variare la ricerca tra ricettario e magazzino
int trovaRic(tabellaRic_t *tabella, char key[]){
    int indice;

    indice = funzioneHash(key, tabella->dimensione);
    while(tabella->key[indice] != NULL && strcmp(tabella->key[indice]->nome, key))
        indice = (indice + 1) % tabella->dimensione;
    if(tabella->key[indice] != NULL && !strcmp(tabella->key[indice]->nome, key) && tabella->key[indice]->totale != 0)
        return indice;
    return -1;
}


