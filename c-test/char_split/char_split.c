#include <stdio.h>
#include <string.h>
#include <stdint.h>

// char *get_char()
// {
//     char str[] = "{4C00FDC14E,0000000000,4C00FDC14E,0000000000,7ED0ADF13C,0000000000,8ED0EFF90C}";
//     // char *str= "4C00FDC14E";

//     return str;
// }

struct rt_code
{
    /* data */

    char c[11];
    char *a;
};
typedef struct rt_code rt_code_t;

void split1(char *ch)
{
    // char ch[] = "{4C00FDC14E,0000000000,4C00FDC14E,0000000000,7ED0ADF13C,0000000000,8ED0EFF90C}";
    // char ch[]="";
    // ch = str;
    const char *p = "{,}";
    char *ret;
    rt_code_t a[7];
    int i = 0;
    ret = strtok(ch, p);

    while (ret)
    {
        // a[i]->c = "";
        strncpy(a[i].c, ret, 11);
        ret = strtok(NULL, p);
        i++;
    }

    for (int i = 0; i < 7; i++)
    {

        printf("%s\n", a[i].c);
    }
}

void split(char *str)
{
    char ch[80];
    char cc[10];
    rt_code_t a[7];

    int i = 0;

    sscanf(str, "%*[{]%s%", ch);
    printf("%s\n", ch);

    char *p = ch;
    int32_t len = strlen(ch);
    // sscanf(p+22,"%50[0-9A-Z]",&cc);
    while (p < ch + len)
    {
        sscanf(p, "%10[0-9A-Z]", cc);

        // *(a[i]->c) = cc[10];
        p += 11;

        // a[i]->a = cc;

        i++;
    }

    // for (int i = 0; i < 7; i++)
    // {

    //     printf("%s\n", a[i]->c);

    // }
}

int main(int argc, char const *argv[])
{
    char str[] = "{4C00FDC14E,0000000000,4C00FDC14E,0000000000,7ED0ADF13C,0000000000,8ED0EFF90C}";
    
    split1(str);

    /* code */
    // char str[60] = "";
    // char str2[100] = "";
    // sscanf("{4C00FDC14E,0000000000,4C00FDC14E,0000000000,7ED0ADF13C,0000000000,0000000000}", "%*[{]%s%",str);

    // printf("str=%s\n", str);
    // printf("str=%s\n", str2);
    return 0;
}
