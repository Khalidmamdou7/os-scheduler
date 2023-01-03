#include "../ProcessStructs/ProcessData.h"

struct TreeNode
{
    int process_id;
    struct TreeNode* left;
    struct TreeNode* right;
    struct TreeNode* parent;
    int flag;    
    int size;
    int begin;
    int end;
};
struct Tree
{
    struct TreeNode* root;
};
struct Tree* createTree();

void insert(struct TreeNode* root);

void printTree(struct TreeNode* root);

