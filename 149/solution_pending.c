struct pair {
    union {
        int m;
        int x;
    };
    union {
        int c;
        int y;
    };
};

typedef struct pair line;  // y = mx + c, unless m = INT_MAX in which case a vertical line x = c

bool init_line(line* l, struct pair* p1, struct pair* p2) {
    int dy = p2->y - p1->y;
    int dx = p2->x - p1->x;

    // normalize dy and dx
    if (dx < 0) {
        dy = -dy;
        dx = -dx;
    }

    // if m is an integer, calculate m and c
    if (dx == 0) {
        l->m = INT_MAX;
        l->c = p1->x;
        return true;
    } else if (dy % dx == 0) {
        int m = dy / dx;
        l->c = p1->y - p1->x * m;
        l->m = m;
        return true;
    } else {
        return false;
    }
}

uint32_t hash_line(line* l) {
    return ((uint32_t)l->m) * 13 + ((uint32_t)l->c) * 7;
}

bool lines_equal(line* l1, line* l2) {
    return (l1->m == l2->m) && (l1->c == l2->c);
}

// Open hashing
typedef struct {
    line l;
    uintptr_t i;
} hash_table_elem;

struct hash_table_elem_open {
    struct hash_table_elem_open* next; 
    hash_table_elem elem; 
};

#define OPEN_HASH_TABLE_CAPACITY 64

typedef struct {
    struct hash_table_elem_open buckets[OPEN_HASH_TABLE_CAPACITY];
    bool has_data[OPEN_HASH_TABLE_CAPACITY];
    int size;
} hash_table_open;

hash_table_open new_hash_table_open() {
    return (hash_table_open){0};
}

hash_table_elem* hash_table_open_create_or_insert(hash_table_open* open_hash_table, line* l) {
    uint32_t h = hash_line(l);
    uint32_t bucket = h % OPEN_HASH_TABLE_CAPACITY;

    if (open_hash_table->has_data[bucket]) {
        struct hash_table_elem_open* node = &open_hash_table->buckets[bucket];
        while (1) {
            if (lines_equal(&node->elem.l, l)) {
                return &node->elem;
            } else if (node->next) {
                node = node->next;
            } else {
                // reached end of bucket, alloc a new one and return it
                node->next = malloc(sizeof(struct hash_table_elem_open));
                *(node->next) = (struct hash_table_elem_open){
                    .elem = (hash_table_elem){
                        .l = *l,
                        .i = 0
                    },
                    .next = NULL
                };
                open_hash_table->size++;
                return &node->next->elem;
            }
        }
    } else {
        open_hash_table->has_data[bucket] = true;
        open_hash_table->buckets[bucket].elem.l = *l;
        open_hash_table->buckets[bucket].elem.i = 0;
        open_hash_table->buckets[bucket].next = NULL;
        open_hash_table->size++;
        return &open_hash_table->buckets[bucket].elem;
    }
}

void free_hash_table_open(hash_table_open* open_hash_table) {
    for (int i = 0; i < OPEN_HASH_TABLE_CAPACITY; i++) {
        // TODO free it backwards
    }
}

int maxPoints(int** points, int pointsSize, int* pointsColSize) {
    // Thought: this will require O(points.length**2) computations, because 
    // you need to compare every point to every other point?
    // Could do /2 by (b - a) = -(a - b)

    // The question is unfortunately vague about what gradients are valid - both examples are on the m = 1 or m = -1, not m = 2, but I will assume arbitrary integer m necessary.
    // TODO this was a wrong assumption. e.g. [[3, 10], [0, 2]] should produce 2 - they are on a line with non-integer m

    // So: for every point i,
    //      then for every point j > i,
    //              (note that j>i is important to avoid counting duplicates)
    //          increment 1 for the counter for the line identified by (i - j) [m, c]
    //          (if m is non-integer, discard)
    // THIS IS WRONG!
    //          if the point j has not already been included in the line identified by (i - j), include it!

    // Uncertain what pointsColSize indicates, it is specified in the question that points[i].length == 2.
    assert(*pointsColSize == 2); 

    if (pointsSize == 1) {
        return 1;
    }

    int maxWitnessedCounter = 0;

    // mapping of (line) -> (pointer to table mapping point -> bool)
    hash_table_open counter_table = new_hash_table_open();

    for (int i = 0; i < pointsSize; i++) {
        for (int j = i + 1; j < pointsSize; j++) {
            struct pair* p1 = (struct pair*)(points[i]);
            struct pair* p2 = (struct pair*)(points[j]);

            line l;
            if (init_line(&l, p1, p2)) {
                printf("(%d %d) - (%d %d) => m = %d, c = %d\n", points[i][0], points[i][1], points[j][0], points[j][1], l.m, l.c);

                // l is valid!
                // check if this point has already been counted
                hash_table_elem* elem = hash_table_open_create_or_insert(&counter_table, &l);
                if (elem->i == NULL) {
                    hash_table_open* table = malloc(sizeof(hash_table_open));
                    *table = new_hash_table_open();
                    elem->i = (uintptr_t)table;
                }
                hash_table_open* point_table = (hash_table_open*)elem->i;
                hash_table_open_create_or_insert(point_table, p1);
                hash_table_open_create_or_insert(point_table, p2);

                int counter_val = point_table->size;
                if (counter_val > maxWitnessedCounter) {
                    printf("max counter %d\n", counter_val);
                    maxWitnessedCounter = counter_val;
                }
            }
        }
    }

    free_hash_table_open(&counter_table);

    return maxWitnessedCounter;
}