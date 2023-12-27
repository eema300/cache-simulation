//cache simulation assignment---------------------//------------------------------------------------



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



//simulation--------------------------------------//------------------------------------------------
int main(){
    Cache L1 = cache_init(1);
    Cache L2 = cache_init(2);
    Cache L3 = cache_init(3);
    Memory main_mem = {MAIN_MEM_LATENCY};

    int inc, cycles = 0;
    int addresses[] = {0x1000, 0x1024, 0x1599, 0x100, 0x10245, 0x10246, 0x1027, 0x10247, 0x1600, 0x1601, 0x1700};

    while(inc < NUM_ADDR){
        load(addresses[inc], L1, L2, L3, main_mem, cycles);
        ++inc;
    }

    printf("Total cycles: %d\n", cycles);

    return 0;
}
//------------------------------------------------//------------------------------------------------



//definitions-------------------------------------//------------------------------------------------
Cache cache_init(int cache_num){
    if(1 == cache_num){
        Cache L = {{L1_SIZE, SETS, L1_LINES, L1_LATENCY}};
        for(int i = 0; i < L3_LINES; ++i)
            L.ages[i] = INT_MAX;
        return L;
    }
    else if(2 == cache_num){
        Cache L = {{L2_SIZE, SETS, L2_LINES, L2_LATENCY}};
        for(int i = 0; i < L3_LINES; ++i)
            L.ages[i] = INT_MAX;
        return L;
    }
    else{
        Cache L = {{L3_SIZE, SETS, L3_LINES, L3_LATENCY}};
        for(int i = 0; i < L3_LINES; ++i)
            L.ages[i] = INT_MAX;
        return L;
    }
}


bool access(const Cache L, const int addr, int &line_index){
    //round down to the nearest multiple of line size
    int rounded_addr = addr - (addr % L.cache.line_size);
    //check if in list of starting addresses
    bool hit = false;
    for(int i = 0; i < L.cache.lines_per_set; ++i){
        if(rounded_addr == L.st_addr[i]){
            hit = true;
            line_index = i;
            return hit;
        }
    }
    return hit;
}


void cache_hit(Cache &L, const int line_index){
    //update all other ages
    for(int i = 0; i < L.cache.lines_per_set; ++i){
        if(i == line_index) continue;
        if(L.ages[i] < INT_MAX) L.ages[i]++;
    }
}


void cache_miss(Cache &L, const int addr){
    //find oldest age
    int index;
    int old = 0;
    for(int i = 0; i < L.cache.lines_per_set; ++i){
        if(old < L.ages[i]){
            old = L.ages[i];
            index = i;
        }
    }
    //load into line with oldest age
    int rounded_addr = addr - (addr % L.cache.line_size);
    L.st_addr[index] = rounded_addr;
    //set loaded line age to 0
    L.ages[index] = 0;
}


void load(const int addr, Cache &L1, Cache &L2, Cache &L3, Memory &main_mem, int &cycles){
    int line_index;
    //check L1
    //on hit
    if(access(L1, addr, line_index)){
        cache_hit(L1, line_index);
        cycles += L1.cache.latency;
        return;
    }
    //on miss
    cache_miss(L1, addr);
    cycles += L1.cache.latency;

    //check L2
    //on hit
    if(access(L2, addr, line_index)){
        cache_hit(L2, line_index);
        cycles += L2.cache.latency;
        return;
    }

    //on miss
    cache_miss(L2, addr);
    cycles += L2.cache.latency;

    //check L3
    //on hit
    if(access(L3, addr, line_index)){
        cache_hit(L3, line_index);
        cycles += L3.cache.latency;
        return;
    }
    //on miss
    cache_miss(L3, addr);
    cycles += L3.cache.latency;

    //resort to memory
    cycles += main_mem.latency;
}
//------------------------------------------------//------------------------------------------------