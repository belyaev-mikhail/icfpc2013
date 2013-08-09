/*
 * OpaqueConstantTerm.h
 *
 *  Created on: Jan 17, 2013
 *      Author: belyaev
 */

#ifndef OPAQUEINTCONSTANTTERM_H_
#define OPAQUEINTCONSTANTTERM_H_

#include "Term/Term.h"

namespace borealis {

class OpaqueIntConstantTerm: public borealis::Term {

    BV value;

    OpaqueIntConstantTerm(uint64_t value):
        Term(
            class_tag(*this),
            util::toString(value)
        ), value(value) {};

public:

    MK_COMMON_TERM_IMPL(OpaqueIntConstantTerm);

    long long getValue() const { return value; }

    template<class Sub>
    auto accept(Transformer<Sub>*) const -> const Self* {
        return new Self( *this );
    }

    virtual BV eval(const BoundVars&) const override {
        return value;
    }

    virtual unsigned char size() const override {
        return 1;
    }

};

} /* namespace borealis */

#endif /* OPAQUEINTCONSTANTTERM_H_ */
