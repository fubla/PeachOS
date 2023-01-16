#ifndef IDT_H
#define IDT_H

#include <stdint.h>


struct idt_desc
{
    uint16_t offset_1; // offset bits 0-15
    uint16_t selector; // selector in GDT
    uint8_t zero; // unused bits
    uint8_t type_attr; // descriptor type and attributes
    uint16_t offset_2; // offset bits 16-31

} __attribute__((packed));

struct idtr_desc
{
    uint16_t limit; // size of desciptor table -1
    uint32_t base; // base address of the start of the IDT
    
} __attribute__((packed));

void idt_init();
void enable_interrupts();
void disable_interrupts();

#endif
