/*
 * UnaryTerm.h
 *
 *  Created on: Jan 17, 2013
 *      Author: belyaev
 */

#ifndef UNARYTERM_H_
#define UNARYTERM_H_

#include "Term/Term.h"

namespace borealis {

enum class UnaryArithType {
    NOT,
    SHL_1,
    SHR_1,
    SHR_4,
    SHR_16
};

inline std::string unaryArithString(UnaryArithType uat) {
    switch (uat) {
    case UnaryArithType::NOT:    return "not";
    case UnaryArithType::SHL_1:  return "shl1";
    case UnaryArithType::SHR_1:  return "shr1";
    case UnaryArithType::SHR_4:  return "shr4";
    case UnaryArithType::SHR_16: return "shr16";
    }
}

class UnaryTerm: public borealis::Term {

    UnaryArithType opcode;
    Term::Ptr rhv;

    UnaryTerm(UnaryArithType opcode, Term::Ptr rhv):
        Term(
            class_tag(*this),
            "(" + unaryArithString(opcode) + " " + rhv->toString() + ")"
        ), opcode(opcode), rhv(rhv) {};

public:

    MK_COMMON_TERM_IMPL(UnaryTerm);

    UnaryArithType getOpcode() const { return opcode; }
    Term::Ptr getRhv() const { return rhv; }

    template<class Sub>
    auto accept(Transformer<Sub>* tr) const -> const Self* {
        auto _rhv = tr->transform(rhv);
        return new Self{ opcode, _rhv };
    }

    virtual BV eval(const BoundVars& env) const override {
        switch (opcode) {
        case UnaryArithType::NOT:    return ~rhv->eval(env);
        case UnaryArithType::SHL_1:  return rhv->eval(env) << 1;
        case UnaryArithType::SHR_1:  return rhv->eval(env) >> 1;
        case UnaryArithType::SHR_4:  return rhv->eval(env) >> 4;
        case UnaryArithType::SHR_16: return rhv->eval(env) >> 16;
        }
    }

    virtual unsigned char size() const override {
        return 1 + rhv->size();
    }

    virtual void registerComponents(std::set<std::string>& c) const override {
        switch (opcode) {
        case UnaryArithType::NOT:    c.insert("not");   break;
        case UnaryArithType::SHL_1:  c.insert("shl1");  break;
        case UnaryArithType::SHR_1:  c.insert("shr1");  break;
        case UnaryArithType::SHR_4:  c.insert("shr4");  break;
        case UnaryArithType::SHR_16: c.insert("shr16"); break;
        }

        rhv->registerComponents(c);
    }

    virtual bool equals(const Term* other) const override {
        if (const Self* that = llvm::dyn_cast_or_null<Self>(other)) {
            return Term::equals(other) &&
                    that->opcode == opcode &&
                    *that->rhv == *rhv;
        } else return false;
    }

    virtual size_t hashCode() const override {
        return util::hash::defaultHasher()(Term::hashCode(), opcode, rhv);
    }

};

} /* namespace borealis */

namespace std {
template<>
struct std::hash<borealis::UnaryArithType> : public borealis::util::enums::enum_hash<borealis::UnaryArithType> {};
} // namespace std

#endif /* UNARYTERM_H_ */
