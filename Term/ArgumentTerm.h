/*
 * ArgumentTerm.h
 *
 *  Created on: Nov 19, 2012
 *      Author: ice-phoenix
 */

#ifndef ARGUMENTTERM_H_
#define ARGUMENTTERM_H_

#include "Term/Term.h"

#include "Util/macros.h"

namespace borealis {

class ArgumentTerm: public borealis::Term {

    unsigned int idx;

    ArgumentTerm(unsigned int idx) :
        Term(
            class_tag(*this),
            "arg_" + util::toString(idx)
        ), idx(idx) {}

public:

    MK_COMMON_TERM_IMPL(ArgumentTerm);

    unsigned getIdx() const { return idx; }

    template<class Sub>
    auto accept(Transformer<Sub>*) const -> const Self* {
        return new Self( *this );
    }

    virtual BV eval(const BoundVars& env) const override {
        ASSERTC(borealis::util::containsKey(env, idx));
        return env.at(idx);
    }

    virtual unsigned char size() const override {
        return 1;
    }

    virtual bool equals(const Term* other) const override {
        if (const Self* that = llvm::dyn_cast_or_null<Self>(other)) {
            return Term::equals(other) &&
                    that->idx == idx;
        } else return false;
    }

    virtual size_t hashCode() const override {
        return util::hash::defaultHasher()(Term::hashCode(), idx);
    }

};

} /* namespace borealis */

#include "Util/unmacros.h"

#endif /* ARGUMENTTERM_H_ */
