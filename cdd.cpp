/*
 * cdd.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include <iostream>

#include "Bruteforce/Bruteforce.h"
#include "Bruteforce/Pruner.h"
#include "BV/BV.h"
#include "Logging/logger.hpp"
#include "REST/REST.h"
#include "Term/TermFactory.h"
#include "Util/util.h"

int main(int argc, const char** argv) {

    using namespace borealis;

    REST rest;

    auto problems = rest.getProblems();

    sleep(20);

    for (const auto& problem : problems) {

        std::cout << problem << std::endl;

        if (problem.size > 8) continue;
        if (problem.solved) continue;
        if (problem.timeLeft <= 0.5) continue;
        if (borealis::util::contains(problem.operators, "if0") ||
            borealis::util::contains(problem.operators, "fold") ||
            borealis::util::contains(problem.operators, "tfold")) continue;

        auto id = problem.id;
        size_t size = problem.size;
        auto components = problem.operators;

        std::cout << "Solving:" << std::endl
                  << id << ":" << size << ":" << components << std::endl;

        Bruteforcer b(TermFactory::get(), components);
        Pruner p(id, components, TermFactory::get(), 4);

        auto vars = b.doit(size);

        for (const auto& var : vars) {
            if (p.test(var)) {
                std::cout << "Submitting:" << std::endl
                          << id << std::endl
                          << var->toString() << std::endl;

                auto response = rest.tryGuess(id, var);

                if ("win" == response.status) {
                    std::cout << "YAY!!!" << std::endl;
                    break;
                } else {

                    std::cout << "Nay..." << std::endl;

                    sleep(4);
                    continue;

                }

                return 0;
            }
        }

        sleep(15);

    }

    return 0;
}
