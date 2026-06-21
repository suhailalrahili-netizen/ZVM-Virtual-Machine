#ifndef __ZVM_H__
#define __ZVM_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> /* <-- أضفنا هذا السطر هنا لتشغيل الـ NULL */
#include "zvm_exception.h"
#include "zvm_instruction.h"
#include "zvm_program.h"

#define ZVM_RX_REGISTERS_COUNT 4

/* Machine */
typedef struct zvm_vm_t zvm_vm_t;
typedef struct zvm_cpu_t zvm_cpu_t;

struct zvm_cpu_t{
    uint8_t IP;
    zvm_instruction_t* IR;
    uint8_t R[ZVM_RX_REGISTERS_COUNT];
    uint32_t OUTPUT;
    uint8_t FLAGS;

    uint8_t *DR;
    zvm_instruction_t *CR;
    uint8_t *SR;

    int8_t SP;
};

#define ZVM_IO_DEVICE_TYPE_IN      0
#define ZVM_IO_DEVICE_TYPE_OUT     1
#define ZVM_IO_DEVICE_TYPE_INOUT   2

#define ZVM_VM_MAX_IO_DEVICES 8

#define ZVM_IO_DEVICE_KEYBOARD 0
#define ZVM_IO_DEVICE_SCREEN   1

typedef struct zvm_io_device_t zvm_io_device_t; 
typedef void *(*zinout_io_handler_t)(zvm_vm_t *vm, uint8_t port, uint8_t type, void *args);

static inline void * zinout_keyboard(zvm_vm_t *vm, uint8_t port, uint8_t operation_type, void *args){
    return NULL;
}

struct zvm_io_device_t {
    uint8_t port;
    uint8_t inout_type;
    zinout_io_handler_t handler;
};

static const zvm_io_device_t io_devices[ZVM_VM_MAX_IO_DEVICES] = {
    {ZVM_IO_DEVICE_KEYBOARD, ZVM_IO_DEVICE_TYPE_IN, zinout_keyboard},
};

struct zvm_vm_t{
    bool has_exception;
    int8_t exception_type;
    int8_t exception_code;

    zvm_cpu_t cpu;
    zvm_program_t program;
};
#endif