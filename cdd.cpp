/*
 * cdd.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include <iostream>

#include "Bruteforce/Bruteforce.h"
#include "Bruteforce/NewBruteforce.h"
#include "Bruteforce/RandomBruteforce.h"
#include "Bruteforce/Pruner.h"
#include "BV/BV.h"
#include "Logging/logger.hpp"
#include "REST/REST.h"
#include "Term/TermFactory.h"
#include "Util/util.h"

int main(int argc, const char** argv) {

    using namespace borealis;

    REST rest;
    std::vector<Problem> problems;

    if (argc == 1) {
        problems = rest.getProblems();
        sleep(20);
    } else {
        Problem p;

        p.solved = false;
        p.timeLeft = 300.0;

        p.id = argv[1];
        std::istringstream iss(argv[2]);
        iss >> p.size;
        for (size_t i = 3; i < argc; ++i) {
            p.operators.insert(argv[i]);
        }

        problems.push_back(p);
    }

    for (const auto& problem : problems) {

        std::cout << problem << std::endl;

        if (problem.size > 21) continue;
        if (problem.solved) continue;
        if (problem.timeLeft <= 0.5) continue;

        if (borealis::util::contains(problem.operators, "fold")) continue;

        auto id = problem.id;
        size_t size = problem.size;
        auto components = problem.operators;
        components.erase("bonus");

        std::cout << "Solving:" << std::endl
                  << id << ":" << size << ":" << components << std::endl;

        auto TF = TermFactory::get(true);
        Pruner p(id, components, TF, 4);

        std::list<Term::Ptr> cores;

        for (int coreSize = 1; coreSize < problem.size / 3; ++coreSize) {

            Bruteforcer coreBrute(TF, components);

            auto core = coreBrute.doitNaked(coreSize);
            core = p.prune(core);

            if (core.size() > 0) {
                std::cout << "Core size: " << coreSize << " matched " << core.size() << " times!" << std::endl;
                cores.splice(cores.begin(), core);
            }
        }

        if (cores.size() == 0) continue;

        RandomBruteforcer b(TF, components, cores);

        unsigned int tryCount = 0;
        unsigned int count = 0;

        while (tryCount < 256) {
            ++tryCount;

            auto vars = b.doit(size, true);

            for (const auto& var : vars) {
                if (p.test(var)) {
                    std::cout << "Submitting:" << std::endl
                              << id << std::endl
                              << var->toString() << std::endl;

                    auto response = rest.tryGuess(id, var);

                    if ("win" == response.status) {
                        std::cout << "YAY!!!" << std::endl;
                        tryCount = 1024;
                        break;
                    } else if ("mismatch" == response.status) {

                        std::cout << "Reinforcing with: " << std::endl
                                  << response.values[0] << std::endl
                                  << response.values[1] << std::endl
                                  << response.values[2] << std::endl;

                        p.reinforce(response.values[0], response.values[1]);

                        std::cout << "Nay..." << std::endl;

                        sleep(5);
                        continue;

                    } else {

                        std::cout << "Huh???" << std::endl;

                        sleep(4);
                        continue;
                    }
                }
            }

            count += vars.size();
            std::cout << "Tried: " << count << std::endl;

        } // while (tryCount < 16)

        std::cout << "Pausing..." << std::endl;
        sleep(15);

    }

    return 0;
}
