#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canPaint(vector<int>& arr,int k,long long limit){
        long long sum=0;
        int painters=1;
        for(int x:arr){
            if(x>limit) return false;
            if(sum+x<=limit) sum+=x;
            else{
                painters++;
                sum=x;
            }
        }
        return painters<=k;
    }

    int minTime(vector<int>& arr, int k) {
        long long low=0,high=0,ans=0;
        for(int x:arr){
            low=max(low,(long long)x);
            high+=x;
        }

        while(low<=high){
            long long mid=(low+high)/2;
            if(canPaint(arr,k,mid)){
                ans=mid;
                high=mid-1;
            }
            else low=mid+1;
        }
        return ans;
    }
};

int main(){
    int n,k;
    cin>>n;
    vector<int> arr(n);
    for(int i=0;i<n;i++) cin>>arr[i];
    cin>>k;

    Solution obj;
    cout<<obj.minTime(arr,k);
}
