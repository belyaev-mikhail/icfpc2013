/*
 * test_term.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: ice-phoenix
 */

#include <gtest/gtest.h>

#include "REST/REST.h"
#include "Term/Pack.h"
#include "Util/util.h"

namespace {

using namespace borealis;
using namespace borealis::util;

TEST(Term, apply) {

    {
        auto TF = TermFactory::get();

        auto l = TF->getLambdaTerm(
            TF->getArgumentTerm(0)
        );

        for (BV i = 0; i < 255; ++i) {
            ASSERT_EQ(i, TF->apply(l, i));
        }
    }

    {
        auto TF = TermFactory::get();

        auto l = TF->getLambdaTerm(
            TF->getUnaryTerm(
                UnaryArithType::SHL_1,
                TF->getBinaryTerm(
                    ArithType::PLUS,
                    TF->getArgumentTerm(0),
                    TF->getOne()
                )
            )
        );

        for (BV i = 0; i < 255; ++i) {
            ASSERT_EQ(
                (i + 1) << 1,
                TF->apply(l, i)
            );
        }
    }

    {
        auto TF = TermFactory::get();
        REST rest(false);

        auto lambda = TF->getLambdaTerm(
            TF->getUnaryTerm(
                UnaryArithType::SHL_1,
                TF->getBinaryTerm(
                    ArithType::PLUS,
                    TF->getArgumentTerm(0),
                    TF->getFoldTerm(
                        TF->getArgumentTerm(0),
                        TF->getZero(),
                        TF->getTernaryTerm(
                            TF->getUnaryTerm(
                                UnaryArithType::SHR_16,
                                TF->getArgumentTerm(1)
                            ),
                            TF->getArgumentTerm(1),
                            TF->getArgumentTerm(2)
                        )
                    )
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
            ASSERT_EQ(expected, actual);
        }
    }

    {
        auto TF = TermFactory::get();
        REST rest(false);

        auto lambda = TF->getLambdaTerm(
            TF->getBinaryTerm(
                ArithType::XOR,
                TF->getBinaryTerm(
                    ArithType::XOR,
                    TF->getZero(),
                    TF->getZero()
                ),
                TF->getZero()
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
            ASSERT_EQ(expected, actual);
        }
    }

    {
        auto TF = TermFactory::get();
        REST rest(false);

        auto lambda = TF->getLambdaTerm(
            TF->getBinaryTerm(
                ArithType::XOR,
                TF->getBinaryTerm(
                    ArithType::XOR,
                    TF->getZero(),
                    TF->getZero()
                ),
                TF->getOne()
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
            ASSERT_EQ(expected, actual);
        }
    }

    {
        auto TF = TermFactory::get();
        REST rest(false);

        // id: L3Hw8zp1UchuL7IqbU8cvGM9
        // (lambda (x_7551) (fold x_7551 0 (lambda (x_7551 x_7552) (not (xor (shl1 x_7551) x_7552)))))
        auto id = "L3Hw8zp1UchuL7IqbU8cvGM9";
        auto lambda = TF->getLambdaTerm(
            TF->getFoldTerm(
                TF->getArgumentTerm(0),
                TF->getZero(),
                TF->getUnaryTerm(
                    UnaryArithType::NOT,
                    TF->getBinaryTerm(
                        ArithType::XOR,
                        TF->getUnaryTerm(
                            UnaryArithType::SHL_1,
                            TF->getArgumentTerm(1)
                        ),
                        TF->getArgumentTerm(2)
                    )
                )
            )
        );

        std::vector<BV> args;
        for (int i = 0; i < 256; ++i) {
            args.push_back(random());
        }

        auto evalResponse = rest.getEvalById(id, args);

        for (int i = 0; i < 256; ++i) {
            auto arg = args[i];
            auto expected = evalResponse.outputs[i];
            auto actual = TF->apply(lambda, arg);
            ASSERT_EQ(expected, actual);
        }
    }
}

TEST(Term, pack) {
    std::cout << unpack("FX0|AIYAZ0!A+I&^|^!DZZZZZYZY")->toString() << std::endl;
}

} // namespace
