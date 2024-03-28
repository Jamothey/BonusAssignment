#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	// printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	// printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}


// creates heap recursively by moving the max value of the true to the root
// used in the heap sort funtion
void heapify(int arr[], int n, int i)
{
	int max = i; // initializes the max to the current root
	int left = 2 * i + 1; 
	int right = 2 * i + 2;
	if(left < n && arr[left] > arr[max]) // checks if left "child" is larger than the "parent"
	{
		max = left; // sets max to the left child if it is larger than the root parent
	}
	if(right < n && arr[right] > arr[max]) // checks if the right "child" is larger than the "parent"
	{
		max = right; // sets max to the right child if it is larger than the root parent
	}
	if(max != i) // checks if the max has changed from the original root
	{
		swap(&arr[i], &arr[max]); // if not the root is swapped with the max
		heapify(arr, n, max); // recursively calls the heapify function from max to heapify the affected sub-tree
	}

}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	for(int i = n / 2 - 1; i >= 0; i--) // creates the max heap
	{
		heapify(arr, n , i);
	}
	for(int i = n - 1; i >= 0; i--) //sorts the heap
	{
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if(l < r) // checks if there are more than one elements in the array
	{
		int middle = l + (r - l) / 2; // divides the array into two arrays to implement merge sort
		mergeSort(pData, l, middle); // recursively calls merge sort for the left side of the array
		mergeSort(pData, middle + 1, r); // recursively calls merge sort for the right side of the array 
		int arr1 = middle - l + 1; // this calculates the size of the left array
		int arr2 = r - middle; // this calcualtes the size of the right array
		int *L = (int *)Alloc(arr1 * sizeof(int)); // allocates memory for the left half
		int *R = (int *)Alloc(arr2 * sizeof(int)); // allocates memory for the right half
		for(int i = 0; i < arr1; i++) // copies the left half into temporary array
		{
			L[i] = pData[l + i];
		}
		for(int i = 0; i < arr2; i++) // copies right half into temporary array
		{
			R[i] = pData[middle + 1 + i];
		}
		int i = 0, j = 0; // initialize the indexes for the temp arrays, i for L and j for R
		int k = l; // k is for the indexes of the main array
		while (i < arr1 && j < arr2) // merges the two temp arrays back into pData
		{
			if(L[i] <= R[j])
			{
				pData[k] = L[i];
				i++;
			}
			else
			{
				pData[k] = R[j];
				j++;
			}
			k++;
		}
		while(i < arr1) // if there are remaining elements in the left half, move them back into pData
		{
			pData[k] = L[i];
			i++;
			k++;
		}
		while(j < arr2) // if there are remaining elements in the right half, move them back into pData
		{
			pData[k] = R[j];
			j++;
			k++;
		}
		DeAlloc(L); // deallocates memory for left half temp array
		DeAlloc(R); // deallocates memory for right half temp array
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, j, num; // initializes i and j for the loop counters and num which is used to store the current element being inserted into its correct position
	for(i = 1; i < n; i++) // iterates through the array from the second element to the last element
	{
		num = pData[i]; //the current element is stored in num to be relocated
		j = i - 1; // sets j to the index of the previous elemenet to compare
		while(j >= 0 && pData[j] > num) // shifts the elements of the sorted portion that are greater than num
		{
			pData[j + 1] = pData[j]; // shifts the element in j to the right (j + 1)
			j--; // decrements j
		}
		pData[j + 1] = num; // inserts num into its correct position
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	for(int i = 0; i < n; i++) // increments the ending element of the sort
	{
		for(int j = 0; j < n - 1 - i; j++) // increments through the array from 0 to the current ending index which comes from n - 1 - i;
		{
			if(pData[j] > pData[j + 1]) // checks if the next element is smaller than the current element
			{
				swap(&pData[j], &pData[j + 1]); // if so the two elements get swapped
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	for(int i = 0; i < n - 1; i++) // increments the starting index
	{
		int minIndex = i; // initializes the minIndex to the current starting index
		int j = 0; // initializes j to 0
		for(j = j + i; j < n; j++) // parses through the array to find the min index
		{
			if(pData[j] < pData[minIndex]) // checks if the data in the jth index is less than the data in the current minIndex
			{
				minIndex = j; // if so, j becomes the new minIndex
			}
		}
		if(minIndex != i) // if minIndex does not equal the starting index
		{
			swap(&pData[minIndex], &pData[i]); // swap the minIndex with the starting index
		}
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r"); // opens the file into the file pointer "inFile"
	int dataSz = 0; // initialize data size to 0, this will change when read in the input files
	*ppData = NULL; // sets the pointer to the pData pointer to NULL
	
	if (inFile) // if infile is not NULL
	{
		fscanf(inFile,"%d\n",&dataSz); // scan input file for data size
		int n, *data; // initializes n and data pointer
		*ppData = (int *)Alloc(sizeof(int) * dataSz); // allocates data for the arrays based on data size
		if(*ppData == NULL) // checks if the pointer for pData is still NULL
		{
			printf("Allocation Failed\n"); // if so allocation failed
			exit(-1); // exit with an error code of -1
		}
		for(int i = 0; i < dataSz; i++) // increments through the input file 'dataSz' amount of times to get the data
		{
			fscanf(inFile,"%d ", &n);
			data = *ppData + i;
			*data = n;
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
// for data sizes of less than 100 the print just prints it normally
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	if(dataSz <= 200)
	{
		for(i = 0; i < dataSz; i++)
		{
			printf("%d ", pData[i]);
		}
	}
	else
	{
		for (i=0;i<100;++i)
		{
			printf("%d ",pData[i]);
		}
		printf("...\n\t...");
	
		for (i=sz;i<dataSz;++i)
		{
			printf("%d ",pData[i]);
		}
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	int n = 0;
	printf("Enter the number of the input file you would like to sort with: "); // prompts the user to input the number of the input file they would like to use, this makes it easier for each file to be tracked
	while(n < 1 || n > 4)
	{
		scanf("%d", &n);
		if(n < 1 || n > 4)
		{
			printf("The value entered must be between 1 and 4.\nEnter the number of the input file you would like to sort with: ");
		}
	}
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt", "input4.txt"}; // added a smaller input file from lab 8 to test the sorting algorithms
	
	for (i=n-1;i<n;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
