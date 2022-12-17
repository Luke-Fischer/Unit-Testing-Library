#include "A1Header.h"
#include <stdlib.h>
#include <string.h>

bool comIntPtrs(void* arg1, void* arg2){
    int *n1 = (int*)arg1;
    int *n2 = (int*)arg2;
    if (*n1 == *n2){
        return true;
    }else{
        return false;
    }
}



void* add1int(void *arg){
    int* num = arg;
    *num = *num + 1;
    /*while(1==1){

    }*/
    return arg;

}

void * divByZero(void *arg){
    raise(SIGFPE);
    return arg;
}

void * segFault(void *arg){
    raise(SIGSEGV);
    return arg;
}

void * IllegalInstruction(void *arg){
    raise(SIGILL);
    
    return arg;
}

int main(void){
    TestArgs test;
    test.timing = true;
    test.logging = true;
    //test.logFileName = "test.txt";

    initUnitTests(&test);
    int expRes = 3;

    /**array sum test*/
    int testArr[5] = {1,2,3,4,5};

    // 1 - Fails - termination from segfault
    runTest(&expRes, segFault, &testArr, comIntPtrs);

    //2 - Fails -> returns incorrect value
    int testArg = 0;
    runTest(&expRes, add1int, &testArg, comIntPtrs);

    //3 - Passes
    testArg = 2;
    runTest(&expRes, add1int, &testArg, comIntPtrs);

    //4 - Passes
    testArg = 10;
    expRes = 11;
    runTest(&expRes, add1int, &testArg, comIntPtrs);

    //Fails - illegal expression
    runTest(&expRes, IllegalInstruction, &testArg, comIntPtrs);

    // 6 Fails divide by 0
    runTest(&expRes, divByZero, &testArg, comIntPtrs);     
    
    //7 Passes
    testArg = 4;
    expRes = 5;
    runTest(&expRes, add1int, &testArg, comIntPtrs);

    cleanupTests();

    return 0;

}