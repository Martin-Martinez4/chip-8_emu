#include "test_helpers.h"
#include "test_opcodes_8.h"

void runOpcodes8Test(){
    LOG_TEST("Running opcode 8 tests...");

    testCases tcs = {
        .cases_count = 23,
        .test_cases = (struct testCase[]){

            // 8xy0
            // set vx = vy
            {
                .test_name = "set Vx = Vy opcode test - single instruction",
                .starting_CPU_state = {
                    .registers = {1, 1, 1, 1, 1, 0},
                
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {1, 1, 1, 1, 1, 1},
                        .instruction = 0x8500
                    }
                }
            },
            {
                .test_name = "set Vx = Vy opcode test - multiple instruction",
                .starting_CPU_state = {
                    .registers = {1, 1, 1, 10, 12, 13, 12, 15 ,19, 20, 200},
                
                },
                .step_count = 3,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers= {1, 1, 1, 10, 12, 1, 12, 15 ,19, 20, 200},
                        },
                        .instruction = 0x8500
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {12, 1, 1, 10, 12, 1, 12, 15 ,19, 20, 200},
                        },
                        .instruction = 0x8060
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {12, 1, 1, 10, 12, 1, 12, 15 ,19, 20, 200, 0, 0, 200},
                        },
                        .instruction = 0x8DA0
                    }
                }
            },
            
            // 8xy1
            // Vx = Vx | Vy
            {
                .test_name = "Vx = Vx | Vy opcode test - single instruction",
                .starting_CPU_state = {1, 1, 1, 10, 12, 13, 12, 15 ,19, 20, 200},
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {11, 1, 1, 10, 12, 13, 12, 15 ,19, 20, 200},
                        },
                        .instruction = 0x8031
                    }
                }
            },
            {
                .test_name = "Vx = Vx | Vy opcode test - multiple instructions",
                .starting_CPU_state = {1, 1, 1, 10, 12, 13, 12, 15 ,19, 20, 200},
                .step_count = 4,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {11, 1, 1, 10, 12, 13, 12, 15 ,19, 20, 200}
                        },
                        .instruction = 0x8031
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {11, 11, 1, 10, 12, 13, 12, 15 ,19, 20, 200}
                        },
                        .instruction = 0x8131
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {11, 11, 1, 10, 12, 13, 12, 15 ,19, 20, 200}
                        },
                        .instruction = 0x8011
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {11, 11, 1, 10, 12, 13, 12, 15 ,19, 20, 220}
                        },
                        .instruction = 0x8A91
                    }
                }
            },

            {
                .test_name =  "Vx = Vx & Vy opcode test - single instruction",
                .starting_CPU_state = {
                    .registers = {2, 202}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 2}
                        },
                        .instruction = 0x8102
                    }
                }
            },
            {
                .test_name = "Vx = Vx & Vy opcode test - multiple instruction",
                .starting_CPU_state = {
                    .registers = {2, 202, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                },
                .step_count = 4,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 2, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                        },
                        .instruction = 0x8102
                    }, 
                    {
                        .expected_CPU_state = {
                            .registers = {2, 2, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                        },
                        .instruction = 0x8012
                    }, 
                    {
                        .expected_CPU_state = {
                            .registers = {2, 2, 255, 1, 1, 0, 0, 1, 1, 15, 2, 15}
                        },
                        .instruction = 0x8A02
                    }, 
                    {
                        .expected_CPU_state = {
                             .registers = {2, 2, 255, 1, 1, 0, 0, 1, 1, 15, 2, 15}
                        },
                        .instruction = 0x85B2
                    }
                }
            },
            {
                .test_name =  "Vx = Vx ^ Vy opcode test - single instruction",
                .starting_CPU_state = {
                    .registers = {2, 202}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 200}
                        },
                        .instruction = 0x8103
                    }
                }
            },
            {
                .test_name = "Vx = Vx ^ Vy opcode test - multiple instruction",
                .starting_CPU_state = {
                    .registers = {2, 202, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                },
                .step_count = 4,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                        },
                        .instruction = 0x8103
                    }, 
                    {
                        .expected_CPU_state = {
                            .registers = {202, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                        },
                        .instruction = 0x8013
                    }, 
                    {
                        .expected_CPU_state = {
                            .registers = {202, 199, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                        },
                        .instruction = 0x81B3
                    }, 
                    {
                        .expected_CPU_state = {
                             .registers = {202, 199, 240, 1, 1, 0, 0, 1, 1, 15, 15, 15}
                        },
                        .instruction = 0x82B3
                    }
                }
            },

            {
                .test_name = "Vx = Vx + Vy with Carry opcode test - single instruction no carry",
                .starting_CPU_state = {
                    .registers = {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15,0, 0, 0, 0}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 215, 255, 1, 1, 0, 0, 1, 1, 15, 15, 15,0, 0, 0, 0}
                        },
                        .instruction = 0x81B4
                    }
                }
            },
            {
                .test_name = "Vx = Vx + Vy with Carry opcode test - single instruction carry",
                .starting_CPU_state = {
                    .registers = {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 0, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 1}
                        },
                        .instruction = 0x81B4
                    }
                }
            },
            {
                .test_name = "Vx = Vx + Vy with Carry opcode test - multiple instructions carry and no carry",
                .starting_CPU_state = {
                    .registers = {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                },
                .step_count = 8,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {2, 0, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 1}
                        },
                        .instruction = 0x81B4
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x8034
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 255, 1, 1, 0, 0, 1, 1, 18, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x8904
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 55, 1, 1, 0, 0, 1, 1, 18, 15, 56,0, 0, 0, 1}
                        },
                        .instruction = 0x82B4
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 56, 1, 1, 0, 0, 1, 1, 18, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x8234
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 112, 1, 1, 0, 0, 1, 1, 18, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x82B4
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 224, 1, 1, 0, 0, 1, 1, 18, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x8224
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {3, 0, 224, 1, 1, 0, 0, 1, 1, 18, 15, 24,0, 0, 0, 1}
                        },
                        .instruction = 0x8B24
                    },
                  
                }
            },

            {
                .test_name = "Vx = Vx - Vy with borrow opcode test - single instruction no borrow",
                .starting_CPU_state = {
                    .registers =  {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers =  {2, 198, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 1}
                        },
                        .instruction = 0x8105
                    }
                }
            },
            {
                .test_name = "Vx = Vx - Vy with borrow opcode test - single instruction with borrow",
                .starting_CPU_state = {
                    .registers =  {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {58, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x8015
                    }
                }
            },
            {
                .test_name = "Vx = Vx - Vy with borrow opcode test - multiple instructions with and without borrow",
                .starting_CPU_state = {
                    .registers = {2, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                },
                .step_count = 5,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {58, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 56,0, 0, 0, 0}
                        },
                        .instruction = 0x8015
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {58, 200, 255, 1, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8B95
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {58, 200, 197, 1, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8205
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {58, 200, 197, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                        },
                        .instruction = 0x8325
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {58, 3, 197, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8125
                    }
                }

            },

            {
                .test_name = "Vx >> 1 opcode test - single instruction shift right even",
                .step_count = 1,
                .starting_CPU_state = {
                    .registers = {58, 3, 197, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {29, 3, 197, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                        },
                        .instruction = 0x8016
                    }
                }
            },
            {
                .test_name = "Vx >> 1 opcode test - single instruction shift right odd",
                .step_count = 1,
                .starting_CPU_state = {
                    .registers = {58, 3, 197, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                },
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {58, 3, 98, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8216
                    }
                }
            },
            {
                .test_name = "Vx >> 1 opcode test - multiple instructions shift right odd and even",
                .step_count = 4, 
                .starting_CPU_state = {
                    .registers = {58, 3, 197, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                },
                .steps = (testStep[]){
                    {   
                        .expected_CPU_state = {
                             .registers = {58, 3, 98, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1},
                        },
                        .instruction = 0x8216
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {29, 3, 98, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                        },
                        .instruction = 0x8006
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {14, 3, 98, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8006
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {14, 3, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                        },
                        .instruction = 0x8236
                    }
                }
            },

            {
                .test_name = "Vx = Vy - Vx with borrow opcode test - single instruction no borrow",
                .starting_CPU_state = {
                    .registers = {13, 3, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                },
                .step_count = 1, 
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {13, 10, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8107
                    }
                }
            },
            {
                .test_name = "Vx = Vy - Vx with borrow opcode test - single instruction with borrow",
                .starting_CPU_state = {
                    .registers = {13, 3, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                },
                .step_count = 1, 
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {246, 3, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                        },
                         .instruction = 0x8017
                    }
                }
            },
            {
                .test_name = "Vx = Vy - Vx with borrow opcode test - muliple instructions with and without borrow",
                .starting_CPU_state = {
                    .registers = {13, 3, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                },
                .step_count = 4,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {246, 3, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 0}
                        },
                        .instruction = 0x8017
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {246, 243, 49, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8107
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {246, 243, 11, 60, 1, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8237
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {246, 243, 11, 60, 59, 0, 0, 1, 1, 15, 15, 41,0, 0, 0, 1}
                        },
                        .instruction = 0x8437
                    }
                }
            },
            {
                .test_name = "VX << 1 opcode test - single instruction Vx msb 1",
                .starting_CPU_state = {
                    .registers = {182, 142, 8, 219, 64, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 231}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {108, 142, 8, 219, 64, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 1}
                        },
                        .instruction = 0x801E
                    }
                }
            },
            {
                .test_name = "VX << 1 opcode test - single instruction Vx msb 0",
                 .starting_CPU_state = {
                    .registers = {108, 142, 8, 219, 64, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 231}
                },
                .step_count = 1,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {108, 142, 16, 219, 64, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 0}
                        },
                        .instruction = 0x821E
                    }
                }
            },
            {
                .test_name = "VX << 1 opcode test - multiple instruction Vx msb varies",
                .starting_CPU_state = {
                    .registers = {37, 142, 8, 219, 64, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 231}
                },
                .step_count = 4,
                .steps = (testStep[]){
                    {
                        .expected_CPU_state = {
                            .registers = {37, 142, 16, 219, 64, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 0}
                        },
                        .instruction = 0x821E
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {37, 142, 16, 219, 128, 173, 91, 255, 26, 118, 203, 75, 156, 44, 0, 0}
                        },
                        .instruction = 0x841E
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {37, 142, 16, 219, 128, 173, 91, 255, 26, 118, 203, 75, 56, 44, 0, 1}
                        },
                        .instruction = 0x8CEE
                    },
                    {
                        .expected_CPU_state = {
                            .registers = {74, 142, 16, 219, 128, 173, 91, 255, 26, 118, 203, 75, 56, 44, 0, 0}
                        },
                        .instruction = 0x80EE
                    }
                }
            }
   
        }
    };

    testCPUStatesAfterInstruction(&tcs);

}