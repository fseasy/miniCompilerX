#include "semantic_analysis.h"
#include "../syntax/analysis_table.h"
#include <stdio.h>
#include <stdlib.h>
/* the act define */
#define ACT_P_X	0
#define ACT_X_funDefine_X	1
#define ACT_X_mainFun	2
#define ACT_funDefine_funHead_funBody	3
#define ACT_funHead_returnType_ID_ACT_1_LR_BRAC_formalParam_RR_BRAC	4
#define ACT_funBody_LB_BRAC_funDomain_RB_BRAC	5
#define ACT_funDomain_declaration_funDomain	6
#define ACT_funDomain_assignment_funDomain	7
#define ACT_funDomain_condition_funDomain	8
#define ACT_funDomain_loop_funDomain	9
#define ACT_funDomain_io_funDomain	10
#define ACT_funDomain_funApply_funDomain	11
#define ACT_funDomain_jumpWord_funDomain	12
#define ACT_funDomain_SEMIC_funDomain	13
#define ACT_funDomain	14
#define ACT_returnType_INT	15
#define ACT_returnType_DOUBLE	16
#define ACT_returnType_CHAR	17
#define ACT_returnType_BOOL	18
#define ACT_returnType_VOID	19
#define ACT_formalParam_paramList	20
#define ACT_formalParam	21
#define ACT_paramList_type_ID_COMMA_paramList	22
#define ACT_paramList_type_ID	23
#define ACT_type_INT	24
#define ACT_type_DOUBLE	25
#define ACT_type_BOOL	26
#define ACT_type_CHAR	27
#define ACT_declaration_type_idList_SEMIC	28
#define ACT_idList_ID_COMMA_idList	29
#define ACT_idList_ID	30
#define ACT_assignment_ID_ASSIGN_EXP_SEMIC	31
#define ACT_assignment_ID_ASSIGN_funApply	32
#define ACT_assignment_singleOP_SEMIC	33
#define ACT_EXP_E_AND_EXP	34
#define ACT_EXP_E_OR_EXP	35
#define ACT_EXP_E	36
#define ACT_E_T_ADD_E	37
#define ACT_E_T_SUB_E	38
#define ACT_E_T	39
#define ACT_T_F_MUL_OR_INDIR_T	40
#define ACT_T_F_DIV_T	41
#define ACT_T_F	42
#define ACT_F_G_relOP_G	43
#define ACT_F_G	44
#define ACT_G_LR_BRAC_EXP_RR_BRAC	45
#define ACT_G_ID	46
#define ACT_G_TRUE	47
#define ACT_G_FALSE	48
#define ACT_G_DIGIT	49
#define ACT_G_CHAR_C	50
#define ACT_DIGIT_INT_C	51
#define ACT_DIGIT_REAL_C	52
#define ACT_relOP_EQ	53
#define ACT_relOP_NEQ	54
#define ACT_relOP_GE	55
#define ACT_relOP_GT	56
#define ACT_relOP_LE	57
#define ACT_relOP_LT	58
#define ACT_funApply_ID_LR_BRAC_acturalParam_RR_BRAC_SEMIC	59
#define ACT_acturalParam_idList	60
#define ACT_acturalParam	61
#define ACT_singleOP_INC_ID	62
#define ACT_singleOP_ID_INC	63
#define ACT_singleOP_DEC_ID	64
#define ACT_singleOP_ID_DEC	65
#define ACT_condition_IF_LR_BRAC_EXP_ACT_2_RR_BRAC_funBody	66
#define ACT_condition_IF_LR_BRAC_EXP_ACT_2_RR_BRAC_funBody_ACT_3_ELSE_funBody	67
#define ACT_loop_FOR_LR_BRAC_forAssignPart_ACT_4_SEMIC_forBoolPart_ACT_5_SEMIC_forAssignPart_ACT_6_RR_BRAC_funBody	68
#define ACT_forAssignPart_forAssignList	69
#define ACT_forAssignPart	70
#define ACT_forAssignList_forAssignment_COMMA_forAssignList	71
#define ACT_forAssignList_forAssignment	72
#define ACT_forAssignment_ID_ASSIGN_EXP	73
#define ACT_forAssignment_singleOP	74
#define ACT_forBoolPart_EXP	75
#define ACT_forBoolPart	76
#define ACT_jumpWord_returnWord	77
#define ACT_jumpWord_CONTINUE_SEMIC	78
#define ACT_jumpWord_BREAK_SEMIC	79
#define ACT_returnWord_RETURN_returnVal_SEMIC	80
#define ACT_returnVal_EXP	81
#define ACT_returnVal	82
#define ACT_io_PRINTF_LR_BRAC_printContent_RR_BRAC_SEMIC	83
#define ACT_io_SCANF_LR_BRAC_STRING_C_COMMA_REFERENCE_ID_RR_BRAC_SEMIC	84
#define ACT_printContent_STRING_C	85
#define ACT_printContent_STRING_C_COMMA_ID	86
#define ACT_mainFun_INT_MAIN_ACT_1_LR_BRAC_formalParam_RR_BRAC_funBody	87
#define ACT_ACT_1	88
#define ACT_ACT_2	89
#define ACT_ACT_3	90
#define ACT_ACT_4	91
#define ACT_ACT_5	92
#define ACT_ACT_6	93

