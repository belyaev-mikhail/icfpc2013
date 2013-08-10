/*
 * Pruner.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef PRUNER_H_
#define PRUNER_H_

#include <iostream>

#include "BV/BV.h"
#include "REST/REST.h"
#include "Term/Term.h"
#include "Term/TermFactory.h"

namespace borealis {

class Pruner {

    std::set<std::string> components;

    std::list<std::vector<BV>> args;
    std::list<std::vector<BV>> outputs;
    TermFactory::Ptr TF;

public:

    Pruner(const std::string& id, const std::set<std::string>& components, TermFactory::Ptr TF, size_t buckets);

    bool test(Term::Ptr lambda) const {

        std::set<std::string> c;
        lambda->registerComponents(c);

        if (c != components) return false;

        auto ait = args.begin();
        auto oit = outputs.begin();

        auto aend = args.end();
        auto oend = outputs.end();

        while (ait != aend && oit != oend) {
            auto& bucket = *ait++;
            auto& oBucket = *oit++;
            for (auto j = 0U; j < bucket.size(); ++j) {
                auto arg = bucket[j];
                auto expected = oBucket[j];
                auto actual = TF->apply(lambda, arg);
                if (expected != actual) return false;
            }
        }
        return true;
    }

    void reinforce(BV arg, BV output) {
        args.push_front({arg});
        outputs.push_front({output});
    }

};

} // namespace borealis

#endif /* PRUNER_H_ */
