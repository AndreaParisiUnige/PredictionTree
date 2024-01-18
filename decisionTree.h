#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;

namespace d_Tree{

    enum Error {OK, FAIL};

    typedef string Label;  

    const Label emptyLabel = "&#&#&";
    const string emptyCond = "***"; //non è presente alcuna condizione per un nodo che possiede questo valore nei campi della struct

    struct decisionArch;    //contiene la condizione da soddisfare, per passare attraverso l'arco che connette un padre ad un figlio
    struct Node;    //definita nel file desionTree.cpp

    typedef Node* decisionTree; //l'albero viene identificato tramite un puntatore alla radice

    const decisionTree emptyTree = NULL;

    decisionTree createEmpty();
    bool isEmpty(const decisionTree&);
    bool member(const Label , const decisionTree& );
    void print_dtVariables(const d_Tree::decisionTree&);
    Error addNode(const Label, const Label, string, decisionTree&); //aggiunta nodo, collegato come figlio della seconda "Label"
    Error deleteNode(const Label, decisionTree&);  //cancella un nodo avente la Label passata come parametro
    Error set(const Label, const Label, const string, decisionTree&); //modifica un nodo, sostituendo la sua Label con la seconda Label ricevuta come parametro
    Error makePrediction(decisionTree&);    //predizione effettuata sulla base dell'albero di decisione letto da file, e su valori presi in input uno alla volta
    Error makePredictionSet(decisionTree&); //predizione effettuata sulla base dell'albero di decisione letto da file, e su un insieme di valori presi in input 
}

//funzioni di lettura da file e stampa (inserite fuori dal namespace perchè non caratterizzano il TDD)
d_Tree::decisionTree readFromFile(string, d_Tree::decisionTree&);
void print(const d_Tree::decisionTree&);