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

              errs() << "===== \n";
            	errs() << inst << "\n";
              if(inst.getOpcode() == Instruction::Load){
                      errs() << "This is Load"<<"\n";
              }
              if(inst.getOpcode() == Instruction::Store){
                      errs() << "This is Store"<<"\n";
              }
                if (inst.isBinaryOp())
                {
                    errs() << "Op Code:" << inst.getOpcodeName()<<"\n"; // print opcode name
                    string op = "";
                    if(inst.getOpcode() == Instruction::Add){
                      errs() << "This is Addition"<<"\n";
                      op = '+';
                    }
                    if(inst.getOpcode() == Instruction::Mul){
                      errs() << "This is Multiplication"<<"\n";
                      op = '*';
                    }
                    
                    // See Other classes, Instruction::Sub, Instruction::UDiv, Instruction::SDiv
                //	errs() << "Operand(0)" << (*inst.getOperand(0))<<"\n";
                    auto* ptr = dyn_cast<User>(&inst);

                    int numOps = ptr->getNumOperands();
                    for (int i=0; i<numOps; i++){
                      Value *v = inst.getOperand(i);
                      string vName = getValueName(v);
                      errs() << "\t" <<  *v << "\n";
                      errs() << "\t" <<  vName << "\n";
                    }
                    
                    Value *op1 = inst.getOperand(0);
                    Value *op2 = inst.getOperand(1);

                    string op1Name = getValueName(op1);
                    string op2Name = getValueName(op2);
                    string destName = getValueName(&inst);

                    int op1Value;
                    int op2Value;
                    int exprValue;
                    int destValue;

                    if(hash.find(op1Name) != hash.end()){
                      op1Value = hash[op1Name];
                    } else {
                      op1Value = count;
                      hash[op1Name] = op1Value;
                      count++;
                    }

                    if(hash.find(op2Name) != hash.end()){
                      op2Value = hash[op2Name];
                    } else {
                      op2Value = count;
                      hash[op2Name] = op2Value;
                      count++;
                    }

                    string exprName = to_string(op1Value) + op + to_string(op2Value);

                    if(hash.find(exprName) != hash.end()){
                      errs() << "redundancy found!!!!!!!!!!!!" << "\n";
                      errs() << inst << "\n";
                      exprValue = hash[exprName];
                    } else {
                      exprValue = count;
                      hash[exprName] = exprValue;
                      count++;
                    }

                    destValue = hash[exprName];
                    hash[destName] = destValue;



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