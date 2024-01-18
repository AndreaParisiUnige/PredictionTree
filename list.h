#include <iostream>
#include <string>

using namespace std;

namespace liste{

    typedef string Elem;

    struct cell {
    Elem info;
    cell *next;
    cell *prev;
    };

    typedef cell *List; //l'albero viene identificato tramite un puntatore alla radice
    
    List createEmpty();
    bool isEmpty(const List&);
    void addCell(Elem, List&);
    Elem deleteCell(List&);
    bool member(Elem, const List&);
}