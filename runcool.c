//  CITS2002 Project 1 2021
//  Name(s):             Jakob Kuriata
//  Student number(s):   23278189

//  compile with:  cc -std=c11 -Wall -Werror -o runcool runcool.c

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//  THE STACK-BASED MACHINE HAS 2^16 (= 65,536) WORDS OF MAIN MEMORY
#define N_MAIN_MEMORY_WORDS (1<<16)

//  EACH WORD OF MEMORY CAN STORE A 16-bit UNSIGNED ADDRESS (0 to 65535)
#define AWORD               uint16_t
//  OR STORE A 16-bit SIGNED INTEGER (-32,768 to 32,767)
#define IWORD               int16_t

//  THE ARRAY OF 65,536 WORDS OF MAIN MEMORY
AWORD                       main_memory[N_MAIN_MEMORY_WORDS];

//  THE SMALL-BUT-FAST CACHE HAS 32 WORDS OF MEMORY
#define N_CACHE_WORDS       32

//  see:  https://teaching.csse.uwa.edu.au/units/CITS2002/projects/coolinstructions.php
enum INSTRUCTION {
    I_HALT       = 0,
    I_NOP,
    I_ADD,
    I_SUB,
    I_MULT,
    I_DIV,
    I_CALL,
    I_RETURN,
    I_JMP,
    I_JEQ,
    I_PRINTI,
    I_PRINTS,
    I_PUSHC,
    I_PUSHA,
    I_PUSHR,
    I_POPA,
    I_POPR
};

//  USE VALUES OF enum INSTRUCTION TO INDEX THE INSTRUCTION_name[] ARRAY
const char *INSTRUCTION_name[] = {
    "halt",
    "nop",
    "add",
    "sub",
    "mult",
    "div",
    "call",
    "return",
    "jmp",
    "jeq",
    "printi",
    "prints",
    "pushc",
    "pusha",
    "pushr",
    "popa",
    "popr"
};

//  ----  IT IS SAFE TO MODIFY ANYTHING BELOW THIS LINE  --------------


//  THE STATISTICS TO BE ACCUMULATED AND REPORTED
int n_main_memory_reads     = 0;
int n_main_memory_writes    = 0;
int n_cache_memory_hits     = 0;
int n_cache_memory_misses   = 0;

void report_statistics(void)
{
    printf("@number-of-main-memory-reads\t%i\n",    n_main_memory_reads);
    printf("@number-of-main-memory-writes\t%i\n",   n_main_memory_writes);
    printf("@number-of-cache-memory-hits\t%i\n",    n_cache_memory_hits);
    printf("@number-of-cache-memory-misses\t%i\n",  n_cache_memory_misses);
}

//  -------------------------------------------------------------------

//  EVEN THOUGH main_memory[] IS AN ARRAY OF WORDS, IT SHOULD NOT BE ACCESSED DIRECTLY.
//  INSTEAD, USE THESE FUNCTIONS read_memory() and write_memory()
//
//  THIS WILL MAKE THINGS EASIER WHEN WHEN EXTENDING THE CODE TO
//  SUPPORT CACHE MEMORY

AWORD read_memory(int address)
{
    //  //count number of memory reads
    ++n_main_memory_reads;  //  each time read_memory is called, this value increments
    return main_memory[address];
}

void write_memory(AWORD address, AWORD value)
{
    //  count number of memory writes
    ++n_main_memory_writes; //  each time write_memory is called, this value increments
    main_memory[address] = value;
}
//  -------------------------------------------------------------------

