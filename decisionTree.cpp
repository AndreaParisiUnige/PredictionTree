#include "decisionTree.h"
#include "list.h"

using namespace d_Tree;

struct d_Tree::decisionArch {
    string var;  //variabile su cui effettuo il confronto
    string operand; //operatore 
    string type; //qualtitativo o quantitativo
};

//l'albero di decisione viene implementato mediante albero generico con struttura primo figlio-prossimo fratello
struct d_Tree::Node {
    Label info;
    decisionArch condition ;    //specifica la condizione per arrivare a tale nodo, partendo dal padre.
    Node *firstChild;
    Node *nextSibling;
};


/*******************************************************************************************/
/**************************************FUNZIONI AUSILIARIE**********************************/
/*******************************************************************************************/

// crea un nodo con etichetta l, e restituisce il puntatore ad esso
decisionTree createNode(const Label l, const decisionArch a)
{ 
    decisionTree t = new Node;
    t->info = l;
    t->condition = a;
    t->firstChild = t->nextSibling = emptyTree;
    return t;
};

// restituisce il puntatore al nodo dell'albero t, etichettato con la Label l
decisionTree getNode(const Label l, const decisionTree& t)
{ 
    if (isEmpty(t) || l==emptyLabel)   // caso albero o etichetta vuoti
        return emptyTree;

    if (t->info == l)  // caso etichetta uguale a quella ricercata
        return t;

    //Chiamata ricorsiva di getNode su ciascuno dei figli di t, finché una delle chiamate non restituisce un valore diverso da emptyTree
    decisionTree auxT = t->firstChild;
    decisionTree resNode;
    while (auxT != emptyTree){
        resNode = getNode(l, auxT);
        if (resNode == emptyTree) // non ho trovato cercando in questo sottoalbero, devo proseguire la scansione dei fratelli 
           auxT = auxT->nextSibling;
        else                      // ho trovato: lo restituisco
           return resNode;
    }   
    return emptyTree; // se arrivo fino a qui, vuole dire che al termine di una ricerca esaustiva nell'albero il nodo non è stato trovato 
};

//restituisce true se il nodo puntato da t ha un figlio con l'etichetta l
bool hasChildWithLabel(const Label l, const decisionTree& t)
{
    if (isEmpty(t)) return false;

    decisionTree child = t->firstChild;
    while (!isEmpty(child)){
        if (child->info == l) 
            return true;
        else 
            child = child->nextSibling;
    }
    return false;
}

//analizza una stringa e stabilisce se si tratta di un numero
bool isNumber(const string& str)
{
    for(int i=0; i<str.length(); i++)
        if(!isdigit(str[i]))
            return false;
    return true;
}

//controlla la dimensione della condizione, con la relativa variabile
//dev'essere necessariamente >=2, se vi è un solo operatore dev'esserci almeno un altro carattere con cui effettuare un confronto
Error controlDim(const string str)
{
    int dim=str.length();
    if(dim<2)   
        return d_Tree::FAIL;
    return OK;
}

//ritorna FAIL se il char della condizione non rientra tra quelli ammessi, OK altrimenti
Error controlCondition(const char c)
{
    if(c==33 || (c<60 || c>62))    //sono accettati solo {=, !=, <, >, <=, >=}, verifico mediante codifica ASCII
        return d_Tree::FAIL;
    return d_Tree::OK;
}

//decodifica la condizione, inserendo i dati e restituendo la struct decisionArch con i relativi valori
decisionArch insertCondition(const string& str)
{
    decisionArch a;
    string strAux=str;    //copia per eventuali modifiche

    if( strAux == emptyCond ){ //questo caso si verifica di norma alla radice
        a.var=emptyCond;    //assegno valori simbolici, significano che non è presente alcuna condizione per tale nodo
        a.operand=emptyCond;
        return a;
    }
    //controllo i primi 2 char, costituiscono il possibile operatore
    //per valori quali {!,=,<,>}, inserisco direttamente nel campo operand
    //esclude tutti i casi in cui tali valori sono posizionati al secondo posto nella stringa, tranne nel caso dell'=
    //in tal modo includo anche {!=,<=,>=}
    for(int i=0; i<2 && controlCondition(strAux[i])==d_Tree::OK ;i++){
        switch(strAux[i]){
            case '!':
            if(i==0)
                a.operand=strAux[i];
            break;

            case '<':
            if(i==0)
                a.operand=strAux[i];
            break;

            case '>':
            if(i==0)
                a.operand=strAux[i];
            break;

            case '=':
            if(i==0)
                a.operand=strAux[i];
            else
                a.operand.push_back(strAux[i]);
            break;
        }
    }
    //riempio il campo variabile della struct, con i restanti caratteri
    a.var=strAux.substr(a.operand.length());
    if(isNumber(a.var))
        a.type = "quantitativo";
    else 
        a.type = "qualitativo";
    return a;
}

