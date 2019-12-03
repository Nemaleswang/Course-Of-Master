#include <fcntl.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>

class ILSerializable
{
public:
    virtual bool Serialize(FILE* fp) = 0;
    virtual ILSerializable* Deserialize(FILE* fp) = 0;
    virtual bool GetType(int& type) = 0;

public:
    ILSerializable()
    {
    }

    virtual ~ILSerializable()
    {
    }
};

class A : public ILSerializable
{
public:
    A()
    {
        i = 0;
    }

    explicit A(int j)
    {
        i = j;
    }

    virtual ~A()
    {
    }

public:
    void f()
    {
        std::cout << "in f(): " << i << std::endl;
    }

public:
    virtual bool GetType(int& type)
    {
        type = 0;
        return true;
    }

    virtual bool Serialize(FILE *fp){
        if(fp == NULL)
            return false;
        fwrite(&i, sizeof(int), 1, fp);
        return true;
    }

    virtual ILSerializable* Deserialize(FILE *fp){
        A *p = new A();
        fread(&(p->i), sizeof(int), 1, fp);
        return p;
    }

private:
    int i;
};

class B : public ILSerializable
{
public:
    B()
    {
        i = 0;
        j = 0;
    }

    explicit B(int k)
    {
        i = k;
        j = k + 1;
    }

    virtual ~B()
    {
    }

public:
    void f()
    {
        std::cout << "in f(): " << i << " " << j << std::endl;
    }

public:
    virtual bool GetType(int& type)
    {
        type = 1;
        return true;
    }

    virtual bool Serialize(FILE *fp)
    {
        if(fp == NULL)
            return false;
        fwrite(&i, sizeof(int), 1, fp);
        fwrite(&j, sizeof(int), 1, fp);
        return true;
    }

    virtual ILSerializable*  Deserialize(FILE* fp)
    {

        B *p = new B();
        fread(&(p->i), sizeof(int), 1, fp);
        fread(&(p->j), sizeof(int), 1, fp);
        return p;
    }

private:
    int i;
    int j;
};

class CLSerializer
{
public:
    bool Serialize(const char *pFilePath, std::vector<ILSerializable*>& v)
    {
        FILE* fp = fopen(pFilePath, "w+");
        if(fp == NULL)
            return false;
        for(int i = 0; i < v.size(); i++)
        {
            int type;
            v[i]->GetType(type);
            fwrite(&type, sizeof(int), 1, fp);
            v[i]->Serialize(fp);
        }
        fclose(fp);
        return true;
    }

    bool Deserialize(const char *pFilePath, std::vector<ILSerializable*>& v)
    {
        FILE* fp = fopen(pFilePath, "r+");
        if(fp == NULL)
            return false;
        for(;;)
        {
            int nType = -1;
            int r = fread(&nType, sizeof(int), 1, fp);

            int type;
            for(int i = 0; i < m_vSerialized.size(); i++)
            {
                m_vSerialized[i]->GetType(type);
                if(type == nType)
                {
                    ILSerializable *p = m_vSerialized[i]->Deserialize(fp);
                    if(p != NULL)
                        v.push_back(p);
                }
            }
            if(r == 0)
                break;

        }
        fclose(fp);
        return true;
    }

    void Register(ILSerializable *pSerialized)
    {
        m_vSerialized.push_back(pSerialized);
    }

private:
    std::vector<ILSerializable*> m_vSerialized;
};

int main()
{
    FILE *fp = fopen("config.txt", "r+");
    char buf[100];
    int len;
    while(fgets(buf,100,fp) != NULL);
    {
        A a1(2);
        B b1(3);
        B b2(4);
        A a2(5);

        std::vector<ILSerializable*> v;

        v.push_back(&a1);
        v.push_back(&b1);
        v.push_back(&b2);
        v.push_back(&a2);

        CLSerializer s;
        s.Serialize(buf, v);
    }

    {
        CLSerializer s;
        A a;
        B b;
        s.Register(&a);
        s.Register(&b);

        std::vector<ILSerializable*> v;
        s.Deserialize(buf, v);

        for(int i = 0; i < v.size(); i++)
        {
            A *p = dynamic_cast<A *>(v[i]);
            if(p != NULL)
                p->f();

            B *q = dynamic_cast<B *>(v[i]);
            if(q != NULL)
                q->f();
        }
    }

    return 0;
}

