static char const * const LETTERS[8] = {
    "abc",
    "def",
    "ghi",
    "jkl",
    "mno",
    "pqrs",
    "tuv",
    "wxyz"
};

static int const LENGTHS[8] = {
    3,
    3,
    3,
    3,
    3,
    4,
    3,
    4
};

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** letterCombinations(char* digits, int* returnSize) {
    // Two-pass algorithm because we don't know strlen up front:
    // 1. Gather length of final array and strlen together
    // 2. Allocate each output string
    // 3. Run through the input string again, setting the requisite characters in each of the output strings O(digits_length * return_size)

    int digits_length = 0;
    int return_size = 1;

    char* digits_temp = digits;
    char dt = *digits_temp;
    while (dt != '\0') {
        digits_length++;

        int num = dt - '2';
        return_size *= LENGTHS[num];

        dt = *(++digits_temp);
    }

    // Allocate array
    char** combos = malloc(sizeof(char*) * return_size);
    for (int i = 0; i < return_size; i++) {
        combos[i] = malloc(sizeof(char) * (digits_length + 1));
        combos[i][digits_length] = '\0';
    }

    // First digit cycles slowly, last digit cycles quickly
    // => maintain cycle period and reduce as you go forward
    int period = return_size;
    for (int i = 0; i < digits_length; i++) {
        int num = digits[i] - '2';
        int num_letters_for_num = LENGTHS[num];

        // This will always divide evenly because return_size starts as multiplicative sum of LENGTHS[num]
        period = period / num_letters_for_num;

        // Step letter forward each time we hit the period, will be incremented when j = 0
        int which_letter = -1;
        for (int j = 0; j < return_size; j++) {
            if (j % period == 0) {
                which_letter = (which_letter + 1) % num_letters_for_num;
            }
            combos[j][i] = LETTERS[num][which_letter];
        }
    }

    *returnSize = return_size;
    return combos;
}