#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "encoder/encoder.h"
#include"jenkins_hash/spooky-c.h"

void DiviedMessage(const char *s, int k, char *res)
{
    int m = (strlen(s) * 8 / k);
    int pointer = 0;
    int count = 0;
    for (int i = 0; i < m; i++)
        res[i] = '\0';

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
        {
            res[i] |= ((s[count] & (1 << 8 - pointer - 1))) >> (8 - pointer - 1) == 1 ? (1 << (k - j - 1)) : 0;
            ++pointer;
            if (pointer == 8)
            {
                ++count;
                pointer = 0;
            }
        }
    }
}

void tempSymbol2Symbols(const int *tmp, char *symbols,int lenth,int packet_len,int c)
{
    int pointer = 0;
    int count = 0;

    for (int i = 0; i < lenth; i++)
        symbols[i] = '\0';

    for (int i = 0; i < packet_len; i++)
    {
        for (int j = 0; j < c; j++)
        {
            symbols[count] |= ((tmp[i] & (1 << c - j - 1))) >> (c - j - 1) == 1 ? (1 << (8 - pointer - 1)) : 0;
            ++pointer;
            if (pointer == 8)
            {
                ++count;
                pointer = 0;
            }
        }
    }
}


void SpinalEncode(const char* message,char* symbols,int symbol_packet_len,int k,int c)
{
    /************************ Step 1: Initialization ***********************/

    //  分割信息
    int packet_len = strlen(message) * 8 / k;
    char divideMessage[packet_len];
    DiviedMessage(message, k, divideMessage);

    /************************ Step 2: Encoding ***********************/

    // 初始化
    int s_0 = 0; // 最初的s0
    RNG rngs[packet_len];

    // RNG[0]
    rngs[0].spineValue = spooky_hash32(&divideMessage[0], sizeof(divideMessage[0]), s_0);
    rngs[0].c = c;

    // 计算后续RNG
    for (int i = 1; i < packet_len; i++)
    {
        rngs[i].spineValue = spooky_hash32(&divideMessage[i], sizeof(divideMessage[i]), rngs[i - 1].spineValue);
        rngs[i].c = c;
    }

    // generate pesudo-random number
    int tmp_symbol[packet_len];
    for (int i = 0; i < packet_len; i++)
    {
        srand(rngs[i].spineValue);
        double range = pow(2, 6);
        tmp_symbol[i] = rand() % (int)range;
    }

    // generate symbols
    tempSymbol2Symbols(tmp_symbol, symbols,symbol_packet_len,packet_len,c);

}