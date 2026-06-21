#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "zvm.h"

void zvm_init(zvm_vm_t *vm){
    vm->cpu.IP = 0;
    vm->cpu.IR = NULL;

    vm->cpu.R[0] = 0;
    vm->cpu.R[1] = 0;
    vm->cpu.R[2] = 0;
    vm->cpu.R[3] = 0;

    vm->cpu.OUTPUT = 0;
    vm->cpu.FLAGS  = 0;

    vm->cpu.DR = NULL;
    vm->cpu.CR = NULL;
    vm->cpu.SR = NULL;
    vm->cpu.SP = -1;
}

void zvm_init_program(zvm_vm_t* vm){
    #define VMP (vm->program)
    VMP.instructions_count = 0;
    VMP.data_count = 0;
    VMP.stack_counts = 0;

    memset(VMP.data, 0, ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE);
    memset(VMP.stack, 0, ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE);

    // 1. شحن قيم تجريبية في السجلات
    LDI(VMP, R0, 0b00001010) // القيمة 10 بالنظام العشري
    LDI(VMP, R1, 0b00001100) // القيمة 12 بالنظام العشري
    
    // 2. تجربة العمليات الثنائية المضافة
    AND(VMP, R0, R1, R2)  // R2 ستصبح قيمته 8  (0b00001000)
    OR(VMP, R0, R1, R3)   // R3 ستصبح قيمته 14 (0b00001110)
    XOR(VMP, R0, R1, R2)  // R2 ستصبح قيمته 6  (0b00000110)
    
    LDI(VMP, R1, 2)       // شحن القيمة 2 في R1 لاستخدامها في الإزاحة
    SHL(VMP, R0, R1, R2)  // إزاحة R0 (وهي 10) لليسار بمقدار خانتين -> R2 تصبح 40
    SHR(VMP, R0, R1, R3)  // إزاحة R0 (وهي 10) لليمين بمقدار خانتين -> R3 تصبح 2

    #undef VMP
}
/* raise exception */
static bool fetch(zvm_vm_t *vm){
    vm->cpu.IR = &vm->program.instructions[vm->cpu.IP];
    vm->cpu.IP++;
    return true;
}

static bool decode(zvm_vm_t* vm){
    /* input */
    uint8_t val0 = vm->cpu.IR->operands[0].value;
    uint8_t val1 = vm->cpu.IR->operands[1].value;
    uint8_t val2 = vm->cpu.IR->operands[2].value;

    switch(vm->cpu.IR->metadata->handler.type){
        case ZVM_INST_HANDLER_TYPE_RRR:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_RX_REGISTERS_COUNT
               || val2 >= ZVM_RX_REGISTERS_COUNT){
                zvm_raise(vm, DECODE, INVALID_REGISTER)
                return false;
            }
        break;
         
        case ZVM_INST_HANDLER_TYPE_RI:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_RM:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_PROGRAM_DEFAULT_DATA_SEGMENT_SIZE
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_RR:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 >= ZVM_RX_REGISTERS_COUNT
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;
        case ZVM_INST_HANDLER_TYPE_R:
            if(val0 >= ZVM_RX_REGISTERS_COUNT
               || val1 != 0
               || val2 != 0){
                zvm_raise(vm, DECODE, BAD_INSTRUCTION)
                return false;
            }
        break;

    }

    return true;
}

static bool execute(zvm_vm_t* vm){
    zfn_instruction_handler_t handler = (zfn_instruction_handler_t)vm->cpu.IR->metadata->handler.action;
    bool instruction_result = false;

    instruction_result = handler(vm, vm->cpu.IR->operands[0].value,
                                 vm->cpu.IR->operands[1].value,
                                 vm->cpu.IR->operands[2].value);

    return instruction_result;
}

bool except(zvm_vm_t* vm){
    if(vm->has_exception){
        int8_t code = zvm_exception_get_code(vm);

        if(zvm_exception_is_handler(code)){
            return exception_handlers[code]
                            .handler(ZVM_EXCEPTION_HANDLER_FUNCTION_ARGS);
        }else{
            fprintf(stderr, "Invalid exception handler code\n");
            return false;
        }
    }
    
    return true;
}

int main(void){
    /* declaration */
    zvm_vm_t vm;

    /* initialization */
    zvm_init(&vm);
    zvm_init_program(&vm);
    
    /* execution */
    while(zvm_has_next_instruction(&vm)){
        /* fetch */
        if(!fetch(&vm)){
            zvm_raise(&vm, FETCH, FETCH)
            goto zvm_catch;
        }
        /* decode  */
        if(!decode(&vm)){
            zvm_raise(&vm, DECODE, DECODE)
            goto zvm_catch;
        }
        /* execute */
        if(!execute(&vm)){
            goto zvm_catch;
        }
        continue;
    /* exception handling */
    zvm_catch:
        if(!except(&vm)){
            break;
        }else{
            zvm_exception_reset(&vm)
        }
    }

    if(vm.has_exception){
        fprintf(stderr, "Exception(%d): %s\n", vm.exception_code,
                    exception_handlers[vm.exception_code].message);
    }

    return 0;
}