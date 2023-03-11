#include <stdio.h>
#include <string.h>
#include "jenkins_hash/spooky-c.h"
#include "multinodetree/multinodetree.h"
#include "encoder/encoder.h"
#include "decoder/decoder.h"

int k = 4; // Each message packet has k bits;
int c = 8; // Each transmitted symbol has c bits;
int B = 4; // B nodes kept after pruning.

int main()
{
    // byte = char, char encoded in ASCII, 8 bits per char.
    char *message = "Wow Spinal code is cool lol";    
    int message_len = strlen(message);

    int symbol_packet_len = (message_len * 8 / k) * c / 8;
    if(((message_len * 8 / k) * c) % 8!=0)
    symbol_packet_len++;


    char symbols[symbol_packet_len];    //symbol transmited in VLC Channel
    SpinalEncode(message, symbols,symbol_packet_len);

    /************************ Step 3: Decoding ***********************/
    char decoded_message[message_len];
    for(int i=0;i<message_len;i++)
    decoded_message[i]='\0';
    SpinalDecode(symbols,symbol_packet_len,decoded_message,message_len);

    for(int i=0;i<message_len;i++)
    printf("%c",decoded_message[i]);

    return 0;
}
