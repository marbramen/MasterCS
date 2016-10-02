#ifndef MYPAGEMANAGER_CPP
#define MYPAGEMANAGER_CPP

#include "MyPageManager.h"
#include <string> 
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

MyPageManager::MyPageManager(string file){
    myFile.open(file.c_str(), ios::in | ios::out | ios::trunc | ios::binary);
}

// template<typename T>
// void MyPageManager::saveMyPage(T reg, long n){
//     // myFile.seekp(n*sizeof(T), ios::beg);
//     // myFile.write(reinterpret_cast<const char *>(&reg), sizeof(reg));
// }

template<class T>
void MyPageManager::eraseMyPage(long id, T& reg){

}

template<typename T>
void MyPageManager::saveMyPage(T reg, long id){

}

void MyPageManager::saveMyPage(int x){
    
}

#endif /*MYPAGAMANAGER_CPP*/