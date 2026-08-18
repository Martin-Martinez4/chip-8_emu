#include "test_opcodes_0_to_7.h"
#include "test_helpers.h"
#include "opcodes.h"

void testCPUStatesAfterInstruction(testCases* testCases){
      for(int i = 0; i < testCases->cases_count; i++){
        testCase tc = testCases->test_cases[i];

        LOG_TEST(tc.test_name);
        
        for(int t = 0; t < tc.step_count; t++){
            testStep step = tc.steps[t];
            
            
            CPU* result_CPU_state = &(tc.starting_CPU_state);

            

            executeInstruction(result_CPU_state, step.instruction);
            

            CPU* expected_CPU_state = &(tc.steps[t].expected_CPU_state);
            ASSERT_CPU_STATE_EQUAL(result_CPU_state, expected_CPU_state);
        }
    }
}

// todo:
// test void cls(CPU *cpu, uint16_t instruction)
void opcode_0_tests(){
    LOG_TEST("Running opcode 0 tests...");

    testCases tcs = {
        .cases_count = 4,
        .test_cases = (struct testCase[]){

            // opcode 0 ret
            {
                .test_name = "ret opcode test - one thing on stack",
                .starting_CPU_state = {
                    .stack_pointer = 0,
                    .stack = {0x2000}
                },
                .step_count = 1,
                .steps = (testStep[]){

                    { 
                        .expected_CPU_state = {

                            .stack_pointer = -1,
                            .stack = {0x2000},
                            .program_counter = 0x2000
                        },
                        .instruction = 0x00EE 
                
                    }
                }

            },

            {
            .test_name = "ret opcode test - multiple stack entries",
            .starting_CPU_state = {
                .stack = {0x0010, 0x0020, 0x3000},
                .stack_pointer = 2,

            },
            .step_count = 1,
            .steps = (testStep[]){
                {
                    .expected_CPU_state = {
                        .stack = {0x0010, 0x0020, 0x3000},
                        .stack_pointer = 1,
                        .program_counter = 0x3000

                    },
                    .instruction = 0x00EE 
                }
            }

            },
            {
            .test_name = "ret opcode test - return from nested subroutine",
                .starting_CPU_state = {
                .stack = {0x0010, 0x0020, 0x3000},
                .stack_pointer = 2,

            },
            .step_count = 3,
            .steps = (testStep[]){
                {
                    .expected_CPU_state = {
                        .stack = {0x0010, 0x0020, 0x3000},
                        .stack_pointer = 1,
                        .program_counter = 0x3000


                    },
                    .instruction = 0x00EE
                },
                {
                    .expected_CPU_state = {
                        .stack = {0x0010, 0x0020, 0x3000},
                        .stack_pointer = 0,
                        .program_counter = 0x0020
                    },
                    .instruction = 0x00EE
                },  
                {
                    .expected_CPU_state = {
                        .stack = {0x0010, 0x0020, 0x3000},
                        .stack_pointer = -1,
                        .program_counter = 0x0010
                    },
                    .instruction = 0x00EE
                }, 
            }
            },
            {
            .test_name = "ret opcode test - empty stack",
            .starting_CPU_state = {
                .stack = {0x010, 0x0020, 0x3000},
                .stack_pointer = -1,

            },
            .step_count = 1,
            .steps = (testStep[]){
                {
                    .expected_CPU_state = {
                        .stack = {0x010, 0x0020, 0x3000},
                        .stack_pointer = -1,

                    },
                    .instruction = 0x00EE 

                }
            }
            },
        }
    };

  testCPUStatesAfterInstruction(&tcs);
}

// Jump Address 1nnn
void opcode_1_test(){
    LOG_TEST("Running opcode 1 tests...");
    testCases tcs = {
        .cases_count = 1,
        .test_cases = (struct testCase[]){
            {
                .test_name = "Normal jump to address",
                .starting_CPU_state = {
                    .program_counter = 0,
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .program_counter = 0x0222
                        },
                        .instruction = 0x1222
                    }

                }

            },
        }
    };

    testCPUStatesAfterInstruction(&tcs);


}

