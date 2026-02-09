static uint64_t bitvec[1000] = {0}; // 50000 bits ~= 64000 bits ~= 1000 u64s, actually don't need that many
// zero-init doesn't help because leetcode runs the function multiple times!

inline static bool hasUsedIdx(int i) {
    // Lookup bit #i where i < 50_000 (50_000 = ~ 64kbits = 2^16)
    // i.e. lookup bit i[5:0] of byte i[16:6]
    return (bitvec[i >> 6] >> (i & 0x3F)) & 1;
}
inline static void useIdx(int i) {
    // Set bit #i
    uint64_t* b = &bitvec[i >> 6];
    (*b) |= (((uint64_t)1) << (i & 0x3F));
}

static bool memoizedCanReach(int* arr, int arrSize, int index) {
    int jump = arr[index];
    if (jump == 0) {
        return true;
    }

    useIdx(index);
    if (index + jump < arrSize && !hasUsedIdx(index + jump)) {
        // printf("%d -> %d\n", index, index+jump);
        if (memoizedCanReach(arr, arrSize, index + jump)) {
            return true;
        }
    }
    if (index >= jump && !hasUsedIdx(index - jump)) {
        if (memoizedCanReach(arr, arrSize, index - jump)) {
            return true;
        }
    }
    return false;
}

bool canReach(int* arr, int arrSize, int start) {
    // Approach: 
    // - cannot reach a value named zero if all the places we *can* go are not zero and eventually become cycles
    // => depth first search through jumps, balking on a jump if it has already been taken

    memset(bitvec, 0, sizeof(bitvec));
    // printf("%x %x %x\n", bitvec[0], bitvec[1], bitvec[2]);

    return memoizedCanReach(arr, arrSize, start);
    
}