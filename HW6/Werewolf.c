#include<stdlib.h>
#include<stdio.h>



int temp [200];

typedef struct wereWolf
{
    int realNum;
    int humanNum;
    int nowNum;
    int* who;//一开始应该全部初始化为-1，这样表示这个人是否为wolf还不确定

}wereWolf;
typedef struct wereWolf* wereWolfP;

typedef struct liers
{
    int realNum;
    int nowNum;
    int* who;//一开始应该全部初始化为-1，这样表示这个人是否为lier还不确定
}liers;

typedef struct liers* liersP;



void isNewSolution(int* solution,wereWolfP wolf,int n){
    int j=0;
    for(int i=n-1;i>=0;i--){
        if(j<wolf->realNum){
            if(wolf->who[i]==1&&solution[j]<i){
            solution[j]=i;
            j++;    
        }
    }
    // for(j=0;j<wolf->realNum;j++){
    //     printf("%d ",solution[j]);
    //     if(j==wolf->realNum-1)printf("\n");
    // }

}
}

void findwolf(int* statement,int n,int i,liersP lier,wereWolfP wolf,int* solution){
    if(i>=n){
        if(lier->realNum!=lier->nowNum);
        else{
            int wolflier = 0;
            int j;
            int count = 0;
            for(j=0;j<n;j++){
                if(lier->who[j]==1&&wolf->who[j]==1)wolflier++;
            }
            for(j=n-1;j>=0;j--){
                if(wolf->who[j]==-1){
                    if(wolf->nowNum<wolf->realNum){
                        if(lier->who[j]==1&&wolflier>=wolf->realNum){
                                wolf->who[j]=0;                    
                        }else{
                            if(lier->who[j]==1)wolflier++;
                            wolf->who[j]=1;
                            wolf->nowNum++;

                        }
                            temp[count]=j;
                            count++;
                    }else{
                        wolf->who[j]=0;
                        temp[count]=j;
                        count++;
                    }
                }    
            }

            if(wolflier==wolf->realNum||wolflier==0||wolf->nowNum!=wolf->realNum);
            else isNewSolution(solution,wolf,n);

            for(j=0;j<count;j++){
                wolf->who[temp[j]]=-1;
            }
        }
    }else{
        int person = statement[i];
        int passCheck=0;
        int already = 1;
        //假设这个人说的是真话

        //先进行检查 看是否此时能说真话
        if(person<0){
            //当且仅当这个人的身份未被确定或者说身份确实为wolf时，i对应的人才可能说真话，并且狼的数量不能查过
            if(((wolf->who[(-person)-1]==-1)&&wolf->nowNum<wolf->realNum)||(wolf->who[(-person)-1]==1)){
                if(wolf->who[(-person)-1]==-1){
                    already=0;
                    wolf->nowNum++; 
                }
                passCheck=1;
                wolf->who[(-person)-1]=1;
                   
            }
            else passCheck=0;
        }

        if(person>0){
            //当且仅当这个人的身份未被确定或者说身份确实为human时，i对应的人才可能说真话,且人的数量也不能超过
            if((wolf->who[person-1]==-1&&wolf->humanNum<n-wolf->realNum)||(wolf->who[person-1]==0)){
                if(wolf->who[person-1]==-1){
                    already=0;
                    wolf->humanNum++;
                }
                passCheck=1;
                wolf->who[person-1]=0;
            }
            else passCheck=0;
        }


        
        
        if(passCheck){
            findwolf(statement,n,i+1,lier,wolf,solution);
            if(person<0){
                if(!already){
                    wolf->nowNum--;
                    wolf->who[(-person)-1]=-1;
                }
            }else{
                if(!already){
                    wolf->who[person-1]=-1;
                    wolf->humanNum--;
                }
            }      
        }
        //假设这个人说的是假话
        passCheck=0;
        already=1;
        //先进行检查看他此时能否说假话

        if(person<0){
            //当且仅当这个人的身份未被确定或者说身份为人时，i对应的人才可能说假话,并且lier数量不能超过
            if(((wolf->who[(-person)-1]==-1&&wolf->humanNum<n-wolf->realNum)||(wolf->who[(-person)-1]==0))&&lier->nowNum<lier->realNum){
                if(wolf->who[(-person)-1]==-1){
                    already=0;
                    wolf->humanNum++;
                }
                passCheck=1;
                wolf->who[(-person)-1]=0;         
            }
            else passCheck=0;
        }

        if(person>0){
            //当且仅当这个人的身份未被确定或者说身份确实为wolf时，i对应的人才可能说假话，并且狼和lier的数量均不能超过
            if(((wolf->who[person-1]==-1&&wolf->nowNum<wolf->realNum)||(wolf->who[person-1]==1))&&lier->nowNum<lier->realNum){
                if(wolf->who[person-1]==-1){
                    already=0;
                    wolf->nowNum++;
                }
                passCheck=1;
                wolf->who[person-1]=1;
            }
            else passCheck=0;
        }

        if(passCheck){
                lier->nowNum++;
                lier->who[i]=1;
                findwolf(statement,n,i+1,lier,wolf,solution);
            if(person<0){
                if(!already){
                    wolf->who[(-person)-1]=-1;
                    wolf->humanNum--;
                }
            }else{
                if(!already){
                    wolf->who[person-1]=-1;
                    wolf->nowNum--;
                }
            }
                lier->nowNum--;
                lier->who[i]=0;      
        }


        


    }
};


int main(){
    int n,m,l,i;
    scanf("%d %d %d",&n,&m,&l);
    int* statement = (int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++)scanf("%d",&statement[i]);

    liersP lier = (liersP)malloc(sizeof(liers));
    wereWolfP wolf = (wereWolfP)malloc(sizeof(wereWolf));
    int* solution = (int*)malloc(m*sizeof(int));
    lier->realNum = l;
    lier->nowNum = 0;
    lier->who = (int*)malloc(n*sizeof(int));
    wolf->realNum = m;
    wolf->humanNum = 0;
    wolf->nowNum = 0;
    wolf->who = (int*)malloc(n*sizeof(int));
    for(i=0;i<n;i++){
        lier->who[i]=0;
        wolf->who[i]=-1;
        if(i<m)solution[i]=-1;
    }


    findwolf(statement,n,0,lier,wolf,solution);


    if(solution[0]==-1)printf("No Solution");
    else for(i=0;i<m;i++){
        
        if(i==0)printf("%d",solution[i]+1);
        else printf(" %d",solution[i]+1);
        }



}