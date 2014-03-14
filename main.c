#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "constant.h"
#include "nameTable.h"
#include "hash.h"
#include "preprocess.h"
#include "dbuffer.h"

int isNonedigit(char c) ;
int main()
{
    char sourceFileName[20] = "hash.h" ;
    //printf("%d",getHashCode(str));
    NameTable * cur = getNameTable(NULL) ;
    //int x = createAndInsertItem(cur,str) ;
    //printf("\n%d\n",x) ;
    char processedFileName[20] ;
    preProcess(sourceFileName,processedFileName) ;
   // printf("%s\n",fileName) ;
    DBuffer * dbuffer = (DBuffer *)malloc(sizeof(DBuffer)) ;
    if(!initDBuffer(dbuffer,processedFileName))
    {
        return FALSE ;
    }
    char c ;
    char tokenNameCopy[] ;
    while((c = getChar(dbuffer)) != EOF)
    {
        if( c == ' ' || c == '\n' || c == '\r') //in fact , there are no '\r' any more
        {
            continue ;
        }
        else if(isNonedigit(c)) /* identifier = nonedigit(nonedigit|digit)* */
        {
            readyCopy(dbuffer) ;
            //read until not  (nonedigit|digit)
            c = getChar(dbuffer) ;
            while(isNonedigit(c) || isdigit(c) )
            {
                c = getChar(dbuffer) ;
            }

        }
    }
    deleteDBuffer(dbuffer) ;

    return 0;
}
int isNonedigit(char c)
{
   return (isalpha(c) || c == '_') ;
}
