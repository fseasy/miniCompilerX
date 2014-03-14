#ifndef DBUFFER_H_INCLUDED
#define DBUFFER_H_INCLUDED

#define DBUF_SIZE (1024*2)
/**
    attention : Macro is just char replace !! so
                3%DBUF_SIZE != 3%6 but = 3%3*2 !!
                so add brackets
*/
typedef struct DBuffer
{
    char * buf ;
    int head ;
    int rear ;
    int hasBack ;
    FILE * fp ;
}DBuffer ;

int initDBuffer(DBuffer * dbuffer , char * fileName)
{
    dbuffer->buf = (char * )malloc(sizeof(char)*DBUF_SIZE) ;
    dbuffer->buf[DBUF_SIZE/2-1] = EOF ;
    dbuffer->buf[DBUF_SIZE -1] = EOF ;
    dbuffer->head = 0 ;
    dbuffer->rear = 0 ;
    dbuffer->hasBack = FALSE ;
    if(!(dbuffer->fp = fopen(fileName,"r")))
    {
        printf("can not open the file named %s \n",fileName) ;
        return FALSE ;
    }
    else
    {
        //load the front part buffer
        char c ;
        int i ;
        for(i = 0 ; (i < DBUF_SIZE/2 -1 )&& c != EOF ; i++ )
        {
            c = getc(dbuffer->fp) ;
            dbuffer->buf[i] = c ;
        }
        //the EOF has been stored in buf even come into EOF
        return TRUE ;
    }
}
void deleteDBuffer(DBuffer * dbuffer)
{
        //free(dbuffer->buf) ;
        free(dbuffer) ;
        dbuffer = NULL ;
}
char getChar(DBuffer * dbuffer)
{
    char c = dbuffer->buf[dbuffer->rear] ;
    if( c != EOF)
    {
        dbuffer->rear++ ;
        return c ;
    }
    else
    {
        //we need to skip the EOF ,and the rear should still pointer the next
        //so rear should + 2
        if(dbuffer->hasBack == FALSE ) //haven't  back
        {
            //now we need to get the behind buffer
            if( (c = getc(dbuffer->fp) )!= EOF) //file hasn't get to the end ,
            {
                if(dbuffer->rear < DBUF_SIZE/2)
               {
                    //fill the behind part
                    int i = DBUF_SIZE/2 ;
                    dbuffer->buf[i++] = c ;
                    for( ; ( i < DBUF_SIZE -1 )&& c != EOF ; i++ )
                    {
                        c = getc(dbuffer->fp) ;
                        dbuffer->buf[i] = c ;
                    }
                    dbuffer->rear = DBUF_SIZE/2 +1 ;
                    return dbuffer->buf[DBUF_SIZE/2] ; //jump the EOF of center ,at DBUF_SIZE -1
               }
               else
               {
                   //at the front
                    int i = 0 ;
                    dbuffer->buf[i++] = c ;
                    for( ; i < DBUF_SIZE/2 -1 && c != EOF ; i++)
                    {
                        c = getc(dbuffer->fp) ;
                        dbuffer->buf[i] = c ;
                    }
                    dbuffer->rear = 1 ;
                    return dbuffer->buf[0] ;
               }
            }
            else
            {
                //the file and the buffer all has went to the end
                return EOF ;
            }
        }
        else
        {
            //has backed , not need to fill the buffer
            int tmppos = (dbuffer->rear + 1)% DBUF_SIZE ;
            //printf("tmppos =%d,%d\n",tmppos,(dbuffer->rear)%(DBUF_SIZE) ;
            dbuffer->rear = (dbuffer->rear+2)%DBUF_SIZE ;
            dbuffer->hasBack = FALSE ;
            return dbuffer->buf[tmppos] ;
        }
    }
}

void readyCopy(DBuffer * dbuffer)
{
    /*let the head behind the rear
      the rear may be in the 1 -> DBUF_SIZE/2 -1 ,or DBUF_SIZE/2 +1 -> DBUF_SIZE -1
      so just return rear -1*/
      dbuffer->head = dbuffer->rear -1 ;
}
/**
    return the length of token
*/
int copyToken(DBuffer * dbuffer , char * token)
{
        /*from the head ,to the rear -1
        need to consider the EOF may at the center
        */
        int i = dbuffer->head ,
            index = 0 ;
        for( ; i < dbuffer->rear ; i++)
        {
            if(dbuffer->buf[i] != EOF)
            {
                token[index++] = dbuffer->buf[i] ;
            }
        }
        token[index] = '\0' ;
        return index ;
}
void retract(DBuffer * dbuffer , int num)
{
    int i ;
    /*
        2 special position , DBUF_SIZE/2 + 1 , 1
        at the 2 pos , we - 2 to move it to EOF , to fit the getChat() function
    */
    for(i = 0 ; i < num ; i++)
    {
        if(dbuffer->rear != 1 && dbuffer->rear != DBUF_SIZE/2 +1)
        {
            --dbuffer->rear ;
        }
        else if(dbuffer->rear == 1)
        {
            dbuffer->rear = DBUF_SIZE -1 ;
            dbuffer->hasBack = TRUE ;
        }
        else
        {
            dbuffer->rear = DBUF_SIZE/2 -1 ;
            dbuffer->hasBack = TRUE ;
        }
    }
}
#endif // DBUFFER_H_INCLUDED
