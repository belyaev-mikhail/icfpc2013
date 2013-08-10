/*
 * Bruteforce.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include "Bruteforce/NewBruteforce.h"

namespace borealis {

NewBruteforcer::NewBruteforcer(const std::set<std::string>& components) :
    components(components.begin(), components.end(), component_compare()),
    totalSize(components.size()),
    currentComponents(),
    hasTFold(borealis::util::contains(components, "tfold")),
    inFold(false),
    nameToUnary {
        {"not",   '!'},
        {"shl1",  'A'},
        {"shr1",  'B'},
        {"shr4",  'C'},
        {"shr16", 'D'}
    },
    nameToBinary {
        {"and",  '&'},
        {"or",   '|'},
        {"xor",  '^'},
        {"plus", '+'}
    } {};

} // namespace borealis
