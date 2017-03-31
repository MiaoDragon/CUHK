#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "manager.h"

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

/* Test Suite setup and cleanup functions: */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void test_case1() {
    uint32_t page_num = 8, frame_num = 4, frame_size = 8;
    uint32_t lru_parameter = 3; 
    uint32_t test_len = 19;
    uint32_t test_set[] = {0,1,2,9,10,24,24,27,28,33,18,19,50,19,9,40,40,40,52}; 

    FILE *fp;
    char buff[255];
    fp = fopen("./testcase_ans/ans1", "r");
    if(fp == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager1 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff, 255, (FILE*)fp) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager1, test_set[i]), (uint32_t)atoi(buff));
    }
    
    deconstruct_manager(manager1);
    fclose(fp);
}

void test_case2() {
    uint32_t page_num = 256, frame_num = 128, frame_size = 1024;
    uint32_t lru_parameter = 1; 
    uint32_t test_len = 5;
    uint32_t test_set[] = {16360,32790,65540,131080,262140}; 

    FILE *fp;
    char buff[255];
    fp = fopen("./testcase_ans/ans2", "r");
    if(fp == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager2 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff, 255, (FILE*)fp) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager2, test_set[i]), (uint32_t)atoi(buff));
    }
    
    deconstruct_manager(manager2);
    fclose(fp);

}


void test_case3() {
    uint32_t page_num = 8, frame_num = 4, frame_size = 4;
    uint32_t lru_parameter = 2; 
    uint32_t test_len = 15;
    uint32_t test_set[] = {1,2,5,8,13,6,21,4,24,28,12,12,24,9,5};
   
    FILE *fp;
    char buff[255];
    fp = fopen("./testcase_ans/ans3", "r");
    if(fp == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager3 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff, 255, (FILE*)fp) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager3, test_set[i]), (uint32_t)atoi(buff));
    }
    
    deconstruct_manager(manager3);
    fclose(fp);
}

void test_case4() {
    uint32_t page_num = 16, frame_num = 8, frame_size = 16;
    uint32_t lru_parameter = 1;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case4", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans4", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager4 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager4, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager4);
    fclose(fp_q);
    fclose(fp_a);
}


void test_case5() {
    uint32_t page_num = 16, frame_num = 8, frame_size = 32;
    uint32_t lru_parameter = 2;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case5", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans5", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager5 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager5, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager5);
    fclose(fp_q);
    fclose(fp_a);
}

void test_case6() {
    uint32_t page_num = 32, frame_num = 32, frame_size = 32;
    uint32_t lru_parameter = 2;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case6", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans6", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager6 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager6, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager6);
    fclose(fp_q);
    fclose(fp_a);
}

void test_case7() {
    uint32_t page_num = 128, frame_num = 32, frame_size = 32;
    uint32_t lru_parameter = 2;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case7", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans7", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager7 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < 50; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        access(manager7, (uint32_t)atoi(buff_q));
    }
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager7, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager7);
    fclose(fp_q);
    fclose(fp_a);
}

void test_case8() {
    uint32_t page_num = 128, frame_num = 64, frame_size = 32;
    uint32_t lru_parameter = 3;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case8", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans8", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager8 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < 100; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        access(manager8, (uint32_t)atoi(buff_q));
    }
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager8, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager8);
    fclose(fp_q);
    fclose(fp_a);
}


void test_case9() {
    uint32_t page_num = 128, frame_num = 64, frame_size = 32;
    uint32_t lru_parameter = 3;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case9", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans9", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager9 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < 300; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        access(manager9, (uint32_t)atoi(buff_q));
    }
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager9, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager9);
    fclose(fp_q);
    fclose(fp_a);
}


void test_case10() {
    uint32_t page_num = 512, frame_num = 128, frame_size = 128;
    uint32_t lru_parameter = 4;
    uint32_t test_len = 20;
   
    FILE *fp_q;
    char buff_q[255];
    fp_q = fopen("./testcase/case10", "r");
    if(fp_q == NULL) {
        perror("Error opening file");
        return;
    }
    FILE *fp_a;
    char buff_a[255];
    fp_a = fopen("./testcase_ans/ans10", "r");
    if(fp_a == NULL) {
        perror("Error opening file");
        return;
    }

    manager_t* manager10 = new_memory_manager(page_num, frame_num, frame_size, lru_parameter);
    for (uint32_t i = 0; i < 300; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        access(manager10, (uint32_t)atoi(buff_q));
    }
    for (uint32_t i = 0; i < test_len; i++) {
        if (fgets(buff_q, 255, (FILE*)fp_q) == NULL) {
            perror("Error in fgets()");
            return;
        }
        if (fgets(buff_a, 255, (FILE*)fp_a) == NULL) {
            perror("Error in fgets()");
            return;
        }
        CU_ASSERT_EQUAL(access(manager10, (uint32_t)atoi(buff_q)), (uint32_t)atoi(buff_a));
    }
    
    deconstruct_manager(manager10);
    fclose(fp_q);
    fclose(fp_a);
}


int main(int argc, char *argv[]) {
    // Usage of running this program
    if (argc != 1) {
        fprintf(stderr, "usage: tester\n");
        exit(1);
    }

    CU_pSuite pSuite = NULL;

    // initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }
    // add a suite to the registry
    pSuite = CU_add_suite( "manager_test_suite", init_suite, clean_suite );
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    // add the tests to the suite
    if ( (NULL == CU_add_test(pSuite, "test_case1", test_case1)) ||
         (NULL == CU_add_test(pSuite, "test_case2", test_case2)) ||
         (NULL == CU_add_test(pSuite, "test_case3", test_case3)) ||
         (NULL == CU_add_test(pSuite, "test_case4", test_case4)) ||
         (NULL == CU_add_test(pSuite, "test_case5", test_case5)) ||
         (NULL == CU_add_test(pSuite, "test_case6", test_case6)) ||
         (NULL == CU_add_test(pSuite, "test_case7", test_case7)) ||
         (NULL == CU_add_test(pSuite, "test_case8", test_case8)) ||
         (NULL == CU_add_test(pSuite, "test_case9", test_case9)) ||
         (NULL == CU_add_test(pSuite, "test_case10", test_case10)) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* test_case10(); */

    // run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_SILENT);
    CU_basic_run_tests();
    printf("\n");
    
    
    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
}
