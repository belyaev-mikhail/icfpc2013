/*
 * Pack.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: belyaev
 */

#include "Term/Pack.h"

#include "Util/macros.h"

namespace borealis {

Term::Ptr Unpacker::read() {
    str++;
    len--;
    return start_read();
}

Term::Ptr Unpacker::start_read() {
    char ch = str[0];
    switch(ch) {
    case '0': return tf->getZero();
    case '1': return tf->getOne();
    case 'X': return tf->getArgumentTerm(0);
    case 'Y': return tf->getArgumentTerm(1);
    case 'Z': return tf->getArgumentTerm(2);
    case '+':
    case '|':
    case '&':
    case '^':
    {
        auto lhv = read();
        auto rhv = read();
        return tf->getBinaryTerm(getBinaryArithType(ch), lhv, rhv);
    }
    case '!':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    {
        auto rhv = read();
        return tf->getUnaryTerm(getUnaryArithType(ch), rhv);
    }
    case 'I':
    {
        auto cond = read();
        auto tru = read();
        auto fls = read();
        return tf->getTernaryTerm(cond, tru, fls);
    }
    case 'F': {
        auto arg1 = read();
        auto arg2 = read();
        auto body = read();
        return tf->getFoldTerm(arg1, arg2, body);
    }
    case 'T': {
        auto arg1 = read();
        auto arg2 = read();
        auto body = read();
        return tf->getTFoldTerm(arg1, arg2, body);
    }
    default: BYE_BYE(Term::Ptr, "WTF???");
    }

}

#include "Util/unmacros.h"

}


