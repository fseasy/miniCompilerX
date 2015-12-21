/**
    description :   at 'analysis_table.h' , has defined some global variables and
                    MACRO .
                variables :
                ---transTable :
                    the first thing we do is to translate the STRING of terminal and
                    non_terminal symbol to the INT from 0 the symbol number -1 ;
                    in this way ,we has the map from the symbol to the analysis table cols.
                    what's more ,using hash can improve the translate speed .
                    has package a function named [tansName()] to make translating
                 ---symbolList
                    nearly negative , make known the num what stands for
                    has package a function named [getName()] to use it convenient.
                 ---productions
                    translate all the production to the number sirials .
                    that means the left and right of the production all has been translated
                    to the number . it has len , order ,pLeft ,pRight member variable
                 ---ic
                    that is ItemCollections , which store all the Item collections ,
                    also can named 'states'.
                 ---analysisTable
                    the final data that we need .
                    {
                        int  table[][] ;
                        int row ;
                        int col ;
                    }
                    after call initAnalysisTable ,we created it !
    at 'analysis_table.c' ,
                    we realize all the needed function , including
                        FIRST , CLOURE , initAnalysisTable() and so on .
    just call [initAnalysisTable()] , we can get all ready
                                                        -----finished time : 2014.4.18

*/
#include <stdlib.h>
#include <stdio.h>
#include "analysis_table.h"
/** for debug */
#define OUTPUT_P
#define OUTPUT_LINE
#define OUTPUT_WORD
#define OUTPUT_SI
#define OUTPUT_IC
#define OUTPUT_GOLIST
#define OUTPUT_REDUCE
#define OUTPUT_LR1
//#define OUPUT_ANALYSISTABLE_FILE  //output analysis to file
extern int getHashCode(char * str) ;
int insertTransTable(char * str , int counter)
{
    int hashCode = getHashCode(str) % SYMBOL_HASH_LEN ;
    if(transTable[hashCode].numExp == EMPTY)
    {
        transTable[hashCode].numExp = counter ;
        strcpy(transTable[hashCode].name,str) ;
    }
    else
    {
        /** conflict */
        /**  hash agin by sub hashCode 2 */
        int findEmptyTimes = 0 ;
        hashCode = hashCode -2 >= 0 ? hashCode -2 : hashCode -2 + SYMBOL_HASH_LEN ;

        while(transTable[hashCode].numExp != EMPTY && findEmptyTimes < SYMBOL_HASH_LEN)
        {
            hashCode = hashCode -2 >= 0 ? hashCode -2 : hashCode -2 + SYMBOL_HASH_LEN ;
            findEmptyTimes++ ;
        }
        /** now get the empty location or the hashList  has full */
        if(findEmptyTimes != SYMBOL_HASH_LEN)
        {
            transTable[hashCode].numExp = counter ;
            strcpy(transTable[hashCode].name , str) ;
        }
        else
        {
            printf("The hashList has full \n") ;
            return -1 ;
        }
    }
    return hashCode ;

}
/**
    init the transTable and symbolList
    return : the number of the symbols
*/
int initTransTableAndSymbolList()
{
    /** init transTable */
    int i ;
    for(i =0 ; i < SYMBOL_HASH_LEN ; i++)
    {
        memset(transTable[i].name,'\0',sizeof(char)) ;
        transTable[i].numExp = EMPTY ;
    }
    /** init the symbolList , the productionIndex[0] will be init at created !*/
    symbolList.symbolNum = 0 ;

    FILE * nonTerminalFile , * terminalFile  ;
    /** add the terminal symbol from file */
    terminalFile = fopen("terminallist.txt","r") ;
    if(!terminalFile)
    {
        printf("failed to open file 'terminallist.txt' \n") ;
        return -1 ;
    }
    char line[20] = "" ;
    int counter = 0 ;
    while(fscanf(terminalFile,"%s",line) != EOF)
    {
#ifndef OUTPUT_LINE
        printf("%s",line) ;
#endif
        if(insertTransTable((char *)line,counter) != -1)
        {
            counter++ ;
            /** add to the symbol list */
            SymbolNumStandsFor * pnt = & symbolList.data[symbolList.symbolNum] ;
            strcpy(pnt->name,line) ;
            pnt->type = TERMINAL ;
            pnt->productionIndex[0] = 0 ;
            symbolList.symbolNum ++ ;
        }
        else
        {
            printf("init failed\n") ;
            return -1 ;
        }
    }
    fclose(terminalFile) ;
    /** add special terminal '#' */
    if(insertTransTable("#",counter++) != -1)
    {
        SymbolNumStandsFor * pnt = & symbolList.data[symbolList.symbolNum] ;
        strcpy(pnt->name , "#") ;
        pnt->type = TERMINAL ;
        pnt->productionIndex[0] = 0 ;
        symbolList.symbolNum ++ ;
    }
    /** add none-terminal symbol */
    nonTerminalFile = fopen("nonterminallist.txt","r") ;
    if(!nonTerminalFile)
    {
        printf("failed to open file nonterminallist.txt! \n") ;
        return -1 ;
    }
    while(fscanf(nonTerminalFile,"%s",line) != EOF)
    {
#ifndef OUTPUT_LINE
        printf("%s",line) ;
#endif
        if(insertTransTable(line,counter) != -1)
        {
            counter++ ;
            SymbolNumStandsFor * pnt = & symbolList.data[symbolList.symbolNum] ;
            strcpy(pnt->name,line) ;
            pnt->type = NON_TERMINAL ;
            pnt->productionIndex[0] = 0 ;
            symbolList.symbolNum ++ ;
        }
    }
    fclose(nonTerminalFile) ;
    return symbolList.symbolNum ;
}
/**
    translate the symbol to the number
    return : the symbol's exprision in number
*/
int transName(char * name)
{
    int hashCode = getHashCode(name) % SYMBOL_HASH_LEN ;
    if(strcmp(transTable[hashCode].name,name) == 0)
    {
        return transTable[hashCode].numExp ;
    }
    /*
        not at first place , let's find it
    */
    int findTimes = 0 ;
    while(strcmp(transTable[hashCode].name,name) != 0 &&
            transTable[hashCode].numExp != EMPTY && findTimes < SYMBOL_HASH_LEN)
    {
        hashCode = hashCode >= 2 ? hashCode -2 : hashCode -2 + SYMBOL_HASH_LEN ;
    }
    if(strcmp(transTable[hashCode].name,name) == 0)
    {
        return transTable[hashCode].numExp ;
    }
    else
    {
        /** not found !*/
        return EMPTY ;
    }
}
char * getName(int index)
{
    /**test*/
#ifndef OUTPUT_P
    if(index >= symbolList.symbolNum || index < 0)
    {
        printf("----SYMBOL error----") ;
    }
#endif
    return index < symbolList.symbolNum && index >= 0 ? symbolList.data[index].name : NULL ;
}
int getType(int index)
{
    return index < symbolList.symbolNum && index >= 0 ? symbolList.data[index].type : EMPTY ;
}
/** build production */
/**
    return : the numbers of the producions
*/
char * restoreP(int index)
{

    char * restoredP = (char *)malloc(sizeof(char) * 150) ;
    memset(restoredP,0,sizeof(char)*150) ;
    if(index >= productions.productionNum)
    {
        sprintf(restoredP,"Bad index\n") ;
        return restoredP ;
    }
    sprintf(restoredP,"%s => ",getName(productions.data[index].pLeft)) ;
    int i ;
    for(i = productions.data[index].len -1 ; i >= 0 ; i-- )
    {
        char tmp[200] = "";
        sprintf(tmp,"%s ",getName(productions.data[index].pRight[productions.data[index].len-1 -i])) ;
        strcat(restoredP,tmp) ;
    }
    return restoredP ;
}
int initProduction()
{
    SProduction * pnt = & productions ; /* for short */
    pnt->productionNum = 0 ;
    FILE * productionFile = fopen("production.txt","r") ;
    if(!productionFile)
    {
        printf("failed to open file 'production.txt'\n") ;
        return -1 ;
    }
    char words[30] ;
    while(fscanf(productionFile,"%s",words) != EOF)
    {

        /** every time get the productions which has the same left non-terminal */
#ifndef OUTPUT_WORD
        printf(" %s ",words) ;
#endif
        int pLeft ;
        if((pLeft = transName(words)) == EMPTY)
        {
            char * tmpProduction = restoreP(pnt->productionNum -1) ;
            printf("production left part error:[%s,%d,%s]\n",words,pnt->productionNum,tmpProduction) ;
            free(tmpProduction) ;
            return -1 ;
        }
        pnt->data[pnt->productionNum].pLeft = pLeft ;
        pnt->data[pnt->productionNum].order = pnt->productionNum ;
        fscanf(productionFile,"%s",words) ;
#ifndef OUTPUT_WORD
        printf(" %s ",words) ;
#endif
        /** it should be '->' or '=>'*/
        if((strcmp(words,"->")!= 0 ) && (strcmp(words,"=>") != 0) )
        {
            char * tmpProduction = restoreP(pnt->productionNum -1 ) ;
            printf("\nnot supported production[%s,%d,%s]\n",words,pnt->productionNum,tmpProduction) ;
            free(tmpProduction) ;
            return -1 ;
        }
        /* get the right part */
        pnt->data[pnt->productionNum].len = 0 ;
        fscanf(productionFile,"%s",words) ;
        while(strcmp(words,";") != 0)
        {
#ifndef OUTPUT_WORD
            printf(" %s ",words) ;
#endif
            if(strcmp(words,"|") == 0)
            {
                /** every time get one production */
                /** one production has over ,
                    build the symbolList's productionIndex
                    and
                    ready the next */
                int *  pListDef = (int *)symbolList.data[pLeft].productionIndex ;
                *(pListDef +(*pListDef) +1 ) = pnt->productionNum ;
                //printf("%d\n",pListDef + (*pListDef)) ;
                /* *pListDef stands for the productionIndex[0] */
                (*pListDef)++ ;

                pnt->productionNum++ ;
                pnt->data[pnt->productionNum].order = pnt->productionNum ;
                pnt->data[pnt->productionNum].len = 0 ;
                pnt->data[pnt->productionNum].pLeft = pLeft ;
                /*____*/
#ifndef OUTPUT_P
                char * tmpProduction = restoreP(pnt->productionNum -1 ) ;
                printf("%s\n",tmpProduction) ;
                free(tmpProduction) ;
#endif

            }
            else
            {
                int tmp = transName(words) ;
                if(tmp == EMPTY)
                {
                    printf("not exist symbols which named %s\n",words) ;
                    return -1 ;
                }
                Production * def = & pnt->data[pnt->productionNum] ;
                def->pRight[def->len] = tmp ;
                def->len++ ;
            }
            fscanf(productionFile,"%s",words) ;
        }
        /* add to the symbolList*/
        int *  pListDef = (int *)symbolList.data[pLeft].productionIndex ;
        *(pListDef +(*pListDef) +1 ) = pnt->productionNum ;
        (*pListDef)++ ;
        pnt->productionNum ++ ;
        /*-----------*/
#ifndef OUTPUT_P
        char * tmpProduction = restoreP(pnt->productionNum -1 ) ;
        printf("%s\n",tmpProduction) ;
        free(tmpProduction) ;
#endif
    }
    fclose(productionFile) ;
    return pnt->productionNum ;
}
/** main part*/
void addToSet(int symbol, int set[] , int *setSize)
{
    int i ;
    for(i =0  ; i < *setSize ; i++)
    {
        if(symbol == set[i])
            return ;
    }
    set[(*setSize)++] = symbol ;
}
int FIRST(int symbol , int set[] , int * setSize)
{
    if(getType(symbol) == TERMINAL)
    {
        addToSet(symbol,set,setSize) ;
        return EMPTY +1 ;
    }
    else
    {
        //find all productions
        int proNum = symbolList.data[symbol].productionIndex[0] ;
        int hasEmpty = EMPTY + 1 ;
        int k ;
        for(k = 0 ; k < proNum  ; k++)
        {
            int len = productions.data[symbolList.data[symbol].productionIndex[k+1]].len ;
            if(len == 0)
            {
                hasEmpty = EMPTY ;
            }
            else
            {
                int * pRight = productions.data[symbolList.data[symbol].productionIndex[k+1]].pRight ;
                int j = 0 ;
                while(j < len)
                {
                    if(FIRST(pRight[j],set , setSize) != EMPTY)
                    {
                        break ;
                    }
                    else
                    {
                        j++ ;
                    }
                }
                if(j == len)
                {
                    //means that all has lead to empty
                    hasEmpty = EMPTY ;
                }
            }
        }
        return hasEmpty ;
    }
}
void getFirstCollection(int * p ,int pLen , int * firstSet ,int * fsLen,int * hasEmpty)
{
    if(pLen == 0)
    {
        (*hasEmpty) = EMPTY ;
        return ;
    }
    else
    {
        if(getType(p[0]) == TERMINAL)
        {
            firstSet[0] = p[0] ;
            (*fsLen) = 1 ;
            (*hasEmpty) = 1 + EMPTY ;
        }
        else
        {
            int i = 0 ;
            while(i < pLen)
            {
                if(FIRST(p[i],firstSet,fsLen) != EMPTY)
                {
                    break ;
                }
                else
                {
                    i++ ;
                }
            }
            if(i == pLen)
            {
                (*hasEmpty) = EMPTY ;
            }
            else
            {
                (*hasEmpty) = EMPTY +1 ;
            }
        }
    }
}
int isLR1_ItemSame(LR1_Item * one , LR1_Item * two)
{
    if(one->productionOrder != two->productionOrder ||
            one->dotPos != two->dotPos ||
            one->exSymNum != two->exSymNum )
    {
        return SAME + 1 ;
    }
    else
    {
        int i ;
        for(i = 0 ; i < one->exSymNum ; i++)
        {
            int j ;
            int hasSame = SAME + 1 ;
            for(j = 0 ; j < two->exSymNum ; j++)
            {
                if(one->expectedSymbol[i] == two->expectedSymbol[j])
                {
                    hasSame = SAME ;
                    break ;
                }
            }
            if(hasSame != SAME)
            {
                return SAME +1 ;
            }
        }
        return SAME  ;
    }
}
int isStateSame(ItemCollection * one , ItemCollection * two)
{
    if(one->len != two->len)
    {
        return SAME + 1 ;
    }
    LR1_Item * oPos = one->items ;
    while(oPos != NULL)
    {
        LR1_Item * iPos = two->items ;
        while(iPos != NULL)
        {
            if(isLR1_ItemSame(iPos,oPos) == SAME)
            {
                break ;
            }
            iPos = iPos->next ;
        }
        if(iPos == NULL)
        {
            //it mean's no same as oPos
            return SAME +1 ;
        }
        oPos = oPos->next ;
    }
    return SAME ;
}
int CLOURE(ItemCollection * state)
{
#ifndef OUTPUT_LR1
    printf("I %d \n",state->order) ;
    LR1_Item * dPos = state->items ;
    while(dPos != NULL )
    {
        char * x = restoreP(dPos->productionOrder) ;
        printf("%s,%d,",x,dPos->dotPos) ;
        free(x) ;
        int di = 0 ;
        for( ; di < dPos->exSymNum ; di++)
        {
            printf("%s / ",getName(dPos->expectedSymbol[di])) ;
        }
        printf("\n") ;
        dPos = dPos->next ;
    }
#endif
    int i ;
    LR1_Item * pos = state->items ;
    while(pos != NULL)
    {
        if(pos->dotPos == productions.data[pos->productionOrder].len)
        {
            /* redece item */
            state->hasReducedItem = TRUE_ANA ;
            pos = pos->next ;
        }
        else
        {
            int symbol = productions.data[pos->productionOrder].pRight[pos->dotPos] ;
            if(getType(symbol) == TERMINAL)
            {
                /* no empty cloure */
                pos = pos->next ;
            }
            else
            {
                /* add the empty cloure of this LR1_Item to the end of the items chain */
                int pNum = symbolList.data[symbol].productionIndex[0] ;
                int j ;
                /* first get the FIRST(Ba)*/
                int firstSet[20] ;
                int firstSetSize = 0 ;
                int leftProduction[PRODUCTION_MAX_LEN] ;
                int leftProductionLen = 0;
                int hasEmpty ;
                for(j = pos->dotPos + 1 ; j < productions.data[pos->productionOrder].len ; j++)
                {
                    leftProduction[leftProductionLen++] = productions.data[pos->productionOrder].pRight[j] ;
                }
                getFirstCollection(leftProduction,leftProductionLen,firstSet,&firstSetSize,&hasEmpty) ;
                if(hasEmpty == EMPTY)
                {
                    //if has empty , add the current exSym to the new Item
                    for(j =0 ; j < pos->exSymNum ; j++)
                    {
                        addToSet(pos->expectedSymbol[j],firstSet,&firstSetSize) ;
                    }
                }
                //now can create the new Item
                for(j = 0 ; j < pNum ; j++)
                {
                    LR1_Item * newItem = (LR1_Item *)malloc(sizeof(LR1_Item)) ;
                    newItem->dotPos = 0 ;
                    newItem->productionOrder = productions.data[symbolList.data[symbol].productionIndex[j+1]].order ;
                    newItem->exSymNum = firstSetSize ;
                    int k ;
                    for(k = 0 ; k < firstSetSize ; k++)
                    {
                        newItem->expectedSymbol[k] = firstSet[k] ;
                    }
                    newItem->next = NULL ;
                    LR1_Item * testPos = state->items ;
                    while(testPos->next != NULL)
                    {
                        if(isLR1_ItemSame(testPos,newItem) == SAME)
                        {
                            free(newItem) ;
                            break ;
                        }
                        testPos = testPos->next ;
                    }
                    //because the testPos->next == NULL escape
                    if(testPos->next == NULL)
                    {
                        if(isLR1_ItemSame(testPos,newItem) != SAME)
                        {
                            testPos->next = newItem ;
#ifndef OUTPUT_LR1
                            char * x = restoreP(newItem->productionOrder) ;
                            printf("%s,%d,",x,newItem->dotPos) ;
                            int g = 0 ;
                            for( ; g < newItem->exSymNum ; g++)
                            {
                                printf("%s / ",getName(newItem->expectedSymbol[g])) ;
                            }
                            printf("\n") ;
                            free(x) ;
#endif
                            state->len++ ;
                        }
                        else
                        {
                            free(newItem) ;
                        }
                    }
                }
                pos = pos->next ;
            }
        }
    }
#ifndef OUTPUT_LR1
    printf("lenth is %d\n-----------\n",state->len) ;
#endif
    return 1 ;
}

