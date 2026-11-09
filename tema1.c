#include"tema1.h"

//NGUYEN DINH PHONG PHAT -311CC
int main()
{
    struct system system112;
    //initializam dummy node pt lista de incidenti
    system112.incidents=initDummy_incident();
    //initializam dummy node pt lista de interventi
    system112.interventions=initDummy_intervention();
    
    //initilizam 4 cozi si 1 stiva 
    system112.queue_high=(coada)malloc(sizeof(struct cozi));
    system112.queue_high->final=NULL;
    
    system112.queue_medium=(coada)malloc(sizeof(struct cozi));
    system112.queue_medium->final=NULL;
    
    system112.queue_low=(coada)malloc(sizeof(struct cozi));
    system112.queue_low->final=NULL;
    
    system112.queue_available_units=(coada)malloc(sizeof(struct cozi));
    system112.queue_available_units->final=NULL;
    
    system112.dispatch_history=(stiva)malloc(sizeof(struct stive));
    system112.dispatch_history->top=NULL;
    
    FILE *f;
    f=fopen("tema1.in","r");//deschide tema1.in in read-mode only
    if(f==NULL)//verificam daca am deschis bine fisier
    {
        printf("ERROR\n");
        return 1;
    }
    
    FILE *f_out;
    f_out=freopen("tema1.out","w",stdout);//se pune toate ouput-uri in stdout in fisier tema1.out
    if(f_out==NULL)
    {
        printf("ERROR\n");
        return 1;
    }

    int nr_unit;
    fscanf(f,"%d",&nr_unit);//citeste un nr intreg in fisier tema1.in in var n care este nr total de echipe
    //alocare mem pt vector unit cu nr_unit de echipe
    system112.units=(struct unit*)malloc(nr_unit*sizeof(struct unit));
    if(system112.units==NULL)
    {
        printf("ERROR\n");
        return 1;
    }
    for(int i=0;i<nr_unit;i++)
    {
        //citeste datele pentru fiecare echipe
        fscanf(f,"%d %c",&system112.units[i].id,&system112.units[i].type);
        system112.units[i].availability=1;//la inceput toate echipe sunt available
    }
    
    add_unit(&system112,nr_unit);//adaugam fiecare echipa in system112->queue_available_units
    
    int nr_command;
    fscanf(f,"%d",&nr_command);//citeste nr total de comenzi
    
    for(int i=0;i<nr_command;i++)
    {
        char command_titlu[50];
        fscanf(f,"%s",command_titlu);
        if(strcmp(command_titlu,"ADD_INCIDENT")==0)
        {
            int id;
            char priority[10];
            char description[256];
            fscanf(f,"%d %s",&id,priority);
            //după ce citește șirul de caractere pentru priority, f indică spațiul dintre șirul priority și description
            //prin urmare, pentru a obține corect șirul description, trebuie să „înghită”(nuốt) și acel spațiu (să deplaseze f până când întâlnește/indicǎ un caracter diferit de spațiu)
            //iar în acest caz este caracterul "
            fscanf(f," ");
            //\" pasul 1: Cere ca fscanf să citească și să „înghită”(nuốt) și ghilimeaua de deschidere " (fără a o stoca într-o variabilă).
            //[] (Scanset - set de scanare): Când folosesc %[] în loc de %s, imi creez propria regulă de citire a șirului.
            //^ (semn de negație - operator NOT): plasat imediat după [, înseamnă „citește orice ÎN AFARĂ DE...”
            //[^\"]: „alege TOT ce întâlnești în cale (inclusiv litere, cifre și, cel mai important, spații). OPREȘTE-TE DOAR când pointer f ajunge la un caracter ghilimea "
            fscanf(f,"\"%[^\"]\"",description);
            add_incident(&system112,id,priority,description);
        }
        else if(strcmp(command_titlu,"SHOW_INCIDENT")==0)
        {
            int id;
            fscanf(f,"%d",&id);
            show_incident(&system112,id);
        }
        else if(strcmp(command_titlu,"CHECK_UNITS_AVAILABILITY")==0)
        {
            check_units_availability(system112.queue_available_units);
        }
        else if(strcmp(command_titlu,"SHOW_UNIT")==0)
        {
            int id;
            fscanf(f,"%d",&id);
            show_unit(&system112,id,nr_unit);
        }
        else if(strcmp(command_titlu,"DISPATCH")==0)
        {
            dispatch(&system112);
        }
        else if(strcmp(command_titlu,"UNDO_LAST_DISPATCH")==0)
        {
            undo_last_dispatch(&system112);
        }
        else if(strcmp(command_titlu,"SOLVED_INCIDENT")==0)
        {
            int ID_incident;
            fscanf(f,"%d",&ID_incident);
            solved_incident(&system112,ID_incident);
        }
        else if(strcmp(command_titlu,"SHOW_INTERVENTIONS")==0)
        {
            show_interventions(&system112);
        }
    }
    fclose(f);//inchidem fisier tema1.in
    fclose(f_out);//inchidem fisier tema1.out
    
    //eliberam memorie pentru fiecare coada si stiva care am initializat
    free_queue(system112.queue_high);
    free_queue(system112.queue_medium);
    free_queue(system112.queue_low);
    free_queue(system112.queue_available_units);
    
    free_stack(system112.dispatch_history);
    
    //eliberam memorie pt lista de interventi respectiv lista de incidenti
    free_intervention(&system112);
    free_incident(&system112);
    
     //eliberam memorie pt vector de units
    free(system112.units);
}