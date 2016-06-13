// CppSortingAlgos.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <type_traits>
#include <algorithm>
#include <random>
#include <chrono>
//using namespace std;

/* helper functions */
template <class T>
void swap(T &a, T &b) { // faster than std::swap for fundamental build in types
	if (std::is_fundamental<T>::value) { // fundamental type -> don't use move semantics 
		T tmp = a;
		a = b;
		b = tmp;
	}
	else {  // compound type -> use move semantics as in std::swap
		T tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
	}
}

void fillRandArr(int Arr[], int n) {
	static std::random_device rd;
	static std::uniform_int_distribution<int> dist(1, 256);
	for (int i = 0; i < n; ++i)
		Arr[i] = dist(rd);
}

/* sorting algorithms */
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


void straightSelectionSort(int Arr[], int n) {  // O(n^2)
	for (int i = 0; i < n - 1; ++i)
		for (int j = i + 1; j < n; ++j)
			if (Arr[i] > Arr[j])
				swap(Arr[i], Arr[j]);
}

void selectionSort(int Arr[], int n) {  // O(n^2)
	int minIndex;
	for (int i = 0; i < n - 1; ++i) {
		minIndex = i;
		for (int j = i + 1; j < n; ++j) {
			if (Arr[minIndex] > Arr[j])
				minIndex = j;
		}
		if (minIndex != i)
			swap(Arr[i], Arr[minIndex]);
	}
}


void insertionSort(int Arr[], int n) {  // O(n^2) but practically better than selectionSort and bubbleSort
	int value, hole;
	for (int i = 1; i < n; ++i) {
		value = Arr[i];
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


void quickSort(int Arr[], int left, int right) {  // O(n.logn) in average (with big probability), O(n^2) in worst case scenario
	int i = left, j = right;
	int pivot = Arr[(left + right) / 2];  // Arr[(left + right) / 2];

	/// partition
	while (i <= j) {
		while (Arr[i] < pivot) i++;
		while (Arr[j] > pivot) j--;
		if (i <= j) {
			swap(Arr[i], Arr[j]);
			i++;
			j--;
		}
	};

	/// recursion
	if (left < j)
		quickSort(Arr, left, j);
	if (i < right)
		quickSort(Arr, i, right);
}


int main()
{
	const int n = 10000;
	int* Arr = new int[n];  // int Arr[n];
	
	fillRandArr(Arr, n);

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	// Sorting Algorithms O(n^2):

	// bubbleSort(Arr, n);            // O(n^2) average, n=10000 -> msec = [2678, 2640, 2702, 2693, 2739]
	   straightSelectionSort(Arr, n); // O(n^2) average, n=10000 -> msec = [282, 283, 273, 299, 266]
	// selectionSort(Arr, n);         // O(n^2) average, n=10000 -> msec = [133, 131, 136, 135, 129]
	// insertionSort(Arr, n);         // O(n^2) average, n=10000 -> msec = [73, 66, 64, 71, 70]

	// Sorting Algorithms O(n.logn): 

	// std::stable_sort(Arr, Arr+n);  // O(n.logn) average, n=10000 -> msec = [21, 22, 24, 21, 21]
									  //                    n=100000 -> msec = [285, 289, 281, 293, 315]
									  //                    n=1000000 -> msec = [3069, 3085, 3092, 3078, 3084]
									  //                    n=10000000 -> msec = [35709, 35943, ...]
	// std::sort(Arr, Arr+n);         // O(n.logn) average, n=10000 -> msec = [13, 10, 12, 9, 11]
									  //                    n=100000 -> msec = [117, 107, 129, 119, 114]
									  //                    n=1000000 -> msec = [1066, 1028, 1045, 1076, 1092]
									  //                    n=10000000 -> msec = [11233,  11265, ...]
	// mergeSort(Arr, n);             // O(n.logn) average, n=10000 -> msec = [10, 10, 9, 13, 9]
									  //                    n=100000 -> msec = [109, 100, 112, 107, 102]
									  //                    n=1000000 -> msec = [998, 987, 989, 996, 981]
									  //                    n=10000000 -> msec = [9967, 9994, 10150, ...]
	// quickSort(Arr, 0, n - 1);      // O(n.logn) average, n=10000 -> msec = [5, 6, 5, 4, 4]
									  //                    n=100000 -> msec = [83, 67, 75, 71, 78]
									  //                    n=1000000 -> msec = [883, 838, 837, 847, 842]
									  //                    n=10000000 -> msec = [10002, 10244, 10059, ...]
	
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Sorting took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds." << std::endl;
	//for (int i=0; i<n; ++i) std::cout << Arr[i] << std::endl;

	delete[] Arr;
	return 0;
}

