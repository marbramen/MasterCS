#ifndef MYPAGEMANAGER_H
#define MYPAGEMANAGER_H

#include <string>
#include <fstream>

using namespace std;

struct node1{
    int a;
};

class MyPageManager{
    private:
        ofstream myFile;
        bool empty;
        long pageIdCount;
    public:
        MyPageManager(string);

        template<typename T>
        void saveMyPage(T, long);

        void saveMyPage(int x);

        template<class T>
        void eraseMyPage(long, T&);    

        template<typename T>
        void add2(T x){

        }   
};


template<typename T>
void MyPageManager::saveMyPage(T reg, long id){

}


#endif /* MYPAGEMANAGER_H */
#include "MyPageManager.cpp"