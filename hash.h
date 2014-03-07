#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED


int hashpjw(char * str)
{
    char * p ;
    unsigned int h = 0 , g ;
    for(p = str ; *p != '\0' ; p = p+1)
    {
        h = (h << 4) + (*p) ;
        if(g = h & 0xf0000000)
        {
            h = h^(g >> 24) ;
            h = h^g ;
        }
    }
    return h % PRIME ;
}
int getHashCode(char * str)
{
    return hashpjw(str) ;
}


#endif // HASH_H_INCLUDED
