/*
 * cdd.cpp
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#include "BV/BV.h"
#include "Logging/logger.hpp"
#include "REST/REST.h"
#include "Term/TermFactory.h"
#include "Util/util.h"

int main(int argc, const char** argv) {

    using namespace borealis;

    auto TF = TermFactory::get();
    REST rest;

    auto lambda = TF->getLambdaTerm(
        TF->getUnaryTerm(
            UnaryArithType::SHL_1,
            TF->getBinaryTerm(
                ArithType::PLUS,
                TF->getArgumentTerm(0),
                TF->getOne()
            )
        )
    );

    std::vector<BV> args;
    for (int i = 0; i < 256; ++i) {
        args.push_back(random());
    }

    auto evalResponse = rest.getEvalByProgram(lambda, args);

    for (int i = 0; i < 256; ++i) {
        auto arg = args[i];
        auto expected = evalResponse.outputs[i];
        auto actual = TF->apply(lambda, arg);
        std::cout << arg << " " << expected << " == " << actual << " is " << (expected == actual ? "true" : "FALSE") << std::endl;
    }



    auto trainId = "NxDtTDCVffP4CxXMnIrfAN76";
    auto trainLambda = TF->getLambdaTerm(
        TF->getUnaryTerm(
            UnaryArithType::SHL_1,
            TF->getArgumentTerm(0)
        )
    );

    auto guessResponse = rest.tryGuess(trainId, trainLambda);

    std::cout << guessResponse.status << std::endl;

    return 0;
}
