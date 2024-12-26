#include<stdio.h>
#include<stdlib.h>


typedef struct node
{
    int v;
    int flag;
}node;

typedef struct queue
{
    node* pnode;
    int num;

}queue;

typedef queue* pqueue;

void push(pqueue Q ,int v,int flag,int check){

    Q->num = Q->num+1;
    int i = Q->num;

    while (i/2!=0)
    {
       if(flag==check&&Q->pnode[i/2].flag!=check){
            Q->pnode[i].flag = Q->pnode[i/2].flag;
            Q->pnode[i].v =  Q->pnode[i/2].v;
            i = i/2;
       }else if(flag==check&&Q->pnode[i/2].flag==check){
            if(Q->pnode[i/2].v>v){
                Q->pnode[i].flag = Q->pnode[i/2].flag;
                Q->pnode[i].v =  Q->pnode[i/2].v;
                i = i/2;                
            }else break;
       }else if(flag!=check&&Q->pnode[i/2].flag==check){
        break;
       }else{
            if(Q->pnode[i/2].v>v){
                Q->pnode[i].flag = Q->pnode[i/2].flag;
                Q->pnode[i].v =  Q->pnode[i/2].v;
                i = i/2;                
            }else break;            
       }
    }
    Q->pnode[i].flag = flag;
    Q->pnode[i].v = v;

}

node pop(pqueue Q){
    int num = Q->num;
    int check = Q->pnode[1].flag;
    node re = Q->pnode[1];
    Q->pnode[1].flag = Q->pnode[num].flag;
    Q->pnode[1].v = Q->pnode[num].v;
    Q->num--;
    num = Q->num;
    int i = 1;
    int v = Q->pnode[1].v;
    int flag = Q->pnode[1].flag;
    while (i*2<=num)
    {

        if(i*2+1<=num){
            if(flag==check&&Q->pnode[2*i].flag!=check){//11111111111
                if(Q->pnode[2*i+1].flag!=check){
                    break;
                }else{
                    if(Q->pnode[2*i+1].v<v){
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;                         
                    }else break;
                }
            }else if(flag==check&&Q->pnode[2*i].flag==check){//2222222222222222
                if(Q->pnode[2*i+1].flag!=check){
                    if(Q->pnode[2*i].v<v){
                        Q->pnode[i].flag = Q->pnode[2*i].flag;
                        Q->pnode[i].v =  Q->pnode[2*i].v;
                        i = 2*i;                         
                    }else break;
                }else{
                    if((Q->pnode[2*i].v<Q->pnode[2*i+1].v)&&(Q->pnode[2*i].v<v)){
                        Q->pnode[i].flag = Q->pnode[2*i].flag;
                        Q->pnode[i].v =  Q->pnode[2*i].v;
                        i = 2*i; 
                    }else if((Q->pnode[2*i].v>Q->pnode[2*i+1].v)&&(Q->pnode[2*i+1].v<v)){
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;                        
                    }else if((Q->pnode[2*i].v==Q->pnode[2*i+1].v)&&(Q->pnode[2*i+1].v<v)){
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;                        
                    }else break;

                }
            }else if(flag!=check&&Q->pnode[2*i].flag==check){//33333333333333333
                if(Q->pnode[2*i+1].flag!=check){
                        Q->pnode[i].flag = Q->pnode[2*i].flag;
                        Q->pnode[i].v =  Q->pnode[2*i].v;
                        i = 2*i;                     
                }else{
                    if(Q->pnode[2*i].v<Q->pnode[2*i+1].v){
                        Q->pnode[i].flag = Q->pnode[2*i].flag;
                        Q->pnode[i].v =  Q->pnode[2*i].v;
                        i = 2*i;
                    }else{
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;                        
                    }
                }
            }else{//44444444444444444444444444444444444
                if(Q->pnode[2*i+1].flag!=check){
                    if(Q->pnode[2*i].v<Q->pnode[2*i+1].v&&Q->pnode[2*i].v<v){
                        Q->pnode[i].flag = Q->pnode[2*i].flag;
                        Q->pnode[i].v =  Q->pnode[2*i].v;
                        i = 2*i; 
                    }else if(Q->pnode[2*i].v>Q->pnode[2*i+1].v&&Q->pnode[2*i+1].v<v){
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;                        
                    }else if((Q->pnode[2*i].v==Q->pnode[2*i+1].v)&&(Q->pnode[2*i+1].v<v)){
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;                        
                    }else break;
                                       
                }else{
                        Q->pnode[i].flag = Q->pnode[2*i+1].flag;
                        Q->pnode[i].v =  Q->pnode[2*i+1].v;
                        i = 2*i+1;
                }
       
            }

        }else{

            if(flag==check&&Q->pnode[2*i].flag!=check){
                break;
        }else if(flag==check&&Q->pnode[2*i].flag==check){
                if(Q->pnode[2*i].v<v){
                    Q->pnode[i].flag = Q->pnode[2*i].flag;
                    Q->pnode[i].v =  Q->pnode[2*i].v;
                    i *= 2;                
                }else break;
        }else if(flag!=check&&Q->pnode[2*i].flag==check){
                Q->pnode[i].flag = Q->pnode[2*i].flag;
                Q->pnode[i].v =  Q->pnode[2*i].v;
                i *= 2;
        }else{
                if(Q->pnode[2*i].v<v){
                    Q->pnode[i].flag = Q->pnode[2*i].flag;
                    Q->pnode[i].v =  Q->pnode[2*i].v;
                    i *= 2;                
                }else break;        
        }

        }

    }

    Q->pnode[i].flag = flag;
    Q->pnode[i].v = v;
    
    return re;
    
}

int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    int i;
    pqueue Q = (pqueue)malloc(sizeof(queue));
    Q->pnode = (node*)malloc((m+1)*sizeof(node));
    Q->num = 0;
    int* data = (int*)malloc((n+1)*sizeof(int));
    for(i=1;i<n+1;i++){
        scanf("%d",&data[i]);
    }
    for(i=1;i<m+1;i++){
        push(Q,data[i],0,0);
    }
    int check  = 0;
    node temp ;
    int blank = 0;
    while(Q->num!=0){
        temp = pop(Q);
        
        if(check==temp.flag){
            if(blank)printf(" %d",temp.v);
            else{
                printf("%d",temp.v);
                blank = 1;
            }
        }else{
            check = temp.flag;
            printf("\n%d",temp.v);
        }

        if(i<n+1){
            if(data[i]<temp.v){
                push(Q,data[i],check^1,check);
            }else{
                push(Q,data[i],check,check);
            }
            i++;
        }


    }

}