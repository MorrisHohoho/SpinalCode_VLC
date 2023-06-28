#include "encoder/encoder.h"
#include "decoder/decoder.h"

#include <stdio.h>

void network_coding(uint8_t * symbolsA, uint8_t* symbolsB)
{
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        symbolsA[i]=symbolsA[i]^symbolsB[i];
    }

}


void menchster_coding(uint8_t * symbols,  uint8_t* ret)
{
    volatile char bit;
    int ret_index=0;
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        for(int j=C-1;j>=0;j--)
        {
            bit = (symbols[i]>>j)&0x01;
            if(bit == 1)
            {
                ret[ret_index++]=0;
                ret[ret_index++]=1;
            }
            else
            {
                ret[ret_index++]=1;
                ret[ret_index++]=0;
            }
        }
    }
}

void decode_OOK(uint8_t* menchster_code, uint8_t* symbols)
{
    int index=1;
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        symbols[i]=0;
        for(int j=0;j<C;j++)
        {
            symbols[i]|=menchster_code[index];
            index+=2;
            symbols[i]<<=1;
        }
        symbols[i]>>=1;
    }
}

int main(int argc, char**argv)
{
    char *message = "abcd";
    char *mesB ="4321";
    uint8_t *symbols=SpinalEncode(message);
    uint8_t *test_symbols = SpinalEncode(mesB);
    printf("origin:%s\t",message);

    printf("symbolsA:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d,",symbols[i]);
    }
    printf("\n");

    printf("symbolsB:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",test_symbols[i]);
    }
    printf("\n");

    network_coding(symbols,test_symbols);
    printf("network coding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",symbols[i]);
    }
    printf("\n");

    uint8_t menchster_symbols[PASS_LENGTH*12]={0};
    menchster_coding(symbols,menchster_symbols);
//    printf("menchster coding symbols:");
//    for(int i=0;i<SPINE_LENGTH*PASS*12;i++)
//    {
//        printf("%d",menchster_symbols[i]);
//    }
//    printf("\n");

    uint8_t  deOOK_symbols[PASS_LENGTH]={0};
    decode_OOK(menchster_symbols,deOOK_symbols);
    printf("decoding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",deOOK_symbols[i]);
    }
    printf("\n");

    network_coding(deOOK_symbols,test_symbols);
    printf("network decoding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",deOOK_symbols[i]);
    }

    uint8_t decoded_message[MES_LENGTH+1];
    decoded_message[MES_LENGTH]='\0';
    SpinalDecode(deOOK_symbols,decoded_message);
    free(symbols);

    printf("decoded:%s",decoded_message);
    printf("\n");
    return 0;
}
