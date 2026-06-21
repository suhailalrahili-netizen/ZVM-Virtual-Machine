#ifndef __ZVM_EXCEPTION_H__
#define __ZVM_EXCEPTION_H__ 

#include <stdint.h>
#include <stdbool.h>

typedef struct zvm_vm_t zvm_vm_t;
typedef struct zvm_exception_entry_t zvm_exception_entry_t;

typedef bool (*zxpt_exception_handler_t)(zvm_vm_t* vm, uint8_t code);

#define ZVM_EXCEPTION_HANDLER_FUNCTION_NAME(name)   zxpt_ ## name
#define ZVM_EXCEPTION_HANDLER_FUNCTION_PARAM \
    zvm_vm_t* vm, uint8_t code

#define ZVM_EXCEPTION_HANDLER_FUNCTION_ARGS \
    vm, code

#define ZVM_EXCEPTION_HANDLER_FUNCTION(name)  \
    bool ZVM_EXCEPTION_HANDLER_FUNCTION_NAME(name) (ZVM_EXCEPTION_HANDLER_FUNCTION_PARAM)

#define ZVM_EXCEPTION_HANDLER_CALL(name) zxpt_ ## name(ZVM_EXCEPTION_HANDLER_FUNCTION_ARGS)


#define ZVM_EXCEPT_TYPE_FETCH   0
#define ZVM_EXCEPT_TYPE_DECODE  1
#define ZVM_EXCEPT_TYPE_EXECUTE 2

#define ZVM_EXCEPT_CODE_FETCH              0
#define ZVM_EXCEPT_CODE_DECODE             1
#define ZVM_EXCEPT_CODE_EXECUTE            2
#define ZVM_EXCEPT_CODE_INVALID_REGISTER   3
#define ZVM_EXCEPT_CODE_BAD_INSTRUCTION    4
#define ZVM_EXCEPT_CODE_BAD_MEMORY_ADDRESS 5
#define ZVM_EXCEPT_CODE_STACK_OVERFLOW     6
#define ZVM_EXCEPT_CODE_STACK_UNDERFLOW    7


ZVM_EXCEPTION_HANDLER_FUNCTION(fetch);
ZVM_EXCEPTION_HANDLER_FUNCTION(decode);
ZVM_EXCEPTION_HANDLER_FUNCTION(execute);
ZVM_EXCEPTION_HANDLER_FUNCTION(invalid_register);
ZVM_EXCEPTION_HANDLER_FUNCTION(bad_instruction);
ZVM_EXCEPTION_HANDLER_FUNCTION(bad_memory_address);
ZVM_EXCEPTION_HANDLER_FUNCTION(stack_overflow);
ZVM_EXCEPTION_HANDLER_FUNCTION(stack_underflow);


#define zvm_raise(vm, t, c) \
    (vm)->has_exception  = true;    \
    (vm)->exception_type = ZVM_EXCEPT_TYPE_ ## t;     \
    (vm)->exception_code = ZVM_EXCEPT_CODE_ ## c;

#define zvm_exception_get_type(vm)  (vm)->exception_type
#define zvm_exception_get_code(vm)  (vm)->exception_code

#define zvm_exception_reset(vm)      \
    (vm)->has_exception  = false;    \
    (vm)->exception_type = -1;       \
    (vm)->exception_code = -1;

struct zvm_exception_entry_t{
    uint8_t type;
    uint8_t code;
    zxpt_exception_handler_t handler;
    char *message;
};

#define ZVM_EXCEPTION_HANDLERS_COUNT (sizeof(exception_handlers)) / sizeof(zvm_exception_entry_t)
#define zvm_exception_is_handler(code)  ((code) < ZVM_EXCEPTION_HANDLERS_COUNT)

#define ZVM_EXCEPTION_HANDLER_ENTRY(type, code, name, message) \
    {ZVM_EXCEPT_TYPE_ ## type, ZVM_EXCEPT_CODE_ ## code,  ZVM_EXCEPTION_HANDLER_FUNCTION_NAME(name)   , message},

static const zvm_exception_entry_t exception_handlers [] = {
    ZVM_EXCEPTION_HANDLER_ENTRY(FETCH,   FETCH,   fetch,   "fetch instruction error")
    ZVM_EXCEPTION_HANDLER_ENTRY(DECODE,  DECODE,  decode,  "decode instruction error")
    ZVM_EXCEPTION_HANDLER_ENTRY(EXECUTE, EXECUTE, execute, "execution: bad instruction error")
    ZVM_EXCEPTION_HANDLER_ENTRY(EXECUTE, INVALID_REGISTER, invalid_register, "invalid register")
    ZVM_EXCEPTION_HANDLER_ENTRY(DECODE,  BAD_INSTRUCTION,  bad_instruction,  "decoding: bad instruction error")
    ZVM_EXCEPTION_HANDLER_ENTRY(DECODE,  BAD_MEMORY_ADDRESS, bad_memory_address,  "decoding: bad memory address")
    ZVM_EXCEPTION_HANDLER_ENTRY(DECODE,  STACK_OVERFLOW, stack_overflow,  "stack overflow")
    ZVM_EXCEPTION_HANDLER_ENTRY(DECODE,  STACK_UNDERFLOW, stack_underflow,  "stack underflow")
};

#endif