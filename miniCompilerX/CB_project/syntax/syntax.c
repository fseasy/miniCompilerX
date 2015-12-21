#include <stdio.h>
#include <stdlib.h>

#include "analysis_table.h"
#include "../semantic/semantic_analysis.h"

/* DEBUG define */
//#define OUTPUT_ANALYSIS_STACK
//#define OUTPUT_SEMANTIC_STACK
#define SYNTAX

/* reopen */
#define FREOPEN

#define STACK_DEPTH 1000
#define NAME_MAX_LEN 30 //the words' len of lexcal's output file
/* if the valOrAddr is empty , we copy the words to it */
int getLineInfo(FILE * fp , char * words ,  char * valOrAddr)
{
    int ret ;
    char tmp[30] = "" ;
    ret = fscanf(fp,"%s",words) ;
    if(ret == EOF)
    {
        return EOF ;
    }
    /* read , */
    ret = fscanf(fp,"%s",tmp) ;
    if(ret == EOF)
    {
        printf("Input File not corrected!\n") ;
        return EOF ;
    }
    ret = fscanf(fp,"%s",valOrAddr) ;
    if(ret == EOF)
    {
        printf("Input File not corrected!\n") ;
        return EOF ;
    }
    if(strcmp(valOrAddr,";") == 0)
    {
        /* no this parameter */
        strcpy(valOrAddr,words) ;
        return 1 ;
    }
    else
    {
        /* read the ';' */
        /* if the val is STRING_C , it has many words !*/
        if(valOrAddr[0] == '"')
        {
            fseek(fp, - strlen(valOrAddr) ,SEEK_CUR) ;
            fgets(valOrAddr,VAL_MAX_LEN,fp) ;
            int len = strlen(valOrAddr) ;
            for( ; len >= 0 ; len--)
            {
                if(valOrAddr[len] == ';')
                {
                    valOrAddr[len] = '\0' ;
                    break ;
                }
            }
        }
        else
        {
            fscanf(fp,"%s",tmp) ;
        }
        return 1 ;
    }
}
int syntaxAnalysis(char * fileName)
{
    FILE * sourceFile = fopen(fileName,"r") ;
    if(!sourceFile)
    {
        printf("can not open file '%s'\n",fileName) ;
        return -1 ;
    }
    char  words[NAME_MAX_LEN] = "" ;
    char valOrAddr[VAL_MAX_LEN] = "" ;
    struct Stack
    {
        int data[STACK_DEPTH] ;
        int top ;
    } ; //EA stack
    struct Stack stateS ;
    struct Stack symbolS ;
    /* -----------init-------------- */
    stateS.top = 0 ;
    symbolS.top = 0 ;
    stateS.data[stateS.top++] = 0 ;
    symbolS.data[symbolS.top++] = transName("#") ;
        /** init semanticS */
    initSemanticS() ;
        /* make a global sybolTb */
    struct symbolTb * globalTb = mkTb(NULL) ;
        /* init the offset stack , tbptr stack */
    initTbptrS() ;
    initOffsetS() ;
    initLbptrS() ;
    offsetS.data[offsetS.top++] = 0 ;
    tbptrS.data[tbptrS.top++] = globalTb ;
    //-------------------
    #ifdef FREOPEN
    char * dotPos = strchr(fileName,'.') ;
    char intermediateFileName[30] ; //get the name
    if(dotPos == NULL)
    {
        strcpy(intermediateFileName,fileName) ;
        strcat(intermediateFileName,".ix") ;
    }
    else
    {
        char * dp = fileName ;
        for( ; dp != dotPos ; dp++)
        {
            intermediateFileName[dp - fileName] = *dp ;
        }
        intermediateFileName[dp - fileName] = '\0' ;
        strcat(intermediateFileName,".ix") ;
    }
    printf("%s",intermediateFileName) ;
    freopen(intermediateFileName , "w" , stdout) ; //to create or clear it
    freopen("CON","w",stdout) ;
    #endif
    getLineInfo(sourceFile,words,valOrAddr) ;
    while(1)
    {
        #ifdef OUTPUT_ANALYSIS_STACK
        int j = 0 ;
        for( ; j < stateS.top ; j++)
        {
            printf("%d,",stateS.data[j]) ;
        }
        printf("\n") ;
        for( j = 0 ; j < stateS.top ; j++)
        {
            printf("%s,",getName(symbolS.data[j]) ) ;
        }
        printf("\n") ;
        #endif
        #ifdef OUTPUT_READLINE
        printf("--words:%s\n--valoraddr:%s\n",words,valOrAddr) ;
        #endif
        #ifdef OUTPUT_SEMANTIC_STACK
        int dk = 0 ;
        printf("--semantic_stack--\n") ;
        for( ; dk < semanticS.top ; dk++)
        {
            printf("[%s,%s,%s] | ",semanticS.data[dk].lexType,semanticS.data[dk].lexVal,semanticS.data[dk].tbName) ;
        }
        printf("\n") ;
        #endif
        int symbol = transName(words) ;
        if(symbol != EMPTY)
        {
                // find ACTION
                #ifdef OUTPUT_STATUS
                int cState = stateS.data[stateS.top -1 ] ;
                printf("current state=%d,symbol=%d,%s",cState,symbol,getName(symbol)) ;
                #endif
                int val = analysisTable.table[stateS.data[stateS.top -1 ]][symbol] ;
                if(val == ACC )
                {
                    #ifdef SYNTAX
                    printf("ACC\n") ;
                    #endif
                    return 0 ;
                }
                else if(val == ERROR)
                {
                    #ifdef SYNTAX
                    printf("something error\n") ;
                    #endif
                    return -1 ;
                }
                else if(val >= 0 )
                {
                    /* shift */
                    symbolS.data[symbolS.top++] = symbol ;
                    stateS.data[stateS.top++] = val ;
                    shiftSemanticAct(words,valOrAddr) ;
                    #ifdef SYNTAX
                    printf("shift %s\n",getName(symbol)) ;
                    #endif
                    /* read next */
                    if(getLineInfo(sourceFile,words,valOrAddr) == EOF)
                    {
                        strcpy(words,"#") ;
                    }
                }
                else if(val < 0)
                {
                    /* reduce */
                    val = - val ;
                    /*print */

                    #ifdef SYNTAX
                    char * proS = restoreP(val) ;
                    printf("%s,%d\n",proS,strlen(proS)) ;
                    free(proS) ;
                    #endif
                    #ifdef FREOPEN
                    freopen(intermediateFileName,"a",stdout) ;
                    #endif

                    if(semanticAct(val) == FALSE_ANA)
                    {
                        #ifdef FREOPEN
                        fflush(stdout) ;
                        freopen( "CON", "w", stdout );
                        #endif
                        return FALSE_ANA ;
                    }


                    #ifdef FREOPEN
                    fflush(stdout) ;
                    freopen( "CON", "w", stdout );
                    #endif
                    //free(proS) ;
                    /* stack pop */
                    int len = productions.data[val].len ;
                    stateS.top = stateS.top - len ;
                    symbolS.top  = symbolS.top - len ;
                    /* stack push */
                        /* we should get the production left */
                    int pLeft = productions.data[val].pLeft ;
                    symbolS.data[symbolS.top++] = pLeft ;
                    /* look up goto table*/
                    int gotoState = analysisTable.table[stateS.data[stateS.top -1]][pLeft] ;
                    if(gotoState == ERROR)
                    {
                        printf("GOTO table occured error") ;
                        return -1 ;
                    }
                    stateS.data[stateS.top++] = gotoState ;
                }
        }
        else
        {
            printf("not supported symbol ,%s!\n",words) ;
            return -1 ;
        }
    }
    return 0 ;
}
int startSyntaxAnalysis(int argc , char * argv[])
{
    if(initAnalysisTable() == -1)
    {
        return -1 ;
    }
    if(argc == 1)
    {
        printf("start run test file \n") ;
        return syntaxAnalysis("test.out");
    }
    else
    {
        if(strcmp(argv[1],"genfile") == 0)
        {
            genfile() ;
            printf("generate file finished") ;
        }
        else
        {
            printf("start run %s file \n",argv[1]) ;
            return syntaxAnalysis(argv[1]);
        }
    }
}
