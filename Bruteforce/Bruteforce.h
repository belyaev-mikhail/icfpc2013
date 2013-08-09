/*
 * Bruteforce.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef BRUTEFORCE_H_
#define BRUTEFORCE_H_

#include <set>

#include "Term/Term.h"
#include "Term/TermFactory.h"
#include "Util/util.h"

#include "Util/macros.h"

namespace borealis {

class Bruteforcer {

    TermFactory::Ptr TF;
    std::set<std::string> components;

    typedef std::list<Term::Ptr> Variants;

    std::unordered_map<std::string, UnaryArithType> nameToUnary;
    std::unordered_map<std::string, ArithType> nameToBinary;

public:

    Variants doit(size_t size) {
        auto subres = generate(size - 1);
        Variants vars;
        std::transform(subres.begin(), subres.end(), std::back_inserter(vars),
            [this](const Term::Ptr& p) { return TF->getLambdaTerm(p); }
        );
        return vars;
    }

private:

    Variants generate(size_t size) {
        if (size < 1) return std::list<Term::Ptr>();

        if (size == 1) return std::list<Term::Ptr>{
            TF->getZero(),
            TF->getOne(),
            TF->getArgumentTerm(0)
        };

        std::list<Term::Ptr> res;
        for (const auto& c : components) {
            auto subres = generateComponent(c, size);
            res.splice(res.begin(), subres);
        }
        return res;
    }

    Variants generateComponent(const std::string& name, size_t size) {
        if ("if0" == name ||
            "fold" == name) {
            ASSERT(false, "Fuck off!");

        } else if (borealis::util::containsKey(nameToUnary, name)) {
            auto type = nameToUnary.at(name);

            auto subres = generate(size - 1);

            Variants vars;
            std::transform(subres.begin(), subres.end(), std::back_inserter(vars),
                [this,&type](const Term::Ptr& p) { return TF->getUnaryTerm(type, p); }
            );
            return vars;

        } else if (borealis::util::containsKey(nameToBinary, name)) {
            auto type = nameToBinary.at(name);

            Variants lhv;
            Variants rhv;
            Variants vars;

            if (size <= 2) return vars;

            for (size_t s = 1; s < size; ++s) {
                auto l = generate(size - 1 - s);
                auto r = generate(s);
                lhv.splice(lhv.begin(), l);
                rhv.splice(rhv.begin(), r);
            }

            for (const auto& l : lhv) {
                for (const auto& r : rhv) {
                    vars.push_back(
                        TF->getBinaryTerm(type, l, r)
                    );
                }
            }
            return vars;
        }
    }

public:

    Bruteforcer(TermFactory::Ptr TF, const std::set<std::string>& components) :
        TF(TF), components(components),
        nameToUnary {
            {"not",   UnaryArithType::NOT},
            {"shl1",  UnaryArithType::SHL_1},
            {"shr1",  UnaryArithType::SHR_1},
            {"shr4",  UnaryArithType::SHR_4},
            {"shr16", UnaryArithType::SHR_16}
        },
        nameToBinary {
            {"and",  ArithType::AND},
            {"or",   ArithType::OR},
            {"xor",  ArithType::XOR},
            {"plus", ArithType::PLUS}
        } {};

};

} // namespace borealis

#include "Util/unmacros.h"

#endif /* BRUTEFORCE_H_ */
