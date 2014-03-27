#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "constant.h"
#include "token_code.h"
#include "nameTable.h"
#include "hash.h"
#include "preprocess.h"
#include "dbuffer.h"
#include "keyword.h"

int isNonedigit(char c) ;
void tokenScanEcho(char * type , char* val) ;
void tokenScanError(char * errorType , char * detail) ;
int main()
{
    char sourceFileName[20] = "minic.c" ;
    //printf("%d",getHashCode(str));
    NameTable * cur = getNameTable(NULL) ;
    //int x = createAndInsertItem(cur,str) ;
    //printf("\n%d\n",x) ;
    char processedFileName[20] ;
    preProcess(sourceFileName,processedFileName) ;
   // printf("%s\n",fileName) ;

    /** init keyword */
    KeyWord * keyWord = (KeyWord *)malloc(sizeof(KeyWord)) ;
    FILE * keywordFile = fopen("keywords.txt","r") ;
    initKeyWord(keyWord) ;
    if(!keywordFile)
    {
        printf("Init keyword error! \n") ;
        return -1 ;
    }
    insertKeyWordFromFile(keyWord,keywordFile) ;

    /** init the buffer */
    DBuffer * dbuffer = (DBuffer *)malloc(sizeof(DBuffer)) ;
    if(!initDBuffer(dbuffer,processedFileName))
    {
        return FALSE ;
    }
    char c ;
    char tokenNameCopy[TOKEN_MAX_LENGTH] ;
    while((c = getChar(dbuffer)) != EOF)
    {
        if( c == ' ' || c == '\n' || c == '\r') //in fact , there are no '\r' any more
        {
            continue ;
        }
        /** recognize the ID , KEYWORD */
        else if(isNonedigit(c)) /* identifier = nonedigit(nonedigit|digit)* */
        {
            readyCopy(dbuffer) ;
            //read until not  (nonedigit|digit)
            c = getChar(dbuffer) ;
            while(isNonedigit(c) || isdigit(c) )
            {
                c = getChar(dbuffer) ;
            }
            /* the identifier has over */
            retract(dbuffer,1) ; /* retract ,we should retract before copy it !!*/
            copyToken(dbuffer,tokenNameCopy) ;
            /* is the key word ?*/
            if(lookUpKeyWord(keyWord,tokenNameCopy))
            {
                capitalize((char *)tokenNameCopy) ;
                tokenScanEcho(tokenNameCopy,"") ;
                continue ;
            }
            /* not the keyword */
            int addr = createAndInsertItem(cur,tokenNameCopy) ;
            /*
            if(addr == -1)
            {
                //failed , because of repeat definition
                //now i think it doesn't word
                char bufForErrorOut[200] = "";
                sprintf(bufForErrorOut,"The ID :%s has occured before !\n" , tokenNameCopy) ;
                tokenScanError("duplicate definition",bufForErrorOut) ;
                continue ;
            }
            */
            tokenScanEcho("ID",(char *)addr) ;
        }
        /** recognize the INT ,REAL */
        else if(isdigit(c))
        {
            readyCopy(dbuffer) ;
            c = getChar(dbuffer) ;
            while(isdigit(c))
            {
               c = getChar(dbuffer) ;
            }
            /* it is not the digit , is '.' ?*/
            if(c == '.')
            {
                c = getChar(dbuffer) ;
                while(isdigit(c))
                {
                    c = getChar(dbuffer) ;
                }
                /* real over */
                retract(dbuffer,1) ;
                copyToken(dbuffer,tokenNameCopy) ;

                /* just echo */
                tokenScanEcho("REAL",tokenNameCopy) ;
            }
            else
            {
                /* it is INT */
                retract(dbuffer,1) ;
                copyToken(dbuffer,tokenNameCopy) ;

                /* just echo */
                tokenScanEcho("INT",tokenNameCopy) ;
            }
        }
        /** recognize the CHAR_C */
        else if(c == '\'')
        {
            readyCopy(dbuffer) ;
            c = getChar(dbuffer) ;
            if(c == '\\')
            {
                /* escape char */
                c = getChar(dbuffer) ;
                /* again */
                c = getChar(dbuffer) ;
                if(c != '\'')
                {
                    tokenScanError("CHAR definition error","char escape error") ;
                }
                copyToken(dbuffer,tokenNameCopy) ;
                tokenScanEcho("CHAR_C",tokenNameCopy) ;
            }
            else
            {
                c = getChar(dbuffer) ;
                if(c != '\'')
                {
                    tokenScanError("CHAR definition error","''should just has one character") ;
                }
                copyToken(dbuffer,tokenNameCopy) ;
                /* do not need retract */
                tokenScanEcho("CHAR_C",tokenNameCopy) ;
            }
        }
        /** recognize the string */
        else if(c == '"')
        {
            readyCopy(dbuffer) ;
            c = getChar(dbuffer) ;
            while(c != '"')
            {
                if(c == '\\')
                {
                    /* escape , be careful */
                    /* we read the char to skip the escape */
                    c = getChar(dbuffer) ;

                }
                c = getChar(dbuffer) ;
            }
            copyToken(dbuffer,tokenNameCopy) ;
            /* do not retract */
            tokenScanEcho("STRING",tokenNameCopy) ;
        }
        /** recognize the operator and separator*/
        else
        {
            /*readyCopy(dbuffer) ;*/
            switch(c)
            {
                case '>' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '=')
                    {
                        /** >= */
                        tokenScanEcho("GE","") ;
                        /*tokenScanEcho("=","") ;*/
                    }
                    else
                    {
                        /** > */
                        retract(dbuffer,1) ;
                        tokenScanEcho("GT","") ;
                    }
                    break ;
                }
                case '<' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '=')
                    {
                        /** <= */
                        tokenScanEcho("LE","") ;
                    }
                    else
                    {
                        /** < */
                        retract(dbuffer,1) ;
                        tokenScanEcho("LT","") ;
                    }
                    break ;
                }
                case '=' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '=')
                    {
                        /** == */
                        tokenScanEcho("EQ","") ;
                    }
                    else
                    {
                        /** = */
                        retract(dbuffer,1) ;
                        tokenScanEcho("ASSIGN","") ;
                    }
                    break ;
                }
                case '!' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '=')
                    {
                        /** != */
                        tokenScanEcho("NEQ","") ;
                    }
                    else
                    {
                        /** ! */
                        retract(dbuffer,1) ;
                        tokenScanEcho("NOT","") ;
                    }
                    break ;
                }
                case '(' :
                     /** ( */
                    tokenScanEcho("LR_BRAC","") ;
                    break ;
                case ')' :
                    /** ) */
                    tokenScanEcho("RR_BRAC","") ;
                    break ;
                case '[' :
                    /** [ */
                    tokenScanEcho("LS_BRAC","") ;
                    break ;
                case ']' :
                    tokenScanEcho("RS_BRAC","") ;
                    break ;
                case '+':
                {
                    c = getChar(dbuffer) ;
                    if(c == '+')
                    {
                        /** ++ */
                        tokenScanEcho("INC","") ;
                    }
                    else if(c == '=')
                    {
                        /** ++ */
                        tokenScanEcho("ADD_ASS","") ;
                    }
                    else
                    {
                        /** + */
                        retract(dbuffer,1) ;
                        tokenScanEcho("ADD","") ;
                    }
                    break ;
                }
                case '-' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '-')
                    {
                        /** -- */
                        tokenScanEcho("DEC","") ;
                    }
                    else if(c == '=')
                    {
                        /** -= */
                        tokenScanEcho("SUB_ASS","") ;
                    }
                    else
                    {
                        /** - */
                        retract(dbuffer,1) ;
                        tokenScanEcho("SUB","") ;
                    }
                    break ;
                }
                case '*' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '=')
                    {
                        /** *= */
                        tokenScanEcho("MUL_ASS","") ;

                    }
                    else
                    {
                        /** * */
                        retract(dbuffer,1) ;
                        tokenScanEcho("MUL_OR_INDIR","") ;
                    }
                    break ;
                }
                case '/' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '=')
                    {
                        /** /= */
                        tokenScanEcho("DIV_ASS","") ;
                    }
                    else
                    {
                        /** / */
                        retract(dbuffer,1) ;
                        tokenScanEcho("DIV","") ;
                    }
                    break ;
                }
                case '&' :
                {
                    c = getChar(dbuffer) ;
                    if(c == '&')
                    {
                        /** && */
                        tokenScanEcho("AND","") ;
                    }
                    else
                    {
                        /** &*/
                        retract(dbuffer,1) ;
                        tokenScanEcho("REFERENCE","") ;
                    }
                    break ;
                }
                case '|' :
                {
                    c = getChar('|') ;
                    if(c == '|')
                    {
                        /** || */
                        tokenScanEcho("OR","") ;
                    }
                    else
                    {
                        tokenScanError("NOT SUPPORTED OPARATOR","'|' is not supported!") ;
                    }
                    break ;
                }
                case ',' :
                    /** , */
                    tokenScanEcho("COMMA","") ;
                    break ;
                case ';' :
                    /** ; */
                    tokenScanEcho("SEMIC","") ;
                    break ;
                case '{' :
                    /** { */
                    tokenScanEcho("LB_BRAC","") ;
                    break ;
                case '}' :
                    /** } */
                    tokenScanEcho("RB_BRAC","") ;
                    break ;
                default :
                {
                    /** not supported */
                    char tokenError[200] ;
                    sprintf(tokenError,"character:%c is not supported!",c) ;
                    tokenScanError("NOT SUPPORTED CHARACTER",tokenError) ;
                }

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
void tokenScanEcho(char * type , char* val)
{
    if(strcmp(type,"ID") == 0)
    {
         printf("( %s , %d [%s])\n",type,(int)val,((NameTableItem *)val)->tokenInfo.name) ;
    }
    else
    {
        printf("( %s , %s )\n",type,val) ;
    }

}
void tokenScanError(char * errorType , char * detail)
{
    printf("ERROR: %s :%s\n",errorType , detail) ;
}
