#include<stdio.h>
#include "CHECKS.h"
#include "Queries.h"

void CHECK_TRENDS(char **menTrends,char **womenTrends){

    printf("Top Women - Men Trends\n");
    int j=0,k=0;
    char *menTrendsResults[4] = {"Sun_Yat-sen", "Constantine_the_Great",
        "Sigmund_Freud", "Hussein_of_Jordan"}; //IDS: {417,11622,468,1398}
    char *womenTrendsResults[4] = {"Adolf_Hitler","Chiang_Kai-shek",NULL,NULL}; //IDS: {138,416,null,null}

    int counterW = 0, counterM = 0;

    for(j = 0; j < 4; ++j){
        if (menTrends[j] != NULL){
            ++counterM;

            for (k = 0; k < 4; ++k){

                if(strcmp(menTrends[j], menTrendsResults[k]) == 0)
                    break;                
            }
            if(k == 4) 
                printf("You wrongly labeled tag with %s as top trend\n", menTrends[j]);
            
        }
    }
    CHECKINT("Query 3: Mens Trends result size", 4, counterM);


    for(j = 0; j < 4; ++j){
        if(womenTrends[j] != NULL){
            ++counterW;
            for (k = 0; k < 2; ++k){

                if(strcmp(womenTrends[j], womenTrendsResults[k]) == 0)
                    break;                
            }
            if(k == 2) 
                printf("You wrongly labeled tag with %s as top trend\n", menTrends[j]);            
        }
    }
    CHECKINT("Query 3: Women Trends result size", 2, counterW);
}
