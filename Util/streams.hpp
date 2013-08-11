/*
 * streams.hpp
 *
 *  Created on: Nov 1, 2012
 *      Author: belyaev
 */

#ifndef STREAMS_HPP_
#define STREAMS_HPP_

#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

#include <Util/collections.hpp>

#include "Util/macros.h"

namespace borealis {
namespace util {

template<class T, bool UseLLVMOstream = false>
struct Stringifier;

template<class T>
struct Stringifier<T, false> {
    static std::string toString(const T& t) {
        std::ostringstream oss;
        oss << t;
        return oss.str();
    }
};

// special cases
template<>
struct Stringifier<bool> {
    static std::string toString(bool t) {
        return t ? "true" : "false";
    }
};

template<>
struct Stringifier<std::string> {
    static std::string toString(const std::string& t) {
        return t;
    }
};

template<>
struct Stringifier<const char*> {
    static std::string toString(const char* t) {
        return t;
    }
};

template<class T>
inline std::string toString(const T& t) {
    return Stringifier<T>::toString(t);
}

////////////////////////////////////////////////////////////////////////////////
//
// borealis::util::streams
//
////////////////////////////////////////////////////////////////////////////////

namespace streams {

template<class T>
struct error_printer {
    const T& val;
    error_printer(const T& v) : val(v) {}
};

template<class T>
std::ostream& operator<<(std::ostream& s, const error_printer<T>& v) {
    return s << "!" << v.val << "!";
}

// prints values in red:
//   errs() << error(42) << endl;
template<class T>
error_printer<T> error(const T& val) { return error_printer<T>(val); }



template<class Func>
std::string with_stream(Func f) {
    std::ostringstream ost;
    f(ost);
    return ost.str();
}

} // namespace streams
} // namespace util
} // namespace borealis

////////////////////////////////////////////////////////////////////////////////
//
// std
//
////////////////////////////////////////////////////////////////////////////////

namespace std {

namespace impl_ {
// Custom output stuff and such...
const auto VECTOR_LEFT_BRACE = '[';
const auto VECTOR_RIGHT_BRACE = ']';
const auto SET_LEFT_BRACE = '(';
const auto SET_RIGHT_BRACE = ')';
const auto ELEMENT_DELIMITER = ", ";
const auto TUPLE_LEFT_BRACE = '{';
const auto TUPLE_RIGHT_BRACE = '}';
const auto NULL_REPR = "<NULL>";
} // namespace impl_



template<typename T, typename U, typename Streamer>
Streamer& operator<<(Streamer& s, const std::pair<T, U>& pp) {
    using namespace std::impl_;

    s << TUPLE_LEFT_BRACE
      << pp.first
      << ELEMENT_DELIMITER
      << pp.second
      << TUPLE_RIGHT_BRACE;

    return s;
}



template<typename Elem, typename SFINAE = void>
struct elemPrettyPrinter;

template< typename Elem >
struct elemPrettyPrinter<Elem, GUARD(std::is_pointer<Elem>::value)> {
    template<class Streamer>
    static Streamer& doit(Streamer& s, const Elem& e) {
        // this is generally fucked up...
        return static_cast<Streamer&>(e == nullptr ? s << impl_::NULL_REPR : s << *e);
    }
};

template< typename Elem >
struct elemPrettyPrinter<Elem, GUARD(!std::is_pointer<Elem>::value)> {
    template<class Streamer>
    static Streamer& doit(Streamer& s, const Elem& e) {
        // this is generally fucked up...
        return static_cast<Streamer&>(s << e);
    }
};

template<>
struct elemPrettyPrinter<const char*> {
    template<class Streamer>
    static Streamer& doit(Streamer& s, const char* e) {
        // this is generally fucked up...
        return static_cast<Streamer&>(e == nullptr ? s << impl_::NULL_REPR : s << e);
    }
};



template< class Container >
struct containerPrettyPrinter {
    template<char LB, char RB, class Streamer>
    static Streamer& doit(Streamer& s, const Container& con) {
        typedef typename Container::const_iterator ConstIter;
        typedef typename Container::value_type Elem;
        typedef elemPrettyPrinter<Elem> epp;

        using namespace std::impl_;
        using borealis::util::view;

        s << LB;
        if (!con.empty()) {
            ConstIter iter = con.begin();
            const Elem& el = *iter++;
            epp::doit(s, el);
            for (const auto& e : view(iter, con.end())) {
                s << ELEMENT_DELIMITER;
                epp::doit(s, e);
            }
        }
        s << RB;

        return s;
    }
};

template<typename T, typename Streamer>
Streamer& operator<<(Streamer& s, const std::vector<T>& vec) {
    typedef std::vector<T> Container;
    return containerPrettyPrinter< Container >::template doit<impl_::VECTOR_LEFT_BRACE, impl_::VECTOR_RIGHT_BRACE>(s, vec);
}

template<typename T, typename Streamer>
Streamer& operator<<(Streamer& s, const std::list<T>& lst) {
    typedef std::list<T> Container;
    return containerPrettyPrinter< Container >::template doit<impl_::VECTOR_LEFT_BRACE, impl_::VECTOR_RIGHT_BRACE>(s, lst);
}

template<typename T, typename Streamer>
Streamer& operator<<(Streamer& s, const std::set<T>& set) {
    typedef std::set<T> Container;
    return containerPrettyPrinter< Container >::template doit<impl_::SET_LEFT_BRACE, impl_::SET_RIGHT_BRACE>(s, set);
}

template<typename T, typename Streamer>
Streamer& operator<<(Streamer& s, const std::unordered_set<T>& set) {
    typedef std::unordered_set<T> Container;
    return containerPrettyPrinter< Container >::template doit<impl_::SET_LEFT_BRACE, impl_::SET_RIGHT_BRACE>(s, set);
}

template<typename K, typename V, typename Streamer>
Streamer& operator<<(Streamer& s, const std::unordered_map<K, V>& map) {
    typedef std::unordered_map<K, V> Container;
    return containerPrettyPrinter< Container >::template doit<impl_::SET_LEFT_BRACE, impl_::SET_RIGHT_BRACE>(s, map);
}

} // namespace std


#include "Util/unmacros.h"

#endif /* STREAMS_HPP_ */
