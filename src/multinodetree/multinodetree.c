#include<math.h>
#include<stdlib.h>
#include<stdint.h>
#include "multinodetree/multinodetree.h"
#include "encoder/encoder.h"
#include "jenkins_hash/spooky-c.h"

void BuildChild(struct MultiTree* parent,int *symbol,int k,int c)
{
    RNG tmp_rng;
    int predict_symbol;
    int tmp_cost;
    int depth = parent->depth+1;
    for(int i=0;i<(1<<k);i++)
    {
        //Create SpineValue
        uint32_t predict_message=i;  
        tmp_rng.spineValue=spooky_hash32(&predict_message,sizeof(predict_message),parent->spine_value);
        tmp_rng.c=c;

        //Create predict symbol
        srand(tmp_rng.spineValue);
        int range = 1<<c;
        predict_symbol = rand()%range;

        //Compute the cost
        tmp_cost = (predict_symbol-symbol[depth])*(predict_symbol-symbol[depth]);

        //填充子节点
        parent->child[i]=(struct MultiTree*)malloc(sizeof(struct MultiTree));
        parent->child[i]->message_int=i;
        parent->child[i]->rng=predict_symbol;
        parent->child[i]->spine_value=tmp_rng.spineValue;
        parent->child[i]->cost=tmp_cost;
        parent->child[i]->parent=parent;
        parent->child[i]->depth=depth;

    }

}


void swap(struct MultiTree * a,struct MultiTree * b)
{
    struct MultiTree c;
    c = *b ;
    *b = *a;
    *a = c;
}

void quick_sort(struct MultiTree* node,int l,int r)
{
    if(l>=r) return;
    int i=l-1,j=r+1,x=node->child[l+r>>1]->cost;
    while(i<j)
    {
        do i++;while(node->child[i]->cost<x);
        do j--;while(node->child[j]->cost>x);
        if(i<j) swap(node->child[i],node->child[j]);
    }
    quick_sort(node,l,j),quick_sort(node,j+1,r);
}

void PruningTree(struct MultiTree* parent,int k,int B)
{
    //Select B trees whose cost is the min;
    quick_sort(parent,0,(1<<k)-1);

    //Free the memory of which is not used.
    for(int i=B;i<1<<k;i++)
    {
        free(parent->child[i]);
        parent->child[i]=NULL;
    }
}

void SortingTree(struct MultiTree* parent,int k)
{
        quick_sort(parent,0,(1<<k)-1);
}
