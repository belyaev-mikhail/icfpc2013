/*
 * BinaryTerm.h
 *
 *  Created on: Jan 17, 2013
 *      Author: belyaev
 */

#ifndef BINARYTERM_H_
#define BINARYTERM_H_

#include "Term/Term.h"

namespace borealis {

enum class ArithType {
    AND,
    OR,
    XOR,
    PLUS
};

inline std::string arithString(ArithType at) {
    switch (at) {
    case ArithType::AND:  return "and";
    case ArithType::OR:   return "or";
    case ArithType::XOR:  return "xor";
    case ArithType::PLUS: return "plus";
    }
}

class BinaryTerm: public borealis::Term {

    ArithType opcode;
    Term::Ptr lhv;
    Term::Ptr rhv;

    BinaryTerm(ArithType opcode, Term::Ptr lhv, Term::Ptr rhv):
        Term(
            class_tag(*this),
            "(" + arithString(opcode) + " " + lhv->toString() + " " + rhv->toString() + ")"
        ), opcode(opcode), lhv(lhv), rhv(rhv) {};

public:

    MK_COMMON_TERM_IMPL(BinaryTerm);

    ArithType getOpcode() const { return opcode; }
    Term::Ptr getLhv() const { return lhv; }
    Term::Ptr getRhv() const { return rhv; }

    template<class Sub>
    auto accept(Transformer<Sub>* tr) const -> const Self* {
        auto _lhv = tr->transform(lhv);
        auto _rhv = tr->transform(rhv);
        return new Self{ opcode, _lhv, _rhv };
    }

    virtual BV eval(const BoundVars& env) const override {
        switch (opcode) {
        case ArithType::AND:  return lhv->eval(env) & rhv->eval(env);
        case ArithType::OR:   return lhv->eval(env) | rhv->eval(env);
        case ArithType::XOR:  return lhv->eval(env) ^ rhv->eval(env);
        case ArithType::PLUS: return lhv->eval(env) + rhv->eval(env);
        }
    }

    virtual unsigned char size() const override {
        return 1 + lhv->size() + rhv->size();
    }

    virtual bool equals(const Term* other) const override {
        if (const Self* that = llvm::dyn_cast_or_null<Self>(other)) {
            return Term::equals(other) &&
                    that->opcode == opcode &&
                    *that->lhv == *lhv &&
                    *that->rhv == *rhv;
        } else return false;
    }

    virtual size_t hashCode() const override {
        return util::hash::defaultHasher()(Term::hashCode(), opcode, lhv, rhv);
    }

};

} // namespace borealis

namespace std {
template<>
struct std::hash<borealis::ArithType> : public borealis::util::enums::enum_hash<borealis::ArithType> {};
} // namespace std

#endif /* BINARYTERM_H_ */
