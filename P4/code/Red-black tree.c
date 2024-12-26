//if you find that the comment below is garbled characters,please change to UTF-8 decoding mode
//设计dp算法的思路如下
/*

对于一颗红黑树，从根节点算起的的BH若固定，则这个棵树的最小内部节点数为2^BH-1。
若要将这个问题转化为dp解决那么便需要考虑怎么转化成子问题
考虑到红黑树的性质，即根节点到任意NIL的BH恒定，那么可以推断出其左右子树的BH相等
所以状态的描述可以表示为当前的节点个数N以及根节点对应的BH值

那么可以得到一个状态转移方程 (BH,N) = (BH-1,i)+(BH-1,N-1-i) , 其中i=0,1,...,N-1

最后对(j,N) j=0,1,...,BH 求和即可

手动画图进行验证的时候发现这个算法有一个小问题，那就是必须把根节点为红色的可能考虑进去，

因此我们需要有两张dp表，一张用来存放根节点为红色的情况，一张用来存放根节点为黑色的情况

记为dp_r[i][j]和dp_b[i][j]

那么状态转移方程可以表示为
dp_b(BH,N) += [dp_b(BH-1,i)+dp_r(BH-1,i)]*[dp_b(BH-1,N-1-i)+dp_r(BH-1,N-1-i)]
dp_r(BH,N) += dp_b(BH,i)*dp_b(BH,N-1-i)//不能连续两个红节点

*/

#include <stdio.h>
#include <stdlib.h>


int main(){
    int n,i,j,k;
    scanf("%d",&n);
    long long int** dp_b = (long long int**)malloc((n+1)*sizeof(long long int*));
    long long int** dp_r = (long long int**)malloc((n+1)*sizeof(long long int*));
    for(i=0;i<n+1;i++){
        dp_b[i] = (long long int*)malloc((n+1)*sizeof(long long int));
        dp_r[i] = (long long int*)malloc((n+1)*sizeof(long long int));
    }
    //初始化
    for(i=0;i<n+1;i++){
        for(j=0;j<n+1;j++){
            if(i==0&&j==0)dp_b[i][j]=1;//根节点不存在的情况下才有可能
            else dp_b[i][j]=0;
            if(i==0&&j==1)dp_r[i][j]=1;//根节点为红色的情况下才可能
            else dp_r[i][j]=0;
        }
    }

    //dp状态转移
    int twosquare = 1;
    int minnode = twosquare-1;
    for(i=1;i<n+1;i++){
        twosquare *= 2;
        minnode = twosquare-1;
        if(minnode>n)break;
        for(j=0;j<n+1;j++){
            for(k=0;k<j;k++){
                dp_b[i][j] = (dp_b[i][j]+(((dp_b[i-1][k]+dp_r[i-1][k])%1000000007)*((dp_b[i-1][j-k-1]+dp_r[i-1][j-k-1])%1000000007))%1000000007)%1000000007;//更新黑表
                dp_r[i][j] = (dp_r[i][j]+((dp_b[i][k]%1000000007)*(dp_b[i][j-k-1]%1000000007))%1000000007)%1000000007; //更新红表
            }
           // printf("%d ",dp_b[i][j]);
        }
        // printf("\n");
    }

    int sum = 0;
    for(j=0;j<i;j++){
        sum+=dp_b[j][n];
        sum%=1000000007; 
    }
    printf("%d\n",sum);

}
