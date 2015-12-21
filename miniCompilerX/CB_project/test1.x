void myPutC(char c)
{
	printf("%c",c) ;
}

int main()
{
	//test declaration 
	int a , b ;
	//test assign
	a = 4 ;
	b = 3 ;
	int k ;
	k = a - b ;
	bool xxb ;
	xxb = a == 4 ;
	//test if
	if(k > 0 && xxb)
	{
		//test printf
		printf("k'val is %s\n",k) ;
	}
	else
	{
		printf("k is less than 0\n") ;
	}
	//test loop
	int i  ;
	char xxc ;
	for(i = 3 , xxc = 'x' ; i > 0 ; i--)
	{
		if(i == k)
		{
			double dnum ;
			dnum = a ;
			//b = dbum ; //cast error
			//test break
			break ;
		}
		else
		{
			xxc = 'e' ;
			myPutC(xxc) ;
			//test continue ;
			continue ;
		}
	}
	//test return 
	return 0 ;
}