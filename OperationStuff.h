#ifndef OPERATIONSTUFF_H
#define OPERATIONSTUFF_H

#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>

char *get_field(char *str,int fld)
{
    static char t0[512];
    int i=0,j=0,k=0;
    while(i<512)
    {
        if(*(str+i) == ',' || *(str+i) == '\n')
        {
            if (fld==j) return t0;
                j++;
                t0[0]=0;
                k=0;
        }
        else
        {
            t0[k++]=*(str+i);
            t0[k]=0;
        }
        i++;
    }
        t0[0]=0;
        return t0;
}

int checksum_gps(char *str)
{
    unsigned char cs=0,sum=0,i=0;
    char sCS[8], exit =0;

    while (i<250 && !exit)
    {
        if(*(str+i) == '*')
        {
            sum=0;
            break;
        }
        if(sum)
            cs^=*(str+i);
        if(*(str+i) == '$')
            sum=1;
        if(*(str+i) == 0)
            exit=1;
        i++;
    }
    sprintf(sCS,"%02X",cs);
    if (memcmp(sCS,str+i+1,2)==0)
        return 1;
    return 0;
}

char *timestamp(void)
{
    static char t0[128];
    long ms;
    time_t now;
    struct tm *t;
    struct timespec spec;

    time(&now);
    t= localtime(&now);
    clock_gettime(CLOCK_REALTIME, &spec);
    ms= round(spec.tv_nsec/1.0e6);

    sprintf(t0, "%04d/%02d/%02d %02d:%02d:%02d.%03ld ",
            t->tm_year+1900, t->tm_mon+1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec, ms);
    return t0;
}
#endif // OPERATIONSTUFF_H
