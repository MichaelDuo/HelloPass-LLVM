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

  bool isInMap(map<pair<Value*, int>, int> valueHash, Value *v){
    for(auto it = valueHash.begin(); it != valueHash.end(); ++it){
      if(it->first.first == v) return true;
    }
    return false;
  }

  bool runOnFunction(Function &F) override {
        if (F.getName() != func_name) return false;

        map<string, int> hash;

        for (auto& basic_block : F)
        {
            map<Value*, int> vershionHash;
            map<pair<Value*, int>, int> valueHash;
            map<int, pair<Value*, int>> valueHash_reversed;
            map<string, int> exprHash;
            int counter = 1;

            for (auto& inst : basic_block)
            {
              if (inst.isBinaryOp()){
                Value *v1 = inst.getOperand(0);
                Value *v2 = inst.getOperand(1);

                int vn1;
                int vn2;

                if(!isInMap(valueHash, v1)){
                  valueHash.insert(make_pair(make_pair(v1, 0), counter));
                  valueHash_reversed[counter] = make_pair(v1, 0);
                  vershionHash[v1] = 0;
                  vn1 = counter;
                  counter++;
                } else {
                  vn1 = valueHash.at(make_pair(v1, vershionHash[v1]));
                }

                if(!isInMap(valueHash, v2)){
                  valueHash.insert(make_pair(make_pair(v2, 0), counter));
                  valueHash_reversed[counter] = make_pair(v2, 0);
                  vershionHash[v2] = 0;
                  vn2 = counter;
                  counter++;
                } else {
                  vn2 = valueHash.at(make_pair(v2, vershionHash[v2]));
                }

                string op;
                if(inst.getOpcode() == Instruction::Add) op = "+";
                if(inst.getOpcode() == Instruction::Sub) op = "-";
                if(inst.getOpcode() == Instruction::Mul) op = "*";
                if(inst.getOpcode() == Instruction::SDiv) op = "/";

                if(inst.isCommutative() && vn1 > vn2) {
                  swap(vn1, vn2);
                }

                string expr = to_string(vn1) + op + to_string(vn2);

                Value* ptr = dyn_cast<Value>(&inst);
                if(vershionHash.find(ptr) == vershionHash.end()){
                  vershionHash.insert(make_pair(ptr, 0));
                }

                if(exprHash.find(expr) == exprHash.end()) {
                    exprHash[expr] = counter;
                    if(!isInMap(valueHash, ptr)){
                        valueHash.insert(make_pair(make_pair(ptr, 0),counter));
                        valueHash_reversed[counter] = make_pair(ptr, 0);
                    }
                    else{
                        vershionHash[ptr] += 1;
                        valueHash.insert(make_pair(make_pair(ptr, vershionHash[ptr]),counter));
                        valueHash_reversed[counter] = make_pair(ptr, vershionHash[ptr]);
                    }
                    errs() << to_string(counter) + "=" + expr << "\n";
                    ++ counter;
                } else {
                    int tmp_counter = exprHash[expr];

                    // errs() << "Redunant Computation: " << *ptr << "\n";
                    if(!isInMap(valueHash, ptr)){
                        valueHash.insert(make_pair(make_pair(ptr, 0),tmp_counter));
                        valueHash_reversed[tmp_counter] = make_pair(ptr, 0);
                    }
                    else{
                        vershionHash[ptr] += 1;
                        valueHash.insert(make_pair(make_pair(ptr, vershionHash[ptr]),tmp_counter));
                        valueHash_reversed[tmp_counter] = make_pair(ptr, vershionHash[ptr]);
                    }

                    errs() << to_string(tmp_counter) + "=" + expr << "\n";
                }

              }
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