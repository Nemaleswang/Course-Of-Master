#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <vector>

using namespace std;

class A{
    public:
    A(){
        i = 0;
    }

    A(int j){
        i = j;
    }

    void f(){
        cout << "i = " << i << endl;
    }

    public:
    bool Serialize(FILE *fp){
        if(fp == NULL)
            return false;
        fwrite(&i, sizeof(int), 1, fp);
        return true;
    }

    bool Deserialize(FILE *fp){
        if(fp == NULL)
            return false;
        fread(this, sizeof(int), 1, fp);
        return true;
    }
    private:
        int i;
};

class B{
    public:
    B(){
        i = 0;
        j = 0;
    }

    B(int k){
        i = k;
        j = k + 1;
    }

    bool Serialize(FILE* fp){
        if(fp == NULL)
            return false;
        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&j, sizeof(int), 1, fp);
        return true;
    }

    bool Deserialize(FILE* fp){
        fread(&(this->i), sizeof(int), 1, fp);
        fread(&(this->j), sizeof(int), 1, fp);
        return true;
    }

    void f(){
        cout << "i = " << i << " , j = " << j << endl;
    }

    private:
        int i, j;

};

struct Serialized{
    int nType;
    void *pObj;
};

class Serializer{
    public:
        bool Serialize(const char* pFilePath, vector<Serialized>& v){
            FILE* fp = fopen(pFilePath, "w+");
            if(fp == NULL)
                return false;
            for(int i = 0; i < v.size(); i++){
                fwrite(&(v[i].nType), sizeof(int), 1, fp);
                if(0 == v[i].nType){
                    A *p = (A*)(v[i].pObj);
                    p->Serialize(fp);
                }else if(1 == v[i].nType){
                    B *p = (B*)(v[i].pObj);
                    p->Serialize(fp);
                }
            }
            fclose(fp);
        }

        bool Deserialize(const char *pFilePath, vector<Serialized> &v){
            FILE* fp = fopen(pFilePath, "r+");
            for(;;){
                int nType;
                int r = fread(&nType, sizeof(int), 1, fp);
                if(-1 == r || 0 == r)
                    break;
                if(0 == nType){
                    A *p;
                    p = new A();
                    p->Deserialize(fp);

                    Serialized s;
                    s.nType = nType;
                    s.pObj = p;
                    v.push_back(s);
                }else if(1 == nType){
                    B *p;
                    p = new B();
                    p -> Deserialize(fp);

                    Serialized s;
                    s.nType = nType;
                    s.pObj = p;
                    v.push_back(s);
                }
            }
            fclose(fp);
            return true;
        }
};

int main() {
    {
        {
            A a1(2);
            Serialized s1;
            s1.nType = 0;
            s1.pObj = &a1;

            B b1(3);
            Serialized s2;
            s2.nType = 1;
            s2.pObj = &b1;

            B b2(4);
            Serialized s3;
            s3.nType = 1;
            s3.pObj = &b2;

            A a2(5);
            Serialized s4;
            s4.nType = 0;
            s4.pObj = &a2;

            vector<Serialized> v;
            v.push_back(s1);
            v.push_back(s2);
            v.push_back(s3);
            v.push_back(s4);

            Serializer s;
            s.Serialize("data", v);
        }

        {
            Serializer s;
            vector<Serialized> v;
            s.Deserialize("data", v);

            for(int i = 0; i < v.size(); i++){
                if(v[i].nType == 0){
                    A *p = (A*)(v[i].pObj);
                    p->f();
                }else if(v[i].nType == 1){
                    B *p = (B*)(v[i].pObj);
                    p->f();
                }
            }
        }
        return 0;
    }
}