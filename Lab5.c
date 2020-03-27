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
    ul len = ftell(data);
    char* str = (char* )malloc(sizeof(char)*(len + 1));

    fseek(data, 0, SEEK_SET);
    fgets(str,len + 1,data);

    fclose(data);

    numS* num = (numS* )malloc(sizeof(numS));
    if ((str[0] < '0') || (str[0] > '9'))
    {
        num->digit = 0;
    }
    else
    {
        num->digit = str[0] - '0';
    }
    num->pPrev = (numS*)malloc(sizeof(numS));;
    num->pPrev->pNext = num;

    numS* tale = num;
    {
        unsigned int numOfNaN = 0;
        for (ul i = 1; i < len; i++, tale = tale->pNext)
        {
            tale->pNext = (numS* )malloc(sizeof(numS));
            tale->pNext->pPrev = tale;
            if ((str[i] < '0') || (str[i] > '9'))
            {
                numOfNaN++;
                tale = tale->pPrev;
                continue;
            }
            tale->pNext->digit = str[i] - '0';
            
        }
        len -= numOfNaN;
    }
    free(num->pPrev);
    num->pPrev = NULL;
    tale->pNext = NULL;

    free(str);

    string* strS = (string* )malloc(sizeof(string));
    strS->pPrev = NULL;
    string* taleStr = strS;
    {
        //string* p = strS;
        for (numS* zero = num ; zero;)
        {
            taleStr->staff = tale->digit % 2 + '0';
            taleStr->pNext = (string* )malloc(sizeof(string));
            taleStr->pNext->pPrev = taleStr;
            taleStr = taleStr->pNext;

            for (numS* pN = zero; pN; pN = pN->pNext)
            {
                if ((pN->digit % 2 == 1) && (pN->pNext))
                {
                    pN->pNext->digit += 10;
                }

                pN->digit /= 2;
            }

            if (!zero->digit)
            {
                zero = zero->pNext;
            }
        }

        taleStr = taleStr->pPrev;
        free(taleStr->pNext);
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
