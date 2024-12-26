
#include <stdio.h>
#include <stdlib.h>

#define  MAXN 10000000000
typedef struct station{
    double cost;
    double mile;
}station;
typedef station* pstation;




int main(){

    double sum =0;
    double cap,dist,mileperunit;
    int n;
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

    stations[n].cost = MAXN;
    stations[n].mile = dist;

    double maxdis = cap*mileperunit;

    int currentstation = 0;
    int cheapeststation = 0;
    int nextstation = 0;
    double nextcost = 0;
    double cheapestcost = MAXN;
    double currentcost = 0;
    double tank=0;
    int flag = 0;
    int takeornot = 0;
    if(stations[0].mile!=0)printf("The maximum travel distance = 0.00");
    else{
        while(1){
            flag =0;
            takeornot=0;
            cheapestcost = MAXN;
            nextcost = MAXN;
            currentcost = stations[currentstation].cost;
            for(i=currentstation+1;i<=n;i++){
                if(stations[i].mile<=stations[currentstation].mile+maxdis){
                    flag = 1;
                    if(i==n)flag = 2;
                    if(stations[i].cost<=cheapestcost){
                        cheapestcost = stations[i].cost;
                        cheapeststation = i;
                    }
                    if(stations[i].cost<=currentcost&&!takeornot){
                        nextcost = stations[i].cost;
                        nextstation = i;
                        takeornot = 1;
                        break;
                    }
                }else break;
            }
            if(cheapestcost>currentcost){
                nextstation = cheapeststation;
                nextcost = cheapestcost;
            }
            
            if(!flag){//如果没有下一个加油站或者终点，则说明无法抵达终点，输出最大距离为当前的mile加上加满油所能抵达的最远距离
                printf("The maximum travel distance = %.2f",stations[currentstation].mile+maxdis);
                break;
            }else{//如果有下一个加油站或是终点

                if(nextcost>currentcost){//若所能到达的加油站的费用大于当前加油站的费用
                    
                    if(flag==2){//先考虑能不能到终点，能的话只需要把油加到刚好抵达终点即可
                        sum += stations[currentstation].cost*((stations[n].mile-stations[currentstation].mile)/mileperunit-tank);
                        printf("%.2f",sum);
                        break;
                    }
                    else{//如果到不了终点，则加满油，然后到达这些加油站中最便宜的那个
                            sum += (cap-tank)*stations[currentstation].cost;
                            tank = cap;
                            tank -= (stations[nextstation].mile-stations[currentstation].mile)/mileperunit;
                            
                            // currentstation = nextstation;
                    } 
                }else{//若所能到到达的加油站的费用小于等于当前加油站的费用，则只需要加油到达该加油站即可
                    sum += stations[currentstation].cost*((stations[nextstation].mile-stations[currentstation].mile)/mileperunit-tank);
                    tank = 0;
                    // currentstation = nextstation;
                }
                //printf("now at num.%d station,next station is num.%d,tankmile is %.2f,sum is %.2f\n",currentstation,nextstation,tank*mileperunit,sum);
                //if(currentstation==nextstation)break;
                currentstation = nextstation;

            }
            



        }
    }


}