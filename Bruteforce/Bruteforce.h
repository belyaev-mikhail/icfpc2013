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
    std::set<std::string> currentComponents;

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
        if (size < 1) return Variants();

        if (size <= currentComponents.size()) return Variants();

        if (size == 1) return Variants{
            TF->getZero(),
            TF->getOne(),
            TF->getArgumentTerm(0)
        };

        std::list<Term::Ptr> res;
        for (const auto& c : components) {
            currentComponents.insert(c);
            auto subres = generateComponent(c, size);
            currentComponents.erase(c);

            res.insert(res.begin(), subres.begin(), subres.end());
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

            Variants vars;

            if (size <= 2) return vars;

            for (size_t s = 0; s < size; ++s) {
                auto lhv = generate(size - s - 1);
                auto rhv = generate(s);
                if (lhv.empty() || rhv.empty()) continue;

                for (const auto& l : lhv) {
                    for (const auto& r : rhv) {
                        vars.push_back(
                            TF->getBinaryTerm(type, l, r)
                        );
                    }
                }
            }

            return vars;
        } else {
            ASSERT(false, "Oh shit...");
        }
    }

public:

    Bruteforcer(TermFactory::Ptr TF, const std::set<std::string>& components);

};

} // namespace borealis

#include "Util/unmacros.h"

#endif /* BRUTEFORCE_H_ */
