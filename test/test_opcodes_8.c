#include "test_helpers.h"
#include "test_opcodes_8.h"

void runOpcodes8Test(){
    LOG_TEST("Running opcode 8 tests...");

    testCases tcs = {
        // .cases_count = 16,
        .cases_count = 4,
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

            // {},
            // {},

            // {},
            // {},

            // {},
            // {},
            // {},

            // {},
            // {},
            // {},

            // {},
            // {},
            // {},

            // {},
            // {},
            // {},
        }
    };

    testCPUStatesAfterInstruction(&tcs);

}