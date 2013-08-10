/*
 * Pruner.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include "Bruteforce/Pruner.h"

namespace borealis {

Pruner::Pruner(const std::string& id, const std::set<std::string>& components, TermFactory::Ptr TF, size_t buckets) :
        components(components), TF(TF) {

    for (auto i = 0U; i < buckets; ++i) {
        std::vector<BV> bucket;
        for (auto j = 0U; j < 256; ++j) {
            bucket.push_back(random());
        }

        auto eval = REST().getEvalById(id, bucket);

        args.push_back(bucket);
        outputs.push_back(eval.outputs);
    }

}

} // namespace borealis
