#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long hoursNeeded(vector<int>& piles, long long k){
        long long hours = 0;
        for(int p : piles){
            hours += (p + k - 1) / k;
        }
        return hours;
    }

    int minEatingSpeed(vector<int>& piles, int h) {
        long long low = 1;
        long long high = *max_element(piles.begin(), piles.end());
        long long ans = high;

        while(low <= high){
            long long mid = (low + high) / 2;

            if(hoursNeeded(piles, mid) <= h){
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return (int)ans;
    }
};

int main(){
    int n,h;
    cin>>n;
    vector<int> piles(n);
    for(int i=0;i<n;i++) cin>>piles[i];
    cin>>h;

    Solution obj;
    cout<<obj.minEatingSpeed(piles,h);
}
