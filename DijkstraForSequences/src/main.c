#include <stdio.h>
#include <string.h>
#include "swap.h"

int main()
{
    char chislo[11];
    int p,dlina,check=0;
    gets(chislo);
    scanf("%d", &p);
    dlina=strlen(chislo);
    if (dlina==1)
        return 0;
    else if (check_symb(chislo)==1||dlina>10||unique(chislo)!=0)
        printf("bad input");
    else
        while (check<p)
        {
            if (proverka(chislo) == -1)
            {
                new_first(chislo);
                printf("%s\n",chislo);
            }
            else
            {
                perestanovka_s_conca(chislo, proverka(chislo));// зацикливается на перестановке
                printf("%s\n",chislo);
            }
            check += 1;
        }
    return 0;
}