int depth(const decisionTree& t){
    if(t==emptyTree)
        return 0;
    if(t->firstChild != emptyTree)
        return depth(t->firstChild)+1;
    else if(t->firstChild==emptyTree && t->nextSibling!=emptyTree)
        depth(t->nextSibling);
    return 0;
}

//rimuove dalla label tutto ciò che si trova dopo il simbolo "_"
Label semplifyLabel(const Label& l){
    Label aux=l;

    for(int i=0; i<aux.length(); i++){
        if(aux[i]=='_')
            aux.erase(i, aux.length());
    }
    return aux;
}

//Funzione ausiliaria per gestire la stampa delle variabili
void print_dtAux(const decisionTree& t, liste::List& l)
{
    if (isEmpty(t)) 
        return;
    if(!liste::member(semplifyLabel(t->info), l) && t->firstChild!=emptyTree)
        liste::addCell(semplifyLabel(t->info), l);      

    decisionTree auxT = t->firstChild;
    while (!isEmpty(auxT)) {
        print_dtAux(auxT, l);
        auxT = auxT->nextSibling;
    }
};

/****************************************************************************************************/
/*****************************************FUNZIONI DEL TDD*******************************************/
/****************************************************************************************************/

//restituisce un albero decisionale vuoto
decisionTree d_Tree::createEmpty()
{
    return emptyTree;
};

// restituisce true se l'albero decisionale è vuoto, false altrimenti
bool d_Tree::isEmpty(const decisionTree& t)
{ 
   return (t==emptyTree);
};

//restituisce true se il nodo etichettato con la label l appartiene all'albero t e false altrimenti
bool d_Tree::member(const Label l, const decisionTree& t)
{ 
    if (isEmpty(t)) // se l'albero e' vuoto, restituisco false
        return false;
    if (t->info == l) //etichetta trovata, restituisco true
        return true;

    //Chiamata ricorsiva di member su ciascuno dei figli di t, finché una delle chiamate non restituisce un valore diverso da false
    decisionTree auxT = t->firstChild;
    while (auxT != emptyTree) {
        if (!member(l, auxT)) // non ho trovato cercando in questo sottoalbero, devo proseguire la scansione dei fratelli 
           auxT = auxT->nextSibling;
        else                      // ho trovato: restituisco true
           return true;
    }   
    return false; // se arrivo fino a qui, vuole dire che al termine di una ricerca esaustiva nell'albero il nodo non è stato trovato 
}

//aggiunge un nodo etichettato labelOfNodeToAdd, come figlio di labelOfNodeInTree, non sono ammessi duplicati
Error d_Tree::addNode(const Label labelOfNodeInTree, const Label labelOfNodeToAdd, string cond, decisionTree& t)
{
    decisionArch a = insertCondition(cond);  //la Add riceve una condizione come stringa da input, la decodifca inserendo nella relativa struct i valori

    if ((labelOfNodeInTree == emptyLabel) && isEmpty(t)){ // labelOfNodeInTree è l'etichetta vuota e l'albero è vuoto
        t = createNode(labelOfNodeToAdd, a);  //attribuisco una radice all'albero precedentemente vuoto
        return OK;                         
    }

    if (member(labelOfNodeToAdd, t)) // nell'albero esiste già un nodo con etichetta labelOfNodeToAdd
       return FAIL; 

    decisionTree auxT = getNode(labelOfNodeInTree, t); // recupero il puntatore al nodo dell'albero che ha etichetta labelOfNodeInTree

    if (auxT == emptyTree)  // nell'albero non esiste un nodo con etichetta labelOfNodeInTree
        return FAIL; 
    else{ // ho trovato il nodo auxT a cui aggiungere il figlio
        decisionTree child = createNode(labelOfNodeToAdd, a); // creo un nodo child con l'etichetta labelOfNodeToAdd
        child->nextSibling = auxT->firstChild;     // il primo fratello di child sarà quello che era il primo figlio di auxT
        auxT->firstChild = child;                  // child diventa il primo figlio di auxT
    }
    return OK;
};

//FUNZIONI AUSILIARIE PER IMPLEMENTARE LA DELETE

