#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pncode.h"

uint8_t PN9 (void);
uint8_t PN15(void);
uint8_t Str2Bin(char *s);


uint8_t PN9 (void)
{
#define TAP1    (5)
#define TAP2    (9)

    static uint8_t  ff[D_PN9] = {1,1,1,1,1,1,1,1,1,};
    uint8_t         v, n;

    v = ff[D_PN9-1];
    n = ff[TAP1 -1] ^ ff[TAP2 -1];
    for (int i=D_PN9-1; i>0; i--) {
        ff[i] = ff[i-1];
    }
    ff[0] = n;

    return (v);

#undef  TAP1
#undef  TAP2
}

uint8_t PN15(void)
{
#define TAP1    (14)
#define TAP2    (15)

    static uint8_t  ff[D_PN15] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};
    uint8_t         v, n;

    v = ff[D_PN15-1];
    n = ff[TAP1 -1] ^ ff[TAP2 -1];
    for (int i=D_PN15-1; i>0; i--) {
        ff[i] = ff[i-1];
    }
    ff[0] = n;

    return (v);

#undef  TAP1
#undef  TAP2
}

uint8_t Str2Bin(char *s)
{
    uint8_t l = strlen(s);
    uint8_t u = 0;

    for (uint8_t i=0; i<l; i++) {
        u <<= 1;
        u |= (*(s+i) == '0') ? 0 : 1;
    }

    return (u);
}


#define D_PREFIX    ("H_")

int main (int argc, char *argv[])
{
    char  filename[256];
    char  ss[256];
    int   pn;
    int   flg;

    pn  = 9;
    flg = 0;

    filename[0] = 0;
    strcpy(filename, D_OUTPUT_FILENAME);

    for (int i=0; i<argc; i++) {
        if (*argv[i] == '-') {
            switch ( *(argv[i]+1) ) {
                case 'f':
                case 'F':
                    filename[0] = 0;
                    strcpy(filename, argv[++i]);
                    break;

                case 'd':
                case 'D':
                    pn = atoi(argv[++i]);
                    break;

                case 'h':
                case 'H':
                    flg = 1;
                    break;
            }
        }
    }
    if (D_PN9 != pn && D_PN15 != pn) {
        //UsagePrint();
        return (1);
    }

    ss[0] = 0;
    sprintf(ss, "%d", pn);
    strcat(filename, ss);
    strcat(filename, D_OUTPUT_EXTENSION);

    FILE  *fp;
    FILE  *hp;

    fp = fopen(filename, "w");
    if (NULL == fp) {
        D_PRINT("%s","fopen error!\n");
        return (2);
    }

    for (int i=0; i<((1<<pn)-1); i++) {
        fprintf(fp, "%x", ((D_PN9 == pn) ? PN9() : PN15()));
    }
    fclose(fp);

    if (flg) {
        ss[0] = 0;
        strcpy(ss, D_PREFIX);
        strcat(ss, filename);

        fp = fopen(filename, "r");
        hp = fopen(ss,       "w");
        if (NULL == fp || NULL == hp) {
            fclose(fp);
            fclose(hp);
            return (3);
        }

        char  s[9];
        while (fgets(s, sizeof(s), fp) != NULL) {
            fprintf(hp, "0x%02x,\n", Str2Bin(s));
        }
        fclose(fp);
        fclose(hp);
    }

    return (0);
}
