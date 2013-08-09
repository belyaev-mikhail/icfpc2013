/*
 * LambdaTerm.h
 *
 *  Created on: Jan 17, 2013
 *      Author: belyaev
 */

#ifndef LAMBDATERM_H_
#define LAMBDATERM_H_

#include "Term/Term.h"

namespace borealis {

class LambdaTerm: public borealis::Term {

    Term::Ptr body;

    LambdaTerm(Term::Ptr body):
        Term(
            class_tag(*this),
            "(lambda (arg_0) " + body->toString() + ")"
        ), body(body) {};

public:

    MK_COMMON_TERM_IMPL(LambdaTerm);

    Term::Ptr getBody() const { return body; }

    template<class Sub>
    auto accept(Transformer<Sub>* tr) const -> const Self* {
        auto _body = tr->transform(body);
        return new Self{ _body };
    }

    virtual BV eval(const BoundVars& env) const override {
        return body->eval(env);
    }

    virtual unsigned char size() const override {
        return 1 + body->size();
    }

    virtual void registerComponents(std::set<std::string>& c) const override {
        body->registerComponents(c);
    }

    virtual bool equals(const Term* other) const override {
        if (const Self* that = llvm::dyn_cast_or_null<Self>(other)) {
            return Term::equals(other) &&
                    *that->body == *body;
        } else return false;
    }

    virtual size_t hashCode() const override {
        return util::hash::defaultHasher()(Term::hashCode(), body);
    }

};

} /* namespace borealis */

#endif /* LAMBDATERM_H_ */
