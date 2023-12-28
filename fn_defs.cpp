#include "cache.h"


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