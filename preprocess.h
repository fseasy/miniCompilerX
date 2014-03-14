#ifndef PREPROCESS_H_INCLUDED
#define PREPROCESS_H_INCLUDED
/**
    finish the pre-process the source file
*/
int preProcess(char * oriFileName , char * processedFileName)
{
    FILE * oriFp, * processedFp ;
    //make the fileName
    processedFileName[0] = '\0' ;
    char * pointedPos = strchr(oriFileName,'.')  ;
    if(pointedPos)
    {
        strncpy(processedFileName,oriFileName,pointedPos-oriFileName) ;
        processedFileName[pointedPos-oriFileName] ='\0' ;
    }
    strcat(processedFileName,".pro") ;
    //fp

    if(!(oriFp = fopen(oriFileName,"r")) ||!(processedFp = fopen(processedFileName,"w")) )
    {
        printf("can't open file !") ;
        return -1 ;
    }
    char c ;
    int state = CODE ;
    char queue[10] ;
    int queueNum = 0 ;
    while((c = getc(oriFp)) != EOF)
    {
        switch(state)
        {
            case CODE :
            {
                switch(c)
                {
                    case ' ' :
                    case '\t':
                        state = WHITE_SPACE ;
                        putc(' ',processedFp) ;
                        break ;
                    case '\r' :
                    case '\n' :
                        /*
                        fseek(oriFp,-2L,SEEK_CUR) ;
                        c = getc(oriFp) ;
                        printf("c is %c\n",c) ;
                        if(c != ';' && c != ' ' && c != '\t')
                        */
                        state = WHITE_SPACE ;
                        putc('\n',processedFp) ;
                        break ;
                        //fseek(oriFp,1,SEEK_CUR) ;//move the file pointer
                        /** give up the process of the \r\n ,in text mode ,it will
                            be transform to \n ,but use seek ,it doesn't process it
                            auto ,so if it is \r\n ,shoud back -3 ,if is \n or \r ,
                            should -2 , it is troublesome
                            so ,just put whitespace
                        */

                    case '"' :
                        state = STR ;
                        putc(c , processedFp) ;
                        break ;
                    case '\'' :
                        state = P_CHAR ;
                        putc(c, processedFp) ;
                        break ;
                    case '/' :
                        state = COM_READY ;
                        queue[queueNum++] = c ;
                        break ;
                   default :
                        putc(c , processedFp) ;
                }
                break ;
            }
            case COM_READY :
            {
                switch(c)
                {
                    case '/' :
                        queueNum = 0 ;
                        state = COM_S ;
                        break ;
                    case '*' :
                        queueNum = 0 ;
                        state = COM_M ;
                        break ;
                    default :
                        putc(queue[0],processedFp) ;
                        queueNum = 0 ;
                        putc(c,processedFp) ;
                        //there we should switch the char ,' '-> space , '"' -> str,other to code
                        //in fact if c is not the / or * ,it is not legal ,but ignore it with warning
                        switch(c)
                        {
                            case ' ' :
                            case '\t':
                                state = WHITE_SPACE ;
                                break ;
                            case '"' :
                                state = STR ;
                                break ;
                            default :
                                state = CODE ;
                        }
                        printf("warning ! afer '/' has character %d \n",(int)c) ;
                        break ;
                }
                 break ;
            }
            case COM_S :
            {
                switch(c)
                {
                    case '\r' :
                    case '\n' :
                        state =CODE ;
                        break ; /*becase CODE state does not process '\n' ,so we do not need to know the ENTER is the \r or \r\b or \n*/
                    default :
                        break ;
                }
                 break ;
            }
            case COM_M :
            {
                switch(c)
                {
                    case '*' :
                        state = COM_M_WAIT_STOP ;
                        break ;
                    default :
                        break ;
                }
            }
                break ;
            case COM_M_WAIT_STOP :
            {
                switch(c)
                {
                    case '/' :
                        state = CODE ;
                        break ;
                    default :
                        state = COM_M ;
                        break ;
                }
                break ;
            }
            case WHITE_SPACE :
            {
                switch(c)
                {
                    case ' '  :
                    case '\t' :
                    case '\r' :
                    case '\n' :
                        break ;
                    case '/' :
                        state = COM_READY ;
                        break ;
                    case '"' :
                        state = STR ;
                        putc(c,processedFp) ;
                        break ;
                    case '\'' :
                        state = P_CHAR ;
                        putc(c , processedFp) ;
                        break ;
                    default :
                        state = CODE ;
                        putc(c,processedFp) ;
                }
                 break ;
            }
            case STR :
            {
                switch(c)
                {
                    case '\\' :
                        state = STR_ESCAPE ;
                        putc(c,processedFp) ;
                        break ;
                    case '"' :
                        state = CODE ;
                        putc(c,processedFp) ;
                        break ;
                    default :
                        putc(c,processedFp) ;
                        break ;
                }
                 break ;
            }
            case P_CHAR :
            {
                switch(c)
                {
                    case '\'' :
                        state = CODE ;
                        putc(c , processedFp) ;
                        break ;
                    case '\\' :
                        state = CHAR_ESCAPE ;
                        putc( c, processedFp) ;
                        break ;
                    default :
                        putc(c ,processedFp) ;
                        break ;
                }
                break ;
            }
            case STR_ESCAPE :
            {
                putc(c,processedFp) ;
                state = STR ;
                break ;
            }
            case CHAR_ESCAPE :
            {
                state = P_CHAR ;
                putc(c,processedFp) ;
                break ;
            }
            default :
                printf("Error at pre-process\n") ;
                return FALSE ;
        }
    }
    fclose(oriFp) ;
    fclose(processedFp) ;
    return TRUE ;
}

#endif // PREPROCESS_H_INCLUDED
