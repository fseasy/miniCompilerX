#ifndef KEYWORD_H_INCLUDED
#define KEYWORD_H_INCLUDED
/**
    though the keywords number is less ,
    but we use a big array to store it !
    it can decrease the conflict and so increase the effictive of
    looking up key word (because get 'null' the paobability will more)
    and because it is stored in pointer , memory has not been waste
    too much
*/
#define KEYWORD_NUM 500
#define KEYWORD_NUM_PRIME 499

typedef struct Word
{
    char name[15] ;
    struct Word * nxt ;
}Word ;
typedef struct KeyWord
{
    Word * words[KEYWORD_NUM] ;
}KeyWord ;
void initKeyWord(KeyWord * keyWord)
{
    int i ;
    for(i = 0 ; i < KEYWORD_NUM ; i++)
    {
        keyWord->words[i] = NULL ;
    }
}
/**
    向keyword 中插入关键字，由hash确定
    如果无冲突，返回hashCode，否则返回-hashCode
*/
int insertKeyWord(KeyWord * keyWord , char name[])
{
    int index = getHashCode((char *)name) % KEYWORD_NUM_PRIME ;
    if(keyWord->words[index] == NULL)
    {
        keyWord->words[index] = (Word *)malloc(sizeof(Word)) ;
        keyWord->words[index]->nxt = NULL ;
        strcpy(keyWord->words[index]->name,name) ;
        /* capitalize*/
        return index ;
    }
    else
    {
        Word * pos = keyWord->words[index] ;
        while(pos->nxt != NULL)
        {
            pos = pos->nxt ;
        }
        pos->nxt = (Word *)malloc(sizeof(Word)) ;
        pos = pos->nxt ;
        strcpy(pos->name,name) ;
        pos->nxt = NULL ;
        return -index ;
    }
}
/**
    fgets will get line with end '\n'!!
    we should clear it
*/
void insertKeyWordFromFile(KeyWord * keyWord , FILE * fp)
{
    char keywordName[20] ;
    while(fgets(keywordName,20 ,fp) != NULL)
    {
        if(keywordName[strlen(keywordName) -1] == '\n')
        {
            keywordName[strlen(keywordName) -1] = '\0' ;
        }
        int x =insertKeyWord(keyWord , keywordName) ;
        printf("%d,%s\n",x,keywordName) ;

    }
}
/**
    look up name in struct keyWord ,
    if it is keyword , return true
    else return false

*/
int lookUpKeyWord(KeyWord * keyWord , char name[] )
{
    int index = getHashCode(name) % KEYWORD_NUM_PRIME ;
    if(keyWord->words[index] == NULL)
    {
        /**
            this is the reason why the array size is too large
        */
        return FALSE ;
    }
    else
    {
        Word * pos = keyWord->words[index] ;
        while(pos != NULL)
        {
            if(strcmp(pos->name,name) == 0)
            {
                return TRUE ;
            }
            pos = pos->nxt ;
        }
        return FALSE ;
    }
}

void capitalize(char * str)
{
    int i ;
    for(i = 0 ; str[i] != '\0' ; i++)
    {
        str[i] = toupper(str[i]) ;
    }
}
#endif // KEYWORD_H_INCLUDED
