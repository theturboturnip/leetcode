// I thought this would zero-init the nodes... hm.
static struct TreeNode nodes[10000] = {0};

// recursive traversal
void recursive_traverse(struct TreeNode* node, int* curr_node) {
    if (node->left) {
        recursive_traverse(node->left, curr_node);
    }
    nodes[*curr_node].val = node->val;
    (*curr_node)++;
    if (node->right) {
        recursive_traverse(node->right, curr_node);
    }
}

struct TreeNode* recursive_build_tree(int start, int end) {
    int pivot = (start + end) / 2;
    struct TreeNode* p = &nodes[pivot];
    // nodes[pivot].val already equals the middle value
    if (pivot != start) {
        // i.e. pivot > start
        p->left = recursive_build_tree(start, pivot - 1);
    } else {
        p->left = NULL;
    }
    if (pivot != end) {
        // i.e. pivot < end
        p->right = recursive_build_tree(pivot + 1, end);
    } else {
        p->right = NULL;
    }
    return p;
}

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
struct TreeNode* balanceBST(struct TreeNode* root) {
    // Traverse tree while building sorted list, then convert to a tree again
    // this check seemingly costs many ms
    // if (root == NULL) return NULL;

    int curr_node = 0;
    recursive_traverse(root, &curr_node);

    // sorted_vec now has a sorted list
    // build a balanced binary tree
    return recursive_build_tree(0, curr_node - 1);
}