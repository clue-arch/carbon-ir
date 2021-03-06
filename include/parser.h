#pragma once

#include <core.h>
#include <scanner.h>
#include <generator.h>
#include <optimizer/optimizer.h>
#include <type.h>
#include <oplist.h>
#include <function.h>

class Parser
{
private:
    Scanner m_scanner;
    Generator *m_generator;
    Optimizer *m_optimizer;

    RegisterList m_regList;
    std::vector<Function> m_functions;

    std::string m_asmFile;

private:
    Type parseType();
    ArrayType parseArrayType();
    std::vector<LARGEINT> parseArrayInit(int amount);
    
    int parsePrimary(Type t, bool shouldBeReg = false);
    OpQuad *parseBinOperator();
    OpQuad *parseOperation();
    OpQuad *parseInitialize();
    OpQuad *parseAssign();
    OpQuad *parseStatement();
    OpQuad *parseReturn();
    OpQuad *parseFunctionCall();
    OpQuad *parseStore();
    OpQuad *parseLoad();
    OpQuad *parseAlloca();
    OpQuad *parseJmpCond();
    OpQuad *parseLabel();
    OpQuad *parseJmp();
    OpQuad *parseCmp();
    OpList parseFunction();
    
    void parseGlobal();


public:
    Parser(Scanner &scan, Generator *gen, Optimizer *opt, std::string asmFile);
    Parser(Generator *gen, Optimizer *opt);
    
    Generator *generator() { return m_generator; }
    Optimizer *optimizer() { return m_optimizer; }
    std::vector<Function> &functions() { return m_functions; }

    void parse();
    int addFunction(Function func);
    int addRegister(int r, Type t);
    void generateFunction(OpList statements);
};