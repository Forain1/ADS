#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct postNode{
    int num;
    int document;
    struct postNode* nextPost;
}postNode;


typedef postNode* pPostNode;

typedef struct termNode{
    char word [21];
    int times;
    struct termNode* nextTerm;
    pPostNode nextPost;
}termNode;

typedef termNode* pTermNode;





int main(){

    int i,n;
    scanf("%d",&n);
    char** documents = (char**)malloc(n*sizeof(char*));
    pTermNode root = NULL;
    pTermNode tempnode; 
    for(i=0;i<n;i++){
        char* document = (char*)malloc(7*sizeof(char));
        gets(document);
        documents[i]=document;
        char tempchar;
        char word [21];
        int j=0;
        tempnode = root;
        while((getchar(tempchar))!='#'){
            //对每个词的处理
            tempnode=root;
            j=0;
            word[j++]=tempchar;
            while((getchar(tempchar))!=' '){
                word[j++]=tempchar;
            }
            word[j++]='\0';

            //根节点为空的情况,即最开始的初始化
            if(tempnode==NULL){
                //Term根节点的初始化
                root = (pTermNode)malloc(sizeof(termNode));
                strcpy(root->word,word);
                root->times=1;
                root->nextPost=NULL;
                pPostNode rootPost = (pPostNode)malloc(sizeof(postNode));
                root->nextPost=rootPost;

                //Post根节点的初始化
                rootPost->document=i;
                rootPost->nextPost=NULL;
                rootPost->num=1;
                
                continue;
            }
            //根节点不为空，这个时候需要在Term表中查找是否有这个词
            while(tempnode!=NULL){
                if(strcmp(word,tempnode->word)==0){
                    tempnode->times++;
                    pPostNode temppost=tempnode->nextPost;
                    while(temppost->num!=i&&temppost->nextPost!=NULL){
                        temppost=temppost->nextPost;
                    }
                    if(temppost->num==i){
                        temppost->num++;
                    }else{
                        pPostNode newpost = (pPostNode)malloc(sizeof(postNode));
                        newpost->nextPost=NULL;
                        newpost->document=i;
                        newpost->num=1;
                        temppost->nextPost=newpost;
                    }

                }else if(tempnode->nextTerm!=NULL){
                    tempnode=tempnode->nextTerm;
                }else{
                    //新建Term节点并初始化
                    pTermNode newterm = (pTermNode)malloc(sizeof(termNode));
                    newterm->times=1;
                    strcpy(newterm->word,word);
                    pPostNode newpost =(pPostNode)malloc(sizeof(termNode));
                    newterm->nextPost=newpost;
                    newterm->nextTerm=NULL;
                    
                    //新建Post节点并初始化
                    newpost->document=i;
                    newpost->nextPost=NULL;
                    newpost->num=1;

                }
            }
        }
    }

}