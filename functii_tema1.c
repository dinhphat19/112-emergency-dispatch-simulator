#include"tema1.h"

//NGUYEN DINH PHONG PHAT -311CC
struct incident* initDummy_incident()
{
    struct incident *incident0=(struct incident*)malloc(sizeof(struct incident));
    if(incident0==NULL)
        return NULL;
    incident0->id=0;
    strcpy(incident0->priority,"low");
    incident0->description=(char*)malloc((strlen("test incident")+1)*sizeof(char));
    if(incident0->description==NULL)
        return NULL;
    strcpy(incident0->description,"test incident");
    strcpy(incident0->status,"solved");
    incident0->next=incident0;
    incident0->prev=incident0;
    return incident0;
}

struct intervention* initDummy_intervention()
{
    struct intervention *intervention0=(struct intervention*)malloc(sizeof(struct intervention));
    if(intervention0==NULL)
        return NULL;
    intervention0->incident=NULL;
    intervention0->unit=NULL;
    intervention0->next=intervention0;
    intervention0->prev=intervention0;
    return intervention0;
}


void enqueue(coada c,struct incident *new_incident)
{
    if(c==NULL)
        return;
    list new=(list)malloc(sizeof(cell));//alocare mem pt un nod nou
    if(new==NULL)
        return;
    new->q_incident=new_incident;
    if(c->final==NULL)
    {
        c->final=new;
        c->final->next=c->final;
        return;
    }
    new->next=c->final->next;// 1.new->next indica la head(primul nod)
    c->final->next=new;// 2. tail vechi (c->final) indica la new 
    c->final=new;// 3. acum tail este new care tocmai am adaugat
}

void dequeue(coada c)
{
    if(c==NULL || c->final==NULL)//verifica daca este disponibil coada
        return;
    if(c->final->next==c->final)//daca coada are doar un element
    {
        list del=c->final;
        c->final=NULL;
        free(del);
        return;
    }
    list del=c->final->next;//sterge primul element adaugat in coada(FIFO)
    c->final->next=c->final->next->next;
    free(del);
}

struct incident* first(coada c)
{
    if(c==NULL || c->final==NULL)
        return NULL;
    return c->final->next->q_incident;
}


void add_incident(struct system *system112,int id,char *prior,char *descript)
{
    //alocare memorie pt un incident nou
    struct incident *new_incident=(struct incident*)malloc(sizeof(struct incident));
    if(new_incident==NULL)
        return;
    //completeaza infomatiile pt incidentul nou(id,priority,description)
    new_incident->id=id;
    strcpy(new_incident->priority,prior);
    //alocare mem pt sirul de description
    new_incident->description=(char*)malloc((strlen(descript)+1)*sizeof(char));
    if(new_incident->description==NULL)
        return;
    strcpy(new_incident->description,descript);
    strcpy(new_incident->status,"queued");
    if(system112->incidents==NULL)
        return;
    //algoritmul de adaugare new_incident in "sfarsitului" lista de incident
    struct incident *dummy=system112->incidents;//node dummy
    struct incident *last=dummy->prev;//ultimul nod
    new_incident->next=dummy;
    last->next=new_incident;
    new_incident->prev=last;
    dummy->prev=new_incident;
    
    //depinde de nivel de prioritate, il adaugam in coada corespunzator
    if(strcmp(new_incident->priority,"high")==0)
    {
        enqueue(system112->queue_high,new_incident);
    }
    else if(strcmp(new_incident->priority,"medium")==0)
        enqueue(system112->queue_medium,new_incident);
    else if(strcmp(new_incident->priority,"low")==0)
        enqueue(system112->queue_low,new_incident);
}

void show_incident(struct system *system112,int ID)
{
    if(system112->incidents==NULL)//exit daca lista de incident e NULL
        return;
    int ok=0;
    struct incident *temp=system112->incidents->next;
    do
    {
       if(temp->id==ID)
       {
            ok=1;
            printf("Incident %d has %s priority, the following description: \"%s\" and is %s\n",temp->id,temp->priority,temp->description,temp->status);
            return;
       }
       temp=temp->next;
    }while(temp!=system112->incidents);
    if(ok==0)
    {
        printf("INVALID OPERATION! ERROR 404\n");
    }
}


void enqueue_units(coada c,struct unit *new_unit)
{
    //functie pentru a adauga new nod in coada de units(system112->queue_available_units)
    if(c==NULL)
        return;
    list new=(list)malloc(sizeof(cell));
    if(new==NULL)
        return;
    new->q_unit=new_unit;
    if(c->final==NULL)
    {
        c->final=new;
        c->final->next=c->final;
        return;
    }
    new->next=c->final->next;// 1.new->next indica la head(primul nod)
    c->final->next=new;// 2. tail vechi (c->final) indica la new 
    c->final=new;// 3. acum tail este new care tocmai am adaugat
}

