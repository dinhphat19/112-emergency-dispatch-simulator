NGUYEN DINH PHONG PHAT-311CC

PUNCTAL OBTINUT: 120/120

I,STRUCTURI
    typedef struct node:    este multifunctional.Este nodul comuna folosita atat pt coada,cat si pentru stica
    typedef struct cozi:    pentru coada(gestionează ca lista circulare)
    typedef struct stive    pentru stiva(gestionează ca lista simplu)
    struct unit:    Reprezintă echipele (ambulanță, pompieri, poliție...). Stochează ID-ul, tipul și starea de disponibilitate/ocupat (availability).
    struct incident:    Reprezinta informatiile incident (ID,priority,description,status),este si o lista dublu inlantuita cu dummy node
    struct intervention:    Reprezintă intervenția. Are rolul unui struct care înglobează cele două structuri unit si incident,
                            asociind un incident (incident) în desfășurare cu o echipă de intervenție (unit) care îl gestionează,este si lista dublu inlantuita cu dummy node
    struct system:          struct central, contine toate ca un system (vector de unit,lista de incident,lista de interventi, toate cozi si stive)

II,functii
+)  struct incident* initDummy_incident() 
        initializeaza nodul dummy pt lista de incidenti
        node dummy este cu id=0,prioritate este low,description este test incident si status este solved;
+) struct intervention* initDummy_intervention()
        initializeaza nodul dummy pt lista de interventi
        node dummy este cu: intervention0->incident=NULL;
                            intervention0->unit=NULL;
+) void enqueue(coada c,struct incident *new_incident)
        adaugare new_incident in lista de incidente
        Se inițializează un nod new, iar acesta va conține toate informațiile din new_incident. Dacă coada nu are niciun element, new va fi primul element și va fi și head.
        Dacă există deja elemente, se leagă new de primul element (head) și de tail, apoi se actualizează new ca noul tail.
+)void dequeue(coada c)
        eliminare primul element adaugat in coada 
        daca coada are doar un element=>coada devine NULL;
        daca nu, new_head=head->next;(c->final->next=c->final->next->next;)
+)struct incident* first(coada c)
        returneaza infomatiile primul element in coada
+)void add_incident(struct system *system112,int id,char *prior,char *descript)
        -alocare memorie pt un incident nou(new_incident)
        -completeaza informatiile pentru incidentul nou din argumentele dat(id,*prior,*descript)
        -adaugare new_incident in "sfarsitului" lista de incidente
        -notam struct incident *dummy=system112->incidents
        -notam struct incident *last=dummy->prev;
        algoritmul de adaugare:
            new_incident->next=dummy;
            last->next=new_incident;
            new_incident->prev=last;
            dummy->prev=new_incident;
        -depinde de prioritate, il punem in coada corespunzator(high,medium,low) cu functie enqueue
+)void show_incident(struct system *system112,int ID)
        -parcurgem pe lista de incidente, verificam daca exista incident care are incident->id=ID
        -daca nu este gasit, afisam ERROR
        -daca este gasit afisam
+)void enqueue_units(coada c,struct unit *new_unit)
        -functie pentru a adauga new nod in coada de units(system112->queue_available_units)
        -functioneaza exact ca functie enqueue
+)void add_unit(struct system *system112,int nr_unit)
        -functie pentru a adauga new echpa in coada de available unit
        -parcurgem in for de la 0 la nr_unit-1, pentru fiecare system112->units[i], il adaugam in system112->queue_available_units
+)void check_units_availability(coada c)
        -parcurgem prin coada de available unit si afisam toate elemente si informatiile lor 
+)void show_unit(struct system *system112,int ID,int nr_unit)
        -parcugem in vector de unit( for(int i=0;i<nr_unit;i++)), trebuie sa gasim echipa care are unit->id=ID
        --daca am gasit, o afisam
        -daca nu , afisam ERROR
+)void push_stack(stiva s,struct intervention *new_intervention) 
        -folosita pentru a adauga new element in stiva
        -folosit pentru stiva de dispatch_history dupa
        -alocare mem pt o intervenție noua new
        -new->s_intervention=new_intervention
        -new->next=s->top;
        -s->top=new;=>new devine element pe varfului stive
+)void pop_stack(stiva s)
        -eliminare elementul pe varfului stiva s cu ajutorul variabila auxiliar del
        - list del=s->top;
        -s->top=s->top->next;
        -free(del)
+)struct intervention* top_stack(stiva s)
        -returneaza informatiile primul element(element pe varful) a lui stack s (s->top->s_intervention)
+)struct unit* first_unit(coada c)
        returneaza prima echipa in coada DE available units(c->final->next->q_unit)
