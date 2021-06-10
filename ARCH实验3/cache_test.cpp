#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;

#define ARRAY_SIZE (1 << 28)                                    // test array size is 2^28
typedef unsigned char BYTE;										// define BYTE as one-byte type

#define test_num 600000

BYTE array[ARRAY_SIZE];											// test array
int L1_cache_size = 1 << 15;
int L2_cache_size = 1 << 19;
int L1_cache_block = 64;
int L2_cache_block = 64;
int L1_way_count = 8;
int L2_way_count = 4;
int write_policy = 0;											// 0 for write back ; 1 for write through

// have an access to arrays with L2 Data Cache'size to clear the L1 cache
void Clear_L1_Cache() {
 	memset(array, 0, L2_cache_size);
}

// have an access to arrays with ARRAY_SIZE to clear the L2 cache
void Clear_L2_Cache() {
	memset(&array[L2_cache_size + 1], 0, ARRAY_SIZE - L2_cache_size);
}

int L1_DCache_Size() {
	cout << "L1_Data_Cache_Test" << endl;
	//add your own code
	clock_t start,finish;
	double time;
	double pre_time = 0;
	double maxtime = 0;
	int L1_size = 0;
	int shift = 128;
	for(int size = 4;size < 256;size = size * 2){//from 4K to 1M 
		int size_kb = size * 1024;
		start = clock();
		for(int num = 0;num <= test_num;num ++){
			for(int i = 0; i < size_kb; i += shift){
				array[i] &= i;
			}
		}
		finish = clock();
		time = (double)(finish - start) / size;
		if((pre_time != 0)&&((time - pre_time) >= 0)){
			if(maxtime < (time - pre_time)){
				maxtime = time - pre_time;
				L1_size = size / 2;
			}
		}
		pre_time = time;
		cout<<"Test_Array_Size = "<<size << "KB   ";
		cout << "Average access time = " << time  << "ms"<< endl;
	}
	cout << "L1_Data_Cache_Size is  " << L1_size  << "KB" << endl;
	cout << "******************************************" << endl;
	return L1_size;
}

int L2_Cache_Size() {
	cout << "L2_Data_Cache_Test" << endl;
	Clear_L1_Cache();
	Clear_L2_Cache();
	clock_t start,finish;
	double time;
	double pre_time = 0;
	double maxtime = 0;
	int L2_size = 0;
	int shift = 128;
	for(int size = 64;size < 4096;size = size * 2){//from 64K to 1024k 
		int size_kb = size * 1024;
		start = clock();
		for(int num = 0;num <= test_num;num ++){
			for(int i = 0; i < size_kb; i += shift){
				array[i] &= i;
			}
		}
		finish = clock();
		time = (double)(finish - start)  / size;
		if((pre_time != 0)&&((time - pre_time) >= 0)){
			if(maxtime < (time - pre_time)){
				maxtime = (time - pre_time);
				L2_size = size / 2;
			}
		}
		pre_time = time;
		cout<<"Test_Array_Size = "<<size << "KB   ";
		cout << "Average access time = " << time  << "ms"<< endl;
	}
	cout << "L2_Data_Cache_Size is  " << L2_size  << "KB" << endl;
	cout << "******************************************" << endl;
	return L2_size;
}

int L1_DCache_Block() {
	cout << "L1_DCache_Block_Test" << endl;
	//add your own code
	Clear_L1_Cache();
	double time;
	double pre_time = 0;
	double maxtime = 0; 
	int L1_size = 0;
	clock_t start,finish;
	for(int step_size = 8;step_size < 256;step_size = step_size *2){
		start = clock();
		for(int num = 0;num < test_num;num ++){
			for(int i = 0,test_size = 0;test_size < 1024;i += step_size,test_size ++){
				array[i] &= test_size;
			}
		}
		finish = clock();
		time = (double)(finish - start) * 1000 / CLOCKS_PER_SEC ;
		if((pre_time != 0)&&((time - pre_time) >= 0)){
			if(maxtime < (time - pre_time)){
				maxtime = time - pre_time;
				L1_size = step_size;
			}
		}
		pre_time = time;
		cout<<"Test_Step_Size = "<<step_size << "B   ";
		cout << "Average access time = " << time  << "ms"<< endl;
	}
	cout << "L1_DCache_Block_Size is  " << L1_size  << "B" << endl;
	cout << "******************************************" << endl;
}

