#include<stdio.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<time.h>
#define MAXN 100005
using namespace std;
int price[MAXN],coupon[MAXN];	//price of items and coupons
bool cmp(int x,int y)	//used to sort in decreasing order
{
	return x>y;
}
int main()
{	
	freopen("data1.out","r",stdin);
	freopen("ans1.out","w",stdout);
	int N,D;
	clock_t start,end;
	scanf("%d %d",&N,&D);
	for(int i=1;i<=N;i++)
		scanf("%d",&price[i]);	//scan price
	for(int i=1;i<=N;i++)
		scanf("%d",&coupon[i]);	//scan coupon
	start=clock();
	sort(price+1,price+N+1);		//sort price
	sort(coupon+1,coupon+N+1,cmp);	//sort coupon in decreasing order
	
	int Current[MAXN];	//Current[i]==u means item i has used coupon[1~u] 
	for(int i=1;i<=N;i++)
		Current[i]=0;
	priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > >H;	//H is a MinHeap to find the cheapest item 
	H.push(make_pair(price[1]-coupon[1],1));	//price[1] with coupon[1] is the only posible cheapest item
	int cnt=0;	//count for the items bought
	while(1)
	{
		int x=H.top().second,w=H.top().first;	//x is the item number, w is the price with coupon
		H.pop();
		if(D>=w)	//Can afford the cheapest item
		{
			Current[x]++;
			D-=w;
			cnt++;
			int v=Current[x]+1;		// price[x] with coupon[v] is posible to be the cheapest one
			H.push(make_pair(price[x]-coupon[v],x));
			if(Current[x]==1 && x+1<=N)		//if the price[x] with coupon[1] is bought, add price[x+1] with coupon[1] to the heap
				H.push(make_pair(price[x+1]-coupon[1],x+1));
		}
		else		//Can not afford
		{
			break;
		}
	}
	end = clock();
	double time_taken = double(end - start) / CLOCKS_PER_SEC;
	printf("Time taken: %f seconds\n", time_taken);
	printf("%d %d\n",cnt,D);	//print the number of items and money left
	return 0;
}
