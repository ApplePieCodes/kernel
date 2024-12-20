import os
import subprocess

def main():
    os.system("sudo rm -rf limine")
    os.system("rm -rf build")
    os.system("rm -rf iso")
    os.system("rm os.iso")
    os.system("rm kernel.elf")

if __name__ == "__main__":
    main()