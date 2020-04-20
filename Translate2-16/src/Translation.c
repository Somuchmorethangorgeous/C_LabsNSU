//
// Created by roxxa on 30.11.2019.
//

#include "Translation.h"

int verify_system(int s1, int s2)
{
    if (s1 >= 2 && s1 <= 16 && s2 >= 2 && s2 <= 16)
        return 0;
    else
        return 1;
}


int cifry(char*a, int n, int sys1)
{
    int p=0;
    char symb[16]="0123456789abcdef";
    char big_symb[16]="0123456789ABCDEF";
    for (int i=0;i<n;i++)
        for (int j=0; j<16;j++)
        {
            if (a[i]==symb[j]||a[i]==big_symb[j])
            {
                a[i] = symb[j];
                if (j < sys1)
                {
                    p += 0;
                }
                else
                {
                    p += 1;
                }
            }
        }
    return p;
}


int verify_len(char* a)
{
    int n;
    n = strlen(a);
    if (n > 14)
        return 1;
    else
        return 0;

}


int dot (char* a)
{
    int p=0,k=0;
    char s='.';
    for (int i=0;i<strlen((a));i++)
        if (a[i]==s)
        {
            k=i;  // number of the dot
            p+=1; //quantity of dots
        }
    if (p>1||a[0]==s||(a[k+1]=='\0'&& k!=0))
        return -1;
    else
        return k;
}


int verify_symb (char*s)
{
    int trash = 0;
    for (int i = 0; i < strlen(s); i++)
    {
        if ((s[i] >= 'a' && s[i]<='f') || ((s[i] >= '0') && (s[i] <= '9')) || (s[i] == '.'))
        {}
        else
        {
            trash += 1;
        }
    }
    return trash;
}


void final_ch2(double z_part, int b2)
{
    int i=0;
    char b[12];
    while ((z_part*b2)!=0 && i<12)
    {
        b[i] = ((z_part) * b2) / 1; // transfer to b2-system 2-nd part
        if (b[i]!=0)
        {
            z_part=fmod(z_part*b2,b[i]);
        }
        else
        {
            z_part*=b2;
        }
        i++;
    }
    for (int j=0;j<i;j++)
        if (j==0) // add the dot
        {
            printf(".%x", b[j]);
        }
        else
        {
            printf("%x", b[j]);
        }
}


void final_ch1(long long int ch, int razmer, int b2)
{
    char a[razmer];
    int i = razmer-1;
    while (ch >= b2) // transfer to b2 - system main part
    {
        a[i]=ch % b2;
        ch /= b2;
        i--;
    }
    a[i] = ch;
    for (int j=0;j<razmer;j++)
        printf("%x",a[j]);
}


void z_part(char*a, int b1, int b2, int c)
{
    long long int n=0,z_num;
    int digits=1;
    char symb[16]="0123456789abcdef";
    for (int i=0;i<c;i++) //  translate a string with integer part
        for (int j=0; j<16;j++)
        {
            if (a[i]==symb[j])
                n+=j*pow(b1,c-i-1);
        }
    z_num=n;

    while (n>=b2)
    {
        n /= b2;
        digits++;
    }
    final_ch1(z_num,digits,b2);
}


void d_part(char*a, int b1, int b2, int c, int d)
{
    int degree=-1;
    double k=0;
    char symb[16]="0123456789abcdef";
    for (int i=c+1;i<d;i++) // translate a string with fractional part
        for (int j=0;j<16;j++)
        {
            if (a[i]==symb[j])
                k+=j*pow(b1,degree--);
        }
    final_ch2(k,b2);
}