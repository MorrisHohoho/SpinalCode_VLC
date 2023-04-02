#include "encoder/encoder.h"
#include"jenkins_hash/spooky-c.h"

extern int k;
extern int c;



void SpinalEncode(const char* message,char* symbols,int symbol_packet_len)
{
    /************************ Step 1: Initialization ***********************/

    //Divide Message into pieces,every piece has k bit.
    int packet_len = strlen(message) * 8 / k;
    if((strlen(message)*8)%k!=0)
    packet_len++;

    uint32_t divideMessage[packet_len]; 
    diviedMessage(message, divideMessage);

    /************************ Step 2: Encoding ***********************/
    int s_0 = 0; 
    RNG rngs[packet_len];

    // RNG[0]
    rngs[0].spineValue = spooky_hash32(&divideMessage[0], sizeof(divideMessage[0]), s_0);

    // Calculate the next RNG
    for (int i = 1; i < packet_len; i++)
    {
        rngs[i].spineValue = spooky_hash32(&divideMessage[i], sizeof(divideMessage[i]), rngs[i - 1].spineValue);
    }

    // generate pesudo-random number
    int tmp_symbol[packet_len];
    int range = 1<<c;       //which equals to pow(2,6);
    for (int i = 0; i < packet_len; i++)
    {
        srand(rngs[i].spineValue); 
        tmp_symbol[i] = rand() % range;
    }

    // generate symbols
    tempSymbol2Symbols(tmp_symbol, symbols,symbol_packet_len,packet_len);

}

void diviedMessage(const char *s, uint32_t *res)
{
    int m = (strlen(s) * 8 / k);
    if((strlen(s)*8)%k!=0)
    m++;

    int pointer = 0;
    int count = 0;

    for (int i = 0; i < m; i++)
        res[i] = 0 ;

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

void tempSymbol2Symbols(const int *tmp, char *symbols,int lenth,int packet_len)
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