ISO_NAME = xv4.iso
KERNEL_ELF = iso/boot/xv4

CC = x86_64-elf-gcc
OBJCOPY = x86_64-elf-objcopy
QEMU = qemu-system-x86_64
NASM = nasm

CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu11 -I./src/include -mcmodel=large
LDFLAGS = -T linker.ld -nostdlib -mcmodel=large

SRCDIR = src
OBJDIR = bin
DRIVERSDIR = $(SRCDIR)/drivers
LIBCDIR = $(SRCDIR)/libc
FONTDIR = $(SRCDIR)/fonts

FONT_OBJ = $(FONTDIR)/font.o
GDT_OBJ = $(OBJDIR)/gdt/gdt.o
GDT_ASM_OBJ = $(OBJDIR)/gdt/gdt_asm.o

SRC = $(SRCDIR)/kernel.c \
	$(wildcard $(LIBCDIR)/xv4/*.c) \
	$(wildcard $(DRIVERSDIR)/*.c) \
	$(SRCDIR)/gdt/gdt.c \

OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o) $(FONT_OBJ) $(GDT_ASM_OBJ)

all: $(ISO_NAME)

$(GDT_ASM_OBJ): $(SRCDIR)/gdt/gdt_asm.s
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(FONT_OBJ): $(FONTDIR)/font.psf
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386 $< $@

$(KERNEL_ELF): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(ISO_NAME): $(KERNEL_ELF)
	xorriso -as mkisofs -b limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso -o $(ISO_NAME)

run: all
	$(QEMU) -cdrom $(ISO_NAME)

clean:
	rm -rf $(OBJDIR)/* $(ISO_NAME) $(KERNEL_ELF)

.PHONY: all clean run
