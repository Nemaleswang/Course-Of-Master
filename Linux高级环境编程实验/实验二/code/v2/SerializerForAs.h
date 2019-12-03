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
        FILE* fp = fopen(pFilePath, "w+");
        for(int i = 0; i < v.size(); i++){
            fwrite(v[i], sizeof(int), 1, fp);
        }
        fclose(fp);
        return 1;
    }

    static int Deserialize(const char* pFilePath, vector<A *>& v){
        FILE *fp = fopen(pFilePath, "r");
        for(int i = 0; i < v.size(); i++){
            fread(v[i], sizeof(int), 1, fp);
        }
        fclose(fp);
        return 1;
    }

};