// Call Address 2nnn
void opcode_2_test(){
    LOG_TEST("Running opcode 2 tests...");
    testCases tcs = {
        .cases_count = 5,
        .test_cases = (struct testCase[]){
            {
                .test_name = "callAddr opcode test - No item in stack",
                .step_count = 1,
                .starting_CPU_state = {
                    .stack_pointer = -1,
                    .stack = {0},
                    .program_counter = 0x2188
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 0,
                            .stack = {0x2188},
                            .program_counter = 0x0FED,

                        },
                        .instruction = 0x2FED
                    }
                }
            },
            {
                .test_name = "callAddr opcode test - One item in stack",
                .step_count = 1,
                .starting_CPU_state = {
                    .stack_pointer = 0,
                    .stack = {0x2000},
                    .program_counter = 0x0188
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 1,
                            .stack = {0x2000, 0x0188},
                            .program_counter = 0x0151,
                        },
                        .instruction = 0x2151 
                    }
                }
            },
            {
                .test_name = "callAddr opcode test - Many items in stack",
                .step_count = 1,
                .starting_CPU_state = {
                    .stack_pointer = 10,
                    .stack = {0x1000, 0x2000, 0x3000, 0x0400, 0x5500, 0x0606, 0x7070, 0x8080, 0x9000, 0xAAAA, 0x2BBB},
                    .program_counter = 0xBEEF
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 11,
                            .stack = {0x1000, 0x2000, 0x3000, 0x0400, 0x5500, 0x0606, 0x7070, 0x8080, 0x9000, 0xAAAA, 0x2BBB, 0xBEEF},
                            .program_counter = 0x0EEF
                        },
                        .instruction = 0x2EEF
                    }
                }
            },
            {
                .test_name = "callAddr opcode test - Stack with data past stack pointer",
                .step_count = 1,
                .starting_CPU_state = {
                    .stack_pointer = 2,
                    .stack = {0x1000, 0x2000, 0x3000, 0x0400, 0x5500, 0x0606, 0x0FFF},
                    .program_counter = 0xBEEF
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 3,
                             .stack = {0x1000, 0x2000, 0x3000, 0xBEEF, 0x5500, 0x0606, 0xBEEF},
                             .program_counter = 0x0FFF
                        },    
                        .instruction = 0x2FFF
                    }
                }
            },
            {
                .test_name = "callAddr opcode test - Many calls to ret",
                .step_count = 4,
                .starting_CPU_state = {
                    .stack_pointer = 2,
                    .stack = {0x1000, 0x2000, 0x3000},
                    .program_counter = 0x0000
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 3,
                            .stack = {0x1000, 0x2000, 0x3000, 0x0000},
                            .program_counter = 0x02AB
                        },
                        .instruction = 0x22AB
                    },
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 4,
                            .stack = {0x1000, 0x2000, 0x3000, 0x0000, 0x02AB},
                            .program_counter = 0x0345

                        },
                        .instruction = 0x2345
                    },
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 5,
                            .stack = {0x1000, 0x2000, 0x3000, 0x0000, 0x02AB, 0x0345},
                            .program_counter = 0xFFF
                        },
                        .instruction = 0x2FFF
                    },
                    {
                        .expected_CPU_state = {
                            .stack_pointer = 6,
                            .stack = {0x1000, 0x2000, 0x3000, 0x0000, 0x02AB, 0x0345, 0x0FFF},
                            .program_counter = 0x0ABC
                        },
                        .instruction = 0x2ABC
                    }
                }
            }
        }
    };

    testCPUStatesAfterInstruction(&tcs);
}

