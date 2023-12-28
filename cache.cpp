//cache simulation assignment---------------------//------------------------------------------------
#include "cache.h"

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