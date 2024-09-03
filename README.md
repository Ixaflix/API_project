# Project Title

Prova finale di algoritmi e strutture dati 2023-2024

## Description

Una pasticceria industriale vuole migliorare il proprio sistema di gestione degli ordini, e vi incarica di sviluppare un software che simuli il funzionamento della pasticceria. 
L’intera simulazione avviene a tempo discreto. Si assuma che, a valle dell’esecuzione di ogni comando ricevuto in ingresso, trascorra un istante di tempo. La simulazione ha inizio al tempo 0. 
Nella simulazione si dovranno considerare i seguenti elementi:
* Gli ingredienti dei dolci, ognuno identificato dal suo nome, costituito da una sequenza di caratteri.
* L’insieme delle ricette offerte dalla pasticceria, identificate anch’esse da un nome. Ogni ricetta utilizza diverse quantità di ciascun ingrediente necessario (indicate da un numero intero, in grammi).
* Il magazzino degli ingredienti della pasticceria, che stocca ogni ingrediente utilizzato. Il magazzino viene
rifornito da nuovi lotti di ingredienti, in base a una pianificazione stabilita dal fornitore. Ogni lotto
è caratterizzato da una quantità (sempre in grammi) e da una data di scadenza, indicata dal numero
dell’istante di tempo a partire dal quale il lotto è scaduto.
* I clienti della pasticceria effettuano ordini di uno o più dolci tramite una piattaforma online o per
telefono. In ogni caso, la pasticceria procede subito a preparare i dolciumi ordinati. Gli avanzati (e
costosi) macchinari che preparano i dolci sono così veloci che si può assumere che la preparazione di un
numero arbitrario di dolci avvenga in un singolo istante della simulazione. Gli ingredienti necessari per ogni
preparazione vengono prelevati dal magazzino privilegiando sempre i lotti con la scadenza più prossima.
Se non sono disponibili ingredienti a sufficienza da consentire la preparazione per intero di un ordine, esso
viene messo in attesa. È possibile avere un numero arbitrario di ordini in attesa. La pasticceria procede
a preparare eventuali ordini successivi. Ad ogni rifornimento, la pasticceria valuta se è possibile, con gli
ingredienti ricevuti, preparare ordini attualmente in attesa. Se questo è il caso, li prepara nello stesso
istante di tempo. Gli ordini in attesa vengono smaltiti in ordine cronologico di arrivo dell’ordine.
* Periodicamente, il corriere si reca dalla pasticceria a ritirare gli ordini pronti. All’arrivo del corriere,
gli ordini da caricare vengono scelti in ordine cronologico di arrivo. Il processo si ferma appena viene
incontrato un ordine che supera la capienza rimasta (in grammi) sul camioncino. Si assuma che il peso di
ogni dolce preparato sia uguale alla somma delle quantità in grammi di ciascun ingrediente. Ogni ordine
viene sempre caricato nella sua interezza. Scelti gli ordini, la pasticceria procede a caricarli in ordine di
peso decrescente. A parità di peso, gli ordini vanno caricati in ordine cronologico di arrivo.

### Inputs description

Si assuma che tutte le quantità siano intere e maggiori di zero, indipendentemente dall’unità di misura. I nomi,
siano essi di ingredienti o ricette, sono definiti sull’alfabeto {a, . . . , z, A, . . . , Z, 0, . . . , 9, _} e sono lunghi fino a
255 caratteri. La simulazione termina in seguito alla lettura dell’ultimo comando.
Il file testuale in ingresso inizia con una riga contenente due interi: la periodicità del corriere e la sua
capienza. Seguono una sequenza di comandi, uno per riga, con il seguente formato. Tutti i valori interi positivi
o nulli sono codificabili in 32 bit.
* aggiungi_ricetta ⟨nome_ricetta⟩ ⟨nome_ingrediente⟩ ⟨quantità⟩ ... Aggiunge una ricetta al catalogo. Il numero di coppie (⟨nome_ingrediente⟩ ⟨quantità⟩) è arbitrario. Se
una ricetta con lo stesso nome è già presente, viene ignorato.
Stampa attesa come risposta: aggiunta oppure ignorato.
* rimuovi_ricetta ⟨nome_ricetta⟩
Rimuove una ricetta dal catalogo. Non ha effetto se la ricetta non è presente, oppure ci sono ordini relativi
ad essa non ancora spediti.
Stampa attesa come risposta: rimossa, ordini in sospeso oppure non presente.
* rifornimento ⟨nome_ingrediente⟩ ⟨quantità⟩ ⟨scadenza⟩ ...
La pasticceria viene rifornita di un insieme di lotti, uno per ingrediente. Il numero di lotti è arbitrario.
Stampa attesa come risposta: rifornito.
* ordine ⟨nome_ricetta⟩ ⟨numero_elementi_ordinati⟩
Effettua un ordine di ⟨numero_elementi_ordinati⟩ dolci con ricetta ⟨nome_ricetta⟩.
Stampa attesa come risposta: accettato oppure rifiutato se non esiste nessuna ricetta col nome
specificato.
* In aggiunta alle stampe sopraelencate, il programma stampa gli ordini contenuti nel camioncino del corriere
come una sequenza di triple ⟨istante_di_arrivo_ordine⟩ ⟨nome_ricetta⟩ ⟨numero_elementi_ordinati⟩, una per
rigo, in ordine di caricamento. Dato un corriere di periodicità n, la stampa è effettuata prima di gestire i comandi
al tempo kn con k ∈ 1, 2, . . .. Se il camioncino è vuoto, viene stampato il messaggio camioncino vuoto.

## Getting Started

### Dependencies

* GCC compiler or any online complier for C.

### Executing program

* Comandi step-by-step per avviare il programma
```
make test
./test
./test <tests/inputs/open1.txt
```

## Authors

Sara Ferrarese