// test for opcode 3xkk
// x is the register number
// kk is the value
// skip if *Vx == kk
void opcode_3_test(){
    // pc starts at 0x0200
    LOG_TEST("Running opcode 3 tests...");
    testCases tcs = {
        .cases_count = 5,

        .test_cases = (struct testCase[]){
            {
                .test_name = "skip if register value equal value opcode test - register value == value",
                .starting_CPU_state = {
                    .registers = {0,0,0, 0x00FF},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0, 0x0FF},
                            .program_counter = 0x0302,
                        },
                        .instruction = 0x33FF
                    }
                }

            },
            {
                .test_name = "skip if register value equal value opcode test - register value == value register B",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                            .program_counter = 0x0302,
                        },
                        .instruction = 0x3BFF
                    }
                }

            },
            {
                .test_name = "skip if register value equal value opcode test - register value == value program counter 0x03AA",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                    .program_counter = 0x03AA,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                            .program_counter = 0x03AC,
                        },
                        .instruction = 0x3BFF
                    }
                }

            },
            {
                .test_name = "skip if register value equal value opcode test - register value != value program counter 0x03AA",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                    .program_counter = 0x03AA,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                            .program_counter = 0x03AA,
                        },
                        .instruction = 0x3BFF
                    }
                }

            },
            {
                .test_name = "skip if register value equal value opcode test - register value != value program counter 0x03AA",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                    .program_counter = 0x03AA,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                            .program_counter = 0x03AA,
                        },
                        .instruction = 0x3BFF
                    }
                }

            },
        }
    };

    testCPUStatesAfterInstruction(&tcs);
}

// test for opcode 4xkk
// x is the register number
// kk is the value
// skip if *Vx != kk
void opcode_4_test(){
    // pc starts at 0x0200
    LOG_TEST("Running opcode 4 tests...");
    testCases tcs = {
        .cases_count = 5,

        .test_cases = (struct testCase[]){
            {
                .test_name = "skip if register value NOT equal value opcode test - register value == value",
                .starting_CPU_state = {
                    .registers = {0,0,0, 0x00FF},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0, 0x0FF},
                            .program_counter = 0x0300,
                        },
                        .instruction = 0x43FF
                    }
                }

            },
            {
                .test_name = "skip if register value NOT equal value opcode test - register value == value register B",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                            .program_counter = 0x0300,
                        },
                        .instruction = 0x4BFF
                    }
                }

            },
            {
                .test_name = "skip if register value NOT equal value opcode test - register value == value program counter 0x03AA",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                    .program_counter = 0x03AA,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00FF},
                            .program_counter = 0x03AA,
                        },
                        .instruction = 0x4BFF
                    }
                }

            },
            {
                .test_name = "skip if register value NOT equal value opcode test - register value != value program counter 0x03AA",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                    .program_counter = 0x03AA,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                            .program_counter = 0x03AC,
                        },
                        .instruction = 0x4BFF
                    }
                }

            },
            {
                .test_name = "skip if register value NOT equal value opcode test - register value != value program counter 0x03AA",
                .starting_CPU_state = {
                    .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                    .program_counter = 0x03AA,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0, 0,0,0,0, 0,0,0, 0x00AF},
                            .program_counter = 0x03AC,
                        },
                        .instruction = 0x4BFF
                    }
                }

            },
        }
    };

    testCPUStatesAfterInstruction(&tcs);
}

void opcode_5_test(){
    LOG_TEST("Running opcode 5 tests...");
    testCases tcs = {
        .cases_count = 3,
        .test_cases = (struct testCase[]){
            {
                .test_name = "skip if register_x value equal register_y value opcode test - register_x value == register_y value",
                .starting_CPU_state = {
                    .registers = {0,0,0, 0x00FF, 0,0,0, 0x00FF},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0, 0x00FF, 0,0,0, 0x00FF},
                            .program_counter = 0x0302,
                        },
                        .instruction = 0x5370
                    }
                }

            },
            {
                .test_name = "skip if register value equal value opcode test - register value == value register",
                 .starting_CPU_state = {
                    .registers = {0,0,0,0,0,0,0,0,0,0,0, 0x00FF, 0x00FF},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0,0,0,0,0,0,0,0, 0x00FF, 0x00FF},
                            .program_counter = 0x0302,
                        },
                        .instruction = 0x5CB0
                    }
                }
            },
            {   
                .test_name = "skip if register value NOT equal value opcode test - register value != value register",
                 .starting_CPU_state = {
                    .registers = {0,0,0,0,0,0,0,0,0,0,0, 0x00FF, 0x00FD},
                    .program_counter = 0x0300,

                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0,0,0,0,0,0,0,0,0, 0x00FF, 0x00FD},
                            .program_counter = 0x0300,
                        },
                        .instruction = 0x5DC0
                    }
                }},
        }
    };

    testCPUStatesAfterInstruction(&tcs);
}

