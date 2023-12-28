#ifndef CACHE_H
#define CACHE_H

//includes----------------------------------------//------------------------------------------------
#include <climits>
#include <cstdio>
using namespace std;
//------------------------------------------------//------------------------------------------------

//macros------------------------------------------//------------------------------------------------
#define NUM_ADDR                    11
#define L1_SIZE                     0x100
#define L2_SIZE                     0x400
#define L3_SIZE                     0x1000
#define L1_LINES                    4
#define L2_LINES                    64
#define L3_LINES                    256
#define L1_LATENCY                  1
#define L2_LATENCY                  10
#define L3_LATENCY                  100
#define MAIN_MEM_LATENCY            1000
#define SETS                        1
//------------------------------------------------//------------------------------------------------

//struct definitions------------------------------//------------------------------------------------
struct Memory {
    int latency;
};

struct CacheP {
    int line_size, sets, lines_per_set, latency;
};

struct Cache {
    CacheP cache;
    int ages[L3_LINES];
    int st_addr[L3_LINES];
};
//------------------------------------------------//------------------------------------------------

//declarations------------------------------------//------------------------------------------------
Cache cache_init(int cache_num);
void load(const int addr, Cache &L1, Cache &L2, Cache &L3, Memory &main_mem, int &cycles);
bool access(const Cache L, const int addr, int &line_index);
void cache_hit(Cache &L, const int line_index);
void cache_miss(Cache &L, const int addr);
//------------------------------------------------//------------------------------------------------

#endif