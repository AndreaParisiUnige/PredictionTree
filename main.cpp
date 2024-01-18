#include <iostream>
#include <string>
#include "decisionTree.h" 
#include "list.h"
using namespace std;

int main(){
   char ch;  //variabile utilizzata per la scelta nel menù
   string nomeFile;  //file da cui leggere input
   d_Tree::Label l1,l2; //variabili utilizzate per raccogliere le label in input
   d_Tree::decisionTree tree=d_Tree::createEmpty();   //creazione albero decisionale vuoto
   string cond; 

   do
   {  //stampa menù
      cout<<"--------------------------------------------------------------------\n";
      cout<<"                                MENU                                \n";
      cout<<"--------------------------------------------------------------------\n";
      cout<<"1. Lettura albero di decisione da file;\n2. Inserimento di un nodo etichettato labelFiglio attaccato a un padre etichettato labelPadre;\n3. Cancellazione di un nodo dell'albero;\n4. Modifica di un nodo dell'albero;\n5. Visualizzazione dell'albero di decisione;\n6. Stampa variabili dell'albero di decisione;\n7. Effettua predizione inserendo i valori uno alla volta;\n8. Effettua predizione inserendo tutti i valori all'inizio;\n0. Uscita;\n\nFornisci la tua scelta ---> ";

      do
      {
         cin>>ch; //inserimento del valore relativo alla scelta dell'operazione da effettuare nel menù
         if(ch<'0' || ch>'8')  //ripeto la richiesta finchè non viene inserito un valore accettabile
            cout<<"\nPuoi inserire soltanto interi compresi tra 0 e 8!\nInserisci un valore consentito: ";
      }
      while(ch<'0' || ch>'8');

      switch(ch)
      {
         case '1':     //lettura da file
         cout<<"\nInserisci il nome del file da cui vuoi leggere l'albero di decisione:\n";
         cin>>nomeFile;
         tree=readFromFile(nomeFile, tree);
         break;

         case '2':     //inserimento di un nodo collegandolo ad un padre specifico
         cout<<"\nInserisci la label del nodo da inserire, seguita dalla label del padre a cui collegarlo:\n";
         cin>>l1>>l2;
         cout<<"\nInserisci la condizione da soddisfare per raggiungere tale nodo, partendo dal padre:\n";
         cout<<"Deve essere scritta nella forma OperandoValore\n";
         cin>>cond;
         if(d_Tree::addNode(l1,l2,cond,tree)==d_Tree::FAIL)  //nel caso la funzione non sia andata a buon fine, restituisco un messaggio di errore
            cout<<"\nInserimento fallito, il padre specificato non è presente.\n";
         else
            cout<<"\nInserimento completato\n";
         break;

         case '3':
         cout<<"\n!!!Attenzione!!!\nIn certi casi la rimozione di un nodo potrebbe compromettere il corretto funzionamento della predizione.\n";
         cout<<"Assicurati di reinserire un'altro nodo se stai cancellando una foglia.\n";
         cout<<"Assicurati di reinserire un'altro nodo se stai cancellando un nodo associato ad una condizione.\n";
         cout<<"\nInserisci la label del nodo da cancellare:\n";
         cin>>l1;
         if(d_Tree::deleteNode(l1,tree)==d_Tree::FAIL)  //nel caso la funzione non sia andata a buon fine, restituisco un messaggio di errore
            cout<<"\nCancellazione fallita, il nodo specificato non è presente, oppure è la radice.\n";
         else
            cout<<"\nCancellazione completata\n";
         break;

         case '4':
         cout<<"\nInserisci i dati nel seguente formato:\nlabelDaSostituire nuovaLabel nuovaCondizione\n";
         cin>>l1>>l2>>cond;
         if(d_Tree::set(l1,l2,cond,tree)==d_Tree::FAIL)  //nel caso la funzione non sia andata a buon fine, restituisco un messaggio di errore
            cout<<"\nSostituzione fallita, il nodo specificato non è presente.\n";
         else
            cout<<"\nSostituzione completata\n";
         break;

         case '5':
         cout<<"\nStampa albero decisionale:\n";
         print(tree);
         break;

         case '6':
         cout<<"\nStampa variabili dell'albero decisionale:\n";
         print_dtVariables(tree);
         break;

         case '7':
         cout<<"\nInserisci uno alla volta i valori per cui vuoi effettuare la predizione:\n\n";
         if(d_Tree::makePrediction(tree)==d_Tree::FAIL)  //nel caso la funzione non sia andata a buon fine, restituisco un messaggio di errore
            cout<<"\nPredizione fallita, non esiste un nodo per il quale non c'è un arco percorribile\n";
         break;

         case '8':
         cout<<"\nInserisci: ";
         if(d_Tree::makePredictionSet(tree)==d_Tree::FAIL)  //nel caso la funzione non sia andata a buon fine, restituisco un messaggio di errore
            cout<<"\nPredizione fallita, non esiste un nodo per il quale non c'è un arco percorribile\n";
         break;

         case '0':
         cout<<"\nHai deciso di uscire dal programma.\n";
         break;

         default:
         cout<<"\nErrore nell'inserimento della scelta, ritenta con un valore compreso tra 0 e 8\n";
         break;
      }
   }
   while(ch>'0');

   return 0;
}