#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "memory.h"


bool memory_allocation( struct ProcessData process,struct TreeNode* root,int* done, int* begin, int* end)
{
    if(root->flag==1)
    {
        return false;
    }
    if(process.size>root->size/2&&root->left->flag==1)
    {
        return false;
    }
    if((process.size) > (root->size)/2)
    {
        if(process.size <= root->size)
        {

        struct TreeNode* temp = (struct TreeNode*)malloc(sizeof(struct TreeNode));
        temp=root;
        while(temp!=NULL)
        {
            if(temp->flag==1)
            {
                return false;
            }
            temp=temp->left;
        }
        temp=root;
        while(temp!=NULL)
        {
            if(temp->flag==1)
            {
                return false;
            }
            temp=temp->right;
            
        }

        root->flag=1;
        root->process_id=process.id;
        *done=1;
        //set begin and end
        *begin = root->begin;
        *end = root->end;
        return true;
        }
        else
        {
            return false;
        }
    }
    bool left,right;
    left=memory_allocation(process,root->left,done,begin,end);
    if(*done==0)
    {
        right=memory_allocation(process,root->right,done,begin,end);
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



bool memory_deallocation(struct ProcessData process, struct TreeNode* root,int* done, int* begin, int* end)
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
       *begin = root->begin;
       *end = root->end;
       return true;
   }
   bool left,right;
   left=memory_deallocation(process,root->left,done,begin,end);
   if(*done==0)
   {
       right=memory_deallocation(process,root->right,done,begin,end);
   }

   return left || right;
}

// int main(int argc, char *argv[])
// {
//     printf("Hello World!\n");
//     struct Tree* tree = createTree();
//     // print the tree using the tree node 
//     printTree(tree->root);
//     struct ProcessData process1;
//     process1.id=1;
//     process1.arrivalTime=1;
//     process1.runningTime=10;
//     process1.priority=1;
//     process1.size=1025;
//     int done=0;
//     int begin,end;

//     bool isAllocated = memory_allocation(process1,tree->root,&done,&begin,&end);
//     printf("isAllocated: %d\n",isAllocated);
//     printf("begin: %d end: %d\n",begin,end);
//     done=0;
//     process1.size=511;
//     isAllocated = memory_allocation(process1,tree->root,&done,&begin,&end);
//     printf("isAllocated: %d\n",isAllocated);
//     printf("begin: %d end: %d\n",begin,end);
//     done=0;
//     process1.size=511;
//     isAllocated = memory_allocation(process1,tree->root,&done,&begin,&end);
//     if(isAllocated==true)
//     {
//     printf("isAllocated: %d\n",isAllocated);
//     printf("begin: %d end: %d\n",begin,end);
        
//     }
//     else
//     {
//         printf("Not enough memory\n");
//     }
//     done=0;


//     return 0;



// }
    