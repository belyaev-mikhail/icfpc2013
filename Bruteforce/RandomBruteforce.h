/*
 * RandomBruteforce.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef RANDOMBRUTEFORCE_H_
#define RANDOMBRUTEFORCE_H_

#include <set>

#include "Term/Term.h"
#include "Term/TermFactory.h"
#include "Util/util.h"

#include "Util/macros.h"

namespace borealis {

class RandomBruteforcer {

    TermFactory::Ptr TF;

    std::vector<Term::Ptr> terminals;

    struct component_compare {
        bool operator() (const std::string& a, const std::string& b) const {
            if (a == b) return false;

            if ("if0" == a) return false;
            if ("if0" == b) return true;

            return a < b;
        }
    };

    std::set<std::string, component_compare> components;
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

    bool inIf;

    typedef std::list<Term::Ptr> Variants;

    std::unordered_map<std::string, UnaryArithType> nameToUnary;
    std::unordered_map<std::string, ArithType> nameToBinary;

public:

    Variants doit(int size, bool ifOnly = false) {

        Variants subres;

        if (hasTFold) {
            currentComponents.push_back("tfold");
            subres = generateComponent("tfold", size - 1);
            currentComponents.pop_back();
        } else if (ifOnly) {
            currentComponents.push_back("tif0");
            subres = generateComponent("tif0", size - 1);
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

    Variants generate(int size) {
        if (size < 1) return Variants();

        if (size == 1) {

            if (size == 1) {
                if (inFold) {
                    auto r = rand() % 5;
                    return r == 0 ? Variants{ TF->getZero() } :
                           r == 1 ? Variants{ TF->getOne() } :
                           r == 2 ? Variants{ TF->getArgumentTerm(0) } :
                           r == 3 ? Variants{ TF->getArgumentTerm(1) } :
                                    Variants{ TF->getArgumentTerm(2) };
                } else {
                    auto r = rand() % 3;
                    return r == 0 ? Variants{ TF->getZero() } :
                           r == 1 ? Variants{ TF->getOne() } :
                                    Variants{ TF->getArgumentTerm(0) };
                }
            }
        }

        // if (sizeLeft() >= size) return Variants();

        Variants res;

        for (const auto& c : components) {
            currentComponents.push_back(c);
            auto subres = generateComponent(c, size);
            currentComponents.pop_back();

            for (auto& s : subres) {
                res.push_back(std::move(s));
            }
        }

        return std::move(res);
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
            if (size < 3) return vars;

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
        if (inIf) return vars;
        inIf = true;

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

        inIf = false;
        return vars;

    } else if ("tif0" == name) {

        Variants vars;
        if (inIf) return vars;
        inIf = true;

        auto tt = rand() % terminals.size();
        auto ff = rand() % terminals.size();

        auto cnds = size - 1 - terminals[tt]->size() - terminals[ff]->size();

        if (cnds <= 0) return vars;

        auto cnd = generate(cnds);

        for (const auto& c : cnd) {
            vars.push_back(
                TF->getTernaryTerm(c, terminals[tt], terminals[ff])
            );
        }

        inIf = false;
        return vars;

    } else {
            ASSERT(false, "Oh shit...");
        }
    }

public:

    RandomBruteforcer(TermFactory::Ptr TF, const std::set<std::string>& components, const std::list<Term::Ptr>& terminals);

};

} // namespace borealis

#include "Util/unmacros.h"

#endif /* RANDOMBRUTEFORCE_H_ */