+)void dispatch(struct system *system112)
        Atribuie o echipă de intervenție (Unit) disponibilă unui incident (Incident) aflat în așteptare. Alegerea incidentului se face strict pe baza nivelului de prioritate (High > Medium > Low). 
        În același timp, funcția va crea o înregistrare de intervenție (Intervention) și o va salva în istoricul pentru a susține funcționalitatea de UNDO (Undo).
        -pasul_1:Verifică dacă există vreun incident în lista principală și dacă coada cu echipe disponibile (queue_available_units) nu este goală. Dacă lipsește unul dintre elemente, se afișează o eroare.
        -pasul_2:Selectarea priorității->verifică pe rând cozile de incidente în ordinea: queue_high → queue_medium → queue_low. Prima coadă care conține elemente (nu este NULL) va fi aleasă ca sursă pentru extragerea incidentului.
        -pasul_3:Se utilizează funcția dequeue pentru a extrage incidentul cu cea mai mare prioritate și prima echipă de intervenție disponibilă din cozile lor
        -pasul_4:Se alocă dinamic (malloc) memorie pentru un new_intervention
                ->Leagă pointerele incidentului și ale echipei de intervenție la această intervenție
                ->Actualizează starea incidentului la „intervened” și schimbă starea echipei de intervenție la ocupat (availability = 0).
        -pasul_5:adaugam new_intervention la sfarsitul listei de interventions
        -pasul_6:Actualizarea istoricului->folosim push_stack(system112->dispatch_history,new_intervention);
+)void enqueue_first(coada c,struct incident *new_incident)
        adauga new element la INCEPUTUL coada(in cazul acesta, adauga new_incident in lista de incidente)
         new->next=c->final->next;// 1. Noul nod pointează către Head (primul element)
        c->final->next=new;// 2. Tail-ul vechi pointează către noul nod
        // 3. NU se actualizează pointerul Tail la noul nod
+)void undo_last_dispatch(struct system *system112)
        se extrag interventii din istoricul de interventii demarate (stivă) până se găseste interventie care nu a fost încă finalizată, interventiile finalizate fiind ignorate
        incidentul asociat interventiei sistate este marcat ca având din nou status-ul "queued" si
        este plasat înapoi la începutul cozii sale specifice de asteptare;echipajul asociat interventiei sistate este marcat ca fiind din nou "available" si va trebui introdus la finalul cozii de disponibilitate
        -pasul_1:Verifică dacă lista de intervenții și stiva istoricului (dispatch_history) sunt inițializate și conțin elemente. Dacă sunt goale, afișează o eroare
        -pasul_2:Caută în vârful stivei și dă pop până când găsește o intervenție al cărei incident are încă statusul "intervened" (astfel sunt ignorate incidentele deja marcate ca "solved"). Dacă stiva se golește fără a găsi una, returnează eroare.
        -pasul_3:Schimbă statusul incidentului înapoi în "queued". Apoi, în funcție de prioritatea acestuia (high, medium, low), îl reintroduce la începutul cozii de așteptare folosing funcția enqueue_first
        -pasul_4:Actualizează statusul echipajului alocat la availability = 1 (liber) și îl adaugă înapoi la finalul cozii de echipaje disponibile (queue_available_units)
        -pasul_5:Desface legăturile (next și prev) nodului curent din lista dublu înlănțuită a intervențiilor. La final, extrage definitiv elementul din stivă și eliberează memoria alocată dinamic (free)
+)void solved_incident(struct system *system112,int ID_incident)
        Această funcție are rolul de a finaliza o misiune aflată în desfășurare. Ea marchează incidentul ca fiind rezolvat și eliberează echipajul care i-a fost alocat, 
        reintroducându-l în circuitul de așteptare pentru a putea prelua noi urgențe.
        -pasul_1:verifică dacă lista de intervenții este inițializată și dacă conține elemente (pe lângă Dummy Node). Dacă este goală
        -pasul_2:Se parcurge liniar lista dublu înlănțuită circulară a intervențiilor pentru a căuta un element care să corespundă cu ID_incident dat ca parametru. Se folosește un semnalizator (ok) pentru a reține dacă ID-ul a fost găsit.
        -pasul_3:Se verifică două condiții esențiale: ID-ul trebuie să existe (ok == 1), iar statusul incidentului trebuie să fie strict "intervened". Dacă incidentul este deja rezolvat sau doar în așteptare, operațiunea este respinsă.
        -pasul_4:Statusul incidentului este actualizat în "solved".
                 (Notă: Intervenția nu este ștearsă din listă, ci rămâne acolo ca istoric permanent al sistemului).
        -pasul_5:Echipajului (unit) care a rezolvat incidentul i se schimbă starea în availability = 1 (liber). Apoi, acesta este reintrodus în coada echipajelor disponibile (queue_available_units) prin apelul funcției enqueue_units
