#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct node {
    string NumIn;
    string gate;
    string gateName;
    string gateInputs;
    struct node *left;
    struct node *right;
    struct node *center;
    //vector<node*> children;
};

struct node *newNode(int data,string name, string gate,string inputs) {
    struct node *node = (struct node *)malloc(sizeof(struct node));
    node->NumIn = data;
    node->gate = gate;
    node->gateName = name;
    node->gateInputs = inputs;
    node->left = NULL;
    node->right = NULL;
    node->center = NULL;
    return(node);
}
/*
void traversePreOrder(struct node *temp) {
    for (int i = 0; i < temp->children.size(); i++) {
        cout << " " << temp->NumIn;
        traversePreOrder(temp->children[i]);
    }
}
*/
void traverseInOrder(struct node *temp) {
    if (temp != NULL) {
        traverseInOrder(temp->left);
        cout << " " << temp->NumIn;
        traverseInOrder(temp->right);
    }
}
/*
void traversePostOrder(struct node *temp) {
    if (temp != NULL) {
        traversePostOrder(temp->left);
        traversePostOrder(temp->right);
        cout << " " << temp->data;
    }
}
*/
