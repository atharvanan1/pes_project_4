# Principles of Embedded Software Project 4

### Readme

---

#### Name - Atharva Nandanwar

#### Folder Structure:

Source - contains all *.c and *.h files

1. main.c/h - main file which contains main subroutine

2. i2c - has i2c subroutines and tmp102 specific functions/macros

3. led_control - has led specific subroutines

4. logger - has logger and errno files which are used as system wide logger

5. POST - has Power On Self Test subroutines

6. state_machine - has state machine related implementations

7. test - contains a test suite to test various test cases

8. uctest - contains source files for unit testing

9. Leveraged_Code_Documentation - has list of links that I referred to while designing my code

#### Compilation instructions:

There are three targets

1. Debug - compiles the code with detailed logging

2. Run - compiles the code with lesser logging than debug

3. Test - compiles the test suite
* Clean the target

* Press on one of the build options

* Debug the code



#### New Observations:

* To make system wide logger, I borrowed the idea from one of the logger projects. Hence, to include some variable or data structure from other files into all the files, put it as extern in header file. In this project, I have used the logger as an object which has it's member functions as methods, which is really cool, I would say.

* For state machine implementation, I tried to make my code really modular. I had to look at many different state machine implementations where I could see how they tried to execute the actions and where they did it. Alternatively, I wanted to give full control to my state machines, however that would have proved to be really complicated and hard to understand by others.

* I found the idea of making colorful logging really innovative in this project.



#### Problems Faced:

* Major problem I faced during this project was getting my I2C to run. I think that I2C is fairly complicated protocol which needs ACK, START, and REPEATED START requirements which are stringent.

* Designing a common state machine implementation where I can use state-driven and table-driven through a common interface.

* Sharing resources between two functions would have been better. In Event Handler functions, I haven't abstracted away
the subroutines because I was bothered about passing a lot of values to subroutine functions

Feel free to give detailed feedback on the code. ^.^


