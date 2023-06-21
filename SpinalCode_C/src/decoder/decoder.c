#include <string.h>
#include <stdint.h>
#include "decoder/decoder.h"

static  Wavefront self_wavefront[1024]={0};
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

void sort_subtrees(Wavefront subtrees[SUBTREES_MAX][WAVEFRONT_MAX],int l, int r)
{
    if(l>=r) return;


    int i=l-1,j=r+1,x= min_subtree_nodes(subtrees[l+r>>1]);
    while(i<j)
    {
        do i++;while(min_subtree_nodes(subtrees[i])<x);
        do j--;while(min_subtree_nodes(subtrees[j])>x);
        if(i<j)
        {
            Wavefront tmp[1<<K]={0};
            memcpy(&tmp,&subtrees[i],sizeof(Wavefront)*(1<<K));
            memcpy(&subtrees[i],&subtrees[j],sizeof(Wavefront)*(1<<K));
            memcpy(&subtrees[j],&tmp,sizeof(Wavefront)*(1<<K));
        }
    }
    sort_subtrees(subtrees,l,j);
    sort_subtrees(subtrees,j+1,r);
}

 void prune_wavefront() {
     const int num_subtree_nodes = (1 << (K * (D - 1)));
     Wavefront subtrees[SUBTREES_MAX][WAVEFRONT_MAX] = {0};
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
}


void advance(const uint8_t* symbols)
{
    Wavefront new_wavefront[WAVEFRONT_MAX]={0};
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
               int distance = symbols[received_symbol]-node_symbol;
               edge_metric+=distance*distance;
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
    prune_wavefront();

}

static void inline initWavefront()
{
    self_wavefront->path_metric=0;
    self_wavefront->spine_value=0;
}

static void get_most_likely(char* ret)
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

    uint64_t message_as_number =0;
    for (int i=node_depth;i>=0;i--)
    {
        message_as_number = (message_as_number<<K)|self_wavefront[best_node].path[i];
    }

    int n = (node_depth*K+7)/8;
    for(int i=0;i<n;i++)
    {
        ret[i]=(char)message_as_number&0xFF;
        message_as_number>>=8;
    }
}
void SpinalDecode(const uint8_t *symbols, uint8_t *decoded_message)
{

    initWavefront();

    uint8_t tmp4advance[PASS+1];
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