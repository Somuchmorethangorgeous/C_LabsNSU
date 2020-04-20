//
// Created by roxxa on 30.11.2019.
//

#include "swap.h"

int unique(char*a)
{
    int k=0;
    for (int i=0; i<strlen(a);i++)
        for (int j=i+1;j<strlen(a);j++)
            if (a[i]==a[j])
            {
                k+=1;
            }
    return k;
}

int proverka(char*a)
{
    int last_element=strlen(a)-1;
    int k=last_element-1;

    while (a[last_element]<a[k] && k>0)
    {
        k--;
    }
    if (k==0)
        return -1;
    else
        return k;
}


char* sortirovka(char* a, int k)
{
    char m;
    for (int i=k+1;i<strlen(a)-1;i++)
        for (int j=i+1; j<strlen(a);j++)
            if (a[i]>a[j])
            {
                m=a[i];
                a[i]=a[j];
                a[j]=m;
            }
    return a;
}




char* new_first(char*a)
{
    int index;
    char min='A';
    for (int j = 1; j < strlen(a); j++)
        if (a[j] > a[0] && a[j] <= min)
        {
            index = j;
            min = a[j];
        }
    a[index]=a[0];
    a[0]=min;
    if (a[0]=='A')
        exit(0);
    else
    {
        sortirovka(a,0);
    }
    return a;
}




int perestanovka_s_conca(char*a, int b)
{
    int i = strlen(a)-1;
    char p;
    if (b != 0)
    {
        p = a[i];
        a[i] = a[b];
        a[b] = p;
        sortirovka(a, b);
        return b;
    }
    else
    {
        return -1;
    }
}


int check_symb(char* a)
{
    int check=0;
    for (int i=0;i<strlen(a);i++)
    {
        if (a[i] >= '0' && a[i] <= '9')
        {}
        else
            check+=1;
    }
    return check;
}