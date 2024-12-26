#include <stdio.h>

#define MAXN 10
#define MAXS 1000

int need_time(const int time[], const int score[], int happy_score, int n);

int main() {
  int n, i, happy_score;
  int time[MAXN], score[MAXN];
  scanf("%d %d", &n, &happy_score);
  for (i = 0; i < n; ++i)
      scanf("%d", &time[i]);
  for (i = 0; i < n; ++i)
      scanf("%d", &score[i]);
  printf("%d\n", need_time(time, score, happy_score, n));
  return 0;
}

/* Your function will be put here */
int need_time(const int time[], const int score[], int happy_score, int n){
    int** dp = (int**)malloc((n+1)*sizeof(int*));
    int i;
    int j;
    int total_time = 0;
    int total_score = 0;
    for(i = 0; i < n; i++)total_score+=score[i];
    for(i = 0; i < n; i++)total_time+=time[i];
    int maxsub_score = total_score-happy_score;
    if(maxsub_score<0)return -1;
    for(i=0;i<n+1;i++){
        dp[i] = (int*)malloc((maxsub_score+1)*sizeof(int));
    }
    for(i=0;i<maxsub_score+1;i++)dp[0][i] = total_time;
    for(i=0;i<n+1;i++)dp[i][0] = total_time;

    for(i=1;i<n+1;i++){
      for(j=1;j<maxsub_score+1;j++){
        if(j>=score[i-1]) dp[i][j] = dp[i-1][j]<dp[i-1][j-score[i-1]]-time[i-1]?dp[i-1][j]:dp[i-1][j-score[i-1]]-time[i-1];
        else dp[i][j] = dp[i-1][j];
      }
    }
  return dp[n][maxsub_score];
}

//该问题实际上是背包问题的变种题
//在背包问题当中，我们以容量作为限制，按理来说，这道题目如果要用dp求解的话也需要
//找到一个限制作为dp的一个维度，但是发现直接套用的话无法实现，所以想方法降问题进行转化，
//先假设将所有的题目都做出来了，接下来要考虑放弃哪些题目的得分，但是放弃后总分不应该小于所设得分
/*
grade(max) = total_grade - happy_score;

dp[i,grade] = min(dp[i-1,grade], dp[i-1,grade-score[i]] - time[i])

dp[0,0~grade(max)] = total_time;
dp[0~n,0] = total_time;

*/