void add_unit(struct system *system112,int nr_unit)
{
    //parcurgem de la 0 pana la nr_unit-1 si adaugam fiecare echipa in coada
    for(int i=0;i<nr_unit;i++)
    {
        enqueue_units(system112->queue_available_units,&system112->units[i]);
    }
}


void check_units_availability(coada c)
{
    if(c==NULL || c->final==NULL)//cazul invalid
    {
        printf("Number of available units: 0\n");
        return;
    }
    int nr_unit_avail=0;
    list temp=c->final->next;
    do{
        nr_unit_avail++;//val total de echipe available in coada
        temp=temp->next;
    }while(temp!=c->final->next);
    printf("Number of available units: %d\n",nr_unit_avail);
}


void show_unit(struct system *system112,int ID,int nr_unit)
{
    if(system112->units==NULL)
        return;
    int ok=0;
    for(int i=0;i<nr_unit;i++)
    {
        if(system112->units[i].id==ID && system112->units[i].availability==1)
        {
            ok=1;
            printf("Unit %d is type %c and is available\n",system112->units[i].id,system112->units[i].type);
        }
        else if(system112->units[i].id==ID && system112->units[i].availability==0)
        {
            //daca am gasit id-ul dar nu este available echipa, atunci afisam unavailable
            ok=1;
            printf("Unit %d is type %c and is unavailable\n",system112->units[i].id,system112->units[i].type);
        }
    }
    if(ok==0)//daca e invalid id
    {
        printf("INVALID OPERATION! ERROR 404\n");
    }
}


void push_stack(stiva s,struct intervention *new_intervention)
{
    if(s==NULL)
        return;
    list new=(list)malloc(sizeof(cell));
    if(new==NULL)
        return;
    new->s_intervention=new_intervention;
    new->next=s->top;
    s->top=new;
}

void pop_stack(stiva s)
{
    if(s==NULL || s->top==NULL)
        return;
    list del=s->top;
    s->top=s->top->next;
    free(del);
}

struct intervention* top_stack(stiva s)
{
    if(s==NULL || s->top==NULL)
        return NULL;
    return s->top->s_intervention;
}

struct unit* first_unit(coada c)
{
    if(c==NULL || c->final==NULL)
        return NULL;
    return c->final->next->q_unit;
}


void dispatch(struct system *system112)
{
    //verificare daca e valid lista//exista element in lista
    if(system112->incidents==NULL || system112->incidents->next==system112->incidents)
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }
    //cautam prima echipa available
    //1. Kiểm tra xem có xe rảnh không (Queue có rỗng ko)
    if(system112->queue_available_units==NULL || system112->queue_available_units->final==NULL)
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }
    //verificam pt fiecare coada bazata de prioritate (high->medium->low)
    coada c=NULL;
    if(system112->queue_high->final!=NULL)
    {
        c=system112->queue_high;
    }
    else if(system112->queue_medium->final!=NULL)
    {
        c=system112->queue_medium;
    }
    else if(system112->queue_low->final!=NULL)
    {
        c=system112->queue_low;
    }
    if(c==NULL)//daca e nuLL -> toate 3 cozi sunt goale
        return;
    coada c_unit=system112->queue_available_units;
    struct unit *temp=first_unit(c_unit);//scoate primul echipa available in queue_available_units
    dequeue(c_unit);
    struct incident *temp1=first(c);////scoate primul incident in queue c
    dequeue(c);
    //face modificari pe  incident status si unit availibility , dupa adauga new_intervention in lista de interventions
    struct intervention *new_intervention=(struct intervention*)malloc(sizeof(struct intervention));
    if(new_intervention==NULL)
        return;
    new_intervention->incident=temp1;
    strcpy(new_intervention->incident->status,"intervened");
    new_intervention->unit=temp;
    new_intervention->unit->availability=0;

    //algoritmul de adaugare new_intervention in lista de interventi
    struct intervention *dummy1=system112->interventions;
    struct intervention *last=dummy1->prev;
    new_intervention->next=dummy1;
    last->next=new_intervention;
    new_intervention->prev=last;
    dummy1->prev=new_intervention;

    //adaugam new_intervention in stiva dispatch_history pt functii urmatoare
    push_stack(system112->dispatch_history,new_intervention);
}


void enqueue_first(coada c,struct incident *new_incident)//adauga new element la INCEPUTUL coada
{
    if(c==NULL)
        return;
    list new=(list)malloc(sizeof(cell));
    if(new==NULL)
        return;
    new->q_incident=new_incident;
    if(c->final==NULL)
    {
        c->final=new;
        c->final->next=c->final;
        return;
    }
    new->next=c->final->next;// 1. Noul nod pointează către Head (primul element)
    c->final->next=new;// 2. Tail-ul vechi pointează către noul nod
    // 3. NU se actualizează pointerul Tail la noul nod
}


