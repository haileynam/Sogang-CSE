#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "user.h"
#include "bitmap.h"
#include "hash.h"
#include "list.h"


extern struct bitmap* myBitmap[10];
extern struct hash* myHash[10];
extern struct list* myList[10];


int main()
{
    srand((unsigned int)time(NULL));
    //FILE *ifp = fopen("a.txt", "r");
    char str[80];
    char *arg[8];
    char* res;
    // if(!ifp){
    //     printf("Error!\n");
    //     return -1;
    // }

    
    while(1)
    {
        res = fgets(str, sizeof(str), stdin);
        //res=fgets(str, sizeof(str), ifp);
        if(res==NULL){
            break;
        }
        str[strlen(str)-1]='\0';
        //printf("input= %s\n", str);
        
        tok(str, arg); 
        // printf("Test tok func\n");
        // for(int i=0;i<4;i++){
        // char* argument = arg[i];
        // printf("%d: %s\n", i,argument);
        // printf("%d: %s\n", i,arg[i]); //arg[i] ith string.
        //}
            
        if((strcmp(arg[0], "create")==0||strcmp(arg[0], "dumpdata")==0||strcmp(arg[0],"delete")==0))
        {
            level_1(arg);
        }

        else if(strcmp(arg[0], "quit")==0)
        {
            break;
        }

        else
        {
            level_2(arg);
        }
        

    }   
    return 0;
}