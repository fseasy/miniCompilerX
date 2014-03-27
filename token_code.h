#ifndef TOKE_CODE_H_INCLUDED
#define TOKE_CODE_H_INCLUDED

//so much defines for token code
//key_word
//#define AUTO 1
#define BREAK 2
//#define CASE 3
#define CHAR 4
//#define CONST 5
#define CONTINUE 6
//#define DEFAULT 7
//#define DO 8
#define DOUBLE 9
#define ELSE 10
//#define ENUM 11
//#define EXTERN 12
//#define FLOAT 13
#define FOR 14
//#define GOTO 15
#define IF 16
#define INT 17
//#define LONG 18
//#define REGISTER 19
#define RETURN 20
//#define SHORT 21
//#define SIGNED 22
//#define SIZEOF 23
//#define STATIC 24
//#define STRUCT 25
//#define SWITCH 26
//#define TYPEDEF 27
//#define UNION 28
//#define UNSIGNED 29
#define VOID 30
//#define VOLATILE 31
//#define WHILE 32
#define BOOL 80
#define B_TRUE 81
#define B_FALSE 82
#define PRINTF 83
#define SCANF 84
#define MAIN 85

//operator
#define LR_BRAC 33 /* ( */
#define RR_BRAC 34 /* ) */
#define LS_BRAC 35 /* [ */
#define RS_BRAC 36 /* ]  */
//#define STR_PNT_DEF 37 /*  -> */
//#define STR_DEF 38      /* .   */
#define NOT 39    /* ! */
//#define B_NOT 40    /* ~ */
#define INC 41      /* ++ */
#define DEC 42      /* -- */
#define ADD 43      /* +  */
#define SUB 44      /* -  */
#define MUL_OR_INDIR 45 /* * */
#define REFERENCE 46 /* & */
//#define CAST 47         /*  (cast) */
//#define SIZEOF 48       /*  sizeof */
#define DIV 49          /*  /   */
//#define MOD 50      /*    %  */
//#define B_LEFT 51   /*    <<   */
//#define B_RIGHT 52  /*    >> */
#define LT 53   /* < */
#define LE 54   /* <=*/
#define GE 55   /* >= */
#define GT 56   /* > */
#define EQ 57   /*  == */
#define NEQ 58  /*  != */

//#define B_XOR 60 /* ^ */
//#define B_OR 61 /*  | */
#define AND 62    /*  &&  */
#define OR 63     /*  ||  */
//#define TERNARY_CONDITION 64    /*  :?  */
#define ASSIGN 65     /*    = */
#define ADD_ASS 66  /*  += */
#define SUB_ASS 67  /*  -=  */
#define MUL_ASS 68  /*  *=  */
#define DIV_ASS 69  /*  /=  */
//#define MOD_ASS 70  /*  %=  */
//#define B_AND_ASS 71/*  &=  */
//#define B_XOR_ASS 72/*  ^=  */
//#define B_OR_ASS 73 /*  |=  */
//#define B_LEFT_ASS 74/* <<= */
//#define B_RIGHT_ASS 75/* >>=   */
#define COMMA 76    /*  ,   */
#define SEMIC 77    /*  ;   */
#define LB_BRAC 78  /*  {   */
#define RB_BRAC 79  /*  }   */


#endif // TOKE_CODE_H_INCLUDED
