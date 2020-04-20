#include <stdio.h>
#include <string.h>
#include "Translation.h"

int main()
{
    int b1,b2,dl,t;
    char dig[20];
    scanf("%d %d\n", &b1,&b2);
    gets(dig);
    t=dot(dig); // dot search
    dl=strlen(dig);
    if ((verify_system(b1,b2)==1)||(verify_len(dig)==1)||(cifry(dig,dl,b1)!=0)||(dot(dig)==-1)||(verify_symb(dig)!=0))
        //checking conditions for input
        printf("Bad input");
    else
    if (t!=0)
    {
        z_part(dig, b1, b2, t);  //  break the number into two parts
        d_part(dig, b1, b2, t, dl);
    }
    else
    {
        z_part(dig,b1,b2,dl);
    }
    return 0;
}