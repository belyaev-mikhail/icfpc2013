/*
 * test_term.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: ice-phoenix
 */

#include <gtest/gtest.h>

#include "Term/TermFactory.h"
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

}

} // namespace
