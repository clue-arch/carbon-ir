#include <api/carbon.h>

#include <core.h>
#include <parser.h>
#include <machine.h>
#include <generator.h>
#include <optimizer/optimizer.h>

struct ctype __i32_type = {.byteSize = 4, .ptr = 0, .isFloat = 0};

struct carbon init_carbon(const char *infile, const char *arch)
{
    struct carbon ret;
    Generator *gen;
    Optimizer *opt;
    std::tie(gen, opt) = getMachine(std::string(arch));
    ret.__parser = new Parser(gen, opt);
    ret.infile = infile;

    /// Initialisation
    gen->genSetupFile(std::string(infile));

    return ret;
}

void end_carbon(struct carbon carb)
{
    delete carb.__parser->optimizer();
    delete carb.__parser->generator();
    delete carb.__parser;
}

struct cfunc c_create_func(struct carbon carb, const char *name, struct ctype t, int argc,
                           void *arglist)
{
    struct cfunc func;

    func.name = name;
    func.__parser = carb.__parser;
    func.__statements = new OpList();

    Function fobj = Function(std::string(name), Type(t));
    for (int i = 0; i < argc; ++i)
    {
        Type arg = Type(((struct ctype *)arglist)[i]);
        fobj.args().push_back(arg);
    }

    func.findx = carb.__parser->addFunction(fobj);
    return func;
}

void c_destroy_func(struct cfunc func)
{
    delete func.__statements;
}

void c_add_func_attribute(struct cfunc f, const char *attrib)
{
    f.__parser->functions()[f.findx].attributes().push_back(std::string(attrib));
}

void c_gen_func(struct cfunc f)
{
    f.__parser->generator()->genFunction(f.__parser->functions()[f.findx]);
    f.__parser->generateFunction(*f.__statements);
}

void c_push_op(struct cfunc func, int op, int ar1, int ar2, int ret, struct ctype t)
{
    OpQuad *quad = new OpQuad(op, ar1, ar2, ret, Type(t));
    func.__statements->push_back(quad);
}

int c_reg(struct cfunc f, int r, struct ctype t)
{
    f.__statements->regList().addRegister(r, Type(t));
}

int c_writeassembly(struct carbon carb, const char *file)
{
    return carb.__parser->generator()->writeOutfile(std::string(file));
}

int c_assemble(struct carbon carb, const char *infile, const char *outfile,
               const char *assembler)
{
    if (!assembler)
        assembler = "";

    return carb.__parser->generator()->assemble(std::string(infile), std::string(outfile),
                                                std::string(assembler));
}

int c_link(struct carbon carb, const char *infile, const char *outfile,
           const char *linker)
{
    if (!linker)
        linker = "";

    return carb.__parser->generator()->link(std::string(infile), std::string(outfile),
                                            std::string(linker));
}