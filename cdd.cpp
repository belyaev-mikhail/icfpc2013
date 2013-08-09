/*
 * cdd.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include "Bruteforce/Bruteforce.h"
#include "BV/BV.h"
#include "Logging/logger.hpp"
#include "REST/REST.h"
#include "Term/TermFactory.h"
#include "Util/util.h"

int main(int argc, const char** argv) {

    using namespace borealis;

    Bruteforcer b(TermFactory::get(), {"not", "and", "or", "xor"});

    for (const auto& l : b.doit(6)) {
        std::cout << l->toString() << std::endl;
    }

    return 0;
}
