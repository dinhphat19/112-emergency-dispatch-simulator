//NGUYEN DINH PHONG PHAT-311CC
#ifndef TEMA1_H
#define TEMA1_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct node
{
    struct incident *q_incident;
    struct unit *q_unit;
    struct intervention *s_intervention;
    
    struct node *next;
}cell,*list;

typedef struct cozi
{
    list final;
}*coada;

typedef struct stive
{
    list top;
}*stiva;

struct unit
{
    int id;
    char type;
    int availability;
};

struct incident
{
    int id;
    char priority[7];
    char *description;
    char status[11];
    
    struct incident *next;
    struct incident *prev;
};

struct intervention
{
    struct incident *incident;
    struct unit *unit;
    
    struct intervention *next;
    struct intervention *prev;
};

struct system
{
    struct unit* units;
    struct incident* incidents;
    struct intervention* interventions;

    coada queue_high;
    coada queue_medium;
    coada queue_low;
    coada queue_available_units;
    
    stiva dispatch_history;
};

struct incident* initDummy_incident();
struct intervention* initDummy_intervention();

void enqueue(coada c,struct incident *new_incident);
void dequeue(coada c);
struct incident* first(coada c);
void add_incident(struct system *system112,int id,char *prior,char *descript);

void show_incident(struct system *system112,int ID);
void enqueue_units(coada c,struct unit *new_unit);
void add_unit(struct system *system112,int nr_unit);
void check_units_availability(coada c);
void show_unit(struct system *system112,int ID,int nr_unit);

void push_stack(stiva s,struct intervention *new_intervention);
void pop_stack(stiva s);
struct intervention* top_stack(stiva s);
struct unit* first_unit(coada c);
void dispatch(struct system *system112);

void enqueue_first(coada c,struct incident *new_incident);
void undo_last_dispatch(struct system *system112);
void solved_incident(struct system *system112,int ID_incident);

void show_interventions(struct system *system112);

void free_queue(coada c);
void free_stack(stiva s);
void free_intervention(struct system *system112);
void free_incident(struct system *system112);
#endif