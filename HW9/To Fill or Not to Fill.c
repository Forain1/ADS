#include <stdio.h>
#include <stdlib.h>

typedef struct station{
    double cost;
    double mile;
}station;
typedef station* pstation;

double sum =0;
double cap,dist,mileperunit;
int n;
int isconnected = 1;
double mindisconnected = -1;
//我们让这里的gas为gas*mileperunit，这样可以直接用gas来表示能走多远
void findprice(int begin,int end,pstation stations){
    double gasbefore=0;
    double maxgasafter = cap*mileperunit;
    int index = begin+1 ;
    double mincost = stations[begin+1].cost ;
    for(int i=begin+1 ; i<end ; i++){
        if(stations[i].cost<mincost){
            mincost = stations[i].cost;
            index = i;
        }   
    }


     if(maxgasafter<stations[index].mile-stations[begin].mile||begin==0)gasbefore = 0;
    else gasbefore = maxgasafter-(stations[index].mile-stations[begin].mile);

    double gasafterneeded = stations[end].mile-stations[index].mile ;

    if(gasbefore==0){//前面的油站没有gas传过来，说明前面的加油站不够，需要再添加
        if(begin + 1 == index){
            if(stations[begin].mile!=0){
                isconnected = 0;
                
                //printf("Not Connected\n");
                return;
            }else{
                sum+=(stations[index].mile-stations[begin].mile)*stations[begin].cost;
               // printf("now at 0 miles,cost:%.2lf\n",(stations[index].mile-stations[begin].mile));
                
            }
        }
        else findprice(begin,index,stations);
    }

    if(maxgasafter<gasafterneeded){//即使现在加满油也不够走的，需要在后面添加一个加油站
        sum+=(maxgasafter-gasbefore)*stations[index].cost;
       // printf("now at %.2lf miles,cost:%.2lf\n",stations[index].mile,maxgasafter-gasbefore);
        if(index+1==end){
            isconnected = 0 ;
            if(mindisconnected==-1)mindisconnected = stations[index].mile+maxgasafter;
            else mindisconnected  = mindisconnected<stations[index].mile+maxgasafter? mindisconnected: stations[index].mile+maxgasafter;
           // printf("Not Connected\n");
            return ;
        } 
        else findprice(index,end,stations);
    }else{
        sum+=(gasafterneeded-gasbefore)*stations[index].cost;
       // printf("now at %.2lf miles,cost:%.2lf\n",stations[index].mile,gasafterneeded-gasbefore);
        return;
       
    }


}

int main(){
    
    scanf("%lf%lf%lf%d",&cap,&dist,&mileperunit,&n);
    pstation stations = (pstation)malloc((n+1)*sizeof(station));
    int i;
    for(i=0;i<n;i++){
        scanf("%lf%lf",&stations[i].cost,&stations[i].mile);
    }
    //按照路上距离进行排序,这里暂时用冒泡排序
    double tempcost,tempmile;
    for(i=n-1;i>=0;i--){
        for(int j=0;j<i;j++){
            if(stations[j].mile>stations[j+1].mile){
                tempcost = stations[j].cost;
                tempmile = stations[j].mile;
                stations[j].cost = stations[j+1].cost;
                stations[j].mile = stations[j+1].mile;
                stations[j+1].cost = tempcost;
                stations[j+1].mile = tempmile;
            }
        }
    }
    stations[n].cost = 0;
    stations[n].mile = dist;
    if(stations[0].mile!=0){
        isconnected = 0;
        mindisconnected = 0;
    }else{
        findprice(0,n,stations);
        sum /= mileperunit;
    }
    
    if(isconnected)printf("%.2lf",sum);
    else printf("The maximum travel distance = %.2lf",mindisconnected);

    

    // for(i=0;i<n;i++)printf("%lf %lf\n",stations[i].cost,stations[i].mile);

}