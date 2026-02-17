#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int lowerBound(vector<int> &arr, int target) {
        int low = 0, high = arr.size() - 1;
        int n = arr.size();

        while(low <= high) {
            int mid = low + (high - low)/2;

            if(arr[mid] >= target){
                n = mid;
                high = mid - 1;
            }
            else low = mid + 1;
        }
        return n;
    }
};

int main(){
    int n,target;
    cin>>n;
    vector<int> arr(n);
    for(int i=0;i<n;i++) cin>>arr[i];
    cin>>target;

    Solution obj;
    cout<<obj.lowerBound(arr,target);
}
