#include "encoder/encoder.h"
#include "decoder/decoder.h"

#include <stdio.h>
#include <time.h>

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
    char *message = "Fo1!";
    char *mesB ="NTU0";
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

//    network_coding(symbols,test_symbols);
    printf("network coding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",symbols[i]);
    }
    printf("\n");

    uint8_t menchster_symbols[PASS_LENGTH*C*2]={0};
    menchster_coding(symbols,menchster_symbols);
    printf("menchster coding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS*C*2;i++)
    {
        printf("%d",menchster_symbols[i]);
    }
    printf("\n");

    uint8_t  deOOK_symbols[PASS_LENGTH]={0};
    decode_OOK(menchster_symbols,deOOK_symbols);
    printf("decoding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",deOOK_symbols[i]);
    }
    printf("\n");

//    network_coding(deOOK_symbols,test_symbols);
    printf("network decoding symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",deOOK_symbols[i]);
    }

    uint8_t decoded_message[MES_LENGTH+1];
    decoded_message[MES_LENGTH]='\0';
    clock_t start_t,end_t;
    start_t=clock();
//    SpinalDecode(deOOK_symbols,decoded_message);
    end_t=clock();
    SpinalDecode(deOOK_symbols,decoded_message);
    printf("time:%f\n",(double)(end_t-start_t)/CLOCKS_PER_SEC);
    free(symbols);

    printf("decoded:%s",decoded_message);
    printf("\n");
    return 0;
}


//int main()
//{
//    int a[30]={0};
//    int b[10]={1,2,3,4,5,6,7,8,9,0};
//    memcpy(&a[0],b,10*sizeof (int));
//    memcpy(&a[20],b,10);
////    memset(a,0,10);
//    for(int i=0;i<30;i++)
//    {
//        printf("%d ",a[i]);
//    }
//}