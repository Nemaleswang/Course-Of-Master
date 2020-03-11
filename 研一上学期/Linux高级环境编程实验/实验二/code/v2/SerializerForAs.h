//
// Created by dj on 2018/10/26.
//

#include "A.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>

using namespace std;

class SerializerForAs{

public:
    static int Serialize(const char* pFilePath, vector<A *>& v){
<<<<<<< HEAD
        FILE* fp = fopen(pFilePath, "w+");
        for(int i = 0; i < v.size(); i++){
=======
        FILE* fp = fopen(pFilePath, "w+");//以可写格式打开文件
        for(int i = 0; i < v.size(); i++){
	    //将v1中的数据写入文件中
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
            fwrite(v[i], sizeof(int), 1, fp);
        }
        fclose(fp);
        return 1;
    }

<<<<<<< HEAD
    static int Deserialize(const char* pFilePath, vector<A *>& v){
        FILE *fp = fopen(pFilePath, "r");
=======
    static int Deserialize(const char* pFilePath, vector<A *>& v){\
	//以可读格式打开文件
        FILE *fp = fopen(pFilePath, "r");
	//读取文件中的数据
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
        for(int i = 0; i < v.size(); i++){
            fread(v[i], sizeof(int), 1, fp);
        }
        fclose(fp);
        return 1;
    }

<<<<<<< HEAD
};
=======
};
>>>>>>> fd5745b4d5a3c151a3d0de2977c5d7451b195e2d
