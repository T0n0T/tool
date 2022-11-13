#include "stdio.h"
#include "stdlib.h"
#include "string.h"
 
 
/**
 * @brief 
 * 
 * @param buf 
 * @param buflen 
 * @param out 
 * @return * int 
 */

int array2str(unsigned char *buf, unsigned int buflen, char *out)
{
    char strBuf[33] = {0};
    char pbuf[32];
    int i;
    for(i = 0; i < buflen; i++)
    {
        sprintf(pbuf, "%02X", buf[i]);
        strncat(strBuf, pbuf, 2);
    }
    strncpy(out, strBuf, buflen * 2);
    printf("out = %s\n", out);
    return buflen * 2;
}

/**
 * @brief 
 * 
 * @param str       输入的字符串
 * @param out       输出的十六进制 数组
 * @param outlen    输出的数组长度
 * @return int 
 */
int str2hex(char *str, unsigned char *out, int *outlen)
{
    if (str == NULL || out == NULL)
        return -1;
 
    int i = 0, ret = 0;
 
    ret = (strlen(str) / (2 * sizeof(char))) + strlen(str) % (2 * sizeof(char));
 
    for (i = 0; i < ret; i++)
        sscanf(str + 2 * i, "%02X", (unsigned int *)(out + i));
 
    if (outlen!= NULL)
        *outlen = ret;
 
    return ret;
}

char *strHex = "4C00FDC14";

int main(int argc, const char *argv)
{
    // char str[33] = {0};
    unsigned char out[5];
    // arrayToStr(ArrayCom, 16, str);
    
    int outlen = 0;

    str2hex(strHex, out, &outlen);
    for (int i = 0; i < outlen; i++)
        printf("%02X ", out[i]);
    // putchar(10);
 
    return 0;
}