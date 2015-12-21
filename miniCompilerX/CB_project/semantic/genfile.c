#include <stdlib.h>
#include <stdio.h>
#include "../syntax/analysis_table.h"

void genfile()
{
    FILE * fp = fopen("semanticTemplate.c","w") ;
    if(!fp)
    {
        printf("failed to open file 'semantication.c'\n") ;
        return ;
    }
    int i ;
    char * defineHead = "#define ACT_" ;
    struct ProductionList
    {
        char data[PRODUCTION_MAX_NUM][150] ;
        int num ;
    } pdtList ;
    pdtList.num = 0 ;
    for(i = 0 ; i< productions.productionNum ; i++)
    {
        fprintf(fp,defineHead) ;
        char pdt[300] = "" ;
        sprintf(pdt,"%s",getName(productions.data[i].pLeft)) ;
        int k = 0 ;
        for( ; k < productions.data[i].len ; k++)
        {
            char tmp[40] = "" ;
            sprintf(tmp,"_%s",getName(productions.data[i].pRight[k])) ;
            strcat(pdt,tmp) ;
        }
        if(strchr(pdt,'\'') != NULL)
        {
            char * x = strchr(pdt , '\'') ;
            for( ; *x != '\0' ; x++)
            {
                *x = *(x+1) ;
            }
            printf("replaced") ;
        }
        fprintf(fp,"%s\t%d\n",pdt,i) ;
        strcpy(pdtList.data[pdtList.num++],pdt) ;
    }
    fprintf(fp,"int semanticAct(int pdtIndex)\n{\n") ;
    fprintf(fp,"\tswitch(pdtIndex)\n\t{\n") ;
    for(i = 0 ; i < pdtList.num ; i++)
    {
        fprintf(fp,"\t\tcase ACT_%s :\n\t\t{\n\n",pdtList.data[i]) ;
        fprintf(fp,"\t\t\tbreak ; \n\t\t}\n") ;
    }
    fprintf(fp,"\t\tdefault:\n\t\t\tbreak ;\n") ;
    fprintf(fp,"\t}\n") ;
    fprintf(fp,"}\n") ;
    fclose(fp) ;
}
