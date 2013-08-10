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

    struct component_compare {
        bool operator() (const std::string& a, const std::string& b) const {
            if (a == b) return false;

            if ("if0" == a) return false;
            if ("if0" == b) return true;

            return a < b;
        }
    };

    const std::set<std::string, component_compare> components;
    int totalSize;

    std::list<std::string> currentComponents;
    int sizeLeft() {
        std::set<std::string> curr(currentComponents.begin(), currentComponents.end());

        std::vector<std::string> diff;
        std::set_difference(
            components.begin(), components.end(),
            curr.begin(), curr.end(),
            std::back_inserter(diff)
        );

        int res;
        for (const auto& d : diff) {
            if ("tfold" == d || "fold" == d) res += 2;
            else res += 1;
        }
        return res;
    }

    bool hasTFold;

    bool inFold;

    typedef std::list<Term::Ptr> Variants;

    std::unordered_map<std::string, UnaryArithType> nameToUnary;
    std::unordered_map<std::string, ArithType> nameToBinary;

public:

    bool allComponentsUsed() {
        for (const auto& c : components) {
            auto r = std::find(currentComponents.begin(), currentComponents.end(), c);
            if (r == currentComponents.end()) {
                return false;
            }

        }
        return true;
    }

    Variants doit(int size) {

        Variants subres;

        if (hasTFold) {
            currentComponents.push_back("tfold");
            subres = generateComponent("tfold", size - 1);
            currentComponents.pop_back();
        } else {
            subres = generate(size - 1);
        }

        Variants vars;
        std::transform(subres.begin(), subres.end(), std::back_inserter(vars),
            [this](const Term::Ptr& p) { return TF->getLambdaTerm(p); }
        );
        return vars;
    }

private:

    bool isFinal(std::list<std::string> l) {
        //std::cout << "final?" << l << std::endl;
        auto cnt = 0;
        l.reverse();
        for(const auto & i : l) {
            if (i == "0") {
                cnt += 1;
            } else if (borealis::util::containsKey(nameToBinary, i)) {
                cnt -= 1;
            } else if (i == "if0" || i == "fold" || i == "tfold") {
                cnt -= 2;
            }
        }
        //std::cout << cnt << std::endl;
        return cnt == 1;

    }


    /**----------*/

    Variants generate(int size) {
        if (size < 1) return Variants();

        if (size == 1) {

            currentComponents.push_back("0");
            //std::cout << currentComponents << std::endl;
//            if (isFinal(currentComponents)) {
//                if (!allComponentsUsed()) {
//                    return Variants();
//                }
//            }

            return inFold ? Variants{
                TF->getZero(),
                TF->getOne(),
                TF->getArgumentTerm(0),
                TF->getArgumentTerm(1),
                TF->getArgumentTerm(2)
            } : Variants{
                TF->getZero(),
                TF->getOne(),
                TF->getArgumentTerm(0)
            };
        }

        // if (sizeLeft() >= size) return Variants();

        std::list<Term::Ptr> res;
        for (const auto& c : components) {
            currentComponents.push_back(c);
            auto subres = generateComponent(c, size);
            while (currentComponents.back() == "0") {
                currentComponents.pop_back();
            }
            currentComponents.pop_back();

            res.splice(res.begin(), subres);
        }
        return res;
    }

    Variants generateComponent(const std::string& name, int size) {
        if ("fold" == name) {
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

            // min: 1 + 1 + 1 = 3
            if (size < 3) {
                return vars;
            }

            for (auto arg0s = 1; arg0s < (size - size/2); ++arg0s) {
                auto arg1s = size - arg0s - 1;

                if (arg1s <= 0) continue;

                auto lhv = generate(arg0s);
                auto rhv = generate(arg1s);

                for (const auto& l : lhv) {
                    for (const auto& r : rhv) {
                        vars.push_back(
                            TF->getBinaryTerm(type, l, r)
                        );
                    }
                }
            }

            return vars;

        } else if ("fold" == name) {

            Variants vars;
            if (inFold) return vars;
            inFold = true;

            // min: 2 + 1 + 1 + 1 = 5
            if (size < 5) return vars;

            for (auto e0s = 1; e0s < size; ++e0s) {
                for (auto e1s = 1; e1s < size - e0s; ++e1s) {
                    auto bodys = size - e0s - e1s - 2;

                    if (bodys <= 0) continue;

                    auto e0 =   generate(e0s);
                    auto e1 =   generate(e1s);
                    auto body = generate(bodys);

                    for (const auto& arg0 : e0) {
                        for (const auto& arg1 : e1) {
                            for (const auto& b : body) {
                                vars.push_back(
                                    TF->getFoldTerm(arg0, arg1, b)
                                );
                            }
                        }
                    }
                }
            }

            inFold = false;
            return vars;

    } else if ("tfold" == name) {

        Variants vars;
        if (inFold) return vars;
        inFold = true;

        // min: 2 + 1 + 1 + 1 = 5
        if (size < 5) return vars;

        auto bodys = size - 1 - 1 - 2;

        auto e0 = TF->getArgumentTerm(0);
        auto e1 = TF->getZero();
        auto body = generate(bodys);

        for (const auto& b : body) {
            vars.push_back(
                TF->getTFoldTerm(e0, e1, b)
            );
        }

        inFold = false;
        return vars;

    } else if ("if0" == name) {

        Variants vars;

        // min: 1 + 1 + 1 + 1 = 4
        if (size < 4) return vars;

        for (auto trus = 1; trus < size; ++trus) {
            for (auto flss = 1; flss < size - trus; ++flss) {
                auto cnds = size - trus - flss - 1;

                if (cnds <= 0) continue;

                auto cnd = generate(cnds);
                auto tru = generate(trus);
                auto fls = generate(flss);

                for (const auto& c : cnd) {
                    for (const auto& t : tru) {
                        for (const auto& f : fls) {
                            vars.push_back(
                                TF->getTernaryTerm(c, t, f)
                            );
                        }
                    }
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
