#ifndef __ZVM_INSTRUCTION_H__
#define __ZVM_INSTRUCTION_H__
#include <stdint.h>
#include <stdbool.h>
#define ZVM_INSTRUCTION_MAX_OPCODE 255
#define zvm_is_valid_instruction(opcode) ((opcode) < ZVM_INSTRUCTION_HANDLERS_COUNT)

typedef struct zvm_vm_t zvm_vm_t;
typedef bool (*zfn_instruction_handler_t)(zvm_vm_t* vm, uint8_t, uint8_t, uint8_t);

typedef struct zvm_instruction_operand_t zvm_instruction_operand_t;
typedef struct zvm_instruction_t zvm_instruction_t;

typedef struct zvm_instruction_metadata_t zvm_instruction_metadata_t;
typedef struct zvm_operand_metadata_t zvm_operand_metadata_t;

#define ZVM_INSTRUCTION_HANDLER_FUNCTION_NAME(name)   zfnx_ ## name
#define ZVM_INSTRUCTION_HANDLER_FUNCTION_PARAM \
    zvm_vm_t* vm, uint8_t left, uint8_t right, uint8_t output

#define ZVM_INSTRUCTION_HANDLER_FUNCTION_ARGS \
    vm, left, right, output

#define ZVM_INSTRUCTION_HANDLER_FUNCTION(name)  \
    bool ZVM_INSTRUCTION_HANDLER_FUNCTION_NAME(name) (ZVM_INSTRUCTION_HANDLER_FUNCTION_PARAM)


#define ZVM_INSTRUCTION_HANDLER_FUNCTION_BEGIN(name) \
    ZVM_INSTRUCTION_HANDLER_FUNCTION(name) {

#define ZVM_INSTRUCTION_HANDLER_FUNCTION_END    \
        return true;                            \
    }

#define ZVM_INSTRUCTION_HANDLER_CALL(name) zfnx_ ## name(ZVM_INSTRUCTION_HANDLER_FUNCTION_ARGS)

#define ZVM_OPERAND_TYPE_REG 0
#define ZVM_OPERAND_TYPE_MEM 1
#define ZVM_OPERAND_TYPE_IMM 2

typedef struct zvm_operand_metadata_t {
    uint16_t type:2; /* ZVM_OPERAND_XXX */
}zvm_operand_metadata_t;

typedef struct zvm_instruction_operand_t{
    zvm_operand_metadata_t* metadata;
    uint8_t value;
}zvm_instruction_operand_t;

#define ZVM_INST_0_OP  0
#define ZVM_INST_1_OP  1
#define ZVM_INST_2_OP  2
#define ZVM_INST_3_OP  3

#define ZVM_INST_MAX_OPERANDS  3


/* instructions opcodes */
#define ZVM_OPCODE_ADD        0  /* ADD reg, reg, reg  */
#define ZVM_OPCODE_SUB        1  /* SUB reg, reg, reg  */
#define ZVM_OPCODE_LDI        2  /* LDI reg, imm       */
#define ZVM_OPCODE_STR        3  /* STR reg, mem       */
#define ZVM_OPCODE_LDM        4  /* LDM reg, mem       */
#define ZVM_OPCODE_LDA        5  /* LDA reg, reg       */
#define ZVM_OPCODE_STA        6  /* STA reg, reg       */
#define ZVM_OPCODE_PUSH       7  /* PUSH reg           */
#define ZVM_OPCODE_POP        8  /* POP reg            */
#define ZVM_OPCODE_INC        9  /* INC reg            */
#define ZVM_OPCODE_DEC        10 /* DEC reg            */

