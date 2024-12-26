#include<stdio.h>
#include<stdlib.h>

typedef struct node{
    int data;
    int height;
    struct node* left;
    struct node* right;
}node;

typedef node* pnode;




int height(pnode root){
    if(root==NULL)
        return -1;
    else return root->height;
}


pnode rotate_L(pnode TF){
    pnode temp=TF->right;
    TF->right=temp->left;
    temp->left=TF;
    TF->height=(height(TF->left)>height(TF->right)?height(TF->left):height(TF->right))+1;
    temp->height=(TF->height>height(temp->right)?TF->height:height(temp->right))+1;
    return temp;

}

pnode rotate_R(pnode TF){
    pnode temp=TF->left;
    TF->left=temp->right;
    temp->right=TF;
    TF->height=(height(TF->right)>height(TF->left)?height(TF->right):height(TF->left))+1;
    temp->height=(TF->height>height(temp->left)?TF->height:height(temp->left))+1;
    return temp;

}



pnode insert(pnode root,int data){
    if(root==NULL){
        pnode newnode=(pnode)malloc(sizeof(node));
        newnode->data=data;
        newnode->height=0;
        newnode->right=newnode->left=NULL;
        return newnode;
    }
    else if(data<root->data){
        root->left = insert(root->left,data);
        if(height(root->left)-height(root->right)>1){
            if(data<root->left->data){
                root=rotate_R(root);
            }else{
                root->left=rotate_L(root->left);
                root=rotate_R(root);
            }
        } 
    }else if(data>root->data){
        root->right = insert(root->right,data);
        if(height(root->right)-height(root->left)>1){
            if(data>root->right->data){
                root=rotate_L(root);
            }else{
                root->right=rotate_R(root->right);
                root=rotate_L(root);
            }
        }
    }

    root->height=(height(root->left)>height(root->right)?height(root->left):height(root->right))+1;

    return root;

}





int main(){
    int n;
    int data;
    pnode root =NULL;
    scanf("%d",&n);
    for(int i =0;i<n;i++){
        scanf("%d",&data);
        root = insert(root,data);
    }
    printf("%d",root->data);
}