void undo_last_dispatch(struct system *system112)
{
    //daca nu este initializat lista de interventi sau nu are element
    if(system112->interventions==NULL || system112->interventions->next==system112->interventions)
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }
     //daca nu este initializat stiva de dispatch history sau nu are element
    if(system112->dispatch_history==NULL || system112->dispatch_history->top==NULL)
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }
    stiva s=system112->dispatch_history;
    struct intervention* temp=top_stack(s);//temp=elementul pe varfului stiva
    while(temp!=NULL && strcmp(temp->incident->status,"intervened")!=0)//loop pana cand se gaseste o interventie cu incident de status "intervened"
    {
        pop_stack(s);
        temp=top_stack(s);
    }
    if(temp==NULL)//daca nu exista 
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }

    //modificam status
    strcpy(temp->incident->status,"queued");

    //depinde de prioritate, adaugam inapoi incident la inceputul coada de incident
    if(strcmp(temp->incident->priority,"high")==0)
    {
        enqueue_first(system112->queue_high,temp->incident);
    }
    else if(strcmp(temp->incident->priority,"medium")==0)
        enqueue_first(system112->queue_medium,temp->incident);
    else if(strcmp(temp->incident->priority,"low")==0)
        enqueue_first(system112->queue_low,temp->incident);

    //modificam unit availibility=>devine available
    temp->unit->availability=1;
    //o adauga inapoi in sfarsitul coada de unit
    enqueue_units(system112->queue_available_units,temp->unit);
    
    //eliberam nodeul "temp" in lista de interventii
    struct intervention *stanga=temp->prev;
    struct intervention *dreapta=temp->next;
    stanga->next=dreapta;
    dreapta->prev=stanga;
    pop_stack(s);//scoatem cazul de interventi care tocmai am lucrat afara
    free(temp);
}


void solved_incident(struct system *system112,int ID_incident)
{
    //cazul care nu exista element in lista de interventi sau nu este initializata 
    if(system112->interventions==NULL || system112->interventions->next==system112->interventions)
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }
    struct intervention *list_intervention=system112->interventions->next;
    int ok=0;
    while(list_intervention!=system112->interventions)
    {
        if(list_intervention->incident->id==ID_incident)
        {
            ok=1;
            break;
        }
        list_intervention=list_intervention->next;
    }

    //daca ID_incident este invalid sau incidentul gasit nu are status"intervened"
    if(ok==0 || strcmp(list_intervention->incident->status,"intervened")!=0)
    {
        printf("INVALID OPERATION! ERROR 404\n");
        return;
    }

    //modificam status
    strcpy(list_intervention->incident->status,"solved");
    //modificam unit availability
    list_intervention->unit->availability=1;
    //adauga echipa in interventie asta inapoi in coada de unit queue_available_units
    enqueue_units(system112->queue_available_units,list_intervention->unit);
}


void show_interventions(struct system *system112)
{
    if(system112->interventions==NULL || system112->interventions->next==system112->interventions)
    {
        printf("No intervention has been initiated\n");
        return;
    }
    struct intervention *list_intervention=system112->interventions->next;
    while(list_intervention!=system112->interventions)
    {
        printf("Incident %d was assigned to unit %d, and has the following status: \"%s\"\n",list_intervention->incident->id,list_intervention->unit->id,list_intervention->incident->status);
        list_intervention=list_intervention->next;
    }
}

void free_queue(coada c)
{
    if(c==NULL)
        return;
    while(c->final!=NULL)
    {
        dequeue(c);
    }
    free(c);
}

void free_stack(stiva s)
{
    if(s==NULL)
        return;
    while(s->top!=NULL)
    {
        pop_stack(s);
    }
    free(s);
}

void free_intervention(struct system *system112)
{
    // 1. Dacă nu a fost inițializat niciodată, nu este nevoie de free
    if(system112->interventions==NULL)
        return;
    // 2. Începe de la nodul real primul (imediat după nodul Dummy)
    struct intervention *list_intervention=system112->interventions->next;
    while(list_intervention!=system112->interventions)
    {
        struct intervention *temp=list_intervention;
        list_intervention=list_intervention->next;
        free(temp);
    }
    // 4. Când bucla se termină,lis_intervention s-a întors la nodul Dummy
    //În acest moment, toate nodurile reale au fost eliminate, rămâne doar nodul Dummy
    free(system112->interventions);
    system112->interventions=NULL;
}

void free_incident(struct system *system112)
{
    if(system112->incidents==NULL)
        return;
    struct incident *list_incident=system112->incidents->next;
    while(list_incident!=system112->incidents)
    {
        struct incident *temp=list_incident;
        list_incident=list_incident->next;
        free(temp->description);// Eliberează memoria pe care am alocat-o pentru description-ul nodurilor in lista
        free(temp);
    }
    free(system112->incidents->description);// Eliberează memoria pe care am alocat-o pentru description-ul nodului dummy
    free(system112->incidents);
    system112->incidents=NULL;
}