void opcode_6_test(){
    LOG_TEST("Running opcode 6 tests...");
    testCases tcs = {
        .cases_count = 4,
        .test_cases = (struct testCase[]){
            {
                .test_name = "load value into register opcode test - load lower two of lSB into register; start zero'd registers",
                .step_count = 1,
                .starting_CPU_state = {
                    0
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0, 0x020},
                        },
                        .instruction = 0x6120
                    },
                    
                }
            },
            {
                .test_name = "load value into register opcode test - load lower two of lSB into register; start some non-zero'd registers",
                .step_count = 1,
                .starting_CPU_state = {0, 0, 4, 5, 6, 7, 8},
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0, 0x00FD, 4, 5, 6, 7, 8},
                        },
                        .instruction = 0x61FD
                    }
                }
            },
            {
                .test_name = "load value into register opcode test - load lower two of lSB into register; overwrite non-zero register value",
                .step_count = 1,
                .starting_CPU_state = {0, 0, 4, 5, 6, 7, 8},
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0, 0, 0x00FD, 5, 6, 7, 8},
                        },
                        .instruction = 0x62FD
                    }
                }
            },
            {
                .test_name = "load value into register opcode test - load lower two of lSB into register; five write instructions non-zero'd registers",
                .step_count = 2,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0, 0x00FD}
                        },
                        .instruction = 0x61FD
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {0,0x00FD,0,0,0, 0,0,0,0,0,0x00AC}
                        },
                        .instruction = 0x6AAC
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {0,0x00FD,0,0,0, 0,0,0,0x001A,0x00AC}
                        },
                        .instruction = 0x691A
                    },
                    {
                        .expected_CPU_state = {
                            .registers =  {0,0x00FD,0,0,0, 0,0,0,0x001A,0x00AC,0,0,0x00FF}
                        },
                        .instruction = 0x6DFF
                    },
                    {
                        .expected_CPU_state = {
                            .registers =  {0,0x00FD,0,0,0, 0,0,0,0x0000,0x00AC,0,0,0x00FF}
                        },
                        .instruction = 0x6900
                    }
                }

            },
           
        }
    };

    testCPUStatesAfterInstruction(&tcs);

}

// 7xkk - ADD Vx, byte
// Set Vx = Vx + kk.
// No carry
void opcode_7_test(){
    LOG_TEST("Running opcode 7 tests...");
    testCases tcs = {
        .cases_count = 3,
        .test_cases = (struct testCase[]){
            {
                .test_name = "add value to register opcode test - registers start zero'd out",
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0x00AA}
                        },
                        .instruction = 0x72AA
                    }
                }
            },{
                .test_name = "add value to register opcode test - some registers DO NOT start zero'd out",
                .step_count = 1,
                .starting_CPU_state = {
                    .registers = {0,0,0x00AA, 0,0,2,4,6,8}
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0x00AA, 0,0,2,0x000E,6,8}
                        },
                        .instruction = 0x760A
                    }
                }
            },{
                .test_name = "add value to register opcode test - some registers DO NOT start zero'd out, multiple addition instructions",
                .step_count = 4,
                .starting_CPU_state = {
                    .registers = {0,0,0x00AA, 0,0,0x0002,0x0004,0x0006,0x0008,0x000E}
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {0,0,0x00AA, 0,0,0x0002,0x000E,0x0006,0x0008,0x000E}
                        },
                        .instruction = 0x760A
                    },{
                        .expected_CPU_state = {
                            .registers = {0,0,0x00AA, 0,0,0x0002,0x001C,0x0006,0x0008,0x000E}
                        },
                        .instruction = 0x760E
                    },{
                        .expected_CPU_state = {
                            .registers = {0,0,0x00AA, 0,0,0x0002,0x001C,0x0006,0x0016,0x000E}
                        },
                        .instruction = 0x780E
                    },{
                        .expected_CPU_state = {
                            .registers = {0,0,0x00AA, 0,0,0x0002,0x001C,0x0006,0x0016,0x000E, 0x0080}
                        },
                        .instruction = 0x7A80
                    }
                }
            }
        }
    };
}

void runOpcodes0To7Test(){
    opcode_0_tests();
    opcode_1_test();
    opcode_2_test();
    opcode_3_test();
    opcode_4_test();
    opcode_5_test();
    opcode_6_test();
}



