#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned short ush;
typedef unsigned long ul;

typedef struct num
{
    struct num* pNext;
    struct num* pPrev;
    ush digit;
} numS;

typedef struct string
{
    struct string* pNext;
    struct string* pPrev;
    char staff;
} string;

void Print(numS*);
void FreeNum(numS*, numS*);
void FreeStrS(string*, string*);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
5.1 (19). Разработать функцию, переводящую многоразрядное десятичное
число в двоичную систему. Для хранения многоразрядного числа
использовать динамический двунаправленный список.
*/
int main()
{
    FILE* data;
    FILE* rez;
    data = fopen("...\\num.txt","r");  //number to convert
    rez = fopen("...\\rez.bin","w");   //result

    if (!data)
    {
        return 1;
    }
    if (!rez)
    {
        fclose(data);
        return 1;
    }

    fseek(data, 0, SEEK_END);
    ul len = ftell(data) + 1;
    char* str = (char* )malloc(sizeof(char)*len);

    fseek(data, 0, SEEK_SET);
    fgets(str,len,data);

    fclose(data);

    numS* num = (numS* )malloc(sizeof(numS));
    num->digit = str[0] - '0';
    num->pPrev = NULL;

    numS* tale = num;
    {
        for (ul i = 1; i < len; i++, tale = tale->pNext)
        {
            tale->pNext = (numS* )malloc(sizeof(numS));
            tale->pNext->digit = str[i] - '0';
            tale->pNext->pPrev = tale;
        }
    }
    tale = tale->pPrev;
    free(tale->pNext);
    tale->pNext = NULL;

    free(str);

    string* strS = (string* )malloc(sizeof(string));
    strS->pPrev = NULL;
    string* taleStr = strS;
    {
        string* p = strS;
        for (numS* zero = num ; zero;)
        {
            p->staff = tale->digit % 2 + '0';
            p->pNext = (string* )malloc(sizeof(string));
            p->pNext->pPrev = p;
            p = p->pNext;

            for (numS* p = num; p; p = p->pNext)
            {
                if ((p->digit % 2 == 1) && (p->pNext))
                {
                    p->pNext->digit += 10;
                }

                p->digit /= 2;
            }

            if (!zero->digit)
            {
                zero = zero->pNext;
            }
        }

        taleStr = p->pPrev;
        free(p);
    }

    for (string* p = taleStr; p; p = p->pPrev)
    {
        fprintf(rez, "%c", p->staff);
    }

    FreeNum(num, tale);
    FreeStrS(strS, taleStr);
    fclose(rez);

    printf("end...");
    system("exit");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void FreeNum(numS* num, numS* tale)
{
    for (numS* p = tale; p->pPrev;)
    {
        p = p->pPrev;
        free(p->pNext);
    }
    free(num);
}

void FreeStrS(string* strS, string* taleStr)
{
    for (string* p = taleStr; p->pPrev;)
    {
        p = p->pPrev;
        free(p->pNext);
    }
    free(strS);
}