//  EXECUTE THE INSTRUCTIONS IN main_memory[]
int execute_stackmachine(void)
{
//  THE 3 ON-CPU CONTROL REGISTERS:
    int PC      = 0;                    // 1st instruction is at address=0
    int SP      = N_MAIN_MEMORY_WORDS;  // initialised to top-of-stack
    int FP      = 0;                    // frame pointer

//  REMOVE THE FOLLOWING LINE ONCE YOU ACTUALLY NEED TO USE FP
    FP = FP+0;

    while(true) {

//  FETCH THE NEXT INSTRUCTION TO BE EXECUTED
        IWORD instruction   = read_memory(PC);

        struct main{    //  struct to hold varibles used by main memory
            IWORD value1;   //  holds a signed 16-bit integer value
            IWORD value2;   //  holds a signed 16-bit integer value
            IWORD offset1;  //  holds a signed 16-bit integer value
            IWORD string1;  //  holds a signed 16-bit integer value
            IWORD char1;    //  holds a signed 16-bit integer value
            IWORD char2;    //  holds a signed 16-bit integer value

            AWORD address1; //  holds a unsigned 16-bit address
            AWORD pointer1; //  holds a unsigned 16-bit address

        }main;

        ++PC;

        //printf("%s\n", INSTRUCTION_name[instruction]);

        if(instruction == I_HALT) {
            break;
        } 
//  SUPPORT OTHER INSTRUCTIONS HERE
//      ....
        //  switch case statement for each instruction of the CPU
        switch(instruction){
            //  no current instruction so move on to the next instruction
            case I_NOP:
            break;
            //  adds two instructions together
            case I_ADD:
            main.value1 = read_memory(SP);   ++SP;   //  set value1 to the current value on top of the stack, then remove it from the stack 
            main.value2 = read_memory(SP);   //  set value2 to the current value on top of the stack
            write_memory(SP, main.value1 + main.value2);  //  write the sum of value1 and value2 to the value of the stack pointer
            break;
            //  subtracts two instructions together
            case I_SUB:
            main.value1 = read_memory(SP);   ++SP;   //  set value1 to the current value on top of the stack, then remove it from the stack
            main.value2 = read_memory(SP);   //  set value2 to the current value on top of the stack
            write_memory(SP, main.value2 - main.value1);  //  write the value of value2 - value1 to the value of the stack pointer
            break;
            //  multiplies two instructions together
            case I_MULT:
            main.value1 = read_memory(SP);   ++SP;   //  set value1 to the current value on top of the stack, then remove it from the stack
            main.value2 = read_memory(SP);   //  set value2 to the current value on top of the stack
            write_memory(SP, main.value1 * main.value2);  //  write the value of the multiplication of value1 and value2 to the value of the stack pointer
            break;
            //  divides two instructions together
            case I_DIV:
            main.value1 = read_memory(SP);   ++SP;   //  set value1 to the current value on top the stack, then remove it from the stack
            main.value2 = read_memory(SP);   //  set value2 to the current value on top of the stack
            write_memory(SP, main.value2 / main.value1);  //  write the value of value2 divided by value1 to the value of the stack pointer
            break;
            //  calls the next instruction to be executed
            case I_CALL:
            main.address1 = read_memory(PC); PC++;   //  read the program counter and copy the address to address1 then move the program counter
            SP--;   write_memory(SP, PC);         
            SP--;   write_memory(SP, FP);
            FP = SP;    //  copy the value on TOS to the frame pointer
            PC = main.address1;
            break;
            //  returns the integer value from the top of the stack
            case I_RETURN:
            main.offset1 = read_memory(PC);  ++PC;   //  read the program counter and copy the address to offset1, then move up the program counter
            main.value1 = read_memory(SP); ++SP;     //  read the stack pointer and copy the address to value1, then move up the stack counter
            main.address1 = FP + main.offset1;
            SP = FP;
            FP = read_memory(SP);   ++SP;
            PC = read_memory(SP);   ++SP;
            SP = main.address1;
            write_memory(main.address1, main.value1);     //  write the value of value1 to the return address
            break;
            //  flow of execution jumps to the address following this instruction
            case I_JMP:
            PC = read_memory(PC);
            break;
            //  the value on TOS is popped of the stack, if the value is zero, the flow of exection continues from the previous instruction
            case I_JEQ:
            main.value1 = read_memory(SP);   ++SP;  //  set value1 to the current value on top of the stack, then remove it from the stack
            if(main.value1 == 0){
                PC = read_memory(PC);
            } else{
                ++PC;
            }
            break;
            //  the value on TOS is popped of the stack, and is printed to stdout
            case I_PRINTI:
            main.value1 = read_memory(SP);   ++SP;  //  set value1 to the current value on top of the stack, then remove it from the stack
            fprintf(stdout, "%i", main.value1);
            break;
            //  the character string following this instruction is printed to stdout
            case I_PRINTS:
            main.pointer1 = read_memory(PC); ++PC;  //  set a pointer to the next instruction in the stack, then move it to the next
            main.string1 = read_memory(main.pointer1);  //  set a string to the word following the prints instruction
            main.char1 = main.string1 % 256;  main.char2 = main.string1 / 256;  //  characters are read in 2 bytes

            while(true){    //  a while loop that runs continuously until it runs into a break statement 
                if(main.char1 == 0){
                    break;
                } else{
                   fprintf(stdout, "%c", main.char1);   //  prints out the first character to stdout
                }
                
                if(main.char2 == 0){
                    break;
                }else{
                   fprintf(stdout, "%c", main.char2);   //  prints out the second character to stdout
                }
                main.pointer1++;
                main.string1 = read_memory(main.pointer1);
                main.char1 = main.string1 % 256;  main.char2 = main.string1 / 256;
            }

            break;
            //  pushes a integer constant following this instruction onto the stack
            case I_PUSHC:
            main.value1 = read_memory(PC); ++PC;    //  set value to the next instruction in the program counter, then move up the program counter
            SP--;   write_memory(SP, main.value1);  //  pushes/writes value to stack pointer
            break;
            //  pushes the word following this instruction onto the stack
            case I_PUSHA:
            main.address1 = read_memory(PC); ++PC;  //  set address to the next instruction in the program counter, then move up the program counter
            main.value1 = read_memory(main.address1); 
            SP--;   write_memory(SP, main.value1); //  pushes/writes value to stack pointer
            break;
            //  pushes the word following this instruction onto the stack
            case I_PUSHR:
            main.offset1 = read_memory(PC);  ++PC;  //  set offset to the next instruction in the program counter, then move up the program counter
            main.address1 = FP + main.offset1; 
            main.value1 = read_memory(main.address1);
            SP--;   write_memory(SP, main.value1);  //  pushes/writes value to stack pointer
            break;
            //  pops the word following this instruction from the stack
            case I_POPA:
            main.value1 = read_memory(SP); ++SP;    //  set value1 to the current value on top of the stack, then remove it from the stack
            main.address1 = read_memory(PC); ++PC;
            write_memory(main.address1, main.value1);   //  writes value to address
            break;
            //  pops the word following this instruction from the stack
            case I_POPR:
            main.value1 = read_memory(SP);   ++SP;  //  set value to the instruction on TOS
            main.offset1 = read_memory(PC); ++PC;   //  set offset to the next instruction in the program counter, then move up the program counter
            write_memory(FP + main.offset1, main.value1); //  offset is added to FP in order to prove the address which the value TOS should be popped
            break;
        }
    }

//  THE RESULT OF EXECUTING THE INSTRUCTIONS IS FOUND ON THE TOP-OF-STACK
    //printf("result = %i\n", read_memory(SP));

    return read_memory(SP);
}

