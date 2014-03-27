#ifndef NAMETABLE_H_INCLUDED
#define NAMETABLE_H_INCLUDED

/**
*实现 支持程序块结构 的符号表，即区分作用域
*
*/
typedef struct TokeExtend
{

}TokenExtend ;
typedef struct TokenInfo
{
    char * name ; /*the token name ,if the type is constant,it's just the value    */
    int tokenType ;  /* the Token type,such as VAR , ARRAY ,which is defined by Macro  */
    int dataType ; /* the data type to store ,such as INT , CHAR , FLOAT and so on*/
    int Addr ;  /* the token's address in the memory ,may be it's not the real*/
    TokenExtend extend ; /* extend info for some token type ,such as ARRAY*/
    int hasDeclar ; /** !! to show this token has been declared before use it */
}TokenInfo ;
typedef struct NameTableItem
{
    TokenInfo tokenInfo ; /*main body*/
    struct nameTableItem * nxtItem ; /* to link the next*/
} NameTableItem ;
typedef struct NameTable
{
    //assist
    struct NameTable * parent ; /* link the parent name table*/
    struct NameTable * children[CHILD_NUM] ; /* link the children*/
    int childrenNum ;
    //body
    NameTableItem * nodeList[NODE_NUM] ; /* hash List*/
    int nodeNum ; /* the item has been inserted */
    char * nameString ;
}NameTable ;

NameTable* getNameTable(NameTable * parent)
{
    NameTable * nameTable = NULL ;
    int i ;
    nameTable = (NameTable *)malloc(sizeof(NameTable)) ;
    nameTable->nameString = (char*)malloc(sizeof(char)*NAME_STRING_LEN) ;
    for(i = 0 ; i < NODE_NUM ; i++)
    {
        nameTable->nodeList[i] = NULL ;
    }
    nameTable->parent = parent ;
    for(i = 0 ; i < CHILD_NUM ; i++)
    {
        nameTable->children[i] = NULL ;
    }
    for(i = 0 ; i < NAME_STRING_LEN ;i++)
    {
        nameTable->nameString[i] = '\0' ;
    }
    nameTable->nodeNum = 0 ;
    nameTable->childrenNum = 0 ;
    return nameTable ;
}

NameTableItem * getNameTableItem()
{
    NameTableItem * newItem =  (NameTableItem * )malloc(sizeof(NameTableItem)) ;
    newItem->nxtItem = NULL ;
    return newItem ;
}
int _insertIntoNameTable(NameTable  *  nameTable , NameTableItem ** preItem , char * tokenName)
{
    NameTableItem * newItem = getNameTableItem() ;
    if(*preItem == NULL)
    {
        /* it is the head*/
        (*preItem) = newItem ;
    }
    else
    {
        (*preItem)->nxtItem = newItem ;
    }
    strcpy(nameTable->nameString,tokenName) ;
    newItem->tokenInfo.name = nameTable->nameString ;
    newItem->tokenInfo.hasDeclar = FALSE ;
    nameTable->nameString += strlen(tokenName) ;
    nameTable->nodeNum ++ ;
    return (int)newItem ;
}
/* at the token_scan , we just know the name of the token */
/* return value : success :the Address of the toke
*                  fail  :  -1 */
int createAndInsertItem(NameTable * nameTable , char * tokenName )
{
    int hashCode ;
    hashCode = getHashCode(tokenName) ;
    //to know if the tokenName is legal , it should have been checked for key word
    NameTableItem ** pos = & nameTable->nodeList[hashCode] ;
    while(*pos != NULL)
    {
        if(strcmp(tokenName,(*pos)->tokenInfo.name) == 0)
        {
            /* return -1 ;  *//* some days i want to return -1 to
                                avoid the dulpication definition ,
                                but it is not correct ,because it may
                                be assign words instead of definition */
            return *pos ;
        }
        pos = &((*pos)->nxtItem) ;
    }
   return _insertIntoNameTable(nameTable,pos,tokenName) ;

}

#endif // NAMETABLE_H_INCLUDED