// deleteChild: funzione ausiliaria, viene chiamata solo se si e' verificato che il nodo etichettato con l e' uno dei figli di t
void deleteChild(const Label l, decisionTree& t)
{
    decisionTree auxT = t->firstChild;    // so che il firstChild c'é perché chiamo deleteChild solo se ho già verificato qs condizione
    decisionTree prev = createEmpty();    // il prev all'inizio e' vuoto
    while(auxT->info != l){
        prev = auxT;
        auxT = auxT->nextSibling;
    }
    // quando esco da questo while, auxT punta al nodo da cancellare
    decisionTree lastSibling = auxT;

    while(!isEmpty(lastSibling->nextSibling))
        lastSibling = lastSibling->nextSibling;
    // quando esco da questo while, lastSibling punta al fratello più a destra del nodo da cancellare
  
    lastSibling->nextSibling = auxT->firstChild; // attacco i figli del nodo da cancellare al suo ultimo fratello

    if (isEmpty(prev))  // se non c'é nessun fratello precedente, il nodo da rimuovere e' il primo: devo cambiare il puntatore al firstChild nel padre 
        t->firstChild = (t->firstChild)->nextSibling;
    else  // altrimenti, devo "saltare" il nodo da rimuovere nella catena dei fratelli
        prev->nextSibling = auxT->nextSibling;
    
    delete auxT; // in ogni caso, alla fine dealloco il nodo da rimuovere        
}

//funzione ausiliaria, rimuove dall'albero il nodo etichettato con la Label l, se esiste
Error deleteElemAux(const Label l, decisionTree& t)
{
    if (isEmpty(t)) 
        return FAIL;  // se t e' vuoto non c'e' niente da cancellare

    if (hasChildWithLabel(l, t)){  // se t è il padre del nodo da rimuovere    
        deleteChild(l, t);         // rimuovo il figlio di t etichiettato con l
        return OK;                 // e restituisco OK
    }
   
    decisionTree child = t->firstChild;   // altrimenti richiamo ricorsivamente sui figli di t finche' o cancello, o non ci sono piu' figli da esplorare
    while (!isEmpty(child)){
        if (deleteElemAux(l, child) == OK)
            return OK;
        else 
            child = child->nextSibling;
    }
    return FAIL;
}

// deleteElem (versione ricorsiva) rimuove dall'albero il nodo etichettato con la Label l
// e collega al padre di tale nodo tutti i suoi figli
// Restituisce FAIL se si tenta di cancellare la radice e questa ha 
// dei figli (non si saprebbe a che padre attaccarli) oppure se non esiste 
// un nodo nell'albero etichettato con la Label; cancella e restituisce OK altrimenti
Error d_Tree::deleteNode(const Label l, decisionTree& t)
{
    if(!isEmpty(t) && t->info == l){ // il nodo da rimuovere e' la radice; si puo' rimuovere solo se non ha figli
        if(t->firstChild == emptyTree){ // posso rimuovere la radice solo se non ha figli, nel qual caso
            delete t;        // dealloco
            t = emptyTree;   // e t diventa l'albero vuoto
        }
        else 
            return FAIL;   // altrimenti non posso rimuoverla e restituisco FAIL
    }
      
    return deleteElemAux(l, t);  // se sono arrivato fino a qui senza uscire dalla funzione, chiamo la funzione ausiliaria, che non fa i controlli relativi al caso "radice"
};

Error d_Tree::set(const Label l1, const Label l2, const string cond, decisionTree& t)
{
    if(!member(l1, t))
        return FAIL;

    decisionTree auxT=getNode(l1,t);
    auxT->info=l2;

    if(auxT->condition.operand != emptyCond){
        decisionArch a=insertCondition(cond);   //inserisco la nuova condizione solo se non sto oeprando sulla radice
        auxT->condition=a;
    }
    return OK;
};

