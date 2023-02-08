#include <stdio.h>
#include <string.h>
#include "jenkins_hash/spooky-c.h"
#include "multinodetree/multinodetree.h"
#include "encoder/encoder.h"
#include "decoder/decoder.h"

int k = 4; // Each message packet has k bits;
int c = 6; // Each transmitted symbol has c bits;

int B = 4; //

int main()
{

    // byte = char, ASCII编码，每个8位
    char *message = "sichuan,university";    
    int message_len = strlen(message);
    int symbol_packet_len = (message_len * 8 / k) * c / 8;
    char symbols[symbol_packet_len];
    SpinalEncode(message, symbols,symbol_packet_len, k, c);

    // TODO:将字节流再进行曼彻斯特编码后再发送

    /************************ Step 3: Decoding ***********************/

    // TODO:先解码曼彻斯特编码

    char decoded_message[message_len];
    for(int i=0;i<message_len;i++)
    decoded_message[i]='\0';
    SpinalDecode(symbols,decoded_message,message_len,k,c,B);

    return 0;
}
