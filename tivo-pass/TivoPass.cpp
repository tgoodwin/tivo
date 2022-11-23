#include <string>

#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace std;
using namespace llvm;

namespace {
struct TivoPass : public FunctionPass {
  static char ID;
  TivoPass() : FunctionPass(ID) {}

  virtual bool runOnFunction(Function &F) {
    // if the function begins with the `__tivo_` prefix, skip
    if (F.getName().str().rfind("__tivo_", 0) == 0) {
      return false;
    }

    LLVMContext &Ctx = F.getContext();
    FunctionCallee logAddFunc = F.getParent()->getOrInsertFunction(
        "logop", Type::getVoidTy(Ctx), Type::getInt32Ty(Ctx));

    FunctionCallee logCASFunc = F.getParent()->getOrInsertFunction(
        "__tivo_sync_bool_compare_and_swap_4", Type::getVoidTy(Ctx),
        Type::getInt32PtrTy(Ctx), Type::getInt32Ty(Ctx), Type::getInt32Ty(Ctx));

    for (auto &B : F) {
      for (auto &I : B) {
        if (auto *op = dyn_cast<AtomicCmpXchgInst>(&I)) {
          // replace I with a function call
          errs() << "Replacing with a function call: " << I << "\n";
          IRBuilder<> builder(op);

          Value *args[] = {op->getOperand(0), op->getOperand(1),
                           op->getOperand(2)};

          builder.CreateCall(logCASFunc, args);
        }
      }
    }
    return false;
  }
};
}  // namespace

char TivoPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerTivoPass(const PassManagerBuilder &,
                             legacy::PassManagerBase &PM) {
  PM.add(new TivoPass());
}
static RegisterStandardPasses RegisterMyPass(
    PassManagerBuilder::EP_EarlyAsPossible, registerTivoPass);
