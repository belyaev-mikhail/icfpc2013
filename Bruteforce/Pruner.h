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

    std::vector<std::vector<BV>> args;
    std::vector<std::vector<BV>> outputs;
    TermFactory::Ptr TF;

public:

    Pruner(const std::string& id, const std::set<std::string>& components, TermFactory::Ptr TF, size_t buckets);

    bool test(Term::Ptr lambda) const {

        std::set<std::string> c;
        lambda->registerComponents(c);

        if (c != components) return false;

        for (int i = 0; i < args.size(); ++i) {
            auto bucket = args[i];
            auto oBucket = outputs[i];
            for (int j = 0; j < bucket.size(); ++j) {
                auto arg = bucket[j];
                auto expected = oBucket[j];
                auto actual = TF->apply(lambda, arg);
                if (expected != actual) return false;
            }
        }
        return true;
    }

};

} // namespace borealis

#endif /* PRUNER_H_ */
