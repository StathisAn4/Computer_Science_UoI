#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#ifdef WIN32
#include <Winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <ctime>
#include<time.h>
#include <iostream>
#include <fstream>
using namespace std;


int main(int argc , char **argv){
    int count =0;
    ofstream my_file;
    my_file.open("my_file.txt");
    for (int i = 0 ; i<100;i++){
        my_file << system("./ldecod.exe -f decoder.cfg");
    }
    my_file.close();

}