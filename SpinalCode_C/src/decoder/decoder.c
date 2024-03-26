#include <string.h>
#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
#include "decoder/decoder.h"


static  Wavefront self_wavefront[WAVEFRONT_MAX]={0};
static int wave_front_length=1;
static int node_depth=0;
static int spine_length=SPINE_LENGTH;

int min_subtree_nodes(Wavefront* subtree)
{
    int ret= UINT16_MAX;
    for(int i=0;i<(1<<(K*(D-1)));i++) {
        if(subtree[i].path_metric<ret)
            ret=subtree[i].path_metric;
    }
    return ret;
}

void sort_subtrees(Wavefront**subtrees,int l, int r)
{
    if(l>=r) return;


    int i=l-1,j=r+1,x= min_subtree_nodes(subtrees[(l+r)>>1]);
    while(i<j)
    {
        do i++;while(min_subtree_nodes(subtrees[i])<x);
        do j--;while(min_subtree_nodes(subtrees[j])>x);
        if(i<j)
        {
            Wavefront tmp[1<<(K*(D-1))]={0};
            memcpy(&tmp,subtrees[i],sizeof(Wavefront)*(1<<(K*(D-1))));
            memcpy(subtrees[i],subtrees[j],sizeof(Wavefront)*(1<<(K*(D-1))));
            memcpy(subtrees[j],&tmp,sizeof(Wavefront)*(1<<(K*(D-1))));
        }
    }
    sort_subtrees(subtrees,l,j);
    sort_subtrees(subtrees,j+1,r);
}

void prune_wavefront() {
    const int num_subtree_nodes = (1 << (K * (D - 1)));
//     Wavefront subtrees[SUBTREES_MAX][WAVEFRONT_MAX] = {0};
    Wavefront** subtrees= malloc(sizeof(Wavefront*)*SUBTREES_MAX);
    for(int i=0;i<SUBTREES_MAX;i++)
    {
        subtrees[i]=malloc(sizeof(Wavefront)*num_subtree_nodes);
        memset(subtrees[i],0,sizeof(Wavefront)*num_subtree_nodes);
    }
    int num_subtrees = 0;
    while (wave_front_length > 0) {
        memcpy(subtrees[num_subtrees], &self_wavefront[num_subtrees*num_subtree_nodes], num_subtree_nodes * sizeof(Wavefront));
        wave_front_length -= num_subtree_nodes;
        num_subtrees++;
    }
    sort_subtrees(subtrees, 0, num_subtrees-1);

    for (int counter_sub_trees = 0; counter_sub_trees < min(num_subtrees, B); counter_sub_trees++) {
        memcpy(&self_wavefront[counter_sub_trees * num_subtree_nodes],
               subtrees[counter_sub_trees],
               num_subtree_nodes * sizeof(Wavefront));
        wave_front_length += num_subtree_nodes;
    }
    for(int i=0;i<SUBTREES_MAX;i++)
    {
        free(subtrees[i]);
    }
    free(subtrees);
}


void advance(const char* symbols)
{
//    Wavefront new_wavefront[WAVEFRONT_MAX]={0};
    Wavefront* new_wavefront = malloc(sizeof(Wavefront)*WAVEFRONT_MAX);
    memset(new_wavefront,0,sizeof(Wavefront)*WAVEFRONT_MAX);
    int tmp_wave_front_length=0;
    for(int i=0;i<wave_front_length;i++)
    {
        for(int edge=0;edge<(1<<K);edge++)
        {
            uint32_t new_spine_value = hash_func(self_wavefront[i].spine_value, edge);
            RNG rng;
            set_rng(&rng,new_spine_value);
            int edge_metric =0;
            for(int received_symbol=0;received_symbol<strlen(symbols);received_symbol++)
            {
                int node_symbol= map_func(next(&rng));
//                int distance = symbols[received_symbol]-node_symbol;
//                edge_metric+=distance*distance;
                 int n = symbols[received_symbol]^node_symbol;
                 int temp=0;
                 int distance=0;
                 while(n)
                 {
                     temp = n%2;
                     if(temp==1)
                     {
                         distance++;
                     }
                     n=n/2;
                 }
                    edge_metric+=distance;
            }
            int new_path_metric = self_wavefront[i].path_metric+edge_metric;
            new_wavefront[tmp_wave_front_length].path_metric=new_path_metric;
            new_wavefront[tmp_wave_front_length].spine_value=new_spine_value;
            for(int j=0;j<node_depth;j++)
            {
                new_wavefront[tmp_wave_front_length].path[j]=self_wavefront[i].path[j];
            }
            new_wavefront[tmp_wave_front_length].path[node_depth]=edge;
            tmp_wave_front_length++;
        }
    }
    memcpy(self_wavefront,new_wavefront,tmp_wave_front_length*sizeof(Wavefront));
    wave_front_length=tmp_wave_front_length;
    node_depth++;
    free(new_wavefront);
    prune_wavefront();

}

static void inline initWavefront()
{
    memset(self_wavefront,0,sizeof(Wavefront)*WAVEFRONT_MAX);
    wave_front_length=1;
    node_depth=0;
}

static void get_most_likely(uint8_t* ret)
{
    int best_node=0;
    int tmp = UINT16_MAX;
    for(int i=0;i<wave_front_length;i++)
    {
        if(self_wavefront[i].path_metric<tmp)
        {
            tmp=self_wavefront[i].path_metric;
            best_node=i;
        }
    }

    uint64_t message_as_number=0;
    uint64_t total_bits = (node_depth+1)*3;
    uint8_t bits_arr[total_bits];
    uint64_t bit_counter = total_bits-1;
    for (int i=node_depth;i>=0;i--)
    {
//        message_as_number = (message_as_number<<K)|self_wavefront[best_node].path[i];
        uint8_t temp = self_wavefront[best_node].path[i];
        for(int j = K-1; j>=0; j--)
        {
            bits_arr[bit_counter] = (temp>>j)&0x01;
            bit_counter--;
        }
    }

    int n = total_bits/8;
    for(int i=0;i<n;i++)
    {
        uint64_t temp = 0;
        for(int j = (i * 8)+(8-1);j>= (i*8);j--)
        {
            temp |=bits_arr[j];
            temp <<=1;
        }
        temp>>=1;
        ret[i] = (char)temp&0xFF;
    }
}
void SpinalDecode(const uint8_t *symbols, uint8_t *decoded_message)
{

    initWavefront();

    char tmp4advance[PASS+1];
    for(int i=0;i<spine_length;i++)
    {
        for(int j=0;j<PASS;j++)
        {
            tmp4advance[j]=symbols[i+j*spine_length];
        }
        tmp4advance[PASS]='\0';
        advance(tmp4advance);
    }

    get_most_likely(decoded_message);
}