int semanticAct(int pdtIndex)
{
    char name[30] ;
    char type[30] ;
    int size ;
    void * addr ;
    struct symbolTb * curTb = tbptrS.data[tbptrS.top -1] ;
    char outputStr[300] ;
	switch(pdtIndex)
	{
		case ACT_P_X :
		{
            //in fact , no this reduce
			break ;
		}
		case ACT_X_funDefine_X :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "X") ;
			break ;
		}
		case ACT_X_mainFun :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "X") ;
			break ;
		}
		case ACT_funDefine_funHead_funBody :
		{
            //over the tb , offset
            tbptrS.data[tbptrS.top -1]->width = (int)offsetS.data[offsetS.top -1] ;
            tbptrS.top -- ;
            offsetS.top -- ;
            //output label
            sprintf(outputStr,"%s:\n",lbptrS.data[0]->data[0]) ;
            genCode(outputStr) ;
            //clear the lbptrS
            int k ;
            for(k = 0 ; k < lbptrS.top ; k++)
            {
                free(lbptrS.data[k]) ;
            }
            lbptrS.top = 0 ;
            //change semanticS
            semanticS.top -= 1;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDefine") ;
			break ;
		}
		case ACT_funHead_returnType_ID_ACT_1_LR_BRAC_formalParam_RR_BRAC :
		{
            semanticS.top -= 5 ;
            strcpy(semanticS.data[semanticS.top - 1].lexType , "funHead") ;
			break ;
		}
		case ACT_funBody_LB_BRAC_funDomain_RB_BRAC :
		{
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funBody") ;
			break ;
		}
		case ACT_funDomain_declaration_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_funDomain_assignment_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_funDomain_condition_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_funDomain_loop_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_funDomain_io_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"funDomain") ;
			break ;
		}
		case ACT_funDomain_funApply_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
            if(semanticS.data[semanticS.top -1].extend != NULL)
            {
                free(semanticS.data[semanticS.top -1].extend) ;
                semanticS.data[semanticS.top -1].extend = NULL ;
            }
			break ;
		}
		case ACT_funDomain_jumpWord_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_funDomain_SEMIC_funDomain :
		{
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_funDomain :
		{
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "funDomain") ;
			break ;
		}
		case ACT_returnType_INT :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "returnType") ;
			break ;
		}
		case ACT_returnType_DOUBLE :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "returnType") ;
			break ;
		}
		case ACT_returnType_CHAR :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "returnType") ;
			break ;
		}
		case ACT_returnType_BOOL :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "returnType") ;
			break ;
		}
		case ACT_returnType_VOID :
		{
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType , "returnType") ;
			break ;
		}
		case ACT_formalParam_paramList :
		{
		    //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType,"formalParam") ;
			break ;
		}
		case ACT_formalParam :
		{
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "formalParam") ;
			break ;
		}
		case ACT_paramList_type_ID_COMMA_paramList :
		{
            strcpy(name , semanticS.data[semanticS.top - 3].lexVal) ;
            strcpy(type , semanticS.data[semanticS.top - 4].lexVal) ;
            size = getSize(type) ;
            enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+= size ;
            //change semanticS
            semanticS.top -= 3 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "paramList") ;

			break ;
		}
		case ACT_paramList_type_ID :
		{
            //enter table
            strcpy(name , semanticS.data[semanticS.top -1].lexVal) ;
            strcpy(type,semanticS.data[semanticS.top -2].lexVal) ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            //change semanticS
            semanticS.top -= 1 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "paramList") ;
			break ;
		}
		case ACT_type_INT :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType,"type") ;
			break ;
		}
		case ACT_type_DOUBLE :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType,"type") ;
			break ;
		}
		case ACT_type_BOOL :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType,"type") ;
			break ;
		}
		case ACT_type_CHAR :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType,"type") ;
			break ;
		}
		case ACT_declaration_type_idList_SEMIC :
		{
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType ,"declaration") ;
			break ;
		}
		case ACT_idList_ID_COMMA_idList :
		{
		    int k = 3 ;
            int isDeclare = 1 ;
            while(semanticS.top - k -1 >= 0)
            {
                if((strcmp(semanticS.data[semanticS.top - k].lexType,"ID") == 0) &&
                    (strcmp(semanticS.data[semanticS.top - k -1].lexType,"type") == 0) )
                {
                    //it is declare
                    break ;
                }
                else if((strcmp(semanticS.data[semanticS.top - k].lexType,"LR_BRAC") == 0) &&
                    (strcmp(semanticS.data[semanticS.top - k -1].lexType,"ID") == 0))
                {
                    //it is funApply
                    isDeclare = 0 ;
                    break ;
                }
                k++ ;
            }
            strcpy(name , semanticS.data[semanticS.top -3].lexVal) ;
            if(isDeclare)
            {
                strcpy(type , semanticS.data[semanticS.top -k -1].lexVal) ;
                #ifdef OUTPUT_TYPE
                printf("----test_type:%s---\n",type) ;
                #endif
                size = getSize(type) ;
                addr = enterTb(curTb , name , type , offsetS.data[offsetS.top -1]) ;
                offsetS.data[offsetS.top -1]+=size ;
                //change semanticS
                semanticS.top -= 2 ;
                strcpy(semanticS.data[semanticS.top -1].lexType , "idList") ;
            }
            else
            {
                addr = lookupTb(curTb , name) ;
                if(addr == NULL)
                {
                    freopen("CON","w",stdout) ;
                    printf("\nvariable '%s' does not declared \n",name) ;
                    return FALSE_ANA ;
                }
                addr = ((struct tbNode *)addr)->addr ;
                #ifdef ECHO_ASM
                sprintf(outputStr,"param %s [push %p]\n",name,addr) ;
                #else
                sprintf(outputStr,"param %s\n",name) ;
                #endif
                //change semanticS
                semanticS.top -= 2 ;
                strcpy(semanticS.data[semanticS.top -1].lexType , "idList") ;
                /* add the param num info*/
                semanticS.data[semanticS.top -1].extend = semanticS.data[semanticS.top + 1].extend + 1;
            }
			break ;
		}
		case ACT_idList_ID :
		{
            //first to know is the declaration or the funApply
            int k = 1 ;
            int isDeclare = 1 ;
            while(semanticS.top - k -1 >= 0)
            {
                if((strcmp(semanticS.data[semanticS.top - k].lexType,"ID") == 0) &&
                    (strcmp(semanticS.data[semanticS.top - k -1].lexType,"type") == 0) )
                {
                    //it is declare
                    break ;
                }
                else if((strcmp(semanticS.data[semanticS.top - k].lexType,"LR_BRAC") == 0) &&
                    (strcmp(semanticS.data[semanticS.top - k -1].lexType,"ID") == 0))
                {
                    //it is funApply
                    isDeclare = 0 ;
                    break ;
                }
                k++ ;
            }
            strcpy(name , semanticS.data[semanticS.top -1].lexVal) ;
            if(isDeclare)
            {
                strcpy(type , semanticS.data[semanticS.top -k -1].lexVal) ;
                #ifdef OUTPUT_TYPE
                printf("----test_type:%s---\n",type) ;
                #endif
                size = getSize(type) ;
                addr = enterTb(curTb , name , type , offsetS.data[offsetS.top -1]) ;
                offsetS.data[offsetS.top -1]+=size ;
                //change semanticS
                strcpy(semanticS.data[semanticS.top -1].lexType , "idList") ;
            }
            else
            {
                addr = lookupTb(curTb , name) ;
                if(addr == NULL)
                {
                    freopen("CON","w",stdout) ;
                    printf("variable '%s' does not declared \n",name) ;
                    return FALSE_ANA ;
                }
                addr = ((struct tbNode *)addr)->addr ;
                #ifdef ECHO_ASM
                sprintf(outputStr,"param %s [push %p]\n",name,addr) ;
                #else
                sprintf(outputStr,"param %s\n",name) ;
                #endif
                genCode(outputStr) ;
                strcpy(semanticS.data[semanticS.top -1].lexType , "idList") ;
                /* add the param num info*/
                semanticS.data[semanticS.top -1].extend = 1 ;
            }
			break ;
		}
		case ACT_assignment_ID_ASSIGN_EXP_SEMIC :
		{
		    //lookup the ID
            strcpy(name , semanticS.data[semanticS.top -4].lexVal) ;
            addr = lookupTb(curTb , name) ;
            if(addr == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("variable '%s' has not been declared \n",name) ;
                return FALSE_ANA ;
            }
		    //decide type
            char * tmpType = decideType(((struct tbNode *)addr)->type,
                                        (char *)semanticS.data[semanticS.top -2].extend) ;
            if(tmpType == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("\nTYPE_CAST ERROR ,can not cast '%s' to '%s' \n",(char *)semanticS.data[semanticS.top -2].extend,
                       ((struct tbNode *)addr)->type) ;
                return FALSE_ANA ;
            }
            free(tmpType) ;
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov ebx , %p ",name,semanticS.data[semanticS.top - 2].lexVal,((struct tbNode *)addr)->addr) ;
			genCode(outputStr) ;
			addr = lookupTb(curTb , semanticS.data[semanticS.top -2].lexVal) ;
			if(addr == NULL)
			{
			    freopen("CON","w",stdout) ;
                printf("\nvariable '%s' has not been declared \n",semanticS.data[semanticS.top -2].lexVal) ;
                return FALSE_ANA ;
			}
			sprintf(outputStr,"         mov %p ,ebx ]",((struct tbNode *)addr)->addr) ;
			#else
			sprintf(outputStr,"%s = %s \n",name,semanticS.data[semanticS.top - 2].lexVal) ;
			#endif
			genCode(outputStr) ;
            //change semanticS
            semanticS.top -= 3 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "assignment") ;
			break ;
		}
		case ACT_assignment_ID_ASSIGN_funApply :
		{

            //fist look this funApply has return val?
            if(strcmp(semanticS.data[semanticS.top -1].lexType,"fnRetVal") != 0)
            {
                freopen("CON","w",stdout) ;
                printf("\nfunction '%s' has no return value \n",semanticS.data[semanticS.top -1].lexVal) ;
                return FALSE_ANA ;
            }
            strcpy(name , semanticS.data[semanticS.top -3].lexVal) ;
            addr = lookupTb(curTb , name) ;
            if(addr == NULL)
            {
                printf("\nvariable '%s' has not been declared \n",name) ;
                return FALSE_ANA ;
            }
            //decide type
            char * tmpType = decideType(((struct tbNode *)addr)->type ,
                                        (char *)semanticS.data[semanticS.top -1].extend) ;
            if(tmpType == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("\nTYPE_CAST ERROR ,can not cast %s to %s \n",(char *)semanticS.data[semanticS.top -1].extend,
                       (char *)semanticS.data[semanticS.top -3].extend) ;
                return FALSE_ANA ;
            }
            free(tmpType) ;
            //genCode

            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov ebx , %p ",name,semanticS.data[semanticS.top - 1].lexVal,((struct tbNode *)addr)->addr) ;
			genCode(outputStr) ;
			addr = lookupTb(curTb , semanticS.data[semanticS.top -1].lexVal) ;
			if(addr == NULL)
			{
			    freopen("CON","w",stdout) ;
                printf("\nvariable '%s' has not been declared \n",semanticS.data[semanticS.top -2].lexVal) ;
                return FALSE_ANA ;
			}
			sprintf(outputStr,"         mov %p ,ebx ]",((struct tbNode *)addr)->addr) ;
			#else
			sprintf(outputStr,"%s = %s \n",name,semanticS.data[semanticS.top - 1].lexVal) ;
			#endif
			genCode(outputStr) ;
			//change semanticS
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "assignment") ;
			break ;
		}
		case ACT_assignment_singleOP_SEMIC :
		{
		    semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "assignment") ;
			break ;
		}
		case ACT_EXP_E_AND_EXP :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            strcpy(type,"BOOL") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s and %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                         semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_EXP_E_OR_EXP :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            strcpy(type,"BOOL") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s or %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                         semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_EXP_E :
		{
            //nothing
			break ;
		}
		case ACT_E_T_ADD_E :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            //decide the type ! at the op state , we should cast the small type to big type
            //and the result type should be the big type
            char * tmpType = decideType((char *)semanticS.data[semanticS.top -3].extend ,
                                        (char *)semanticS.data[semanticS.top -1].extend) ;
            if(tmpType == NULL)
            {
                tmpType = decideType((char *)semanticS.data[semanticS.top -1].extend ,
                                    (char *)semanticS.data[semanticS.top -3].extend) ;
            }
            strcpy(type,tmpType) ;
            free(tmpType) ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s + %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                         semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_E_T_SUB_E :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            //decide the type ! at the op state , we should cast the small type to big type
            //and the result type should be the big type
            char * tmpType = decideType((char *)semanticS.data[semanticS.top -3].extend ,
                                        (char *)semanticS.data[semanticS.top -1].extend) ;
            if(tmpType == NULL)
            {
                tmpType = decideType((char *)semanticS.data[semanticS.top -1].extend ,
                                    (char *)semanticS.data[semanticS.top -3].extend) ;
            }
            strcpy(type,tmpType) ;
            free(tmpType) ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s - %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                         semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_E_T :
		{
            //nothing
			break ;
		}
		case ACT_T_F_MUL_OR_INDIR_T :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            //decide the type ! at the op state , we should cast the small type to big type
            //and the result type should be the big type
            char * tmpType = decideType((char *)semanticS.data[semanticS.top -3].extend ,
                                        (char *)semanticS.data[semanticS.top -1].extend) ;
            if(tmpType == NULL)
            {
                tmpType = decideType((char *)semanticS.data[semanticS.top -1].extend ,
                                    (char *)semanticS.data[semanticS.top -3].extend) ;
            }
            strcpy(type,tmpType) ;
            free(tmpType) ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s * %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                         semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_T_F_DIV_T :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            //decide the type ! at the op state , we should cast the small type to big type
            //and the result type should be the big type
            char * tmpType = decideType((char *)semanticS.data[semanticS.top -3].extend ,
                                        (char *)semanticS.data[semanticS.top -1].extend) ;
            if(tmpType == NULL)
            {
                tmpType = decideType((char *)semanticS.data[semanticS.top -1].extend ,
                                    (char *)semanticS.data[semanticS.top -3].extend) ;
            }
            strcpy(type,tmpType) ;
            free(tmpType) ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s / %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                         semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_T_F :
		{
            //nothing
			break ;
		}
		case ACT_F_G_relOP_G :
		{
            char * tmpName = getTmpName() ;
            strcpy(name ,tmpName) ;
            free(tmpName) ;
            strcpy(type,"BOOL") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top-1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            sprintf(outputStr,"%s = %s %s %s \n",name,semanticS.data[semanticS.top -3].lexVal ,
                        semanticS.data[semanticS.top -2].lexVal, semanticS.data[semanticS.top - 1].lexVal ) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_F_G :
		{
            //nothing
			break ;
		}
		case ACT_G_LR_BRAC_EXP_RR_BRAC :
		{
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , semanticS.data[semanticS.top].lexType) ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , semanticS.data[semanticS.top].lexVal) ;
            semanticS.data[semanticS.top -1].extend = semanticS.data[semanticS.top].extend ;
			break ;
		}
		case ACT_G_ID :
		{
		    strcpy(name , semanticS.data[semanticS.top -1].lexVal) ;
            addr = lookupTb(curTb ,name) ;
            strcpy(type,((struct tbNode *)addr)->type) ;
            if(addr == NULL)
            {
                printf("variable '%s' has not been declare \n",name) ;
                return FALSE_ANA ;
            }
            //change semanticS
            semanticS.data[semanticS.top -1].extend = malloc(sizeof(char)*10) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_G_TRUE :
		{
            char * tmpName = getTmpName() ;
            strcpy(name,tmpName) ;
            free(tmpName) ;
            strcpy(type,"BOOL") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            //output
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov %p , %s]\n",name,semanticS.data[semanticS.top -1].lexVal ,
                                addr , semanticS.data[semanticS.top -1].lexVal) ;
            #else
            sprintf(outputStr , "%s = %s \n",name,semanticS.data[semanticS.top -1].lexVal) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType , "ID") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
            semanticS.data[semanticS.top -1].extend = malloc(sizeof(char)*10) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_G_FALSE :
		{
            char * tmpName = getTmpName() ;
            strcpy(name,tmpName) ;
            free(tmpName) ;
            strcpy(type,"BOOL") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            //output
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov %p , %s]\n",name,semanticS.data[semanticS.top -1].lexVal ,
                                addr , semanticS.data[semanticS.top -1].lexVal) ;
            #else
            sprintf(outputStr , "%s = %s \n",name,semanticS.data[semanticS.top -1].lexVal) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType , "ID") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
            semanticS.data[semanticS.top -1].extend = malloc(sizeof(char)*10) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_G_DIGIT :
		{
            //nothing
			break ;
		}
		case ACT_G_CHAR_C :
		{
            /* create a tmp variable and enter it */
            char * tmpName = getTmpName() ;
            strcpy(name,tmpName) ;
            free(tmpName) ;
            strcpy(type,"CHAR") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            //output
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov %p , %s]\n",name,semanticS.data[semanticS.top -1].lexVal ,
                                addr , semanticS.data[semanticS.top -1].lexVal) ;
            #else
            sprintf(outputStr , "%s = %s \n",name,semanticS.data[semanticS.top -1].lexVal) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType , "ID") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
            semanticS.data[semanticS.top -1].extend = malloc(sizeof(char)*10) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_DIGIT_INT_C :
		{
		    /* create a tmp variable and enter it */
            char * tmpName = getTmpName() ;
            strcpy(name,tmpName) ;
            free(tmpName) ;
            strcpy(type,"INT") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            //output
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov %p , %s]\n",name,semanticS.data[semanticS.top -1].lexVal ,
                                addr , semanticS.data[semanticS.top -1].lexVal) ;
            #else
            sprintf(outputStr , "%s = %s \n",name,semanticS.data[semanticS.top -1].lexVal) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType , "ID") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
            semanticS.data[semanticS.top -1].extend = malloc(sizeof(char)*10) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_DIGIT_REAL_C :
		{
		    /* create a tmp variable and enter it */
            char * tmpName = getTmpName() ;
            strcpy(name,tmpName) ;
            free(tmpName) ;
            strcpy(type,"DOUBLE") ;
            size = getSize(type) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            offsetS.data[offsetS.top -1]+=size ;
            //output
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov %p , %s]\n",name,semanticS.data[semanticS.top -1].lexVal ,
                                addr , semanticS.data[semanticS.top -1].lexVal) ;
            #else
            sprintf(outputStr , "%s = %s \n",name,semanticS.data[semanticS.top -1].lexVal) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType , "ID") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal,name) ;
            semanticS.data[semanticS.top -1].extend = malloc(sizeof(char)*10) ;
			strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			break ;
		}
		case ACT_relOP_EQ :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "relOP") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , "==") ;
			break ;
		}
		case ACT_relOP_NEQ :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "relOP") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , "!=") ;
			break ;
		}
		case ACT_relOP_GE :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "relOP") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , ">=") ;
			break ;
		}
		case ACT_relOP_GT :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "relOP") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , ">") ;
			break ;
		}
		case ACT_relOP_LE :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "relOP") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , "<=") ;
			break ;
		}
		case ACT_relOP_LT :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "relOP") ;
            strcpy(semanticS.data[semanticS.top -1].lexVal , "<") ;
			break ;
		}
		case ACT_funApply_ID_LR_BRAC_acturalParam_RR_BRAC_SEMIC :
		{
		    sprintf(outputStr,"call %s , %d\n",semanticS.data[semanticS.top -5].lexVal ,
                                            (int)semanticS.data[semanticS.top -3].extend) ;
            genCode(outputStr) ;
            //we should know if it has return val
            //because function define all at global scope , so we can find it
            if(tbptrS.top <= 0)
            {
                freopen("CON","w",stdout) ;
                printf("the scope has been in mess \n") ;
                return FALSE_ANA ;
            }
            struct symbolTb * globalTb = tbptrS.data[0] ;
            int k = 0 ;
            for( ; k < globalTb->counter ; k++)
            {
                if(strcmp(globalTb->data[k]->name,semanticS.data[semanticS.top -5].lexVal) == 0)
                {
                    break ;
                }
            }
            if(k == globalTb->counter)
            {
                freopen("CON","w",stdout) ;
                printf("\nfunction '%s' has not been defined \n",semanticS.data[semanticS.top -5].lexVal) ;
                return FALSE_ANA ;
            }
            //get return type
            strcpy(type,(char *)(globalTb->data[k]->type) + strlen(globalTb->data[k]->type) +1) ;
			//now we should build a block for the return val if it is not void
			//first ,we clear the semanticS
			semanticS.top -= 4 ;
			if(strcmp(type,"VOID") != 0)
			{
			    strcpy(semanticS.data[semanticS.top -1].lexType,"fnRetVal") ;
			    char * tmpName = getTmpName() ;
			    size = getSize(type) ;
			    addr = enterTb(curTb,tmpName,type,offsetS.data[offsetS.top -1]) ;
			    offsetS.data[offsetS.top -1]+=size ;
			    #ifdef ECHO_ASM
			    sprintf(outputStr,"%s = eax [mov %p , exa]\n",tmpName, addr) ;
			    #else
			    sprintf(outputStr,"%s = eax\n",tmpName) ;
			    #endif
			    genCode(outputStr) ;
			    strcpy(semanticS.data[semanticS.top -1].lexVal,tmpName) ;
			    free(tmpName) ;
			    semanticS.data[semanticS.top-1].extend = malloc(sizeof(char) * strlen(type)) ;
			    strcpy((char *)semanticS.data[semanticS.top -1].extend , type) ;
			}
            else
            {
                strcpy(semanticS.data[semanticS.top -1].lexType,"VOID") ;
            }
			break ;
		}
		case ACT_acturalParam_idList :
		{
            //change semanticS
            strcpy(semanticS.data[semanticS.top -1].lexType,"acturalParam")  ;
			break ;
		}
		case ACT_acturalParam :
		{
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"acturalParam")  ;
			break ;
		}
		case ACT_singleOP_INC_ID :
		{
            strcpy(name,semanticS.data[semanticS.top -1].lexVal) ;
		    addr = lookupTb(curTb , name)->addr ;
            //genCode
            #ifdef ECHO_ASM
            sprintf(outputStr,"INC %s [INC %p]\n",name,addr) ;
            #else
            sprintf(outputStr,"INC %s \n",name) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "singleOP") ;
			break ;
		}
		case ACT_singleOP_ID_INC :
		{
            strcpy(name,semanticS.data[semanticS.top -2].lexVal) ;
		    addr = lookupTb(curTb , name)->addr ;
            //genCode
            #ifdef ECHO_ASM
            sprintf(outputStr,"INC %s [INC %p]\n",name,addr) ;
            #else
            sprintf(outputStr,"INC %s \n",name) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "singleOP") ;
			break ;
		}
		case ACT_singleOP_DEC_ID :
		{
            strcpy(name,semanticS.data[semanticS.top -1].lexVal) ;
		    addr = lookupTb(curTb , name)->addr ;
            //genCode
            #ifdef ECHO_ASM
            sprintf(outputStr,"DEC %s [DEC %p]\n",name,addr) ;
            #else
            sprintf(outputStr,"DEC %s \n",name) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "singleOP") ;
			break ;
		}
		case ACT_singleOP_ID_DEC :
		{
		    strcpy(name,semanticS.data[semanticS.top -2].lexVal) ;
		    addr = lookupTb(curTb , name)->addr ;
            //genCode
             #ifdef ECHO_ASM
            sprintf(outputStr,"DEC %s [DEC %p]\n",name,addr) ;
            #else
            sprintf(outputStr,"DEC %s \n",name) ;
            #endif
            genCode(outputStr) ;
            //change semanticS
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "singleOP") ;
			break ;
		}
		case ACT_condition_IF_LR_BRAC_EXP_ACT_2_RR_BRAC_funBody :
		{
            //genCode
            sprintf(outputStr,"%s:\n",lbptrS.data[lbptrS.top -1]->data[1]) ;
            genCode(outputStr) ;
            //clear
            free(lbptrS.data[lbptrS.top -1]) ;
            lbptrS.top -- ;
            //pop
            curTb->width = offsetS.data[offsetS.top] ;
            tbptrS.top-- ;
            offsetS.top -- ;
            //semanticS change
            semanticS.top -= 5 ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"condition") ;
			break ;
		}
		case ACT_condition_IF_LR_BRAC_EXP_ACT_2_RR_BRAC_funBody_ACT_3_ELSE_funBody :
		{
            //genCode
            sprintf(outputStr,"%s:\n",lbptrS.data[lbptrS.top -1]->data[2]) ;
            genCode(outputStr) ;
            //clear
            free(lbptrS.data[lbptrS.top -1]) ;
            lbptrS.top -- ;
            //pop
            curTb->width = offsetS.data[offsetS.top] ;
            tbptrS.top-- ;
            offsetS.top -- ;
            //semanticS change
            semanticS.top -= 8 ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"condition") ;
			break ;
		}
		case ACT_loop_FOR_LR_BRAC_forAssignPart_ACT_4_SEMIC_forBoolPart_ACT_5_SEMIC_forAssignPart_ACT_6_RR_BRAC_funBody :
		{
            //genCode
            sprintf(outputStr,"goto %s\n%s:\n",lbptrS.data[lbptrS.top-1]->data[3],
                    lbptrS.data[lbptrS.top -1]->data[2]) ;
            genCode(outputStr) ;
            //clear
            free(lbptrS.data[lbptrS.top -1]) ;
            lbptrS.top -- ;
            //pop
            curTb->width = offsetS.data[offsetS.top] ;
            tbptrS.top-- ;
            offsetS.top -- ;
            //semanticS change
            semanticS.top -= 11 ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"condition") ;
			break ;
		}
		case ACT_forAssignPart_forAssignList :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "forAssignPart") ;
			break ;
		}
		case ACT_forAssignPart :
		{
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType ,"forAssignPart") ;
			break ;
		}
		case ACT_forAssignList_forAssignment_COMMA_forAssignList :
		{
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType ,"forAssignList") ;
			break ;
		}
		case ACT_forAssignList_forAssignment :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "forAssignList") ;
			break ;
		}
		case ACT_forAssignment_ID_ASSIGN_EXP :
		{
            strcpy(name , semanticS.data[semanticS.top -3].lexVal) ;
            addr = lookupTb(curTb , name) ;
            if(addr == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("\nvariable '%s' has not been declared \n",name) ;
                return FALSE_ANA ;
            }
            //decide type
            char * tmpType = decideType(((struct tbNode *)addr)->type,
                                        (char *)semanticS.data[semanticS.top -1].extend) ;
            if(tmpType == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("\nTYPE_CAST ERROR ,can not cast %s to %s \n",(char *)semanticS.data[semanticS.top -1].extend,
                       (char *)semanticS.data[semanticS.top -3].extend) ;
                return FALSE_ANA ;
            }
            free(tmpType) ;
            #ifdef ECHO_ASM
            sprintf(outputStr , "%s = %s [mov ebx , %p ",name,semanticS.data[semanticS.top - 1].lexVal,((struct tbNode *)addr)->addr) ;
			genCode(outputStr) ;
			addr = lookupTb(curTb , semanticS.data[semanticS.top -1].lexVal) ;
			if(addr == NULL)
			{
			    freopen("CON","w",stdout) ;
                printf("\nvariable '%s' has not been declared \n",semanticS.data[semanticS.top -2].lexVal) ;
                return FALSE_ANA ;
			}
			sprintf(outputStr,"         mov %p ,ebx ]",((struct tbNode *)addr)->addr) ;
			#else
			sprintf(outputStr,"%s = %s \n",name,semanticS.data[semanticS.top - 1].lexVal) ;
			#endif
			genCode(outputStr) ;
            //change semanticS
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "assignment") ;
			break ;
		}
		case ACT_forAssignment_singleOP :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "forAssignment") ;
			break ;
		}
		case ACT_forBoolPart_EXP :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType , "forBoolPart") ;
			break ;
		}
		case ACT_forBoolPart :
		{
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType ,"forBoolPart") ;
			break ;
		}
		case ACT_jumpWord_returnWord :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType ,"jumpWord") ;
			break ;
		}
		case ACT_jumpWord_CONTINUE_SEMIC :
		{
            //find the FOR LABEL , if there is not , ERROR
            int k = semanticS.top - 3 ;
            while( k >= 0)
            {
                if(strcmp(semanticS.data[k].lexType,"FOR") == 0)
                {
                    break ;
                }
                k-- ;
            }
            if(k < 0 )
            {
                freopen("CON","w",stdout) ;
                printf("\ncontinue Error ! loop not find \n") ;
                return FALSE_ANA ;
            }
            else
            {
                char * tmp = (char *)semanticS.data[k].extend ;
                sprintf(outputStr,"goto %s\n",tmp) ;
                genCode(outputStr) ;
            }
            //semanticS change
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"jumpWord") ;
			break ;
		}
		case ACT_jumpWord_BREAK_SEMIC :
		{
            //find the FOR LABEL , if there is not , ERROR
            int k = semanticS.top - 3 ;
            while( k >= 0)
            {
                if(strcmp(semanticS.data[k].lexType,"FOR") == 0)
                {
                    break ;
                }
                k-- ;
            }
            if(k < 0 )
            {
                freopen("CON","w",stdout) ;
                printf("\nbreak Error ! loop not find \n") ;
                return FALSE_ANA ;
            }
            else
            {
                char * tmp = (char *)semanticS.data[k].extend ;
                tmp = tmp + strlen(tmp)+1 ;
                sprintf(outputStr,"goto %s\n",tmp) ;
                genCode(outputStr) ;
            }
            //semanticS change
            semanticS.top -- ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"jumpWord") ;
			break ;
		}
		case ACT_returnWord_RETURN_returnVal_SEMIC :
		{
            strcpy(name ,semanticS.data[semanticS.top -2].lexVal) ;
            addr = lookupTb(curTb,name) ;
            if(addr == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("\nvariable '%s' has not been declared \n",name) ;
                return FALSE_ANA ;
            }
            addr = ((struct tbNode *)addr)->addr ;
            #ifdef ECHO_ASM
            sprintf(outputStr , "eax = %s [mov eax , %p]\n",name , addr) ;
            #else
            sprintf(outputStr ,"eax = %s \n",name) ;
            #endif
            genCode(outputStr) ;
            //echo goto label
            if(lbptrS.top <= 0 )
            {
                freopen("CON","w",stdout) ;
                printf("lbptrS has been in mess\n") ;
                return FALSE_ANA ;
            }
            sprintf(outputStr,"goto %s\n",lbptrS.data[0]->data[0]) ;//the function's end all have been save at here
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 2 ;
			strcpy(semanticS.data[semanticS.top -1].lexType,"returnWord") ;
			break ;
		}
		case ACT_returnVal_EXP :
		{
            strcpy(semanticS.data[semanticS.top -1].lexType,"returnVal") ;
			break ;
		}
		case ACT_returnVal :
		{
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"returnVal") ;
			break ;
		}
		case ACT_io_PRINTF_LR_BRAC_printContent_RR_BRAC_SEMIC :
		{
            sprintf(outputStr,"call printf , %d\n",(int)semanticS.data[semanticS.top -3].extend) ;
			genCode(outputStr) ;
			//change semanticS
			semanticS.top -= 4 ;
			strcpy(semanticS.data[semanticS.top -1].lexType ,"io") ;

			break ;
		}
		case ACT_io_SCANF_LR_BRAC_STRING_C_COMMA_REFERENCE_ID_RR_BRAC_SEMIC :
		{
            sprintf(outputStr,"param %s\n",(char*)(semanticS.data[semanticS.top -6].extend)) ;
			genCode(outputStr) ;
			free(semanticS.data[semanticS.top -6].extend) ;
			addr = lookupTb(curTb,semanticS.data[semanticS.top -3].lexVal) ;
			if(addr == NULL)
			{
			    freopen("CON","w",stdout) ;
			    printf("\nvariable '%s' has not been declared \n",semanticS.data[semanticS.top -3].lexVal) ;
			    return FALSE_ANA ;
			}
			addr = ((struct tbNode *)addr)->addr ;
			sprintf(outputStr,"param %p\n",addr) ;
			genCode(outputStr) ;
            sprintf(outputStr,"call scanf , 2\n") ;
            genCode(outputStr) ;
            //change semanticS
            semanticS.top -= 7 ;
            strcpy(semanticS.data[semanticS.top -1].lexType,"io") ;
			break ;
		}
		case ACT_printContent_STRING_C :
		{
            sprintf(outputStr,"param %s \n",(char *)semanticS.data[semanticS.top -1].extend) ;
            genCode(outputStr) ;
            //chage semanticS
            free(semanticS.data[semanticS.top -1].extend) ;
            strcpy(semanticS.data[semanticS.top -1].lexType ,"printContent") ;
            //recode the paramNum
            semanticS.data[semanticS.top -1].extend = 1 ;
			break ;
		}
		case ACT_printContent_STRING_C_COMMA_ID :
		{
		    //output
		    strcpy(name , semanticS.data[semanticS.top -1].lexVal) ;
            addr = lookupTb(curTb,name) ;
            if(addr == NULL)
            {
                freopen("CON","w",stdout) ;
                printf("\nvariable '%s' has not been declared\n",name) ;
                return FALSE_ANA ;
            }
            #ifdef ECHO_ASM
            sprintf(outputStr,"param %s [push %p]\n",name , ((struct tbNode *)addr)->addr) ;
            #else
            sprintf(outputStr,"param %s \n",name) ;
            #endif
            genCode(outputStr) ;
            sprintf(outputStr,"param %s \n",(char *)semanticS.data[semanticS.top -3].extend) ;
            genCode(outputStr) ;
            free(semanticS.data[semanticS.top - 3].extend) ;
            //chage semanticS
            semanticS.top -= 2 ;
            strcpy(semanticS.data[semanticS.top -1].lexType ,"printContent") ;
            //recode the paramNum
            semanticS.data[semanticS.top -1].extend = 2 ;
			break ;
		}
		case ACT_mainFun_INT_MAIN_ACT_1_LR_BRAC_formalParam_RR_BRAC_funBody :
		{
            //over the tb , offset
            tbptrS.data[tbptrS.top -1]->width = (int)offsetS.data[offsetS.top -1] ;
            tbptrS.top -- ;
            offsetS.top -- ;
            //output label
            sprintf(outputStr,"%s:\n",lbptrS.data[0]->data[0]) ;
            genCode(outputStr) ;
            //clear the lbptrS
            int k ;
            for(k = 0 ; k < lbptrS.top ; k++)
            {
                free(lbptrS.data[k]) ;
            }
            lbptrS.top = 0 ;
            //change semanticS
            semanticS.top -= 6;
            strcpy(semanticS.data[semanticS.top -1].lexType , "mainFun") ;
			break ;
		}
		case ACT_ACT_1 :
		{
            //create label
            struct LbS * glLabel = getNewLabelS() ;
            lbptrS.data[lbptrS.top ++] = glLabel ;
            createLabel(glLabel,1) ;
            //create symbol table , just enter the ID with type = TABLE
            //first , is it a fundefine or the MAIN , but it does not affect
            strcpy(name , semanticS.data[semanticS.top -1].lexVal) ;
            strcpy(type,"TABLE") ;
            strcpy((char *)type + strlen(type) + 1,semanticS.data[semanticS.top -2].lexVal) ;
            addr = enterTb(curTb,name,type,offsetS.data[offsetS.top -1]) ;
            //push stack
            tbptrS.data[tbptrS.top++] = (struct symbolTb *)addr ;
            offsetS.data[offsetS.top++] = 0 ;
            /*out put label */
            sprintf(outputStr,"%s:\n",name) ;
            genCode(outputStr) ;
            //change semanticS
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top - 1].lexType ,"ACT_1") ;
			break ;
		}
		case ACT_ACT_2 :
		{
            struct LbS * newLsS = getNewLabelS() ;
            createLabel(newLsS,2) ;
            lbptrS.data[lbptrS.top++] = newLsS ;
            sprintf(outputStr,"if %s goto %s \ngoto %s\n%s:\n",semanticS.data[semanticS.top-1].lexVal,
                        lbptrS.data[lbptrS.top-1]->data[0],lbptrS.data[lbptrS.top -1]->data[1],
                        lbptrS.data[lbptrS.top-1]->data[0]) ;
            genCode(outputStr) ;
            //new symbol table
            char * tmpName = getTmpName() ;
            strcpy(type,"TABLE") ;
            strcpy((char *)type + strlen("TABLE") +1 , "IF") ;
            addr = enterTb(curTb,tmpName,type,offsetS.data[offsetS.top -1]) ;
            //change table
            tbptrS.data[tbptrS.top++] = (struct symbolTb *)addr ;
            offsetS.data[offsetS.top++] = 0 ;
            //change semantic
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "ACT_2") ;
			break ;
		}
		case ACT_ACT_3 :
		{
            createLabel(lbptrS.data[lbptrS.top -1],1) ;
            sprintf(outputStr ,"goto %s\n%s\n",lbptrS.data[lbptrS.top-1]->data[2],
                    lbptrS.data[lbptrS.top -1]->data[1]) ;
            genCode(outputStr) ;
            //finish if's symbol table
            curTb->width = (int)offsetS.data[offsetS.top -1] ;
            tbptrS.top -- ;
            offsetS.top -- ;
            curTb = tbptrS.data[tbptrS.top -1] ;
            //new symbolTable
            char * tmpName = getTmpName() ;
            strcpy(type,"TABLE") ;
            strcpy((char *)type + strlen("TABLE") +1 , "ELSE") ;
            addr = enterTb(curTb,tmpName,type,offsetS.data[offsetS.top -1]) ;
            //change table
            tbptrS.data[tbptrS.top++] = (struct symbolTb *)addr ;
            offsetS.data[offsetS.top++] = 0 ;
            //change semantic
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "ACT_3") ;
			break ;
		}
		case ACT_ACT_4 :
		{
            //create label
            struct LbS * newLabel = getNewLabelS() ;
            createLabel(newLabel,4) ;
            lbptrS.data[lbptrS.top++] = newLabel ;
            sprintf(outputStr,"%s:\n",lbptrS.data[lbptrS.top -1]->data[0]) ;
            genCode(outputStr) ;
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "ACT_4") ;
            /* notice ! because the 'break' and 'continue' should know the
             label of the loop angin and loop over , and the break and continue
             can be at anywhere ,so it is hard to find if we just store the
             label at the lbptrS ,so we store this two label at FOR block at
             semanticS  */
             /* find the FOR block */
             int ForPos = semanticS.top - 4 ;
             if(strcmp(semanticS.data[ForPos].lexType,"FOR") != 0)
             {
                 freopen("CON","w",stdout) ;
                 printf("The semantic stack has been in mess !\n") ;
                 return FALSE_ANA ;
             }
             char * labelRecord = (char *)malloc(sizeof(char)*10*2) ;
             strcpy(labelRecord ,lbptrS.data[lbptrS.top -1]->data[3]) ;//get continue label
             strcpy(labelRecord + strlen(labelRecord) + 1,lbptrS.data[lbptrS.top -1]->data[2]) ;//break label
             semanticS.data[ForPos].extend = (void *)labelRecord ;
			break ;
		}
		case ACT_ACT_5 :
		{
            //genCode
            sprintf(outputStr,"if %s goto %s \ngoto %s\n%s:\n",semanticS.data[semanticS.top -1].lexVal,
                    lbptrS.data[lbptrS.top -1]->data[1] ,
                    lbptrS.data[lbptrS.top -1]->data[2] ,
                    lbptrS.data[lbptrS.top -1]->data[3]) ;
            genCode(outputStr) ;
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "ACT_5") ;
			break ;
		}
		case ACT_ACT_6 :
		{
            sprintf(outputStr,"goto %s\n%s:\n",lbptrS.data[lbptrS.top -1]->data[0],
                                lbptrS.data[lbptrS.top -1]->data[1]) ;
            genCode(outputStr) ;
            //create symbolTb
            char * tmpName = getTmpName() ;
            strcpy(type,"TABLE") ;
            strcpy((char *)type + strlen("TABLE") +1 , "FOR") ;
            addr = enterTb(curTb,tmpName,type,offsetS.data[offsetS.top -1]) ;
            //change table
            tbptrS.data[tbptrS.top++] = (struct symbolTb *)addr ;
            offsetS.data[offsetS.top++] = 0 ;
            //change semantic
            semanticS.top ++ ;
            strcpy(semanticS.data[semanticS.top -1].lexType , "ACT_6") ;
			break ;
		}
		default:
            freopen("CON","w",stdout) ;
            printf("\nERROR!\n") ;
			break ;
	}
	return TRUE_ANA ;
}
