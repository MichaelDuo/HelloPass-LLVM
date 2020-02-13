#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include <string>
#include <sstream>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "ValueNumbering"

using namespace llvm;

string getValueName(Value *v){
  Value *vPrt = v;
  std::stringstream ss;
  ss << vPrt;  
  std::string name = ss.str(); 
  return name;
}

namespace {
struct ValueNumbering : public FunctionPass {
  string func_name = "test";
  static char ID;
  ValueNumbering() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "ValueNumbering: ";
    errs() << F.getName() << "\n";

        if (F.getName() != func_name) return false;

        map<string, int> hash;

        for (auto& basic_block : F)
        {
            int count = 1;
            Value *tInst;
            for (auto& inst : basic_block)
            {
              
            }
        }
    return false;
  }
}; // end of struct ValueNumbering
}  // end of anonymous namespace

char ValueNumbering::ID = 0;
static RegisterPass<ValueNumbering> X("ValueNumbering", "ValueNumbering Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);