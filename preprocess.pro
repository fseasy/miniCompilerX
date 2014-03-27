#ifndef PREPROCESS_H_INCLUDED
#define PREPROCESS_H_INCLUDED
int preProcess(char * oriFileName , char * processedFileName)
{
FILE * oriFp, * processedFp ;
 processedFileName[0] = '\0' ;
char * pointedPos = strchr(oriFileName,'.') ;
if(pointedPos)
{
strncpy(processedFileName,oriFileName,pointedPos-oriFileName) ;
processedFileName[pointedPos-oriFileName] ='\0' ;
}
strcat(processedFileName,".pro") ;

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
putchar(c) ;
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

state = WHITE_SPACE ;
putc('\n',processedFp) ;
break ;
 
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
}
break ;
case COM_S :
{
switch(c)
{
case '\r' :
case '\n' :
state =CODE ;
break ; 
default :
break ;
}
}
break ;
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
}
break ;
case WHITE_SPACE :
{
switch(c)
{
case ' ' :
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
}
break ;
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
#endif 