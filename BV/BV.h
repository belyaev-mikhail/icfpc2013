/*
 * BV.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef BV_H_
#define BV_H_

#include <stdint.h>
#include <unordered_map>

namespace borealis {

typedef uint64_t BV;

typedef std::unordered_map<unsigned char, BV> BoundVars;

} // namespace borealis

#endif /* BV_H_ */
