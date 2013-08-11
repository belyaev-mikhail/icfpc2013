/*
 * NewBruteforce.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef NEWBRUTEFORCE_H_
#define NEWBRUTEFORCE_H_

#include <set>

#include "Util/util.h"

#include "Util/macros.h"

namespace borealis {

using borealis::util::uniq;

class NewBruteforcer {

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

    typedef std::vector< std::string > Variants;

    std::unordered_map<std::string, char> nameToUnary;
    std::unordered_map<std::string, char> nameToBinary;

public:

    Variants doit(int size) {

        Variants subres;

        if (hasTFold) {
            currentComponents.push_back("tfold");
            subres = generateComponent("tfold", size - 1);
            currentComponents.pop_back();
        } else {
            subres = generate(size - 1);
        }

        return std::move(subres);
    }

private:

    Variants generate(int size) {
        if (size < 1) return Variants();

        if (size == 1) {
            currentComponents.push_back("0");

            return inFold ? Variants{
                std::string{"0"},
                std::string{"1"},
                std::string{"x"},
                std::string{"y"},
                std::string{"z"},
            } : Variants{
                std::string{"0"},
                std::string{"1"},
                std::string{"x"},
            };
        }

        // if (sizeLeft() >= size) return Variants();

        Variants res;
        for (const auto& c : components) {
            currentComponents.push_back(c);
            auto subres = generateComponent(c, size);
            while (currentComponents.back() == "0") {
                currentComponents.pop_back();
            }
            currentComponents.pop_back();

            res.reserve(subres.size());
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
            vars.reserve(subres.size());
            for (auto& p : subres) {

                p.reserve(30);
                p += type;
                vars.push_back(std::move(p));
            }

            return std::move(vars);

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

                vars.reserve(vars.size() + lhv.size() * rhv.size());
                for (auto& l : lhv) {
                    for (auto& r : rhv) {

                        r.reserve(30);
                        r += l;
                        r += type;

                        vars.push_back(std::move(r));
                    }
                }
            }

            return std::move(vars);

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

                    vars.reserve(vars.size() + e0.size() * e1.size() * body.size());
                    for (auto& arg0 : e0) {
                        for (auto& arg1 : e1) {
                            for (auto& b : body) {

                                b.reserve(30);
                                b += arg1;
                                b += arg0;
                                b += "f";

                                vars.push_back(std::move(b));
                            }
                        }
                    }
                }
            }

            inFold = false;
            return std::move(vars);

    } else if ("tfold" == name) {

        Variants vars;
        if (inFold) return vars;
        inFold = true;

        // min: 2 + 1 + 1 + 1 = 5
        if (size < 5) return vars;

        auto bodys = size - 1 - 1 - 2;

        auto body = generate(bodys);

        vars.reserve(body.size());
        for (auto& b : body) {

            b.reserve(30);
            b += '0';
            b += 'x';
            b += 't';

            vars.push_back(std::move(b));
        }

        inFold = false;
        return std::move(vars);

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

                vars.reserve(vars.size() + cnd.size() * tru.size() * fls.size());
                for (auto& c : cnd) {
                    for (auto& t : tru) {
                        for (auto& f : fls) {

                            f.reserve(30);
                            f += t;
                            f += c;
                            f += "f";

                            vars.push_back(std::move(f));
                        }
                    }
                }
            }
        }

        return std::move(vars);

    } else {
            ASSERT(false, "Oh shit...");
        }
    }

public:

    NewBruteforcer(const std::set<std::string>& components);

};

} // namespace borealis

#include "Util/unmacros.h"

#endif /* NEWBRUTEFORCE_H_ */
