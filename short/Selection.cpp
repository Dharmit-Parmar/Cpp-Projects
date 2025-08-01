#include <iostream>
#include <string>
#include <vector>
using namespace std;

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
void Selectionsort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[i]) {
                swap(arr[i], arr[j]);
            }
        }
         
    }
}

int main() {
    int n = 5;
    int arr[5] = { 5, 8, 0, 1, 3 }; 
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    Selectionsort(arr, n);
    cout << "Sorted array" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}