//  -------------------------------------------------------------------

//  READ THE PROVIDED coolexe FILE INTO main_memory[]
void read_coolexe_file(char filename[])
{
    memset(main_memory, 0, sizeof main_memory);   //  clear all memory
//  READ CONTENTS OF coolexe FILE   
    //  attempt to open the file
    char buffer[N_MAIN_MEMORY_WORDS];
    FILE *fp = fopen(filename, "rb");   //  rb = read in binary 
    //  checks if anything is wrong with the file
    if(fp == NULL){  
        printf("cannot open '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    fread(main_memory, 2, sizeof buffer, fp); //    reads the file, 2 bytes at a time
    fclose(fp);     //  closes file
}

//  -------------------------------------------------------------------

int main(int argc, char *argv[])
{
//  CHECK THE NUMBER OF ARGUMENTS
    if(argc != 2) {
        fprintf(stderr, "Usage: %s program.coolexe\n", argv[0]);
        exit(EXIT_FAILURE);
    }

//  READ THE PROVIDED coolexe FILE INTO THE EMULATED MEMORY
    read_coolexe_file(argv[1]);

//  EXECUTE THE INSTRUCTIONS FOUND IN main_memory[]
    int result = execute_stackmachine();

    report_statistics();

    return result;          // or  exit(result);
}