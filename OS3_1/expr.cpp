#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <thread>
#include <atomic>
#include <chrono>
#include <cmath>

using namespace std;



void tokenize(string const& str, const char delim, vector<string>& out) {
	stringstream ss(str);
	string s;
	while (getline(ss, s, delim)) {
		out.push_back(s);
	}
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
	const char delim = ' ';
	vector<string> out;
	tokenize(line, delim, out);

	for (auto& line : out) {
		int i = stoi(line);
		nums.push_back(i);
		//std::cout << i << endl;
	}
	int len = nums.size();
	//std::cout << "len " << len << endl;
	int signs_num = len - 2;
	atomic_int success = { 0 };
	vector<thread*> threads;
	double tmp = pow(2, signs_num);
	auto calc = [=, &success, &nums](int therad_num) {
		for (int t = 0; t < tmp; t += thread_count) {
			int i = t + therad_num;
			if (therad_num >= tmp)break;
			int acc = nums[0];
			for (int s = 0; s < signs_num; s++)
			{
				bool plus = ((i >> s) & 1) > 0;
				//cout << ((i >> s) & 1) << endl;
				if (plus)
				{
					acc += nums[s + 1];
				}
				else {
					acc -= nums[s + 1];
				}
			}
			if (acc == nums[len - 1]) {
				++success;

				/*std::cout << nums[0];
				for (int j = 0; j < signs_num; j++)
				{
					bool plus = ((i >> j) & 1) > 0;
					std::cout << (plus ? "+" : "-") << nums[j + 1];
				}
				std::cout << "=" << acc << endl;*/
			}
		}
	};
	auto start = chrono::steady_clock::now(); // начальное время
	for (int k = 0; k < thread_count; k++)
	{
		auto thread = new std::thread(calc, k);
		threads.push_back(thread);
	}

	for (auto thread : threads) {
		thread->join();
		delete(thread);
	}
	auto end = chrono::steady_clock::now(); // конечное время
	unsigned int search_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	cout << "time: " << search_time << endl;
	cout << success << endl;
	fstream file_out;
	file_out.open("output.txt", ios::out);
	if (file_out.is_open())
	{
		file_out << thread_count << endl;
		file_out << nums_count << endl;
		file_out << success << endl;
	}
	file_out.close();
	fstream time_out;
	time_out.open("time.txt", ios::out);
	if (time_out.is_open())
		time_out << search_time << endl;
	time_out.close();
	return 0;
}