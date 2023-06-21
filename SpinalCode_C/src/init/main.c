#include "encoder/encoder.h"
#include "decoder/decoder.h"

#include <stdio.h>


int main(int argc, char**argv)
{
    char *message = "1234";
    uint8_t *symbols=SpinalEncode(message);
    printf("origin:%s\t",message);

    printf("symbols:");
    for(int i=0;i<SPINE_LENGTH*PASS;i++)
    {
        printf("%d ",symbols[i]);
    }
    printf("\n");

    uint8_t decoded_message[MES_LENGTH+1];
    decoded_message[MES_LENGTH]='\0';
    SpinalDecode(symbols,decoded_message);
    free(symbols);

    printf("decoded:%s",decoded_message);
    printf("\n");
    return 0;
}
