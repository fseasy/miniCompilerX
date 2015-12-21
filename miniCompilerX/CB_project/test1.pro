void myPutC(char c)
{
printf("%c",c) ;
}
int main()
{
 int a , b ;
 a = 4 ;
b = 3 ;
int k ;
k = a - b ;
bool xxb ;
xxb = a == 4 ;
 if(k > 0 && xxb)
{
 printf("k'val is %s\n",k) ;
}
else
{
printf("k is less than 0\n") ;
}
 int i ;
char xxc ;
for(i = 3 , xxc = 'x' ; i > 0 ; i--)
{
if(i == k)
{
double dnum ;
dnum = a ;
  break ;
}
else
{
xxc = 'e' ;
myPutC(xxc) ;
 continue ;
}
}
 return 0 ;
}