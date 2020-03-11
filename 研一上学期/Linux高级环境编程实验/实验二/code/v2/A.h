#ifndef A_H
#define A_H

#include <iostream>
#include <fstream>
#include <string>

using  namespace std ;
class A{
private:
    int i;

public:
    A(){
        i = 0;
    }

    A(int j){
        i = j;
    }


    int Serialize(char* pBuffer){
        long length = sizeof(this);
        ofstream *o = new ofstream(pBuffer);
        o->write((char*)this, length);
        o->close();
        delete o;
        return 1;
    }

    static A Deserialize(const char* pBuffer){
        char buf[1000];
        ifstream is(pBuffer);
        is >> buf;
        A *a = (A*) buf;
        is.close();
        return (*a);
    }

    void f(){
        cout << "i = " << this->i << endl;
    }

    int GetObjSize(unsigned int * pObjSize){
        *pObjSize = 4;
        return 1;
    }

};


#endif
