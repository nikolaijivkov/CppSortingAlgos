// CppSortingAlgos.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <algorithm>
#include <functional>
#include <cstdio>
#include <random>
#include <ctime>
#include <chrono>
using namespace std;

void selectionSort(int Arr[], int n) {  // O(n^2)
	for (int i = 0; i < n - 1; ++i) {
		int min_el_inx = i;
		for (int j = i + 1; j < n; ++j) {
			if (Arr[min_el_inx] > Arr[j])
				min_el_inx = j;
		}
		if (min_el_inx != i)
			swap(Arr[i], Arr[min_el_inx]);
	}
}


void bubbleSort(int Arr[], int n) {  // O(n^2)
	bool swapped = true;
	for (int k = 1; swapped && k < n; ++k) {
		swapped = false;
		for (int i = 0; i < n - k; ++i)
			if (Arr[i] > Arr[i + 1]) {
				swap(Arr[i], Arr[i + 1]);
				swapped = true;
			}
	}
}


void insertionSort(int Arr[], int n) {  // O(n^2) but practically better than selectionSort and bubbleSort
	for (int i = 0; i < n; ++i) {
		int value = Arr[i];
		int hole;
		for (hole = i; hole && Arr[hole - 1]>value; --hole)
			Arr[hole] = Arr[hole - 1];
		Arr[hole] = value;
	}
}


void merge(int Arr[], int L[], int leftCount, int R[], int rightCount) {
	int i, j, k;
	i = j = k = 0;

	while (i < leftCount && j < rightCount)
		if (L[i] < R[j])
			Arr[k++] = L[i++];
		else
			Arr[k++] = R[j++];

	while (i < leftCount) Arr[k++] = L[i++];
	while (j < rightCount) Arr[k++] = R[j++];
}

void mergeSort(int Arr[], int n) {  // O(n.logn), but it is not in-place algorithm, space complexity is O(n)
	if (n < 2) return;

	int i, mid = n / 2;
	int *L = new int[mid];
	int *R = new int[n - mid];

	for (i = 0; i < mid; ++i) L[i] = Arr[i];
	for (i = mid; i < n; ++i) R[i - mid] = Arr[i];
	
	mergeSort(L, mid);
	mergeSort(R, n-mid);
	merge(Arr, L, mid, R, n-mid);
	delete[] L;
	delete[] R;
}


int partition(int Arr[], int start, int end) {
	int pivot = Arr[end];
	int pIndex = start;

	for (int i = start; i < end; ++i)
		if (Arr[i] <= pivot) {
			swap(Arr[i], Arr[pIndex]);
			++pIndex;
		}
	swap(Arr[end], Arr[pIndex]);
	return pIndex;
}

void quickSort(int Arr[], int start, int end) {  // O(n.logn) in average (with big probability), O(n^2) in worst case scenario
	if (end > start) {
		int pIndex = partition(Arr, start, end);
		quickSort(Arr, start, pIndex - 1);
		quickSort(Arr, pIndex + 1, end);
	}
}


void fillRandArr(int Arr[], int n) {
	random_device rd;
	uniform_int_distribution<int> dist(1, 256);
	for (int i = 0; i < n; ++i)
		Arr[i] = dist(rd);
}


int main()
{
	const int n = 10000;
	int Arr[n];
	
	fillRandArr(Arr, n);

	chrono::steady_clock::time_point start = chrono::steady_clock::now();

	/* Sorting Algos O(n^2): */
	// bubbleSort(Arr, n);            // O(n^2) average, n=10000, msec = [2678, 2640, 2702, 2693, 2739]
	// selectionSort(Arr, n);         // O(n^2) average, n=10000, msec = [133, 131, 136, 135, 129]
	// insertionSort(Arr, n);         // O(n^2) average, n=10000, msec = [73, 66, 64, 71, 70]

	/* Sorting Algos O(n.logn): */
	// quickSort(Arr, 0, n-1);        // O(n.logn) average, n=10000, msec = [28, 30, 27, 27, 26]
	mergeSort(Arr, n);                // O(n.logn) average, n=10000, msec = [10, 10, 9, 13, 9]
	
	/* STD build in sort guaranteed O(n.logn): */
	// std::sort(Arr, Arr+n);         // O(n.logn) average, n=10000, msec = [13, 10, 12, 9, 11]
	// std::stable_sort(Arr, Arr+n);  // O(n.logn) average, n=10000, msec = [21, 22, 24, 21, 21]

	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	cout << "Sorting took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds." << endl;
	//for (auto &item : Arr) cout << item << endl;

	getchar();
	return 0;
}

