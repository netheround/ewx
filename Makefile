# === General Settings ===
ISO_NAME     = ewx.iso
KERNEL_ELF   = iso/boot/ewx

CC           = x86_64-elf-gcc
NASM         = nasm
OBJCOPY      = x86_64-elf-objcopy
QEMU         = qemu-system-x86_64

CFLAGS       = -ffreestanding -O2 -Wall -Wextra -std=gnu11 -I./src/include -mcmodel=large
LDFLAGS      = -T linker.ld -nostdlib -mcmodel=large
QEMU_DEBUG_FLAGS = -M smm=off -no-shutdown -no-reboot -d int

# === Source Directories ===
SRCDIR       = src
OBJDIR       = bin
DRIVERSDIR   = $(SRCDIR)/drivers
LIBCDIR      = $(SRCDIR)/libc
GDTDIR       = $(SRCDIR)/gdt
IDTDIR       = $(SRCDIR)/idt
FONTDIR      = $(SRCDIR)/fonts

# === Object Files ===
OBJ_C_FILES  = $(wildcard $(SRCDIR)/*.c) \
               $(wildcard $(DRIVERSDIR)/*.c) \
               $(wildcard $(LIBCDIR)/ewx/*.c) \
               $(GDTDIR)/gdt.c \
               $(wildcard $(IDTDIR)/*.c)

OBJ_C        = $(OBJ_C_FILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

OBJ_ASM      = $(OBJDIR)/gdt/gdt_asm.o $(OBJDIR)/idt/idt_asm.o
FONT_OBJ     = $(FONTDIR)/font.o

OBJ_ALL      = $(OBJ_C) $(OBJ_ASM) $(FONT_OBJ)

# === Targets ===
all: $(ISO_NAME)

# Compile C source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble NASM .s files to object files
$(OBJDIR)/gdt/gdt_asm.o: $(GDTDIR)/gdt_asm.s
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

$(OBJDIR)/idt/idt_asm.o: $(IDTDIR)/idt_asm.s
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

# Convert PSF font to object file
$(FONT_OBJ): $(FONTDIR)/font.psf
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf64-x86-64 -B i386 $< $@

# Link all objects into final ELF
$(KERNEL_ELF): $(OBJ_ALL)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Generate bootable ISO using Limine
$(ISO_NAME): $(KERNEL_ELF)
	xorriso -as mkisofs \
		-b limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		iso -o $(ISO_NAME)

# Run in QEMU
run: all
	$(QEMU) -cdrom $(ISO_NAME)

# Debug in QEMU
debug: all
	$(QEMU) $(QEMU_DEBUG_FLAGS) -cdrom $(ISO_NAME)

# Clean build artifacts
clean:
	rm -rf $(OBJDIR)/* $(ISO_NAME) $(KERNEL_ELF)

.PHONY: all clean run debug
