#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.h"


bool memory_allocation( struct ProcessData process,struct TreeNode* root,int* done)
{
    if(root->flag==1)
    {
        return false;
    }
    if(process.size>root->size/2&&root->left->flag==1)
    {
        return false;
    }
    if((process.size) > (root->size))
    {
        root->parent->flag=1;
        root->parent->process_id=process.id;
        *done=1;
        return true;
    }
    bool left,right;
    left=memory_allocation(process,root->left,done);
    if(*done==0)
    {
        right=memory_allocation(process,root->right,done);
    }
    if(root->left->flag==1 && root->right->flag== 1)
    {
        root->flag=1;
    }
    if(left==true || right==true)
    {
        return true;
    }
    return false;
}



bool memory_deallocation(struct ProcessData process, struct TreeNode* root,int* done)
{
   if (root->left==NULL)
   {
         return false;
   }
   if(root->process_id==process.id)
   {
       root->flag=0;
       root->process_id=-1;
       root->parent->flag=0;
       *done=1;
       return true;
   }
   bool left,right;
   left=memory_deallocation(process,root->left,done);
   if(*done==0)
   {
       right=memory_deallocation(process,root->right,done);
   }

   return left || right;
}