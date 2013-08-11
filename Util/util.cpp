/*
 * util.cpp
 *
 *  Created on: Aug 22, 2012
 *      Author: belyaev
 */
#include <cstdlib>
#include <unordered_set>

#include "Util/util.h"

#include "Util/macros.h"

namespace llvm {

////////////////////////////////////////////////////////////////////////////////

typedef std::pair<std::string, ConditionType> ConditionDescription;

ConditionDescription analyzeCondition(const int cond) {
	typedef ConditionType CT;

	return { "хер", CT::UNKNOWN };
}

ConditionType conditionType(int cond) {
    return analyzeCondition(cond).second;
}

std::string conditionString(int cond) {
	return analyzeCondition(cond).first;
}

std::string conditionString(ConditionType cond) {
    switch(cond) {
    case ConditionType::EQ:    return "==";
    case ConditionType::NEQ:   return "!=";

    case ConditionType::GT:    return ">";
    case ConditionType::GE:    return ">=";
    case ConditionType::LT:    return "<";
    case ConditionType::LE:    return "<=";

    case ConditionType::UGT:   return "+>";
    case ConditionType::UGE:   return "+>=";
    case ConditionType::ULT:   return "+<";
    case ConditionType::ULE:   return "+<=";

    case ConditionType::TRUE:  return "true";
    case ConditionType::FALSE: return "false";
    default: BYE_BYE(std::string, "Unreachable!");
    }
}

std::string arithString(ArithType opCode) {
    switch (opCode) {
    case ArithType::ADD:  return "+";
    case ArithType::SUB:  return "-";
    case ArithType::MUL:  return "*";
    case ArithType::DIV:  return "/";
    case ArithType::REM:  return "%";
    case ArithType::BAND: return "&";
    case ArithType::BOR:  return "|";
    case ArithType::LAND: return "&&";
    case ArithType::LOR:  return "||";
    case ArithType::XOR:  return "^";
    case ArithType::SHL:  return "<<";
    case ArithType::ASHR: return ">>";
    case ArithType::LSHR: return ">>>";
    default: BYE_BYE(std::string, "Unreachable!");
    }
}

std::string unaryArithString(UnaryArithType opCode) {
    switch (opCode) {
    case UnaryArithType::NOT:  return "!";
    case UnaryArithType::BNOT: return "~";
    case UnaryArithType::NEG:  return "-";
    default: BYE_BYE(std::string, "Unreachable!");
    }
}


} // namespace llvm

namespace borealis {
namespace util {

std::string nospaces(const std::string& v) {
	return nospaces(std::string(v));
}

std::string nospaces(std::string&& v) {
	v.erase(std::remove_if(v.begin(), v.end(), isspace), v.end());
	return v;
}

bool endsWith(const std::string& fullString, const std::string& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

std::string nochar(const std::string& v, char c) {
    return nochar(std::string(v), c);
}

std::string nochar(std::string&& v, char c) {
    v.erase(std::remove_if(v.begin(), v.end(), [&c](char x) { return c == x; }), v.end());
    return v;
}

std::string& replace(const std::string& from, const std::string& to, std::string& in) {
    auto pos = in.find(from);
    if (pos == std::string::npos) return in;
    else return in.replace(pos, from.length(), to);
}
} // namespace util
} // namespace borealis

#include "Util/unmacros.h"
