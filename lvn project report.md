# LVN project report

Because we only consider Binary operation, so we check if the operation is binary operation from the beginning by calling `inst.isBinaryOp()`. 

Then we get Operand `0` and `1`,  by calling `inst.getOperand(0)`, the return value is a `llvm::Use class`, which is a subclass of llvm::value, same as llvm::instruction, thus we can use `llvm::value` as key in type in our valueHash.

We also need to consider subscript of variable, we also need a `versionHash` to store it, and our `valueHash` be comes type: `map<pair<Value*, int>, int>`, where `int` in the pair is our version number, and another `int` is our versionNumber. 

We can store our operands and destination variable in our map because they are all `llvm::Value` type, however, not the expressions, because expression consists of two value number and one Operation symbol, we expect it to be a string, thus, another Hash table is needed, we call it `exprHash`, with type `map<string, int>`.

When we analyis each instruction,

1. Look up if the operand is in valueHash, if it exists, set the valueNumber, otherwise, we give it a subscript, put it in `valueHash`, and increase the global counter for the valueNumber. 

2. construct expression string,  `expr` by composing valueNumber1, operation, and valueNumber2.

3. we look up `exprHash` for expression, if it already exists in the hash table, then we found a redundancy, if not, we update `exprHash` table, and store the instruction into `valueHash`

To run the project, there is a convenient script, under `./scripts` folder, just run `./scripts/run.sh` under project folder, `Test.out` will be generated under `./test` folder




