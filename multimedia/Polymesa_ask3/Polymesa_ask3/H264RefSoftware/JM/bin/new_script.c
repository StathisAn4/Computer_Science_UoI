#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc , char **argv){

    int sum =0;
    ssize_t read = 0;
    FILE *file;
    char buf[1000];
    char str[10]= "SNR Y(dB)";
    int times=0;
    int lines =0;
    /*
    0. In decoder.cfg remeber change test.264->result.264  
    1. 100 times use rtp_looss.c result.264    
    2. then decoder 
    3. all this write in log.dec 100 lines with results  
    4.
    
    */
    for (int i = 0 ; i<100;i++){
        system("./rtp_loss.exe /home/gx2001/Desktop/Polymesa_ask3/H264RefSoftware/JM/bin/test.264 /home/gx2001/Desktop/Polymesa_ask3/H264RefSoftware/JM/bin/result.264 2 0");
        system("./ldecod.exe -f decoder.cfg");
        times++;
    }
    printf("\nTimes:%d",times);
    

    for (int j = 0 ; j<1;j++){

        
        file = fopen("log.dec","r");

    
        while(fgets(buf, sizeof(buf), file) != NULL) {
                //printf("Nice %s",buf);
                lines++;
                char new_str = '|';
                int counter = 0 ;

                for (int i = 0 ; i < strlen(buf); i++){
                    if (buf[i] == new_str){
                        counter++;
                        //printf("\n\nNice,%d\n",counter);
                        
                        if(counter ==13){
                            //printf("\n\nNice %s\n",buf);
                            double num = 0;
                            char copy[4];
                            strncpy(copy,&buf[i+1],4);
                            char * new_copy = strtok(copy,"S");
                            num = atof(new_copy);
                            printf("\n PSNR (Y) :%s\n",new_copy);
                            sum +=num;
                            break;
                        
                    }
                }
            }
        }
        
        double mesos_oros = sum/100;
        printf("\n Mesos oros einai : %f \n",mesos_oros);
        //system("./lencod.exe -f encoder.cfg");
    }

    fclose(file);
    
    
}
