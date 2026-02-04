#include <bits/stdc++.h>
using namespace std;

int main() {
    int K, N;
    cin >> K >> N;

    priority_queue<int, vector<int>, greater<int>> minHeap;
    vector<int> result;   

    for (int i = 0; i < N; i++) {
        int score;
        cin >> score;

        if (minHeap.size() < K) {
            minHeap.push(score);
            result.push_back(-1);
        } 
        else {
            if (score > minHeap.top()) {
                minHeap.pop();
                minHeap.push(score);
            }
            result.push_back(minHeap.top());
        }
    }

    for (int x : result) {
        cout << x << endl;
    }

    return 0;
}
