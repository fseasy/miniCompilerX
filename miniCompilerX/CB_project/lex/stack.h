#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#define STACK_SIZE 200
/**
    a FA Stack
    full Asc £º ÂúµÝÔö
*/
typedef struct Stack
{
    char list[STACK_SIZE] ;
    int top ;
} Stack ;
void initStack(Stack * stack)
{
    stack->top = -1 ;
}
int isEmpty(Stack * stack)
{
    if(stack->top == -1)
    {
        return TRUE ;
    }
    else
    {
        return FALSE ;
    }
}
int isFull(Stack * stack)
{
    if(stack->top != STACK_SIZE -1)
    {
        return FALSE ;
    }
    else
    {
        return TRUE ;
    }
}
int push(Stack * stack,char c)
{
    if(isFull(stack))
    {
        return FALSE ;
    }
    stack->top++ ;
    stack->list[stack->top] = c ;
    return TRUE ;
}
int pop(Stack * stack , char * pC)
{
    if(isEmpty(stack))
    {
        pC = NULL ;
        return FALSE ;
    }
    *pC = stack->list[stack->top--] ;
    return TRUE ;
}

#endif // STACK_H_INCLUDED
