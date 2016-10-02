#ifndef MYLIST_CPP
#define MYLIST_CPP

#include <cstdio>
#include "MyList.h"
#include "MyPageManager.h"

MyList::MyList(void){
    tam = 0;
    root = NULL;
}

void MyList::add(int x){
    node *nuevo = new node;
    node test;
    nuevo->valor = x;
    nuevo->next = NULL;
    if(tam == 0)
        root = nuevo;
    else{
        node* temp = root;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = nuevo;
    }

    tam++;
    MyPageManager pageManger("file.txt");
    long id = 1;
    // pageManger.saveMyPage(tam);
    pageManger.add2(test);  
    node1 temp2;  
    pageManger.saveMyPage(test, id);

}

void MyList::deleteElem(int pos){
    int i = 0;
    if(i < tam){
        int i = 0;
        node* temp = root;
        while (i != pos){
            temp = temp->next;
            i++;
        }
        temp = temp->next;
        tam--;
    }
}

void MyList::levantarLista(){

}

void MyList::imprimir(){
    node* temp = root;
    for(int i = 0; i < tam; i++){
        printf("%d%c", temp->valor, i + 1 != tam ? ' ' : '\n');
        temp = temp->next;        
    }
}

#endif