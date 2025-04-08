#ifndef KERNEL_H_
#define KERNEL_H_

#define halt() \
    __asm__ __volatile__ ("hlt");

#endif /* KERNEL_H_ */
