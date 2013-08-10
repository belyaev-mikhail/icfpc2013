/*
 * Pack.h
 *
 *  Created on: Aug 10, 2013
 *      Author: belyaev
 */

#ifndef PACK_H_
#define PACK_H_

#include "Term/TermFactory.h"

#include "Util/util.h"
#include "Util/macros.h"

namespace borealis {

class Unpacker {

    std::string data;
    const char* str;
    size_t len;

    TermFactory::Ptr tf;

    Term::Ptr read();
public:

    Unpacker(const std::string& str):
        data(str),
        str(data.data()),
        len(data.length()),
        tf(TermFactory::get()) {
    }

    Unpacker(std::string&& str):
        data(str),
        str(data.data()),
        len(data.length()),
        tf(TermFactory::get()) {
    }

    static ArithType getBinaryArithType(char ch) {
        switch(ch) {
        case '+': return ArithType::PLUS;
        case '|': return ArithType::OR;
        case '&': return ArithType::AND;
        case '^': return ArithType::XOR;
        default: BYE_BYE(ArithType, "WTF???");
        }
    }

    static UnaryArithType getUnaryArithType(char ch) {
        switch(ch) {
        case '!': return UnaryArithType::NOT;
        case 'A': return UnaryArithType::SHR_1;
        case 'B': return UnaryArithType::SHR_4;
        case 'C': return UnaryArithType::SHR_16;
        case 'D': return UnaryArithType::SHL_1;
        default: BYE_BYE(UnaryArithType, "WTF???");
        }
    }
    Term::Ptr start_read();
};

inline Term::Ptr unpack(const std::string& data) {
    return Unpacker(data).start_read();
}

}

#include "Util/unmacros.h"

#endif /* PACK_H_ */
