/*
 * test_kolya.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: belyaev
 */

#include <gtest/gtest.h>

#include "Bruteforce/Bruteforce.h"


namespace {

using namespace borealis;
using namespace borealis::util;

TEST(Term, apply) {

    std::cout << "running..." << std::endl;

    //NewBruteforcer b(std::set<std::string>{ "if0", "or", "plus", "shr4", "tfold" });

    Bruteforcer b(TermFactory::get(), std::set<std::string>{ "if0", "or", "plus", "shr4", "tfold" });
    auto what = b.doit(14);

    std::cout<<what.size()<<std::endl;

//    for(auto s : what) {
//        std::cout << s->toString() << std::endl;
//    }

}

}
