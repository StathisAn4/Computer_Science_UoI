#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc , char **argv){

    int sum =0;
    ssize_t read = 0;
    FILE *file;
    char buf[100];
    char str[10]= "SNR Y(dB)";

    //system("./lencod.exe -f encoder.cfg");
    //file = fopen("my_file.txt","r");
    for (int j = 0 ; j<1;j++){
        if (j == 0){
            // -------------- 1. call system ------------- //
            //system("./rtp_loss.exe /home/gx2001/Desktop/Polymesa_ask3/H264RefSoftware/JM/bin/test.264 /home/gx2001/Desktop/Polymesa_ask3/H264RefSoftware/JM/bin/result.264 5 0");
            //system("./ldecod.exe -f decoder.cfg");
        }
        /*
        else {
            system("./rtp_loss.exe result.264 result.264 5 0");
            system("./ldecod.exe -f decoder.cfg");
        }
        */
        file = fopen("log.dec","r");

    
        while(fgets(buf, sizeof(buf), file) != NULL) {
      
            if (strstr(buf,str)){
                
                char new_str = "|";
                int counter = 0 ;
                for (int i = 0 ; i < sizeof(buf); i++){
                    if (buf[i] == new_str){
                        counter++;
                        //printf("\n\nNice %s\n",buf);
                        if(counter ==10){
                            printf("\n\nNice %s\n",buf);
                            double num = 0;
                            char copy[10];
                            strncpy(copy,&buf[i-4],5);
                            //num = atof(copy);
                            printf("\n num is :%s\n",copy);
                            sum +=num;
                            break;
                        }
                    }
                }
                break;
            }
            //double mesos_oros = sum/100;
            //printf("\n Mesos oros einai : %f",mesos_oros);

        }
        double mesos_oros = sum/100;
        printf("\n Mesos oros einai : %f",mesos_oros);
        //system("./lencod.exe -f encoder.cfg");
    }
    fclose(file);

}