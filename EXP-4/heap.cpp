#include <bits/stdc++.h>
using namespace std;

#define MAX 100

int heap[MAX];
int heapSize = 0;

/* Helper functions */
int parent(int i) { return (i - 1) / 2; }
int left(int i) { return 2 * i + 1; }
int right(int i) { return 2 * i + 2; }

/* Heapify Down */
void heapifyDown(int i) {
    int smallest = i;
    int l = left(i);
    int r = right(i);

    if (l < heapSize && heap[l] < heap[smallest])
        smallest = l;
    if (r < heapSize && heap[r] < heap[smallest])
        smallest = r;

    if (smallest != i) {
        swap(heap[i], heap[smallest]);
        heapifyDown(smallest);
    }
}

/* Heapify Up */
void heapifyUp(int i) {
    while (i != 0 && heap[parent(i)] > heap[i]) {
        swap(heap[i], heap[parent(i)]);
        i = parent(i);
    }
}

/* Insert element */
void insert(int val) {
    heap[heapSize] = val;
    heapSize++;
    heapifyUp(heapSize - 1);
}

/* Search element */
int search(int val) {
    for (int i = 0; i < heapSize; i++) {
        if (heap[i] == val)
            return i;
    }
    return -1;
}

/* Delete any element */
void deleteElement(int val) {
    int index = search(val);

    if (index == -1) {
        cout << "Element not found\n";
        return;
    }

    heap[index] = heap[heapSize - 1];
    heapSize--;

    heapifyDown(index);
    heapifyUp(index);
}

/* Display heap */
void display() {
    for (int i = 0; i < heapSize; i++)
        cout << heap[i] << " ";
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        insert(x);
    }

    cout << "Heap elements: ";
    display();

    int key;
    cout << "Enter element to search: ";
    cin >> key;
    int pos = search(key);
    if (pos != -1)
        cout << "Element found at index " << pos << endl;
    else
        cout << "Element not found\n";

    cout << "Enter element to delete: ";
    cin >> key;
    deleteElement(key);

    cout << "Heap after deletion: ";
    display();

    return 0;
}


// Time Complexity (for n elements)

// Build heap (n inserts) → O(n log n)

// Search → O(n)


// Delete any element → O(n)
