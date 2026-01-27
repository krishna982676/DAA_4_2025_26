#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    
   cout<<"Number of attendance records: ";
    cin >> n;

    vector<char> arr(n);

    cout<<"Enter attendance records (P/A): ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
        arr[i] = toupper(arr[i]);   
    }

    unordered_map<int, int> mp;
    int sum = 0;
    int maxLen = 0;

    mp[0] = -1;

    for (int i = 0; i < n; i++) {
        if (arr[i] == 'P') {
            sum += 1;
        } 
        else if (arr[i] == 'A') {
            sum -= 1;
        }

       
        if (mp.count(sum)) {
            maxLen = max(maxLen, i - mp[sum]);
        } 
        else {
            mp[sum] = i;   
        }
    }


    cout << "Maximum length of stable attendance window: "<<maxLen << endl;

    return 0;
}

/*
Time Complexity: O(N)
Space Complexity: O(N)

Explanation:
P is treated as +1 and A as -1
Prefix sum technique is used
unordered_map stores the first occurrence of each prefix sum
If the same prefix sum repeats, the subarray in between has equal P and A
unordered_map provides O(1) average-time lookup and insertion
*/
