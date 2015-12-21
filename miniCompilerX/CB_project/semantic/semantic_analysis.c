#include "semantic_analysis.h"
#include "../syntax/analysis_table.h"
#include <stdio.h>

int tmpID = 0 ;
int labelID = 0 ;

void genCode(char * str)
{
    printf("%s",str) ;
}
void initSemanticS()
{
    semanticS.top = 0 ;
}
void shiftSemanticAct(char lexType[] , char lexVal[])
{
    strcpy(semanticS.data[semanticS.top].lexType , lexType ) ;
    if(strcmp(lexType , "STRING_C") != 0)
    {
        strcpy(semanticS.data[semanticS.top].lexVal , lexVal ) ;
        semanticS.data[semanticS.top].extend  = NULL ;
        /** notice : we should free the extend at here ,
            so that we don't need free it at other place ,
            but because , at now ,i do not know where i have free it ,
            and it seems that i forget to judge if if is NULL ,
            so i give up to free it  here*/
    }
    else
    {
        semanticS.data[semanticS.top].extend = malloc(sizeof(char)*VAL_MAX_LEN) ;
        strcpy((char *)semanticS.data[semanticS.top].extend ,lexVal) ;
    }
    semanticS.top ++ ;
}
char * decideType(char * type1 , char * type2)
{
    //is all legal?
    //ensure that it is type2 -> type1
    char * tmp = (char *) malloc(sizeof(char) *10) ;
    #ifdef OUTPUT_TYPE
    printf("----test_type:%s---\n",type1) ;
    printf("----test_type:%s---\n",type2) ;
    #endif
    if(strcmp(type1,type2) == 0)
    {
        strcpy(tmp,type1) ;
    }
    else
    {
        if(getSize(type1) < getSize(type2))
        {
            free(tmp) ;
            tmp = NULL ;
        }
        else
        {
            strcpy(tmp,type1) ;
        }
    }
    return tmp ;
}
struct symbolTb * mkTb(struct symbolTb * previous)
{
    struct symbolTb * newTb = (struct symbolTb *)malloc(sizeof(struct symbolTb)) ;
    newTb->counter = 0 ;
    newTb->previous = previous ;
    newTb->width = 0 ;
    return newTb ;
}

/*  enter to the table ,including not only the variable but also the new table
    if type == "TABLE" , it is to add a table link !
    return the addr of the new new variable
    or the addr of the new table */
void * enterTb(struct symbolTb * tb , char * name ,char * type , void * offset)
{
    /*first ensure has the same name ? */
    int k ;
    for(k = 0 ; k < tb->counter ; k++)
    {
        if(strcmp(tb->data[k]->name , name) == 0)
        {
            return NULL ;
        }
    }
    tb->data[tb->counter] = (struct tbNode *) malloc(sizeof(struct tbNode)) ;
    strcpy(tb->data[tb->counter]->name , name ) ;
    /* if the type is TABLE , we should create a new table and return the table's pointer */
    strcpy(tb->data[tb->counter]->type , type) ;
    if(strcmp(type,"TABLE") == 0)
    {
        /*enter a table , don't change the width */
        tb->data[tb->counter]->addr = (void *)mkTb(tb) ;
        strcpy((char *)(tb->data[tb->counter]->type) + strlen("TABLE") + 1,type+strlen("TABLE")+1) ;
    }
    else
    {
        tb->data[tb->counter]->addr = offset ;
    }
    tb->counter++ ;
    return tb->data[tb->counter -1]->addr ;
}
/* return the addr or null */
struct tbNode * lookupTb(struct symbolTb * tb , char * name)
{
    int k ;
    for(k = 0 ; k < tb->counter ; k++)
    {
        if(strcmp(tb->data[k]->name,name) == 0)
        {
            return tb->data[k] ;
        }
    }
    /* find previous */
    struct symbolTb * pos = tb->previous ;
    struct symbolTb * cur = tb ;
    while(pos != NULL)
    {

        for(k = 0 ; pos->data[k]->addr != cur ; k++)
        {
            if(strcmp(pos->data[k]->name , name) == 0)
            {
                return pos->data[k] ;
            }
        }
        cur = pos ;
        pos = cur->previous ;
    }
    freopen("CON","w",stdout) ;
    printf("\nERROR!\nno such variable named '%s' \n",name) ;
    return NULL ;
}

/* offset stack , tbptr stack op */
void initOffsetS()
{
    offsetS.top = 0 ;
}
void initTbptrS()
{
    tbptrS.top = 0 ;
}
char * getTmpName()
{
    char * tmpName =  (char *)malloc(sizeof(char) * 10) ;
    sprintf(tmpName,"tmp_%d",tmpID) ;
    tmpID++ ;
    return tmpName ;
}
void initLbptrS()
{
    lbptrS.top = 0 ;
}
struct LbS * getNewLabelS()
{
    struct LbS * newLabel = (struct LbS *)malloc(sizeof(struct LbS)) ;
    newLabel->top = 0 ;
    return newLabel ;
}
void createLabel(struct LbS * curLbS , int labelNum)
{
    int i ;
    char labelVal[10] ;
    for(i = 0 ; i < labelNum ; i++)
    {
        if(curLbS->top >= LABEL_MAX_NUM)
        {
            printf("LABEL STACK OUT !") ;
            return ;
        }
        sprintf(labelVal,"label_%d",labelID) ;
        labelID++ ;
        strcpy(curLbS->data[curLbS->top],labelVal) ;
        curLbS->top ++ ;
    }
}
int getSize(char * type)
{
    if(strcmp(type,"INT") == 0 )
    {
        return 4 ;
    }
    else if(strcmp(type,"CHAR") == 0 || strcmp(type , "BOOL") == 0)
    {
        return 1 ;
    }
    else if(strcmp(type , "DOUBLE") == 0)
    {
        return 8 ;
    }
    else
    {
        return 0 ;
    }
}


