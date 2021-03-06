#pragma once

#include <generator.h>

#define REGAMOUNT 7

class GeneratorAARCH64 : public Generator
{

private:
    /// Helpers
    void writeInst(std::string, std::string, std::string, std::string);
    void writeInst(std::string, std::string, std::string);
    void writeInst(std::string, std::string);
    void writeInst(std::string, int);
    void writeInst(std::string);
    void writeMov(std::string r1, std::string r2);
    std::string registerToString(Register r);

protected:
    void genIntlitLoad(Type t, int val, Register ret);
    void genGlobLoad(Type t, std::string glob, Register ret);
    void genMul(Type t, Register r1, Register r2, Register ret);
    void genAdd(Type t, Register r1, Register r2, Register ret);
    void genSub(Type t, Register r1, Register r2, Register ret);
    void genDiv(Type t, Register r1, Register r2, Register ret);
    void genMod(Type t, Register r1, Register r2, Register ret);
    void genReg(Type t, Register r, Register ret);

    void genAlloca(Type t, Register r, Register ret);
    void genStore(Type t, Register r1, Register r2);
    void genLoad(Type t, Register r, Register ret);
    void genLoad(Type t, std::string glob, Register ret);
    void genSpillLoad(Type t, Register r, Register ret);
    void genSpillStore(Type t, Register r, Register ret);
    void genFunctionCall(Type t, std::string function, Register ret, std::vector<Register> args);
    void genReturn(Type t, Register ret);
    void genJmpCond(Type t, int op, Register r1, Register r2, std::string label);
    void genLabel(std::string label);
    void genJmp(std::string label);
    void genCmp(Type t, int op, Register r1, Register r2, Register ret);

public:
    void genFunction(Type t, Function f);
    void genGlobalVariable(std::string name, ArrayType t, std::vector<LARGEINT> init);
    void genSetupFile(std::string file);
    void genExternSymbol(std::string sym);

    int registerAmount() { return REGAMOUNT; }

    int assemble(std::string infile, std::string outfile, std::string assembler);
    int link(std::string infile, std::string outfile, std::string linker);

};