+)void show_interventions(struct system *system112)
        Afiseaza infomatiile pentru toate interventiile in lista de interventi(indiferent dacă misiunile sunt încă în desfășurare sau au fost deja finalizate)
        -pasul_1:verifică dacă lista de intervenții nu a fost inițializată sau dacă conține exclusiv elementul santinelă (Dummy Node). Dacă lista este goală, se afișează mesajul informativ "No intervention has been initiated" și funcția se oprește.
        -pasul_2:Se inițializează un pointer (list_intervention) care pleacă de la primul nod real, situat imediat după Dummy Node (system112->interventions->next). Lista dublu înlănțuită este parcursă nod cu nod, 
                ciclul oprindu-se atunci când pointerul ajunge înapoi la elementul santinelă.
        -pasul_3:Pentru fiecare intervenție vizitată în timpul parcurgerii, funcția extrage și afișează în mod formatat: ID-ul incidentului, ID-ul echipajului care a preluat cazul și statusul actualizat al incidentului
+)void free_queue(coada c) si void free_stack(stiva s)
        Ambele functii au rol de a elibera memorie cozi si stive care am folosit pana acum
+)void free_intervention(struct system *system112)
        Are rol de a elibera memorie pentru lista de interventi (system112.interventions)
        1. Dacă nu a fost inițializat niciodată, nu este nevoie de free
        2. Începe parcurge de la nodul real primul (imediat după nodul Dummy) -struct intervention *list_intervention=system112->interventions->next;
            struct intervention *temp=list_intervention;
            list_intervention=list_intervention->next;
            free(temp);
        eliberam memorie pentru fiecare element intalnit
        3. Când bucla se termină,lis_intervention s-a întors la nodul Dummy.În acest moment, toate nodurile reale au fost eliminate, rămâne doar nodul Dummy
        =>eliberam si node dummy 
            free(system112->interventions);
            system112->interventions=NULL;
+) void free_incident(struct system *system112)
        Are rol de a elibera memorie pentru lista de incidente (system112.incidents)
        1. Dacă nu a fost inițializat niciodată, nu este nevoie de free
        2. Începe parcurge de la nodul real primul (imediat după nodul Dummy) - struct incident *list_incident=system112->incidents->next;
            struct incident *temp=list_incident;
            list_incident=list_incident->next;
            free(temp->description);// Eliberează memoria pe care am alocat-o pentru description-ul nodurilor in lista
            free(temp);
        eliberam memorie pentru fiecare element intalnit
        -trebuie sa eliberam (tem->description) separat pentru ca am alocat dinamic memorie pentru description la inceputul
        3. Când bucla se termină,list_incident s-a întors la nodul Dummy.În acest moment, toate nodurile reale au fost eliminate, rămâne doar nodul Dummy
        =>eliberam si node dummy 
             free(system112->incidents->description);// Eliberează memoria pe care am alocat-o pentru description-ul nodului dummy
            free(system112->incidents);
         -trebuie sa eliberam (system112->incidents->description) separat pentru ca am alocat dinamic memorie pentru description la inceputul

III,FUNCTIE MAIN()
    pasul_1:Se alocă dinamic memoria pentru componentele de bază ale sistemului: se creează elementele santinelă (Dummy Nodes) pentru cele două liste dublu înlănțuite și se inițializează cele 4 cozi și stiva de istoric.
    pasul_2:se deschide fișierul tema1.in pentru citire. (cu freopen),si se deschide fisierul tema1.out pentru a scrie rezultate
        f=fopen("tema1.in","r");->deschide tema1.in in read-mode only
        f_out=freopen("tema1.out","w",stdout);->se pune toate ouput-uri in stdout in fisier tema1.out
    a redirecționat automat fluxul standard de ieșire (stdout) către fișierul tema1.out. Astfel, toate apelurile standard de tip printf din funcții scriu direct în fișierul de output
    pasul_3:Se citește numărul total de echipaje, se alocă dinamic vectorul principal units (verificând cu strictețe rezultatul alocării pentru a preveni erori) și se introduc echipajele în coada de disponibilitate.
    pasul_4:Se citește numărul de comenzi, urmat de un bloc for care interpretează și execută fiecare acțiune.
        OBSERVATIE:Pentru comanda ADD_INCIDENT, am folosit o metodă de citire a textului. Deoarece descrierea incidentului conține spații și este încadrată de ghilimele, am utilizat funcția fscanf cu un "format" specific: "%[^\"]".
                    Această expresie ordonă funcției să citească tot fluxul de caractere (inclusiv spațiile) până la întâlnirea următoarei ghilimele, evitând astfel trunchierea textului.
    pasul_5:După încheierea buclei de comenzi, fișierele sunt închise, iar sistemul apelează o serie de funcții dedicate de eliberare a memoriei (free_queue, free_stack, free_intervention, free_incident, free). 
    Memoria este dezalocată in ordine  (întâi referințele din cozi/stive, apoi structurile originale) queue->stack->interventions->incidents.