/* 10 Bitwise Operations Opcodes */
#define ZVM_OPCODE_AND        11 /* AND reg, reg, reg  */
#define ZVM_OPCODE_OR         12 /* OR reg, reg, reg   */
#define ZVM_OPCODE_XOR        13 /* XOR reg, reg, reg  */
#define ZVM_OPCODE_NOT        14 /* NOT reg            */
#define ZVM_OPCODE_SHL        15 /* SHL reg, reg, reg  */
#define ZVM_OPCODE_SHR        16 /* SHR reg, reg, reg  */
#define ZVM_OPCODE_NAND       17 /* NAND reg, reg, reg */
#define ZVM_OPCODE_NOR        18 /* NOR reg, reg, reg  */
#define ZVM_OPCODE_XNOR       19 /* XNOR reg, reg, reg */
#define ZVM_OPCODE_ROL        20 /* ROL reg, reg, reg  */


/* instructions handlers */
ZVM_INSTRUCTION_HANDLER_FUNCTION(add);
ZVM_INSTRUCTION_HANDLER_FUNCTION(sub);
ZVM_INSTRUCTION_HANDLER_FUNCTION(ldi);
ZVM_INSTRUCTION_HANDLER_FUNCTION(str);
ZVM_INSTRUCTION_HANDLER_FUNCTION(ldm);
ZVM_INSTRUCTION_HANDLER_FUNCTION(lda);
ZVM_INSTRUCTION_HANDLER_FUNCTION(sta);
ZVM_INSTRUCTION_HANDLER_FUNCTION(push);
ZVM_INSTRUCTION_HANDLER_FUNCTION(pop);
ZVM_INSTRUCTION_HANDLER_FUNCTION(inc);
ZVM_INSTRUCTION_HANDLER_FUNCTION(dec);

/* Bitwise Handlers */
ZVM_INSTRUCTION_HANDLER_FUNCTION(and);
ZVM_INSTRUCTION_HANDLER_FUNCTION(or);
ZVM_INSTRUCTION_HANDLER_FUNCTION(xor);
ZVM_INSTRUCTION_HANDLER_FUNCTION(not);
ZVM_INSTRUCTION_HANDLER_FUNCTION(shl);
ZVM_INSTRUCTION_HANDLER_FUNCTION(shr);
ZVM_INSTRUCTION_HANDLER_FUNCTION(nand);
ZVM_INSTRUCTION_HANDLER_FUNCTION(nor);
ZVM_INSTRUCTION_HANDLER_FUNCTION(xnor);
ZVM_INSTRUCTION_HANDLER_FUNCTION(rol);


#define ZVM_INST_HANDLER_TYPE_RRR  0
#define ZVM_INST_HANDLER_TYPE_RI   1
#define ZVM_INST_HANDLER_TYPE_RM   2
#define ZVM_INST_HANDLER_TYPE_RR   3
#define ZVM_INST_HANDLER_TYPE_R    4


typedef struct zvm_instruction_handler_t zvm_instruction_handler_t;

struct zvm_instruction_handler_t{
    int8_t type;
    void *action;
};

typedef struct zvm_instruction_metadata_t{
    char *mnemonic;
    uint8_t opcode;
    uint8_t operands_count;
    zvm_operand_metadata_t operands[ZVM_INST_MAX_OPERANDS];
    zvm_instruction_handler_t handler;
}zvm_instruction_metadata_t;


#define ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, argc) \
    [ZVM_OPCODE_ ## name] = {#name, ZVM_OPCODE_ ## name, ZVM_INST_ ## argc ## _OP, {

#define ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, type) \
    },{ZVM_INST_HANDLER_TYPE_ ## type, ZVM_INSTRUCTION_HANDLER_FUNCTION_NAME(fn)}},

#define ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(type) \
    {ZVM_OPERAND_TYPE_ ## type},

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 3) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RRR)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RI(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 2) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(IMM) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RI)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RM(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 2) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(MEM) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RM)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_RR(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 2) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, RR)

#define ZVM_INSTRUCTION_HANDLER_ENTRY_R(name, fn) \
     ZVM_INSTRUCTION_HANDLER_ENTRY_BEGIN(name, 1) \
        ZVM_INSTRUCTION_HANDLER_OPERAND_ENTRY(REG) \
    ZVM_INSTRUCTION_HANDLER_ENTRY_END(fn, R)

