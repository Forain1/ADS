#include <stdio.h>
#include <stdlib.h>


typedef struct node{
    int is_leaf;
    int element_num;
    int element [4];
    struct node* child [4];
    struct node* parent;
}node;

typedef struct node* pnode;


pnode Find(pnode root, int key,int* flag){
    int i;
    if (root == NULL) return NULL;
    else{
        if(root->is_leaf==0){
            for(i=0;i<root->element_num;i++){
                if(root->element[i]>key){
                    return Find(root->child[i],key,flag);
                }
            }
            return Find(root->child[i],key,flag);
        }else{
            for(i=0;i<root->element_num;i++){
                if(key==root->element[i])*flag=1;
            }
            return root;
        }
    }
}


pnode Split(pnode root){
    int i;
    if(root->is_leaf==1){
        if(root->parent==NULL){
             //需要分裂的节点为叶子节点且没有父节点
            pnode newparent=(pnode)malloc(sizeof(node));
            pnode split1=(pnode)malloc(sizeof(node));
            pnode split2=(pnode)malloc(sizeof(node));
            //处理分裂的子结点
            split1->element_num=split2->element_num=2;
            split1->parent=split2->parent=newparent;
            split1->is_leaf=split2->is_leaf=1;
            split1->element[0]=root->element[0];
            split1->element[1]=root->element[1];
            split2->element[0]=root->element[2];
            split2->element[1]=root->element[3];
            split1->child[0]=split1->child[1]=split1->child[2]=split1->child[3]=NULL;
            split2->child[0]=split2->child[1]=split2->child[2]=split2->child[3]=NULL;
            //处理分裂后的父节点
            newparent->parent=NULL;
            newparent->element_num=1;
            newparent->child[0]=split1;
            newparent->child[1]=split2;
            newparent->element[0]=split2->element[0];
            newparent->is_leaf=0;
            free(root);
            return newparent;
        }else{
            //需要分裂的节点为叶子节点且有父节点
            pnode parent=root->parent;
            pnode split1=(pnode)malloc(sizeof(node));
            pnode split2=(pnode)malloc(sizeof(node));
            //处理分裂的子结点
            split1->element_num=split2->element_num=2;
            split1->parent=split2->parent=parent;
            split1->is_leaf=split2->is_leaf=1;
            split1->element[0]=root->element[0];
            split1->element[1]=root->element[1];
            split2->element[0]=root->element[2];
            split2->element[1]=root->element[3];
            split1->child[0]=split1->child[1]=split1->child[2]=split1->child[3]=NULL;
            split2->child[0]=split2->child[1]=split2->child[2]=split2->child[3]=NULL;
            //处理分裂后的父节点
            int index = split2->element[0];
            
            for(i=parent->element_num;i>0;i--){
                if(index<parent->element[i-1]){
                    parent->element[i]=parent->element[i-1];
                    parent->child[i+1]=parent->child[i];
                }else break;
            }
            parent->element[i]=index;
            parent->child[i+1]=split2;
            parent->child[i]=split1;
            parent->element_num++;
            free(root);
            root=parent;
            if(parent->element_num==3) return Split(root);
            else return root;
        }
    }else{
        if(root->parent==NULL){
            pnode newparent=(pnode)malloc(sizeof(node));
            pnode split1=(pnode)malloc(sizeof(node));
            pnode split2=(pnode)malloc(sizeof(node));
            //处理分裂的子结点
            split1->element_num=split2->element_num=1;
            split1->parent=split2->parent=newparent;
            split1->is_leaf=split2->is_leaf=0;
            split1->element[0]=root->element[0];
            split2->element[0]=root->element[2];
            split1->child[0]=root->child[0];
            split1->child[1]=root->child[1];
            root->child[0]->parent=root->child[1]->parent=split1;
            split2->child[0]=root->child[2];
            split2->child[1]=root->child[3];
            root->child[2]->parent=root->child[3]->parent=split2;
            //处理分裂新的父节点
            newparent->parent=NULL;
            newparent->element_num=1;
            newparent->child[0]=split1;
            newparent->child[1]=split2;
            newparent->element[0]=root->element[1];
            newparent->is_leaf=0;
            free(root);
            root = newparent;
            return root;
        }else{
            pnode parent=root->parent;
            pnode split1=(pnode)malloc(sizeof(node));
            pnode split2=(pnode)malloc(sizeof(node));
            //处理分裂的子结点
            split1->element_num=split2->element_num=1;
            split1->parent=split2->parent=parent;
            split1->is_leaf=split2->is_leaf=0;
            split1->element[0]=root->element[0];
            split2->element[0]=root->element[2];
            split1->child[0]=root->child[0];
            split1->child[1]=root->child[1];
            root->child[0]->parent=root->child[1]->parent=split1;
            split2->child[0]=root->child[2];
            split2->child[1]=root->child[3];
            root->child[2]->parent=root->child[3]->parent=split2;
            //处理分裂后的父节点
            int index = root->element[1];
            
            for(i=parent->element_num;i>0;i--){
                if(index<parent->element[i-1]){
                    parent->element[i]=parent->element[i-1];
                    parent->child[i+1]=parent->child[i];
                }else break;
            }
            parent->element[i]=index;
            parent->child[i+1]=split2;
            parent->child[i]=split1;
            parent->element_num++;
            free(root);
            root=parent;
            if(parent->element_num==3)return Split(root);
            else return root;
        }
    }

}


pnode insert(pnode root,int key,int* flag){
    pnode aimnode,temproot;
    int i;
    aimnode = Find(root,key,flag);
    temproot=root;
    if(*flag==1){
        return temproot;
    }
    if(aimnode==NULL){
        root = (pnode)malloc(sizeof(node));
        root->element_num=1;
        root->parent=NULL;
        root->element[0]=key;
        root->is_leaf=1;
        root->child[0]=root->child[1]=root->child[2]=root->child[3]=NULL;
    }else{
        for(i=aimnode->element_num;i>0;i--){
            if(key<aimnode->element[i-1]){
                aimnode->element[i]=aimnode->element[i-1];
            }else break;
        }
        aimnode->element[i]=key;
        aimnode->element_num++;
        if(aimnode->element_num==4) root = Split(aimnode);
    }

    if(root->parent==NULL)return root;
    else return temproot;

}


void PrintBtree(pnode root){
    pnode Queue [10000];
    pnode tempnode;
    int front=0,rear=0;
    int i;
    int befornum;
    if(root==NULL)return;
    else befornum = root->element[0];
    Queue[0]=root;
    while(front<=rear){
        int flag=0;
        tempnode = Queue[front++];
        if(befornum>tempnode->element[0])printf("\n");
        printf("[");
        for(i=0;i<tempnode->element_num;i++){
            
            if(flag==0) {
                 printf("%d",tempnode->element[i]);
                 flag=1;
            }else{
                printf(",%d",tempnode->element[i]);
            }
            
            if(tempnode->child[i]!=NULL)Queue[++rear]=tempnode->child[i];
        }
        befornum=tempnode->element[i-1];
        if(tempnode->child[i]!=NULL)Queue[++rear]=tempnode->child[i];
        printf("]");  
    }
}


int main(){

    int n;
    scanf("%d",&n);
    int i;
    int key;
    int flag;
    pnode root=NULL;
    for(i=0;i<n;i++){
        flag=0;
        scanf("%d",&key);
        root = insert(root,key,&flag);
        if(flag==1){
            printf("Key %d is duplicated\n",key);
        }
    }
    PrintBtree(root);
}