/*
 * TermFactory.h
 *
 *  Created on: Nov 19, 2012
 *      Author: ice-phoenix
 */

#ifndef TERMFACTORY_H_
#define TERMFACTORY_H_

#include <memory>

#include "BV/BV.h"
#include "Term/Term.def"

#include "Util/macros.h"

namespace borealis {

class TermFactory {

    template<class Fst, class Snd, class Thrd>
    using triple = std::tuple<Fst, Snd, Thrd>;

    bool caching;
public:

    typedef std::shared_ptr<TermFactory> Ptr;

    Term::Ptr getArgumentTerm(unsigned char idx) {
        static Term::Ptr arg0{ new ArgumentTerm(0) };
        static Term::Ptr arg1{ new ArgumentTerm(1) };
        static Term::Ptr arg2{ new ArgumentTerm(2) };

        if (idx == 0) return arg0;
        if (idx == 1) return arg1;
        if (idx == 2) return arg2;
        ASSERT(false, "Oops");
    }

    Term::Ptr getOne() {
        static Term::Ptr one{ new OpaqueIntConstantTerm(1) };
        return one;
    }

    Term::Ptr getZero() {
        static Term::Ptr zero{ new OpaqueIntConstantTerm(0) };
        return zero;
    }

    Term::Ptr getFoldTerm(Term::Ptr arg1, Term::Ptr arg2, Term::Ptr body) {
        typedef std::unordered_map< triple<Term::Ptr, Term::Ptr, Term::Ptr>, Term::Ptr > triMap;

        static triMap trms;

        if(caching) {
            auto key = std::make_tuple(arg1, arg2, body);
            auto it = trms.find(key);
            if(it != trms.end()) {
                return it -> second;
            }

            auto res = Term::Ptr{
                new FoldTerm(
                    arg1, arg2, body
                )
            };

            trms[key] = res;
            return res;
        } else return Term::Ptr {
            new FoldTerm(
                arg1, arg2, body
            )
        };
    }

    Term::Ptr getTFoldTerm(Term::Ptr arg1, Term::Ptr arg2, Term::Ptr body) {
        return Term::Ptr{
            new FoldTerm(
                arg1, arg2, body, "tfold"
            )
        };
    }

    Term::Ptr getLambdaTerm(Term::Ptr body) {
        return Term::Ptr{
            new LambdaTerm(
                body
            )
        };
    }

    Term::Ptr getTernaryTerm(Term::Ptr cnd, Term::Ptr tru, Term::Ptr fls) {
        typedef std::unordered_map< triple<Term::Ptr, Term::Ptr, Term::Ptr>, Term::Ptr > triMap;

        static triMap trms;

        if(caching) {

            auto key = std::make_tuple(cnd, tru, fls);
            auto it = trms.find(key);
            if(it != trms.end()) {
                return it -> second;
            }

            auto res = Term::Ptr{
                new TernaryTerm(
                    cnd, tru, fls
                )
            };

            trms[key] = res;
            return res;

        } else return Term::Ptr{
            new TernaryTerm(
                cnd, tru, fls
            )
        };
    }

    Term::Ptr getBinaryTerm(ArithType opc, Term::Ptr lhv, Term::Ptr rhv) {
        typedef std::unordered_map< std::pair<Term::Ptr, Term::Ptr>, Term::Ptr > binMap;

        static binMap xorMap;
        static binMap orMap;
        static binMap andMap;
        static binMap plusMap;
        static std::unordered_map<ArithType, binMap> cache {
            { ArithType::AND,   andMap },
            { ArithType::OR,    orMap },
            { ArithType::XOR,   xorMap },
            { ArithType::PLUS,  plusMap },
        };

        if(caching) {
            auto& map = cache.at(opc);
            auto key = std::make_pair(lhv, rhv);
            auto it = map.find(key);
            if(it != map.end()) return it->second;

            auto res = Term::Ptr{
                new BinaryTerm(
                    opc, lhv, rhv
                )
            };

            map[key] = res;
            return res;
        } else return Term::Ptr{
            new BinaryTerm(
                opc, lhv, rhv
            )
        };
    }

    Term::Ptr getUnaryTerm(UnaryArithType opc, Term::Ptr rhv) {
        static std::unordered_map<Term::Ptr, Term::Ptr> notMap;
        static std::unordered_map<Term::Ptr, Term::Ptr> shl1Map;
        static std::unordered_map<Term::Ptr, Term::Ptr> shr1Map;
        static std::unordered_map<Term::Ptr, Term::Ptr> shr4Map;
        static std::unordered_map<Term::Ptr, Term::Ptr> shr16Map;

        static std::unordered_map<UnaryArithType, std::unordered_map<Term::Ptr, Term::Ptr>> cache{
            {UnaryArithType::NOT,    notMap},
            {UnaryArithType::SHL_1,  shl1Map},
            {UnaryArithType::SHR_1,  shr1Map},
            {UnaryArithType::SHR_4,  shr4Map},
            {UnaryArithType::SHR_16, shr16Map}
        };

        if(caching) {
            auto& map = cache.at(opc);
            auto it = map.find(rhv);
            if (it != map.end()) {
                return it -> second;
            }

            auto res = Term::Ptr{
                new UnaryTerm(opc, rhv)
            };
            map[rhv] = res;
            return res;
        } else return Term::Ptr{
            new UnaryTerm(opc, rhv)
        };
    }

    static TermFactory::Ptr get() {
        static TermFactory::Ptr instance(new TermFactory());
        return instance;
    }

    static BV apply(Term::Ptr lambda, BV arg) {
        BoundVars env;
        env[0] = arg;
        return lambda->eval(env);
    }

private:

    TermFactory();

};

} /* namespace borealis */

#include "Util/unmacros.h"

#endif /* TERMFACTORY_H_ */
