#include <stdio.h>
#include <math.h>

typedef struct Binomial_Node{
    int key;
    struct Binomial_Node* parent;
    struct Binomial_Node* child;
    struct Binomial_Node* sibling;
    int degree;
}Binomial_Node;

Binomial_Node* merge(Binomial_Node* Tree1, Binomial_Node* Tree2){
    if(Tree1 == NULL){
        return Tree1;
    }
    if(Tree2 == NULL){
        return Tree2;
    }
    Binomial_Node* curH = NULL;

}


int main(int argc, char** argv) {

    return 0;
}
