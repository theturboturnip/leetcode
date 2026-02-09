# 149. Max Points on a Line
Attempted
Hard
Topics
premium lock iconCompanies

Given an array of points where points[i] = [xi, yi] represents a point on the X-Y plane, return the maximum number of points that lie on the same straight line.

 

## Example 1:

Input: points = [[1,1],[2,2],[3,3]]
Output: 3

## Example 2:

Input: points = [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]
Output: 4

 

# Constraints:

    1 <= points.length <= 300
    points[i].length == 2
    -104 <= xi, yi <= 104
    All the points are unique.

# Commentary

I wrote the open hash table pretty well without realising I needed table-of-tables, then tried to bodge hash-table-of-hash-tables using the same struct twice...
This would have been fine if both could be keyed by a two-int structure. however, the line actually needs three I think
(need to represent dy, dx, and c). Previously I tried compressing dy and dx into one int m, but that requires constraining to integer m.
This has a problem in terms of freeing too, wherein it isn't implemented yet. It is simple emough to do, but you do need separate "free hash map" functions for the table-of-tables and the leaf table.