int initItemCollection()
{
    LR1_Item * startItem = (LR1_Item*)malloc(sizeof(LR1_Item)) ;
    startItem->productionOrder = 0 ;
    startItem->dotPos = 0 ;
    startItem->exSymNum = 1 ;
    startItem->expectedSymbol[0] = transName("#") ;
    startItem->next = NULL ;
    ItemCollection * I0 = (ItemCollection * )malloc(sizeof(ItemCollection)) ;
    I0->items = startItem ;
    I0->len = 1 ;
    I0->order = 0 ;
    I0->hasReducedItem = FALSE_ANA ;
    ic.icNum = 0 ;
    ic.data[ic.icNum++] = I0 ;
    CLOURE(I0) ;
    /*ready the goList , reduceList */
    goHead = NULL ;
    goRear = NULL ;
    reduceHead = NULL ;
    reduceRear = NULL ;
    //create the successive item array
    struct SucItem
    {
        int readSymbol ;
        LR1_Item * sucItems ;
        int itemNum ;
    } ;
    struct SucItemList
    {
        struct SucItem data[SUCCESSIVE_ITEM_MAX_LEN] ;
        int siNum ;
    } si ;
    //
    int i ;
    for(i = 0 ; i < ic.icNum ; i++)
    {
        //first , init the successive Item array for current ItemCollection
        si.siNum = 0 ;
        /* for each itemCollection ,
            get successive item of each items in current itemCollection
             if the item has successicve item
        */
        ItemCollection * current = ic.data[i] ;
        LR1_Item * pos = current->items ;
        while(pos != NULL)
        {
            if(pos->dotPos == productions.data[pos->productionOrder].len)
            {
                /* redeced item*/
                if(reduceHead != NULL)
                {
                    struct ReduceList * newRL = (struct ReduceList *)malloc(sizeof(struct ReduceList)) ;
                    newRL->next = NULL ;
                    newRL->state = i ;
                    newRL->reducedItem = pos ;
                    reduceRear->next = newRL ;
                    reduceRear = newRL ;
                }
                else
                {
                    reduceHead = (struct ReduceList *)malloc(sizeof(struct ReduceList)) ;
                    reduceHead->state = current->order ;/* that is i */
                    reduceHead->reducedItem = pos ;
                    reduceHead->next = NULL ;
                    reduceRear = reduceHead ;
                }
                pos = pos->next ;
            }
            else
            {
                /* it has successive item */
                /* new a LR1_Item , which is this item's successive item*/
                LR1_Item * newItem = (LR1_Item *)malloc(sizeof(LR1_Item)) ;
                newItem->dotPos = pos->dotPos+1 ;
                newItem->productionOrder = pos->productionOrder ;
                newItem->exSymNum = pos->exSymNum ;
                int j ;
                for(j = 0 ; j < newItem->exSymNum ; j++)
                {
                    newItem->expectedSymbol[j] = pos->expectedSymbol[j] ;
                }
                newItem->next = NULL ;
                int readSymbol = productions.data[pos->productionOrder].pRight[pos->dotPos] ;
                int hasFound = FALSE_ANA ;
                /* add the successive to the si */
                for(j = 0 ; j < si.siNum ; j++)
                {
                    if(readSymbol == si.data[j].readSymbol)
                    {
                        /* in si has the same symbol */
                        newItem->next = si.data[j].sucItems ;
                        si.data[j].sucItems = newItem ;
                        si.data[j].itemNum++ ;
                        hasFound = TRUE_ANA ;
                        break ;
                    }
                }
                if(hasFound == FALSE_ANA)
                {
                    //si has not the symbol
                    //create one and add it
                    si.data[si.siNum].readSymbol = readSymbol ;
                    si.data[si.siNum].sucItems = newItem ;
                    si.data[si.siNum].itemNum = 1 ;
                    si.siNum++ ;
                }
                pos = pos->next ;
            }
        }
#ifndef OUTPUT_SI
        int d_i ;
        printf("siNum = %d\n",si.siNum) ;
        for(d_i = 0 ; d_i < si.siNum ; d_i++)
        {
            printf("readSymbol:%s\n",getName(si.data[d_i].readSymbol)) ;
            LR1_Item * dPos = si.data[d_i].sucItems ;
            while(dPos != NULL)
            {
                printf("----%s,exSymNum:%d\n",restoreP(dPos->productionOrder),dPos->exSymNum) ;
                dPos = dPos->next ;
            }
        }
#endif
        //and then , for each successive item , get is's empty cloure
        int x ;
        for(x = 0 ; x < si.siNum ; x++)
        {
            ItemCollection * newIC = (ItemCollection *) malloc(sizeof(ItemCollection)) ;
            newIC->len = si.data[x].itemNum ;
            newIC->items = si.data[x].sucItems ;
            newIC->order = ic.icNum ;
            //order has not init
            CLOURE(newIC) ;
            int k ;
            for(k = 0 ; k < ic.icNum ; k++)
            {
                if(isStateSame(newIC,ic.data[k]) == SAME)
                {
                    //do not add the IC , but goList should be update
                    if(goHead != NULL)
                    {
                        struct GOList * newGOL = (struct GOList *)malloc(sizeof(struct GOList)) ;
                        newGOL->startState = i ;
                        newGOL->gotoState = k ;
                        newGOL->symbol = si.data[x].readSymbol ;
                        newGOL->next = NULL ;
                        goRear->next = newGOL ;
                        goRear = newGOL ;
                    }
                    else
                    {
                        goHead = (struct GOList *)malloc(sizeof(struct GOList)) ;
                        goHead->startState = i ;
                        goHead->gotoState = k ;
                        goHead->symbol = si.data[x].readSymbol ;
                        goHead->next = NULL ;
                        goRear = goHead ;
                    }
                    free(newIC) ;
                    break ;
                }
            }
            if(k == ic.icNum)
            {
                //mean newIC is new one ,add
                ic.data[ic.icNum] = newIC ;
                newIC->order = ic.icNum ;
                // add this to the GOList
                if(goHead != NULL)
                {
                    struct GOList * newGOL = (struct GOList *)malloc(sizeof(struct GOList)) ;
                    newGOL->startState = i ;
                    newGOL->gotoState = ic.icNum ;
                    newGOL->symbol = si.data[x].readSymbol ;
                    newGOL->next = NULL ;
                    goRear->next = newGOL ;
                    goRear = newGOL ;
                }
                else
                {
                    goHead = (struct GOList *)malloc(sizeof(struct GOList)) ;
                    goHead->startState = i ;
                    goHead->gotoState = ic.icNum ;
                    goHead->symbol = si.data[x].readSymbol ;
                    goHead->next = NULL ;
                    goRear = goHead ;
                }
                ic.icNum++ ;
            }
        }
    }
#ifndef OUTPUT_IC
    printf("\n%d",ic.icNum) ;
#endif
#ifndef OUTPUT_GOLIST
    struct GOList * gdpos = goHead ;
    while(gdpos != NULL)
    {
        printf("%s,%d->%d\n",getName(gdpos->symbol),gdpos->startState,gdpos->gotoState) ;
        gdpos = gdpos->next ;
    }
#endif
#ifndef OUTPUT_REDUCE
    struct ReduceList * rdpos = reduceHead ;
    while(rdpos != NULL)
    {
        printf("%d,%s\n",rdpos->state,restoreP(rdpos->reducedItem->productionOrder));
        rdpos = rdpos->next ;
    }
#endif
}
int initAnalysisTable()
{
    if(initTransTableAndSymbolList() == -1)
    {
        return -1 ;
    }
    if(initProduction() == -1)
    {
        return -1 ;
    }
    initItemCollection() ;
    analysisTable.row = ic.icNum ;
    analysisTable.col = symbolList.symbolNum ;
    int k , l ;
    for(k = 0 ; k < analysisTable.row ; k++)
    {
        for(l = 0 ; l < analysisTable.col ; l++)
        {
            analysisTable.table[k][l] = ERROR ;
        }
    }
    /**
        if shift , the num is positive or 0 ,stands for the state to goto
        if reduce , the num is negative , stands for the production order which reduced by
        if ACC , use MACRO ,which is expressed by a big positive num
        if error , use MACRO , which is expressed by a small negative num(abstract is large)
    */
    struct GOList * gpos = goHead ;
    while(gpos != NULL)
    {
        analysisTable.table[gpos->startState][gpos->symbol] = gpos->gotoState ;
        gpos = gpos->next ;
    }
    struct ReduceList * rpos = reduceHead ;
    while(rpos != NULL)
    {
        int i ;
        for(i = 0 ; i < rpos->reducedItem->exSymNum ; i++)
        {
            analysisTable.table[rpos->state][rpos->reducedItem->expectedSymbol[i]] = - rpos->reducedItem->productionOrder ;
            if(rpos->reducedItem->productionOrder == 0)
            {
                #ifdef SYNTAX
                printf("find ACC\n") ;
                #endif
                analysisTable.table[rpos->state][rpos->reducedItem->expectedSymbol[i]] = ACC ;
            }
        }
        rpos = rpos->next ;
    }
#ifdef OUPUT_ANALYSISTABLE_FILE
    FILE * anaFile = fopen("analysisTable.txt","w") ;

    if(!anaFile)
    {
        printf("can not open file 'analysisTable.txt\n'") ;
        return -1 ;
    }
    char * x = " " ;
    fprintf(anaFile,"%10s",x) ;
    for(k = 0 ; k < analysisTable.col ; k++)
    {
        fprintf(anaFile,"%20s",getName(k)) ;
    }
    fprintf(anaFile,"\n") ;
    for(k = 0 ; k < analysisTable.row ; k++)
    {
        fprintf(anaFile,"%10d",k) ;
        for(l = 0 ; l < analysisTable.col ; l++)
        {
            int tVal = analysisTable.table[k][l] ;
            if(tVal == ERROR)
            {

                fprintf(anaFile,"%20s",x) ;
            }
            else if(tVal >= 0 && tVal != ACC)
            {
                fprintf(anaFile,"%20d",analysisTable.table[k][l]) ;
            }
            else if(tVal < 0 )
            {
                char * s = restoreP(-tVal) ;
                fprintf(anaFile,"%20s",s) ;
                free(s) ;
            }
            else
            {
                fprintf(anaFile,"ACC") ;
            }
        }
        fprintf(anaFile,"\n") ;
    }
#endif
    return 0 ;
}
