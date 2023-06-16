#include <iostream>
//#include <thread>
#include <time.h>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <stdio.h>
using namespace std;

vector<int> a;
int nums_count = 0;
int part = 0;

// merge function for merging two parts
void merge(int low, int mid, int high)
{
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];

    // n1 is size of left part and n2 is size
    // of right part
    int n1 = mid - low + 1, n2 = high - mid, i, j;

    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = a[i + low];

    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];

    int k = low;
    i = j = 0;

    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    // insert remaining values from left
    while (i < n1) {
        a[k++] = left[i++];
    }

    // insert remaining values from right
    while (j < n2) {
        a[k++] = right[j++];
    }
}

// merge sort function
void merge_sort(int low, int high)
{
    // calculating mid point of array
    int mid = low + (high - low) / 2;
    if (low < high) {

        // calling first half
        merge_sort(low, mid);

        // calling second half
        merge_sort(mid + 1, high);

        // merging the two halves
        merge(low, mid, high);
    }
}

void tokenize(string const& str, const char delim, vector<string>& out) {
    stringstream ss(str);
    string s;
    while (getline(ss, s, delim)) {
        out.push_back(s);
    }
}

DWORD WINAPI merge_sorting(void* param) {
    // int thread_part = part++;
    char i = (char*)param - (char*)0;
    int low = i * part;
    int high = low + part - 1;
    if (high > nums_count - 1) high = nums_count - 1;
    merge_sort(low, high);
    return 0;
}

int main(){
    fstream f;
    f.open("input.txt", ios::in);
    string line;
    int thread_count;
    //int nums_count;
    getline(f, line);
    thread_count = stoi(line);
    getline(f, line);
    nums_count = stoi(line);
    getline(f, line);
    f.close();
    //vector <int> nums;
    const char delim = ' ';
    vector<string> out;
    tokenize(line, delim, out);
    for (int j = 0; j < nums_count; j++) {
        int n = stoi(out[j]);
        a.push_back(n);
    }
    //vector<thread*> threads;
    HANDLE* handles = (HANDLE*)malloc(sizeof(HANDLE) * thread_count);
    part = ceil(nums_count / (double)thread_count);

    
    clock_t start, end;
    start = clock();
    for (int i = 0; i < thread_count; i++) {
     //   auto thread = new std::thread(merge_sorting, i);
      //  threads.push_back(thread);
        handles[i] = CreateThread(0, 0, merge_sorting, (char*)0 + i, 0, NULL);
    }
    
    WaitForMultipleObjects(thread_count, handles, TRUE, INFINITE);
    end = clock();
    clock_t time = (end - start) * 100.0 / CLK_TCK;
    //cout << "time: " << time << endl;
    for (int i = 0; i < thread_count; i++)
        CloseHandle(handles[i]);
    merge_sort(0, nums_count - 1);
    fstream file_out;
    file_out.open("output.txt", ios::out);
    if (file_out.is_open())
    {
        file_out << thread_count << endl;
        file_out << nums_count << endl;
        for (int i = 0; i < nums_count; i++)
            file_out << a[i] << " ";
    }
    file_out.close();
    fstream time_out;
    time_out.open("time.txt", ios::out);
    if (time_out.is_open())
        time_out << time << endl;
    time_out.close();

    return 0;
}