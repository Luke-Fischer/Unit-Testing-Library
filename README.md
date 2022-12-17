# Unit-Testing-Library
Using processes and IPC to create a robust unit test library 

# Usage
compile: make

link: gcc SampleMain.o exec -lutest -L{pwd}

export path: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

Execute: ./exec

# What makes it robust?

 - Each unit test is handled by its own process (prevents the entire test suite to crash when one process terminates abnormally) and the following is captured:
    - termination status of the test case (i.e did it terminate abnormally?)
    - If it terminated abnormally (i.e segfault) this information is caught and displayed to stdout   
    - test ID (starting from 0)
    - test result (i.e pass or fail)
 - Test that hang
    - a custom CTRL-C signal handler has been implemeneted
       - Asks to confirm that you mean to press CTRL-C
       - if so, it kills that processes and moves to the next


# More Info
The user may send in a "TestArgs" structure to the "initUnitTests" function to enable some custom features
  - timing: toggles how long a unit test takes to complete
  - logging: toggles whether the unit test output is displayed to stdout or to a file (specified by logFileName)

