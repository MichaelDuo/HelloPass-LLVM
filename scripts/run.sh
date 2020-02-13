cd ./Pass/build && cmake -DCMAKE_BUILD_TYPE=Release ../Transforms/ValueNumbering && make -j4 && cd -

cd test 
clang -Xclang -disable-O0-optnone Test.c -O0 -S -emit-llvm -o Test.ll 
opt Test.ll -mem2reg -S -o Test.bc
opt -load ../Pass/build/libLLVMValueNumberingPass.so  -ValueNumbering < Test.bc > /dev/null
