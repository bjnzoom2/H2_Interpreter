cmake_minimum_required(VERSION 4.1)
project(H2_Interpreter)

set(CMAKE_CXX_STANDARD 20)

find_program(NASM_EXECUTABLE nasm HINTS ENV PATH)
if(NOT NASM_EXECUTABLE)
    message(FATAL_ERROR "NASM not found. Install NASM and add it to PATH (or set NASM_EXECUTABLE).")
endif()

# Source assembly and object paths
# Change ASM_SRC if your compiler generates the .asm into a different directory.
set(ASM_SRC "${CMAKE_SOURCE_DIR}/cmake-build-debug/out.asm")
set(ASM_OBJ "${CMAKE_BINARY_DIR}/cmake-build-debug/out.obj")

# Custom command: assemble out.asm -> out.obj
add_custom_command(
        OUTPUT "${ASM_OBJ}"
        COMMAND "${NASM_EXECUTABLE}" -f win64 -o "${ASM_OBJ}" "${ASM_SRC}"
        DEPENDS "${ASM_SRC}"
        VERBATIM
)

# Custom target so we can depend on the assembled object
add_custom_target(asm_out DEPENDS "${ASM_OBJ}")

# Create an executable by linking the produced object.
# If your assembly defines `main` then no additional source is required.
# If you need helper C/C++ files, add them here alongside ${ASM_OBJ}.
add_executable(H2_Compiled_Program ${ASM_OBJ})
set_target_properties(H2_Compiled_Program PROPERTIES LINKER_LANGUAGE CXX)

# Make sure the exe depends on the object being built
add_dependencies(H2_Compiled_Program asm_out)

# Optional: Run the produced .exe automatically after a successful build.
# CLion captures output of the build; POST_BUILD runs the exe and its stdout/stderr
# will appear in CLion's Build output.
add_custom_command(TARGET H2_Compiled_Program POST_BUILD
        COMMAND $<TARGET_FILE:H2_Compiled_Program>
        VERBATIM
)

add_executable(H2_Interpreter main.cpp
        tokenization.h
        parsing.h
        generation.h)
configure_file(File.h2 ${CMAKE_CURRENT_BINARY_DIR}/File.h2 COPYONLY)