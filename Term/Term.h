/*
 * Term.h
 *
 *  Created on: Nov 16, 2012
 *      Author: ice-phoenix
 */

#ifndef TERM_H_
#define TERM_H_

#include <string>

#include "BV/BV.h"
#include "Util/typeindex.hpp"
#include "Util/util.h"

namespace borealis {

// Forward declarations
template<class SubClass> class Transformer;
// End of forward declarations

class Term : public ClassTag {

public:

    typedef std::shared_ptr<const Term> Ptr;

protected:

    Term(id_t classTag, const std::string& str) :
        ClassTag(classTag), str(str) {};
    Term(const Term&) = default;

public:

    virtual ~Term() {};

    const std::string& toString() const {
        return str;
    }

    virtual BV eval(const BoundVars&) const = 0;
    virtual unsigned char size() const = 0;
    virtual void registerComponents(std::set<std::string>&) const { return; }

    virtual bool equals(const Term* other) const {
        if (other == nullptr) return false;
        return classTag == other->classTag &&
                str == other->str;
    }

    bool operator==(const Term& other) const {
        if (this == &other) return true;
        else return false;
        //return this->equals(&other);
    }

    virtual size_t hashCode() const {
        return util::hash::defaultHasher()(classTag, str);
    }

    static bool classof(const Term*) {
        return true;
    }

protected:

    std::string str;

};

} /* namespace borealis */

namespace std {
template<>
struct hash<borealis::Term::Ptr> {
    size_t operator()(const borealis::Term::Ptr& t) const {
        return t->hashCode();
    }
};
template<>
struct hash<const borealis::Term::Ptr> {
    size_t operator()(const borealis::Term::Ptr& t) const {
        return t->hashCode();
    }
};
} // namespace std

#define MK_COMMON_TERM_IMPL(CLASS) \
private: \
    typedef CLASS Self; \
    CLASS(const CLASS&) = default; \
public: \
    friend class TermFactory; \
    virtual ~CLASS() {}; \
    static bool classof(const Self*) { \
        return true; \
    } \
    static bool classof(const Term* t) { \
        return t->getClassTag() == class_tag<Self>(); \
    }

#endif /* TERM_H_ */
