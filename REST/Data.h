/*
 * Data.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef DATA_H_
#define DATA_H_

#include "Logging/logger.hpp"
#include "Util/json.hpp"
#include "Util/json_traits.hpp"

namespace borealis {

struct Problem {
    std::string id;
    int size;
    std::set<std::string> operators;
    /*optional*/ bool solved;
    /*optional*/ double timeLeft;
};

template<class Streamer>
Streamer& operator<<(Streamer& s, const Problem& p) {
    auto endl = "\n";
    s << "Problem: " << endl
      << "  id: " << p.id << endl
      << "  size: " << p.size << endl
      << "  ops: " << p.operators << endl
      << "  solved: " << p.solved << endl
      << "  timeLeft: " << p.timeLeft << endl;
    return static_cast<Streamer&>(s);
}

namespace util {

template<>
struct json_traits<Problem> {
    typedef std::unique_ptr<Problem> optional_ptr_t;

    static optional_ptr_t fromJson(const Json::Value& json) {
        using borealis::util::json_object_builder;

        json_object_builder<Problem, std::string, int, std::set<std::string>> builder {
            "id", "size", "operators"
        };

        auto* obj = builder.build(json);

        if (obj) {
            obj->solved = json.get("solved", false).asBool();
            obj->timeLeft = json.get("timeLeft", 300.0).asDouble();
            return optional_ptr_t{ obj };
        } else {
            return optional_ptr_t{ nullptr };
        }
    }
};

} // namespace util

} // namespace borealis

#endif /* DATA_H_ */
