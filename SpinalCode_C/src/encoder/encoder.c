#include "encoder/encoder.h"

static int spine_length=SPINE_LENGTH;

//dived message into k bits.
static inline void diviedMessage(const char *s, uint32_t *res)
{
    int m = (MES_LENGTH* 8 / K);
    if((MES_LENGTH*8)%K!=0)
        m++;

    int pointer = 0;
    int count = 0;

    for (int i = 0; i < m; i++)
        res[i] = 0 ;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < K; j++)
        {
            res[i] |= ((s[count] & (1 << pointer))) >> (pointer) == 1 ? (1 << (j)) : 0;
            ++pointer;
            if (pointer == 8)
            {
                ++count;
                pointer = 0;
            }
        }
    }
}

char*  SpinalEncode(const char* message)
{

    uint32_t divideMessage[spine_length];
    diviedMessage(message, divideMessage);

    uint32_t spine[spine_length];
    uint32_t spine_value=0;
    RNG rngs[spine_length];
    for(int i=0;i<spine_length;i++)
    {
        spine_value= hash_func(spine_value,divideMessage[i]);
        spine[i]=spine_value;
        set_rng(&rngs[i],spine_value);
    }

    int pass_length=spine_length*PASS;
    uint8_t * out_symbols = malloc(pass_length*sizeof(uint8_t));
    for(int i=0;i<pass_length;i++)
    {
        out_symbols[i]= map_func(next(&rngs[i%spine_length]));
    }

    return out_symbols;
}
