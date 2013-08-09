/*
 * TermFactory.h
 *
 *  Created on: Nov 19, 2012
 *      Author: ice-phoenix
 */

#ifndef TERMFACTORY_H_
#define TERMFACTORY_H_

#include <memory>

#include "BV/BV.h"
#include "Term/Term.def"

#include "Util/macros.h"

namespace borealis {

class TermFactory {

public:

    typedef std::shared_ptr<TermFactory> Ptr;

    Term::Ptr getArgumentTerm(unsigned char idx) {
        return Term::Ptr{
            new ArgumentTerm(idx)
        };
    }

    Term::Ptr getOne() {
        return Term::Ptr{
            new OpaqueIntConstantTerm(1)
        };
    }

    Term::Ptr getZero() {
        return Term::Ptr{
            new OpaqueIntConstantTerm(0)
        };
    }

    Term::Ptr getFoldTerm(Term::Ptr arg1, Term::Ptr arg2, Term::Ptr body) {
        return Term::Ptr{
            new FoldTerm(
                arg1, arg2, body
            )
        };
    }

    Term::Ptr getLambdaTerm(Term::Ptr body) {
        return Term::Ptr{
            new LambdaTerm(
                body
            )
        };
    }

    Term::Ptr getTernaryTerm(Term::Ptr cnd, Term::Ptr tru, Term::Ptr fls) {
        return Term::Ptr{
            new TernaryTerm(
                cnd, tru, fls
            )
        };
    }

    Term::Ptr getBinaryTerm(ArithType opc, Term::Ptr lhv, Term::Ptr rhv) {
        return Term::Ptr{
            new BinaryTerm(
                opc, lhv, rhv
            )
        };
    }

    Term::Ptr getUnaryTerm(UnaryArithType opc, Term::Ptr rhv) {
        return Term::Ptr{
            new UnaryTerm(
                opc, rhv
            )
        };
    }

    static TermFactory::Ptr get() {
        return TermFactory::Ptr{
            new TermFactory()
        };
    }

    static BV apply(Term::Ptr lambda, BV arg) {
        BoundVars env;
        env[0] = arg;
        return lambda->eval(env);
    }

private:

    TermFactory();

};

} /* namespace borealis */

#include "Util/unmacros.h"

#endif /* TERMFACTORY_H_ */
