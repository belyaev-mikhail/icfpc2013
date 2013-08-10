/*
 * Bruteforce.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include "Bruteforce/RandomBruteforce.h"

namespace borealis {

RandomBruteforcer::RandomBruteforcer(TermFactory::Ptr TF, const std::set<std::string>& components, const std::list<Term::Ptr>& terminals) :
    TF(TF),
    terminals(terminals.begin(), terminals.end()),
    components(components.begin(), components.end(), component_compare()),
    totalSize(components.size()),
    currentComponents(),
    hasTFold(borealis::util::contains(components, "tfold")),
    inFold(false),
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

} // namespace borealis
