#ifndef MYLIST_H
#define MYLIST_H

struct node{
    node* next;
    int valor;
};

class MyList{
    public:         
        node* root;
        MyList(void);        
        void add(int);
        void deleteElem(int);
        void levantarLista();
        void imprimir();
   private:
        int tam;
};
#endif /*MYLIS_H*/