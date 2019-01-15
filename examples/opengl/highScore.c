#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "highScore.h"

//writes a score and some message to a high score table file with a specified number of entries,
//optionally prints the table contents to stdout
void highScore(int score, char* message, char* fileName, int entries, int display){ 
    FILE *fp;
    char fileLines[entries][255];
    char entry[255];
    sprintf(entry, "%i\t%s", score, message);
    //printf("The entry is %s\n",entry);
    int numbers[entries];
    //printf("The input was %i\n",score);
    fp = fopen(fileName, "r");
    if(fp){
        //printf("The file already existed!\n");
        int i;
        int j;
        int k;
        for(i=0; i<entries; ++i){
            if(fgets(fileLines[i], 255, fp)){
                numbers[i] = atoi(fileLines[i]);
                //replace \n with \0 (remove trailing newline)
                fileLines[i][strcspn(fileLines[i],"\n")] = 0; 
            }
            else break;
        }
        //printf("Numbers are:\n");
        //for(j=0;j<i;++j){
        //    printf("%i\n",numbers[j]);
        //}
        fclose(fp);
        fopen(fileName,"w");
        //printf("i = %i\n",i);
        //if score is smaller than all other entries and table is not full...
        if(i < entries && numbers[i-1] > score){ 
            strcpy(fileLines[i], entry);
        }
        else{ //otherwise, place score at correct position
            for(j=0; j<i; ++j){
                if(score > numbers[j]){
                    for(k=i-1; k>=j; --k){
                        if(k < entries-1){
                            strcpy(fileLines[k+1], fileLines[k]);
                        }
                    }
                    //printf("Put value %i at position %i\n",score,j);
                    strcpy(fileLines[j], entry);
                    break;
                }
            }
        }
        if(i<entries){ //if the table was not full to begin with
            ++i; //the table will have grown by one value
        }
        for(k=0; k<i; ++k){
            if(k==j){
                printf("\33[1;32m");
                printf("%s\n",fileLines[k]);
                printf("\33[0m");
            }
            else{
                printf("%s\n",fileLines[k]);
            }
            fprintf(fp,"%s\n",fileLines[k]);
        }
        fclose(fp);
    }
    else{
        fp = fopen(fileName, "w+");
        //printf("The file didn't exist!\n");
        fprintf(fp,"%s\n",entry);
        printf("\33[1;32m");
        printf("%s\n",entry);
        printf("\33[0m");
        fclose(fp);
    }
}
