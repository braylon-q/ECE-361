#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../tally.h"

static int fails = 0;
static int passes = 0;

static int test_output_is_alphabetical(const tally_t *t) {
    /*
    This function is entirely written by chatgpt-6.
    */
    if (t == NULL) return 0;

    FILE *output = tmpfile();
    if (output == NULL) {
        return 0;
    }

    tally_print(t, output);
    rewind(output);

    char line[TALLY_LEN + 32];
    char previous[TALLY_LEN + 1] = "";
    int has_previous = 0;
    int alphabetical = 1;

    while (fgets(line, sizeof line, output) != NULL) {
        char *separator = strstr(line, ": ");
        if (separator == NULL) {
            alphabetical = 0;
            break;
        }

        size_t name_length = (size_t)(separator - line);
        if (name_length == 0 || name_length > TALLY_LEN) {
            alphabetical = 0;
            break;
        }

        *separator = '\0';
        if (has_previous && strcmp(previous, line) > 0) {
            alphabetical = 0;
            break;
        }

        memcpy(previous, line, name_length + 1);
        has_previous = 1;
    }

    if (ferror(output)) {
        alphabetical = 0;
    }
    fclose(output);
    return alphabetical;
}

#define CHECK(cond) do{ \
    if (cond) { \
        printf("PASS: %s\n", #cond); \
        passes++; \
    } \
    else { \
        printf("FAIL: %s\n", #cond); \
        fails++; \
    } \
} while(0)

int main(void) {
    /*
    TESTS FOR tally.c:
    counting up; a name never added; an
    empty name; a name of exactly TALLY_LEN characters and one of TALLY_LEN + 1; NULL
    arguments; a full tally (a new name is refused, an existing one still counts); 
    TALLY_MAX tallies, then one more (NULL); create after destroy; 
    two tallies are independent; the print order
    */

    //Incrementing name count:
    tally_t *test1 = tally_create();
    if (test1 == NULL) return -1; //protect against unwanted memory access
    char name1[] = "Jane Doe";
    int err_chk = tally_add(test1, name1);
    CHECK(tally_count(test1, name1) == 1);
    err_chk = tally_add(test1, name1);
    CHECK(tally_count(test1, name1) == 2);
    if (err_chk == -1) return -1;
    //A name never added
    CHECK(tally_count(test1, "noname") == -1);

    //Empty name
    CHECK(tally_add(test1, "") == -1);

    //Name of TALLY_LEN characters
    char name2[TALLY_LEN+1] = "123456789012345";
    CHECK(tally_add(test1, name2) == 1 && tally_count(test1, name2) == 1); 
    /*
    Above is correct. tally_add inserts a NULL (\0) char at the 15th index of the
    char array, which is after the final 5 of name2.
    */

    //Name of TALLY_LEN+1 characters
    char name3[TALLY_LEN+2] = "1234567890123456";
    CHECK(tally_add(test1, name3) == -1);

    //A full tally
    char test_arr1[TALLY_NAMES][TALLY_LEN+1] = {"p", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "a"};
    //^array of 16 names
    tally_t *test2 = tally_create();
    if (test2 == NULL) return -1; //protect against unwanted memory access
    for (int i= 0; i < TALLY_NAMES; i++) {
        tally_add(test2, test_arr1[i]);
    }
    CHECK(tally_add(test2, "q") == -1 && tally_count(test2, "a") == 1);

    //Max number of tally structures
    tally_t *test3 = tally_create();
    tally_t *test4 = tally_create();
    CHECK(tally_create() == NULL);

    //Create after destroy
    tally_destroy(&test3);
    tally_t *test5 = tally_create();
    CHECK(test5 != NULL);

    //Destroyed tally is un-accessible
    CHECK(test3==NULL);

    //NULL arguments for all functions
    CHECK(tally_add(NULL, NULL) == -1);
    CHECK(tally_count(NULL, NULL) == -1);
    CHECK(tally_size(NULL) == -1);
    tally_destroy(NULL);
    CHECK(test5 != NULL && test4 != NULL && test2 != NULL && test1 != NULL);
    CHECK(!test_output_is_alphabetical(NULL));
    
    //alphabetical output of array
    CHECK(test_output_is_alphabetical(test2));

    //SUMMARY:
    printf("\n--SUMMARY--\nPasses: %d\nFails: %d\n Total: %d\n\n", 
          passes, fails, passes + fails);

    //Destroy all allocated memory
    tally_destroy(&test1);
    tally_destroy(&test2);
    tally_destroy(&test4);
    tally_destroy(&test5);

    

    return 0;
}