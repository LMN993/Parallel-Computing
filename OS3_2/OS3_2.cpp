#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <chrono>
#include <algorithm>
#include <thread>
#include <atomic>


using namespace std;
atomic_int counter = { 0 };
void tokenize(string const& str, const char delim, vector<string>& out) {
	stringstream ss(str);
	string s;
	while (getline(ss, s, delim)) {
		out.push_back(s);
	}
}


int thread_sort(tp* tp, vector<int>::iterator begin, vector<int>::iterator end) {
    auto const size = end - begin;
    if (size <= 1) return 0;

    auto pivot = begin + size / 2;
    auto const pivot_v = *pivot;

    swap(*pivot, *(end - 1));
    auto p = partition(begin, end, [&](const int& a) { return a < pivot_v; });
    swap(*p, *(end - 1));

    tp->add_task(thread_sort, tp, begin, p);
    tp->add_task(thread_sort, tp, p + 1, end);
    //thread_sort(begin, p);
    //thread_sort(p + 1, end);

    return 0;
}

void quick_sort(vector<int>& arr, tp* tp) {
    thread_sort(tp, arr.begin(), arr.end());
    tp->wait_all();
}


int main() {
	fstream f;
	f.open("input.txt", ios::in);
	string line;
	int thread_count;
	int nums_count;
	getline(f, line);
	thread_count = stoi(line);
	getline(f, line);
	nums_count = stoi(line);
	getline(f, line);
	f.close();
    vector <int> nums;
    counter = thread_count-1;
	const char delim = ' ';
	vector<string> out;
	tokenize(line, delim, out);

	for (auto& line : out) {
		int i = stoi(line);
		nums.push_back(i);
	}
    cout << "sorting..." << endl;
    auto start = chrono::steady_clock::now(); // начальное время
    quick_sort(nums);
    auto end = chrono::steady_clock::now(); // конечное время
    unsigned int search_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "time: " << search_time << endl;
   // for(int i = 0; i< nums_count; i++)
    //cout << nums[i] << " ";
	
    fstream file_out;
    file_out.open("output.txt", ios::out);
    if (file_out.is_open())
    {
        file_out << thread_count << endl;
        file_out << nums_count << endl;
        for (int i = 0; i < nums_count; i++)
            file_out << nums[i] << " ";
    }
    file_out.close();
    fstream time_out;
    time_out.open("time.txt", ios::out);
    if (time_out.is_open())
        time_out << search_time << endl;
    time_out.close();
    return 0;
}