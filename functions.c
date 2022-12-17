#include "A1Header.h"
#include <stdlib.h>
#include <signal.h>
#include <string.h>


int timing;
int logging;
FILE*fp;
int doneWaiting;
static int test_id;
struct timeval	start;
double			elapsed;

void initUnitTests(TestArgs* arg){
    test_id = 0;
    timing = 0;
    logging = 0;
    doneWaiting = 1;
    if(arg == NULL){
        timing = 0;
        logging = 0;
    }

    else{
        int temp1 = arg -> logging;
        if(temp1 == 1){
            logging = 1;
            //Open file named default.txt
            if(arg -> logFileName == NULL){
                fp = fopen("defaultLog.txt", "w");
            }
            //open file
            else{
                fp = fopen(arg -> logFileName, "w");
            }
        }
        int temp = arg -> timing;
        if(temp == 1){
            timing = 1;
        }
    }
    //Initialize signal handling
    if (signal(SIGINT, sigUsr) == SIG_ERR){
        while(1){ 
            sleep(1);
        }
    }
}

void runTest(void *refValue, testCase *testFunc, void *testArg, equalComp *compFunc){
    int status = 0;
    test_id++;
	int pid = fork();
	
    //child
	if (pid == 0){
        if(logging == 0){
            printf("\n");
        }
        if(logging == 1){
            fprintf(fp, "\n");
            fflush(fp);
        }
		//Grab return value of function we are testing
        
        //Begin test timer
        gettimeofday(&start, NULL);

        void* resVal = testFunc(testArg);

		//Get the child's PID - the child has no other way of obtaining it
        if(logging == 0){
            printf("Test ID: %d ", test_id);
        }
        if(logging == 1){
            fprintf(fp,"Test ID: %d ", test_id);
        }
        if (compFunc(refValue, resVal)){
            if(logging == 0){
                printf("PASS\n");
            }
            if(logging == 1){
                fprintf(fp, "PASS\n");
            }
            if(timing == 1 && logging == 0){
                elapsed = calcTime(start);
                printf("Test took %.6f seconds to complete\n", elapsed);
            }
            if(timing == 1 && logging == 1){
                elapsed = calcTime(start);
                fprintf(fp,"Test took %.6f seconds to complete\n", elapsed);
            }
            exit(0);
        }
        else{
            if(logging == 0){
                printf( "FAIL\n");
            }
            if(logging == 1){
                fprintf(fp, "FAIL\n");
            }
            if(timing == 1 && logging == 0){
                elapsed = calcTime(start);
                printf("Test took %.6f seconds to complete\n", elapsed);
            }
            if(timing == 1 && logging == 1){
                elapsed = calcTime(start);
                fprintf(fp,"Test took %.6f seconds to complete\n", elapsed);
            }
            exit(0);
        }
        //Terminated abnormally
        exit(1);	 
	}
    else{
        if ((pid = wait(&status)) == -1)
            perror("wait() error");
        else {
            if (WIFEXITED(status)) {
            /* The integer status variable is a bit unusual.  It’s a 32-bit int, where different bits have different meaning:
                - The upper 24 bits are used to store several pieces of information about how the process terminated
                - Its lower 8 bits store the returned status - i.e. the value from the child's return or exit() statement
               Therefore we have to use the WEXITSTATUS macro to unscramble the status variable and pull out the its that give us the status
            */
            
            //printf("Child %d exited with status %d\n",wpid, WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status)){
                gettimeofday(&start, NULL);
                char* exitSignalStr = strsignal(WTERMSIG(status));
                if(logging == 0){
                    printf("Test ID: %d ", test_id);
                }
                if(logging == 1){
                    fprintf(fp,"Test ID: %d ", test_id);
                }
                if(logging == 0){
                    printf( "FAIL\n");
                }
                if(logging == 1){
                    fprintf(fp, "FAIL\n");
                }
                if(timing == 0 && logging == 0){
                    elapsed = calcTime(start);
                    printf("Test was terminated by a (%s).\n", exitSignalStr);
                }
                if(timing == 1 && logging == 0){
                    elapsed = calcTime(start);
                    printf("Test took %.6f seconds to complete\n", elapsed);
                    printf("Test was terminated by a (%s).\n", exitSignalStr);
                }
                if(timing == 1 && logging == 1){
                    elapsed = calcTime(start);
                    fprintf(fp,"Test took %.6f seconds to complete\n", elapsed);
                    fprintf(fp, "Test was terminated by a (%s).\n", exitSignalStr);
                }
                if(timing == 0 && logging == 1){
                    elapsed = calcTime(start);
                    fprintf(fp, "Test was terminated by a (%s).\n", exitSignalStr);
                }
            }
            fflush(fp);
        }
    }
}

void cleanupTests(void){
    if(logging == 1){
        fclose(fp);
    }
}

double calcTime(struct timeval start){
    
    long long		startusec, endusec;
    struct timeval	end;
    
    gettimeofday(&end, NULL);
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    return (double)(endusec - startusec) / 1000000.0;
}

void sigUsr(int sigNum) {
    if (signal(SIGINT, sigUsr) == SIG_ERR){

    }
    if(sigNum == SIGINT){
        char response;
        printf("\nDo you wish to terminate the test suite: (y/n): ");
        while(1){
            scanf("%c", &response);
            if(response == 'y' || response == 'n'){
                break;
            }
            else{
                printf("\nDo you wish to terminate the test suite: (y/n): ");
            }
        }
        if(response == 'y'){
            printf("Terminating test suite...\n");
            if(logging == 1 && timing == 1){
                elapsed = calcTime(start);
                fprintf(fp, "Test ID %d was terminated by a SIGINT after running for %.6f seconds\n", test_id, elapsed);
            }
            else if(logging == 0 && timing == 0){
                printf("Test ID %d was terminated by a SIGINT", test_id);
            }
            else if(logging == 0 && timing == 1){
                elapsed = calcTime(start);
                printf("Test ID %d was terminated by a SIGINT after running for %.6f seconds\n", test_id, elapsed);
            }
            else if(logging == 1 && timing == 0){
                fprintf(fp, "Test ID %d was terminated by a SIGINT", test_id);
            }
            cleanupTests();
            kill(0, SIGKILL);
        }
    }
}