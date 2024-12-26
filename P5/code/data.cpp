#include<bits/stdc++.h>
using namespace std;
int main()
{
	freopen("data1.out","w",stdout);
	const int Bound=1e9;
	int N=3000,D=1000000;		//change the numbers to your need
	int coupon[100005],price[100005];
	srand(time(0));
	int MaxCoupon=rand()%Bound+1;
	int priceMod=Bound-MaxCoupon;
	for(int i=1;i<=N;i++)
	{
		coupon[i]=rand()%MaxCoupon+1;
		price[i]=rand()%priceMod+1+MaxCoupon;
	}
	
	//print
	cout<<N<<" "<<D<<endl;
	for(int i=1;i<=N;i++)
		cout<<price[i]<<" ";
	cout<<endl;
	for(int i=1;i<=N;i++)
		cout<<coupon[i]<<" ";
	cout<<endl;
	return 0;
}