static const zvm_instruction_metadata_t instruction_handlers [ZVM_INSTRUCTION_MAX_OPCODE + 1] = {
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(ADD, add)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(SUB, sub)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RI(LDI, ldi)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RM(STR, str)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RM(LDM, ldm)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RR(LDA, lda)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RR(STA, sta)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(PUSH, push)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(POP, pop)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(INC, inc)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(DEC, dec)
    
    /* Bitwise handlers mapping */
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(AND, and)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(OR, or)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(XOR, xor)
    ZVM_INSTRUCTION_HANDLER_ENTRY_R(NOT, not)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(SHL, shl)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(SHR, shr)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(NAND, nand)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(NOR, nor)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(XNOR, xnor)
    ZVM_INSTRUCTION_HANDLER_ENTRY_RRR(ROL, rol)
};

#define ZVM_INSTRUCTION_HANDLERS_COUNT (sizeof(instruction_handlers) / sizeof(zvm_instruction_metadata_t))

struct zvm_instruction_t {
    zvm_instruction_metadata_t *metadata;
    zvm_instruction_operand_t operands[ZVM_INST_MAX_OPERANDS];
};


#define zvm_has_next_instruction(vm) \
  ((vm)->cpu.IP < (vm)->program.instructions_count)

#define zvm_set_instruction(p, i, opcode, v1, v2, v3)\
    (p).instructions[(i)].metadata = (zvm_instruction_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode];\
    (p).instructions[(i)].operands[0].metadata = (zvm_operand_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode].operands[0];\
    (p).instructions[(i)].operands[1].metadata = (zvm_operand_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode].operands[1];\
    (p).instructions[(i)].operands[2].metadata = (zvm_operand_metadata_t*)&instruction_handlers[ZVM_OPCODE_ ## opcode].operands[2];\
    (p).instructions[(i)].operands[0].value = (v1); \
    (p).instructions[(i)].operands[1].value = (v2); \
    (p).instructions[(i)].operands[2].value = (v3);

#define R0  0
#define R1  1
#define R2  2
#define R3  3

#define INST(p, opcode, v1, v2, v3) \
    zvm_set_instruction(p, ((p).instructions_count), opcode, v1, v2, v3) \
    (p).instructions_count++;


#define ADD(p, v1, v2, v3) INST(p, ADD, v1, v2, v3)
#define SUB(p, v1, v2, v3) INST(p, SUB, v1, v2, v3)
#define LDI(p, v1, v2) INST(p, LDI, v1, v2, 0)

#define LDM(p, v1, v2) INST(p, LDM, v1, v2, 0)
#define STR(p, v1, v2) INST(p, STR, v1, v2, 0)

#define LDA(p, v1, v2) INST(p, LDA, v1, v2, 0)
#define STA(p, v1, v2) INST(p, STA, v1, v2, 0)

#define PUSH(p, v1) INST(p, PUSH, v1, 0, 0)
#define POP(p, v1) INST(p, POP, v1, 0, 0)

#define INC(p, v1) INST(p, INC, v1, 0, 0)
#define DEC(p, v1) INST(p, DEC, v1, 0, 0)

/* Bitwise Macros */
#define AND(p, v1, v2, v3)  INST(p, AND, v1, v2, v3)
#define OR(p, v1, v2, v3)   INST(p, OR, v1, v2, v3)
#define XOR(p, v1, v2, v3)  INST(p, XOR, v1, v2, v3)
#define NOT(p, v1)          INST(p, NOT, v1, 0, 0)
#define SHL(p, v1, v2, v3)  INST(p, SHL, v1, v2, v3)
#define SHR(p, v1, v2, v3)  INST(p, SHR, v1, v2, v3)
#define NAND(p, v1, v2, v3) INST(p, NAND, v1, v2, v3)
#define NOR(p, v1, v2, v3)  INST(p, NOR, v1, v2, v3)
#define XNOR(p, v1, v2, v3) INST(p, XNOR, v1, v2, v3)
#define ROL(p, v1, v2, v3)  INST(p, ROL, v1, v2, v3)

#endif