#include <stdbool.h>
#include "zvm.h"
#include "zvm_exception.h"

bool zxpt_fetch(zvm_vm_t* vm, uint8_t code)   {return false;}
bool zxpt_decode(zvm_vm_t* vm, uint8_t code)  {return false;}
bool zxpt_execute(zvm_vm_t* vm, uint8_t code) {return false;}
bool zxpt_invalid_register(zvm_vm_t* vm, uint8_t code) {return false;}
bool zxpt_bad_instruction(zvm_vm_t* vm, uint8_t code) {return false;}
bool zxpt_bad_memory_address(zvm_vm_t* vm, uint8_t code) {return false;}
bool zxpt_stack_overflow(zvm_vm_t* vm, uint8_t code) {return false;}
bool zxpt_stack_underflow(zvm_vm_t* vm, uint8_t code) {return false;}