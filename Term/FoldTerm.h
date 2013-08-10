/*
 * FoldTerm.h
 *
 *  Created on: Jan 17, 2013
 *      Author: belyaev
 */

#ifndef FOLDTERM_H_
#define FOLDTERM_H_

#include "Term/Term.h"

namespace borealis {

class FoldTerm: public borealis::Term {

    Term::Ptr arg1;
    Term::Ptr arg2;
    Term::Ptr body;
    std::string type;

    FoldTerm(Term::Ptr arg1, Term::Ptr arg2, Term::Ptr body, const std::string& type = "fold"):
        Term(
            class_tag(*this),
            "(fold " +
                arg1->toString() + " " +
                arg2->toString() + " " +
                "(lambda (arg_1 arg_2) " +
                    body->toString() +
                ")" +
            ")"
        ), arg1(arg1), arg2(arg2), body(body), type(type) {};

public:

    MK_COMMON_TERM_IMPL(FoldTerm);

    Term::Ptr getArg1() const { return arg1; }
    Term::Ptr getArg2() const { return arg2; }
    Term::Ptr getBody() const { return body; }

    template<class Sub>
    auto accept(Transformer<Sub>* tr) const -> const Self* {
        auto _arg1 = tr->transform(arg1);
        auto _arg2 = tr->transform(arg2);
        auto _body = tr->transform(body);
        return new Self{ _arg1, _arg2, _body };
    }

    virtual BV eval(const BoundVars& env) const override {
        BV arg1_ = arg1->eval(env);
        BV arg2_ = arg2->eval(env);

        auto acc = arg2_;
        auto shifted = arg1_;
        auto byte = shifted & 0xFF;

        for (unsigned char i = 0; i < 8; ++i) {
            BoundVars nestedEnv(env);
            nestedEnv[1] = byte;
            nestedEnv[2] = acc;

            acc = body->eval(nestedEnv);
            shifted = shifted >> 8;
            byte = shifted & 0xFF;
        }

        return acc;
    }

    virtual void registerComponents(std::set<std::string>& c) const override {
        c.insert(type);

        arg1->registerComponents(c);
        arg2->registerComponents(c);
        body->registerComponents(c);
    }

    virtual unsigned char size() const override {
        return 2 + arg1->size() + arg2->size() + body->size();
    }

    virtual bool equals(const Term* other) const override {
        if (const Self* that = llvm::dyn_cast_or_null<Self>(other)) {
            return Term::equals(other) &&
                    *that->arg1 == *arg1 &&
                    *that->arg2 == *arg2 &&
                    *that->body == *body;
        } else return false;
    }

    virtual size_t hashCode() const override {
        return util::hash::defaultHasher()(Term::hashCode(), arg1, arg2, body);
    }

};

} /* namespace borealis */

#endif /* FOLDTERM_H_ */
