#include "Tree.h"



struct Tree* createTree()
{
    struct Tree* tree = (struct Tree*)malloc(sizeof(struct Tree));
    struct TreeNode* root = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    root->size=1024;
    tree->root=root;
    return tree;
}





void insert(struct TreeNode* root)
{
    if(root->size==1)
         return;

    struct TreeNode* leftNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    leftNode->size=root->size/2;
    leftNode->begin=root->begin;
    leftNode->end=leftNode->begin+leftNode->size-1;
    root->left=leftNode;
    leftNode->parent=root;

    struct TreeNode* rightNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    rightNode->size=root->size/2;

    rightNode->begin=leftNode->end+1;
    
    rightNode->end=root->end;
    
    root->right=rightNode;
    rightNode->parent=root;

    createTree(leftNode);
    createTree(rightNode);
}