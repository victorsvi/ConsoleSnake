## COMPILE INSTRUCTIONS

To compile this program in Code::Blocks you must do these steps inside Code::Blocks:
- Open "Setting/Compiler/Toolchain executables" and make sure that a 32bit version of the MinGW GCC Compiler is installed
- Open "Setting/Compiler/Search directiories" and add the path to the folder "lib" of this repository to the Compiler and Linker tabs.
- Open "Setting/Compiler/Linker settings" and include the file "lib/libconio.a" of this repository and add the linker option "-lconio".