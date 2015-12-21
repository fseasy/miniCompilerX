#ifndef ANALYSIS_TABLE_H_INCLUDED
#define ANALYSIS_TABLE_H_INCLUDED

#include <string.h>

#define EMPTY -1
#define SAME 0
#define TRUE_ANA 1
#define FALSE_ANA -1
#define ACC (0x10000000)
#define ERROR (- 0x10000000)
#define TERMINAL 0
#define NON_TERMINAL 1
#define SYMBOL_NAME_MAX_LEN 25
#define PRODUCTION_MAX_LEN 15
#define EXPECTED_SYMBOL_MAX_LEN 30
#define SUCCESSIVE_ITEM_MAX_LEN 50
#define SYMBOL_MAX_NUM 100
#define STATE_MAX_NUM 1000
#define PRODUCTION_MAX_NUM 140
#define SYMBOL_HASH_LEN 457

/** symbol , tell the num what stands for
    what's more ,store the producitons !! can highly improve the speed
*/
typedef struct SymbolNumStandsFor
{
    char name[SYMBOL_NAME_MAX_LEN] ;
    short type ;
    /* attention ! the productionIndex[0] stands for the number of the producion*/
    int productionIndex[11] ;
} SymbolNumStandsFor ;
/** symbol , tell the symbol which named 'name' expressed by numExp*/
typedef struct SymbolNumExp
{
    int numExp ;
    char name[SYMBOL_NAME_MAX_LEN] ;
} SymbolNumExp ;
/** production */
typedef struct Production
{
    int pLeft ;
    int pRight[PRODUCTION_MAX_LEN] ;
    int order ;/* the order of the production */
    int len ; /* the length of the production */
} Production ;
/** LR(1) item */
typedef struct LR1_Item
{
    int productionOrder ;
    int dotPos ;
    int expectedSymbol[EXPECTED_SYMBOL_MAX_LEN] ;
    int exSymNum ;/* the expected symbol num */
    struct LR1_Item * next ;
} LR1_Item ;
/** state / item Collection */
typedef struct ItemCollection
{
    int len ;
    int order ;
    short hasReducedItem ;
    LR1_Item * items ;
} ItemCollection ;
/** symbols list */
struct SymbolList
{
    SymbolNumStandsFor data[SYMBOL_MAX_NUM] ;
    int symbolNum ;
}
symbolList ;
/** the table , for tranalating  the symbol to the number*/
SymbolNumExp transTable[SYMBOL_HASH_LEN] ;
/** production */
typedef struct SProduction
{
    Production data[PRODUCTION_MAX_NUM] ;
    int productionNum ;
} SProduction ;
SProduction productions ;
/** itemCollections */
typedef struct SItemCollection
{
    ItemCollection * data[STATE_MAX_NUM] ; /* conveniente*/
    int icNum ;
} SItemCollection ;
SItemCollection ic ;
/** Alalysis Table */
typedef struct AnalysisTable
{
    int table[STATE_MAX_NUM][SYMBOL_MAX_NUM] ;
    int row ;
    int col ;
} AnalysisTable ;
AnalysisTable analysisTable ;

struct GOList
{
        int startState ;
        int gotoState ;
        int symbol ;
        struct GOList * next ;
} * goHead , * goRear ;
struct ReduceList
{
    int state ;
    LR1_Item * reducedItem ;
    struct ReduceList * next ;
} * reduceHead , * reduceRear ;
#endif // ANALYSIS_TABLE_H_INCLUDED
