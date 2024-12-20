import os
import subprocess
import sys

CPPCOMPILER = "x86_64-elf-gcc"
CPPFLAGS = "-ffreestanding"
LINKER = CPPCOMPILER
LINKFLAGS = "-T linker.ld -nostdlib"

SOURCE_DIR = sys.argv[1] if len(sys.argv) > 1 else "src"
BUILD_DIR = "build"
OUTPUT_FILE = "kernel.elf"

def compile_file(src_file):
    rel_path = os.path.relpath(src_file, SOURCE_DIR)
    obj_file = os.path.join(BUILD_DIR, os.path.splitext(rel_path)[0] + ".o")

    os.makedirs(os.path.dirname(obj_file), exist_ok=True)

    try:
        print(f"Compiling {src_file} -> {obj_file}")
        subprocess.run(
            [CPPCOMPILER, *CPPFLAGS.split(), "-c", src_file, "-o", obj_file, "-I", "src/include", "-I", "limine", "-mcmodel=kernel"],
            check=True
        )
        return obj_file
    except subprocess.CalledProcessError:
        print(f"Failed to compile {src_file}")
        sys.exit(1)

def compile_file_asm(src_file):
    rel_path = os.path.relpath(src_file, SOURCE_DIR)
    obj_file = os.path.join(BUILD_DIR, os.path.splitext(rel_path)[0] + ".o")

    os.makedirs(os.path.dirname(obj_file), exist_ok=True)

    try:
        print(f"Compiling {src_file} -> {obj_file}")
        subprocess.run(
            ["nasm", "-f", "elf64", src_file, "-o", obj_file],
            check=True
        )
        return obj_file
    except subprocess.CalledProcessError:
        print(f"Failed to compile {src_file}")
        sys.exit(1)

def link_files(obj_files):
    try:
        str = "Linking files ( "
        for file in obj_files:
            str = str + file
        print(str + " ) into {OUTPUT_FILE}")
        subprocess.run(
            [LINKER, *LINKFLAGS.split(), "-o", OUTPUT_FILE, *obj_files],
            check=True
        )
    except subprocess.CalledProcessError:
        print("Failed to link object files")
        sys.exit(1)

def main():
    if not os.path.isdir("limine"):
        subprocess.run(
            ["git", "clone", "-bv8.x-binary", "https://github.com/limine-bootloader/limine"]
        )
    print(f"Scanning directory: {SOURCE_DIR}")
    obj_files = []
    

    for root, dirs, files in os.walk(SOURCE_DIR):
        if 'limine' in dirs:
            dirs.remove('limine')

        for file in files:
            if file.endswith(".c"):
                src_file = os.path.join(root, file)
                obj_file = compile_file(src_file)
                obj_files.append(obj_file)
            elif file.endswith(".asm") or file.endswith(".s"):
                src_file = os.path.join(root, file)
                obj_file = compile_file_asm(src_file)
                obj_files.append(obj_file)

    if obj_files:
        link_files(obj_files)
    else:
        print("No .c files found to compile.")
        sys.exit(1)
    os.system("make -C limine")
    os.system("mkdir -p iso/boot/limine")
    os.system("cp kernel.elf iso/boot/kernel.elf")
    os.system("cp limine/limine-bios.sys iso/boot/limine/")
    os.system("cp limine/limine-uefi-cd.bin iso/boot/limine/")
    os.system("cp limine/limine-bios-cd.bin iso/boot/limine/")
    os.system("cp limine.conf iso/boot/limine")
    os.system("mkdir -p iso/EFI/BOOT")
    os.system("cp limine/BOOTX64.EFI iso/EFI/BOOT/")
    os.system("cp limine/BOOTIA32.EFI iso/EFI/BOOT/")
    os.system("xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso -o os.iso")
    os.system("./limine/limine bios-install os.iso")


if __name__ == "__main__":
    main()
