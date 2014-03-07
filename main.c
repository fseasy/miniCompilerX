#include <stdio.h>
#include <stdlib.h>
#include "constant.h"
#include "nameTable.h"
#include "hash.h"
#include "preprocess.h"
int main()
{
    char str[20] = "nameTable.h" ;
    printf("%d",getHashCode(str));
    NameTable * cur = getNameTable(NULL) ;
    int x = createAndInsertItem(cur,str) ;
    printf("\n%d\n",x) ;
    char fileName[20] ;
    preProcess(str,fileName) ;
    printf("%s\n",fileName) ;
    return 0;
}
