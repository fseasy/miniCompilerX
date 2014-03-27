int main()
{
int num ;
bool isTrue ;
char x ;
double dnumber ;
num = 1 ;
isTrue = true ;
x = '\t' ;
dnumber = 23.67 ;
num *= 2 ;
for( ; num >= 0; num--)
{
if(num >= 1)
{
continue ;
}
if(num == 0)
{
break ;
}
}
if(isTrue)
{
printf("Has tested output keyword\n") ;
}
else
{
scanf("%lf",&dnumber) ;
if(dnumber < 20.0 && num == 0)
{
testFn() ;
}
}
return 0 ;
}
void testFn()
{
return false ;
}