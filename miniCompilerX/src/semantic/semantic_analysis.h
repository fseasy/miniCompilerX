#ifndef SEMANTIC_ANALYSIS_H_INCLUDED
#define SEMANTIC_ANALYSIS_H_INCLUDED

#define SEMANTIC_STACK_DEPTH 1000
#define TB_NODE_MAX_SIZE 200
#define TB_MAX_NUM 40
#define LABEL_MAX_NUM 10
#define VAL_MAX_LEN 500 //the val's len of lexcal's output file
struct SemanticNode
{
    /* it is the type at lexical result */
    char lexType[25] ;
    /* it is the value at the lexical result */
    char lexVal[25] ;
    /* it is the variable's name at symbol table*/
    char tbName[20] ;
    /* extend , for addr , for param num , the STRING_C addr */
    void * extend ;
} ;
struct SemanticStack
{
    struct SemanticNode data[SEMANTIC_STACK_DEPTH] ;
    int top ;
} semanticS ;
/* symbol table*/
struct tbNode
{
    char name[20] ;
    char type[16] ;
    void * addr ;
} ;
struct symbolTb
{
    struct symbolTb * previous ;
    int width ;
    int counter ;
    struct tbNode * data[TB_NODE_MAX_SIZE] ;
} ;
/* offset stack and tbptr stack */
struct OffsetStack
{
    void * data[TB_MAX_NUM] ;
    int top ;
} offsetS ;
struct TbptrStack
{
    struct symbolTb  * data[TB_MAX_NUM] ;
    int top ;
} tbptrS ;
/* Label */
struct LbS
{
    char data[LABEL_MAX_NUM][10] ; //that means max 9999 labels
    int top ;
} ;
struct LbptrStack
{
    struct LbS * data[TB_MAX_NUM] ;
    int top ;
} lbptrS ;


void initSemanticS() ;
int semanticAct(int pdtIndex) ;
void shiftSemanticAct(char lecType[] , char lexVal[]) ;
char * decideType(char *type1 , char *type2) ;
/*tb operator */
struct symbolTb * mkTb(struct symbolTb * previous) ;
/* return the addr */
void * enterTb(struct symbolTb * tb , char * name ,char * type , void * offset) ;
int getSize(char * type) ;
/* return the addr of the tbNode  or null */
struct tbNode * lookupTb(struct symbolTb * tb , char * name) ;

/* offset stack , tbptr stack op */
void initOffsetS() ;
void initTbptrS() ;

/* newtmp */
char * getTmpName() ;

/*label*/
void initLbptrS() ;
void createLabel(struct LbS * lbS , int labelNum) ;
#endif // SEMANTIC_ANALYSIS,H_H_INCLUDED