Error d_Tree::makePrediction(decisionTree& t)
{
    decisionTree auxT=t;    //albero ausiliario per muovermi tra i nodi
    string val;
    bool found;
    decisionTree child=t;     //serve per mantenere un puntatore al nodo su cui opero

    while(auxT->firstChild->firstChild!=emptyTree && child!=emptyTree){ //ciclo su tutta la profondità dell'albero
        cout << semplifyLabel(auxT->info) << ":";
        cin >> val;
        child=auxT->firstChild;    
        do{
            found=false;
            if(child->condition.operand == "="){
                if(child->condition.var==val){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == ">"){
                if(val > child->condition.var){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == "<"){
                if(val < child->condition.var){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == "!="){
                if(child->condition.var != val){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == ">="){
                if(val >= child->condition.var ){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == "<="){
                if(val <= child->condition.var){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
        }
        while(!found && child!=emptyTree);
    }
    if(child == emptyTree)  
        cout << "La predizione non può avere luogo in esiste un nodo per il quale non c'è un arco percorribile.\n";
    else
        cout << semplifyLabel(child->info) << ": " << child->firstChild->condition.var << endl;
    
    return OK;
}


Error d_Tree::makePredictionSet(decisionTree& t)
{
    decisionTree auxT=t;
    string val;
    bool found;
    decisionTree child=t;
    int cont=0;

    liste::List l=liste::createEmpty();   //utilizzo una lista per salvare le variabili dell'albero
    print_dtAux(t,l);   //memorizza nella lista tutte le variabili

    liste::List auxList=l;
    while(auxList->next!=auxList->prev){
        cout << auxList->next->info;
        liste::deleteCell(auxList);
        cout << " ";
        cont++;
    }
    cout << endl;

    string predictionVar = liste::deleteCell(l);    //prelevo l'elemento per cui non è richiesto inserimento da input

    for(int i=cont; i>0; i--){  //inserisco nella lista i valori letti da input
        cin >> val;
        liste::addCell(val, l);
    }
        
    while(auxT->firstChild->firstChild!=emptyTree && child!=emptyTree && !liste::isEmpty(l)){ 
        child=auxT->firstChild; 
        found=false;
        if(found==false)
            val=liste::deleteCell(l);
        do{
            if(child->condition.operand == "="){
                if(child->condition.var==val){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == ">"){
                if(val > child->condition.var){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == "<"){
                if(val < child->condition.var){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == "!="){
                if(child->condition.var != val){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == ">="){
                if(val >= child->condition.var ){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
            else if(child->condition.operand == "<="){
                if(val <= child->condition.var){
                    auxT=child;
                    found=true;
                }
                else
                    child=child->nextSibling;
            }
        }
        while(!found && child!=emptyTree);
    }

    if(child == emptyTree)  
        cout << "La predizione non può avere luogo in esiste un nodo per il quale non c'è un arco percorribile.\n";
    else
        cout << predictionVar << ": " << child->firstChild->condition.var << endl;
    
    return OK;    
};


/**********************************FUNZIONI PER INPUT ED OUTPUT************************************/

// Funzione ausiliaria per la visualizzazione strutturata
void printTree(const decisionTree& t, int depth) 
{
    if (isEmpty(t)) return;
    string indent = "--";
    // Inserisco indentazione corrispondente alla profondita' raggiunta
    for (int i=0; i<depth; i++)
        cout << indent;

    // Visualizzo il contenuto informativo associato a t
    cout << "(" << t->info ;
    if(t->condition.operand == emptyCond)
        cout<< ")" << endl;
    else
        cout << "," << t->condition.operand << t->condition.var << ")" << endl;

    // Chiamata ricorsiva di printTree su ciascuno dei figli di t (profondita' incrementata di 1)
    decisionTree auxT = t->firstChild;
    while (!isEmpty(auxT)) {
        printTree(auxT, depth+1);
        auxT = auxT->nextSibling;
    }
}

void print(const d_Tree::decisionTree& t)
{
    printTree(t, 0);
};

void d_Tree::print_dtVariables(const d_Tree::decisionTree& t)
{
    if(d_Tree::isEmpty(t)){ //caso albero vuoto 
        cout << "L'albero risulta vuoto\n";
        return;
    }       

    liste::List l=liste::createEmpty();   //utilizzo una coda per salvare le variabili dell'albero
    print_dtAux(t,l);

    string e;
    while(!liste::isEmpty(l)){
        e=liste::deleteCell(l);
        cout << e << " " ;
    }
    cout << endl;
};  

d_Tree::decisionTree readFromStream(istream& str, d_Tree::decisionTree& t)
{
    string line;
    Label rootLabel, fatherLabel, childLabel;
    string cond;
    getline(str, line);  
    istringstream instream;            // uso una variabile di tipo istringstream per poter scandire i pezzi di ogni riga usando >>
    instream.clear();
    instream.str(line);
    instream >> rootLabel;             // il primo elemento che si incontra nel file e' l'etichetta della radice, per convenzione su come deve essere fatto il file
    addNode(emptyLabel, rootLabel, emptyCond, t); // la si inserisce nell'albero vuoto, indicando che il padre non c'e' (primo argomento emptyLabel)
    getline(str, line);                // poi si iniziano a scansionare le righe seguenti
    instream.clear();
    instream.str(line);
    while (!str.eof()){        
	    instream >> fatherLabel; // in ogni riga del file, il primo elemento e' l'etichetta del nodo padre e gli altri sono le etichette dei figli
            while (!instream.eof()){             // finche' la riga corrente non e' terminata           
                instream >> childLabel;              // leggo la prossima etichetta
                instream >> cond;   // leggo la condizione                                 
                d_Tree::addNode(fatherLabel, childLabel, cond, t);               
            }
        getline(str, line);  //per il corretto funzionamento è necessario che il file di testo si concluda in una nuova riga vuota
        instream.clear();
        instream.str(line);
    }
    str.clear();
    return t;
}

d_Tree::decisionTree readFromFile(string nome_File, d_Tree::decisionTree& t)
{
    ifstream ifs(nome_File.c_str()); // apertura di uno stream associato ad un file, in lettura
    if (!ifs){
        cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n"; 
        return createEmpty();
    }  
    return readFromStream(ifs, t);
};

