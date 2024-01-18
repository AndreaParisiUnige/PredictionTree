#include "list.h"

using namespace liste;

//TDD coda, implementato mediante liste doppiamente collegate, con inserimento in coda e rimozione in testa

//crea lista vuota con sentinella
List liste::createEmpty(){
    List l = new cell;
    l->next = l;
    l->prev = l;
    return l;
};

bool liste::isEmpty(const List& l){
    return l->next == l;
}

void liste::addCell(Elem e, List& l){
    List aux = l->prev;
    List temp = new cell;
    temp->info = e;
    temp->next = l;
    temp->prev = aux;
    aux->next = temp;
    l->prev = temp;
};

Elem liste::deleteCell(List& l){
    List aux = l->next;
    Elem e = aux->info;
    aux->prev->next = aux->next;
    aux->next->prev = aux->prev;
    delete aux;
    return e;
};

bool liste::member(Elem e, const List& l)
{
    if(isEmpty(l))
        return false;

    List aux=l->next;

    while(aux!=l){
        if(aux->info == e)
            return true;
        aux=aux->next;
    }
    return false;
};