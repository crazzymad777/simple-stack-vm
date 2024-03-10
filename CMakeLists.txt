cmake_minimum_required(VERSION 2.8.9)
project (simple-stack-vm)

include_directories(includes/)
add_executable(ssvm src/main.c src/ssvm_op.c src/ssvm_matrix.c src/ssvm_branches.c)
target_link_libraries(ssvm m)

