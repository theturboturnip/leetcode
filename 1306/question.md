# 1306. Jump Game III

Given an array of non-negative integers arr, you are initially positioned at start index of the array. When you are at index i, you can jump to i + arr[i] or i - arr[i], check if you can reach any index with value 0.

Notice that you can not jump outside of the array at any time.

 

## Example 1:

Input: arr = [4,2,3,0,3,1,2], start = 5
Output: true
Explanation: 
All possible ways to reach at index 3 with value 0 are: 
index 5 -> index 4 -> index 1 -> index 3 
index 5 -> index 6 -> index 4 -> index 1 -> index 3 

## Example 2:

Input: arr = [4,2,3,0,3,1,2], start = 0
Output: true 
Explanation: 
One possible way to reach at index 3 with value 0 is: 
index 0 -> index 4 -> index 1 -> index 3

## Example 3:

Input: arr = [3,0,2,1,2], start = 2
Output: false
Explanation: There is no way to reach at index 1 with value 0.

 

## Constraints:

    1 <= arr.length <= 5 * 104
    0 <= arr[i] < arr.length
    0 <= start < arr.length

# Commentary

As described in comments, traverse paths and stop when you see a cycle.
Use a bitmask as storage-intensive (ish) mapping of (index) to (have seen index before).
0ms but 14MB, beat 100% runtime but only 31% memory (11MB median). I consider this fine, it looks like other approaches allocate 50k ints and 50k bools but don't always touch all of them => those pages won't count towards the memory usage?
I have to zero everything out each time which may hurt cache/make pages resident.

Dad recommends 1ull everywhere to get 64-bit literals.