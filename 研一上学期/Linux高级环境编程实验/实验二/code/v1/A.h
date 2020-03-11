#include <iostream>
#include <fstream>
#include <string>

using  namespace std ;
class A{

    public:
        A(){
            i = 0;
        }

        A(int j){
            i = j;
        }


        int Serialize(char* pBuffer){
<<<<<<< HEAD
            long length = sizeof(this);
            ofstream *o = new ofstream(pBuffer);
            o->write((char*)this, length);
=======
            long length = sizeof(this);//this指针记录对象的内存地址,这里length代表整个对象的大小,length = 8
	    //cout<<"length = "<<length<<endl;
            ofstream *o = new ofstream(pBuffer);//创建一个输出流来打开字节序
            o->write((char*)this, length);//从this指针指向的位置（即该类）写length个字符到文件中，这里就相当于把整个对象写入到字节序中
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
            o->close();
            delete o;
            return 1;            
        }

<<<<<<< HEAD
        static A Deserialize(const char* pBuffer){
            char buf[1000];
            ifstream is(pBuffer);
            is >> buf;
            A *a = (A*) buf;
=======
        static A Deserialize(const char* pBuffer){//该函数返回的是一个对象
            char buf[1000];
            ifstream is(pBuffer);//以输入方式打开文件
            is >> buf;//从磁盘文件输入,现在这个buffer就是文件序了
            A *a = (A*) buf;//强转成一个对象
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
            is.close();
            return (*a);
        }

        void f(){
<<<<<<< HEAD
            cout << "i = " << this->i << endl;
=======
            cout << "i = " << this->i << endl;//输出i的结果
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
        }

        private:
        int i;

<<<<<<< HEAD
};
=======
};
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
