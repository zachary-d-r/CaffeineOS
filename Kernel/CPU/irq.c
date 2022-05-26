#include "../system.h"

// These are own ISR's that point to our special IRQ handler instead of reagulatr faultHandler
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// Arrat of functon pointers we use to handle custom irq handlers for a given IRQ
void *irqRoutines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

// This installs a custom IRQ handler for the given IRQ
void installIrqHandler(int irq, void (*handler)(struct regs *r)) {
    irqRoutines[irq] = handler;
}
// This uninstall a custom IRQ handler for the given IRQ
void uninstallIrqHandler(int irq) {
    irqRoutines[irq] = 0;
}

// Remape IRQs in order for it to work in real world
void remapIrqs(void) {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

// We first remap the interrupt controllers and install the ISRs
void installIrqs() {
    remapIrqs();

    setIdtGate(32, (unsigned)irq0, 0x08, 0x8E);
    setIdtGate(33, (unsigned)irq1, 0x08, 0x8E);
    setIdtGate(34, (unsigned)irq2, 0x08, 0x8E);
    setIdtGate(35, (unsigned)irq3, 0x08, 0x8E);
    setIdtGate(36, (unsigned)irq4, 0x08, 0x8E);
    setIdtGate(37, (unsigned)irq5, 0x08, 0x8E);
    setIdtGate(38, (unsigned)irq6, 0x08, 0x8E);
    setIdtGate(39, (unsigned)irq7, 0x08, 0x8E);

    setIdtGate(40, (unsigned)irq8, 0x08, 0x8E);
    setIdtGate(41, (unsigned)irq9, 0x08, 0x8E);
    setIdtGate(42, (unsigned)irq10, 0x08, 0x8E);
    setIdtGate(43, (unsigned)irq11, 0x08, 0x8E);
    setIdtGate(44, (unsigned)irq12, 0x08, 0x8E);
    setIdtGate(45, (unsigned)irq13, 0x08, 0x8E);
    setIdtGate(46, (unsigned)irq14, 0x08, 0x8E);
    setIdtGate(47, (unsigned)irq15, 0x08, 0x8E);
}

// Each IRQ ISR points to this function
void irqHandler(struct regs *r) {
    void (*handler)(struct regs *r);  // Blank function pointer

    // Find out if we have a custom handler to run for this IRQ and run it
    handler = irqRoutines[r->int_no - 32];
    if (handler) { handler(r); }

    if (r->int_no >= 40) { outportb(0xA0, 0x20); }  // If the IDT entry that was invoked is greater than 40 we need to send an EOI to the slave controller.
    outportb(0x20, 0x20);  // Send an EOI to the master interrupt controller

}
