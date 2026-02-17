#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isPossible(int mid, int k, vector<int>& stalls) {
        int count = 1, lastCow = stalls[0];
        for(int i = 1; i < stalls.size(); i++) {
            if(stalls[i] - lastCow >= mid) {
                count++;
                lastCow = stalls[i];
                if(count >= k) return true;
            }
        }
        return false;
    }

    int aggressiveCows(vector<int>& stalls, int k) {
        sort(stalls.begin(), stalls.end());
        int n = stalls.size();
        int low = 0, high = stalls[n-1] - stalls[0], ans = 0;

        while(low <= high) {
            int mid = (low + high) / 2;
            if(isPossible(mid,k,stalls)){
                ans = mid;
                low = mid + 1;
            }
            else high = mid - 1;
        }
        return ans;
    }
};

int main(){
    int n,k;
    cin>>n;
    vector<int> stalls(n);
    for(int i=0;i<n;i++) cin>>stalls[i];
    cin>>k;

    Solution obj;
    cout<<obj.aggressiveCows(stalls,k);
}