int L2_Cache_Block() {
	cout << "L2_Cache_Block_Test" << endl;
	//add your own code
	Clear_L2_Cache();
	double time;
	double pre_time = 0;
	double maxtime = 0;
	int L1_size = 0;
	clock_t start,finish;
	for(int step_size = 8;step_size < 256;step_size = step_size *2){
		start = clock();
		for(int num = 0;num < test_num;num ++){
			for(int i = 0,test_size = 0;test_size < 1700;i += step_size,test_size ++){
				array[i] ^= test_size;
			}
		}
		finish = clock();
		time = (double)(finish - start) * 1000 / CLOCKS_PER_SEC ;
		if((pre_time != 0)&&((time - pre_time) >= 0)){
			if(maxtime < (time - pre_time)){
				maxtime = time - pre_time;
				L1_size = step_size;
			}
		}
		pre_time = time;
		cout<<"Test_Step_Size = "<<step_size << "B   ";
		cout << "Average access time = " << time  << "ms"<< endl;
	}
	cout << "L2_Cache_Block_Size is  " << L1_size  << "B" << endl;
	cout << "******************************************" << endl;
}

int L1_DCache_Way_Count() {
	cout << "L1_DCache_Way_Count" << endl;
	//add your own code
	//用一个两倍于Cache大小的数组来实现
	Clear_L1_Cache();
	int array_size = 2 * L1_cache_size;
	double time;
	double pre_time = 0,maxtime = 0;
	int L1_way = 0;
	int shift = 64;
	for(int way = 2;way < 32;way = way * 2){
		int group = way *2;
		clock_t start = clock();
		int length = array_size / group;
		for(int num = 0;num < test_num;num ++){
			for(int i = 0;i < group / 2;i ++){
				int begin = 2*i*length;
				for(int k = begin;k < begin + length;k +=shift){
					array[k] &= k;
				}
			}
		}
		clock_t finish = clock();
		time = (double)(finish - start) * 1000 / CLOCKS_PER_SEC ;
		if((pre_time != 0)&&((time - pre_time) >= 0)){
			if(maxtime < (time - pre_time)){
				maxtime = time - pre_time;
				L1_way = way / 2;
			}
		}
		pre_time = time;
		cout<<"way = "<<way << "\t";
		cout << "Average access time = " << time  << "ms"<< endl;

	}
	cout << "L1_DCache_Way_Count is  " << L1_way << endl;
	cout << "******************************************" << endl;	
	return L1_way;
}


int L2_Cache_Way_Count() {
	cout << "L2_Cache_Way_Count" << endl;
	//用一个两倍于Cache大小的数组来实现
	Clear_L2_Cache();
	Clear_L1_Cache();
	int array_size = 2 * L2_cache_size;
	double time;
	double pre_time = 0,maxtime = 0;
	int L2_way = 0;
	int shift = 64;
	for(int way = 2;way < 32;way = way * 2){
		int group = way *2;
		int length = array_size / group;
		clock_t start = clock();
		for(int num = 0;num < test_num;num ++){
			for(int i = 0;i < group / 2;i ++){
				int begin = 2*i*length;
				for(int k = begin;k < begin + length;k += shift){
					array[k] &= k;
				}
			}
		}
		clock_t finish = clock();
		time = (double)(finish - start) * 1000 / CLOCKS_PER_SEC ;
		if((pre_time != 0)&&((time - pre_time) >= 0)){
			if(maxtime < (time - pre_time)){
				maxtime = time - pre_time;
				L2_way = way / 2;
			}
		}
		pre_time = time;
		cout<<"way = "<<way << "\t";
		cout << "Average access time = " << time  << "ms"<< endl;

	}
	cout << "L2_DCache_Way_Count is  " << L2_way << endl;
	cout << "******************************************" << endl;	
	return L2_way;
}

// int Cache_Write_Policy() {
// 	cout << "Cache_Write_Policy" << endl;
// 	//add your own code
// }

// void Check_Swap_Method() {
// 	cout << "L1_Check_Replace_Method" << endl;
// 	//add your own code
	
// }

int main() {
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	L1_cache_size = L1_DCache_Size();
	L2_cache_size = L2_Cache_Size();
	L1_cache_block = L1_DCache_Block();
	L2_cache_block = L2_Cache_Block();
	L1_way_count = L1_DCache_Way_Count();
	L2_way_count = L2_Cache_Way_Count();
	// write_policy = Cache_Write_Policy();
	// Check_Swap_Method();
	system("pause");
	return 0;
}

