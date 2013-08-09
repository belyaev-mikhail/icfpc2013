/*
 * TernaryTerm.h
 *
 *  Created on: Jan 17, 2013
 *      Author: belyaev
 */

#ifndef TERNARYTERM_H_
#define TERNARYTERM_H_

#include "Term/Term.h"

namespace borealis {

class TernaryTerm: public borealis::Term {

    Term::Ptr cnd;
    Term::Ptr tru;
    Term::Ptr fls;

    TernaryTerm(Term::Ptr cnd, Term::Ptr tru, Term::Ptr fls):
        Term(
            class_tag(*this),
            "(if0 " + cnd->toString() + " " + tru->toString() + " " + fls->toString() + ")"
        ), cnd(cnd), tru(tru), fls(fls) {};

public:

    MK_COMMON_TERM_IMPL(TernaryTerm);

    Term::Ptr getCnd() const { return cnd; }
    Term::Ptr getTru() const { return tru; }
    Term::Ptr getFls() const { return fls; }

    template<class Sub>
    auto accept(Transformer<Sub>* tr) const -> const Self* {
        auto _cnd = tr->transform(cnd);
        auto _tru = tr->transform(tru);
        auto _fls = tr->transform(fls);
        return new Self{ _cnd, _tru, _fls };
    }

    virtual BV eval(const BoundVars& env) const override {
        if (cnd->eval(env)) {
            return tru->eval(env);
        } else {
            return fls->eval(env);
        }
    }

    virtual unsigned char size() const override {
        return 1 + cnd->size() + tru->size() + fls->size();
    }

    virtual void registerComponents(std::set<std::string>& c) const override {
        c.insert("if0");

        cnd->registerComponents(c);
        tru->registerComponents(c);
        fls->registerComponents(c);
    }

    virtual bool equals(const Term* other) const override {
        if (const Self* that = llvm::dyn_cast_or_null<Self>(other)) {
            return Term::equals(other) &&
                    *that->cnd == *cnd &&
                    *that->tru == *tru &&
                    *that->fls == *fls;
        } else return false;
    }

    virtual size_t hashCode() const override {
        return util::hash::defaultHasher()(Term::hashCode(), cnd, tru, fls);
    }

};

} /* namespace borealis */

#endif /* TERNARYTERM_H_ */
