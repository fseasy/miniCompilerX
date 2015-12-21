#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define DEMO
int main()
{
    char srcName[20] = "test1.x" ;
    char outName[20] = "" ;
    int errc ;
    #ifdef DEMO
    printf("\n----lexical analysis start----\n") ;
    #endif
    errc = lexAnalysis(srcName,outName) ;
    #ifdef DEMO
    printf("----lexical analysis finished----\n") ;
    printf("\t\nAt lexical analysis , find %d errors\n\n-----\n",errc) ;
    #endif

    #ifdef DEMO
    clock_t start = clock() ;
    while(clock() - start <= 1000) ;
    system("CLS") ;
    printf("\n----syntax and semantic analysis start----\n") ;
    start = clock() ;
    while(clock() - start <= 1000) ;
    #endif
    char * argv[2] ;
    argv[1] = outName ;
    if(startSyntaxAnalysis(2,argv) == 0)
    {
        #ifdef DEMO
        printf("\n----all finished , 3-addr code has generated ----\n") ;
        #endif
    }
    else
    {
        printf("\n----syntax or semantic analysis Error!----\n") ;
    }

    printf("\n\npress Enter to exit .\n") ;
    getchar() ;
    return 0;
}
