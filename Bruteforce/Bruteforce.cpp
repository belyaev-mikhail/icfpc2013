/*
 * Bruteforce.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include "Bruteforce/Bruteforce.h"

namespace borealis {

Bruteforcer::Bruteforcer(TermFactory::Ptr TF, const std::set<std::string>& components) :
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

} // namespace borealis
