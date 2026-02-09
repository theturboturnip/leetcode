typedef struct {
    uintptr_t* data;
    int capacity;
    int size;
} vec;

#define INITIAL_CAPACITY 16

vec new_vec() {
    return (vec){0};
}
// abort() on OOM
// TODO zeroing on malloc,realloc
// TODO error checks on argument NULL pointers
void push_vec(vec* v, uintptr_t val) {
    if (v->capacity == v->size) {
        if (v->size == 0) {
            v->capacity = INITIAL_CAPACITY;
            v->data = malloc(sizeof(uintptr_t) * v->capacity);
        } else {
            v->capacity = v->capacity * 2;
            v->data = realloc(v->data, sizeof(uintptr_t) * v->capacity);
        }
        if (v->data == NULL) {
            abort();
        }
    }

    v->data[v->size] = val;
    v->size++;
}
bool pop_vec(vec* v, uintptr_t* val) {
    if (v->size == 0) {
        return false;
    }
    *val = v->data[v->size - 1];
    v->size--;
    return true;
}
bool peek_vec(vec* v, uintptr_t* val) {
    if (v->size == 0) {
        return false;
    }
    *val = v->data[v->size - 1];
    return true;
}
void free_vec(vec* v) {
    if (v->data) {
        free(v->data);
    }
    (*v) = (vec){0};
}

// abort() on OOM
struct TreeNode* alloc_node() {
    struct TreeNode* node = malloc(sizeof(struct TreeNode));
    (*node) = (struct TreeNode){0};
    if (node == NULL) {
        abort();
    }
    return node;
}

// recursive traversal
void recursive_traverse(struct TreeNode* node, vec* data_vec) {
    if (node->left) {
        recursive_traverse(node->left, data_vec);
    }
    push_vec(data_vec, (uintptr_t)node->val);
    if (node->right) {
        recursive_traverse(node->right, data_vec);
    }
}

void recursive_build_tree(vec* v, int start, int end, struct TreeNode* node) {
    int pivot = (start + end) / 2;
    node->val = (int)(v->data[pivot]);
    if (pivot != start) {
        node->left = alloc_node();
        recursive_build_tree(v, start, pivot - 1, node->left);
    }
    if (pivot != end) {
        node->right = alloc_node();
        recursive_build_tree(v, pivot + 1, end, node->right);
    }
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
    if (root == NULL) return NULL;

    vec sorted_data_vec = new_vec();
    recursive_traverse(root, &sorted_data_vec);

    // // stack-based traversal would require a vec of (node, next_step)
    // // to mimic the return stack of the recursive_traverse function
    // vec traversal_vec = new_vec();
    // // Get to the first element
    // struct TreeNode* curr = root;
    // while (curr->left != NULL) {
    //     push_vec(&traversal_vec, (void*)curr);
    //     curr = curr->left;
    // }

    // sorted_vec now has a sorted list
    // build a balanced binary tree
    // we know the size of the vec is >=1
    assert(sorted_data_vec.size >= 1);
    // Heap allocate, needs to outlive stack
    struct TreeNode* new_root = alloc_node();
    recursive_build_tree(&sorted_data_vec, 0, sorted_data_vec.size - 1, new_root);

    // We have now constructed the tree, we don't need the sorted data anymore
    free_vec(&sorted_data_vec);

    return new_root;
}