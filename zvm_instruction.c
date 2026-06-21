#include <stdio.h>
#include "zvm_instruction.h"
#include "zvm.h"

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(add)
    vm->cpu.R[output] = vm->cpu.R[left] + vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(sub)
    vm->cpu.R[output] = vm->cpu.R[left] - vm->cpu.R[right];
    printf("R=%u\n", vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(ldi)
    vm->cpu.R[left] = right;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(str)
    /* STR reg, mem */
    vm->program.data[right] = vm->cpu.R[left];
    printf("MEM=%u\n", vm->program.data[right]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(ldm)
    /* LDM reg, mem */
    vm->cpu.R[left] = vm->program.data[right];
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(lda)
    uint8_t address = vm->cpu.R[left];
    if(!zvm_is_address(address)){
        zvm_raise(vm, EXECUTE, BAD_MEMORY_ADDRESS)
        return false;
    }
    vm->cpu.R[right] = vm->program.data[address];
    printf("R=%u\n", vm->cpu.R[right]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(sta)
    /* STA reg, reg */
    uint8_t address = vm->cpu.R[left];
    if(!zvm_is_address(address)){
        zvm_raise(vm, EXECUTE, BAD_MEMORY_ADDRESS)
        return false;
    }
    vm->program.data[address] = vm->cpu.R[right];
    printf("address=%u\n", vm->program.data[address]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(push)
    uint8_t value;
    if(vm->cpu.SP >= ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE){
        zvm_raise(vm, EXECUTE, STACK_OVERFLOW)
        return false;
    }

    vm->cpu.SP++;
    value = vm->cpu.R[left];
    vm->program.stack[(ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE - 1) - vm->cpu.SP] = value;
    printf("stack=%u\n", value);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(pop)
    uint8_t value;
    if(vm->cpu.SP < 0){
        zvm_raise(vm, EXECUTE, STACK_UNDERFLOW)
        return false;
    }

    value = vm->program.stack[(ZVM_PROGRAM_DEFAULT_STACK_SEGMENT_SIZE - 1) - vm->cpu.SP];
    vm->cpu.SP--;
    
    vm->cpu.R[left] = value;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(inc)
    /* INC reg*/
    vm->cpu.R[left]++;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(dec)
    /* DEC reg*/
    vm->cpu.R[left]--;
    printf("R=%u\n", vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END


/* --- Implementation of 10 Bitwise Operations --- */

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(and)
    vm->cpu.R[output] = vm->cpu.R[left] & vm->cpu.R[right];
    printf("AND R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(or)
    vm->cpu.R[output] = vm->cpu.R[left] | vm->cpu.R[right];
    printf("OR R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(xor)
    vm->cpu.R[output] = vm->cpu.R[left] ^ vm->cpu.R[right];
    printf("XOR R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(not)
    vm->cpu.R[left] = ~vm->cpu.R[left];
    printf("NOT R%u = %u\n", left, vm->cpu.R[left]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(shl)
    vm->cpu.R[output] = vm->cpu.R[left] << vm->cpu.R[right];
    printf("SHL R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(shr)
    vm->cpu.R[output] = vm->cpu.R[left] >> vm->cpu.R[right];
    printf("SHR R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(nand)
    vm->cpu.R[output] = ~(vm->cpu.R[left] & vm->cpu.R[right]);
    printf("NAND R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(nor)
    vm->cpu.R[output] = ~(vm->cpu.R[left] | vm->cpu.R[right]);
    printf("NOR R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(xnor)
    vm->cpu.R[output] = ~(vm->cpu.R[left] ^ vm->cpu.R[right]);
    printf("XNOR R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END

ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(rol)
    uint8_t val = vm->cpu.R[left];
    uint8_t shift = vm->cpu.R[right] % 8;
    vm->cpu.R[output] = (val << shift) | (val >> (8 - shift));
    printf("ROL R%u = %u\n", output, vm->cpu.R[output]);
ZVM_INSTRUCTION_HANDLER_FUNCTION_END