/*
 * Data.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef DATA_H_
#define DATA_H_

#include <algorithm>

#include "BV/BV.h"
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
            auto solved = util::fromJson<bool>(json["solved"]);
            obj->solved = solved ? *solved : false;
            auto timeLeft = util::fromJson<double>(json["timeLeft"]);
            obj->timeLeft = timeLeft ? *timeLeft : 300.0;
            return optional_ptr_t{ obj };
        } else {
            return optional_ptr_t{ nullptr };
        }
    }
};

} // namespace util



struct EvalProgramRequest {
    std::string program;
    std::vector<std::string> arguments;

    EvalProgramRequest(const std::string& program, const std::vector<BV>& arguments) {
        this->program = program;
        std::transform(arguments.begin(), arguments.end(), std::back_inserter(this->arguments),
            [](const BV& bv) -> std::string {
                std::ostringstream oss;
                oss << std::hex << bv;
                return oss.str();
            }
        );
    }
};

namespace util {

template<>
struct json_traits<EvalProgramRequest> {
    typedef std::unique_ptr<EvalProgramRequest> optional_ptr_t;

    static Json::Value toJson(const EvalProgramRequest& val) {
        Json::Value dict;
        dict["program"] = util::toJson(val.program);
        dict["arguments"] = util::toJson(val.arguments);
        return dict;
    }
};

} // namespace util



struct EvalIdRequest {
    std::string id;
    std::vector<std::string> arguments;

    EvalIdRequest(const std::string& id, const std::vector<BV>& arguments) {
        this->id = id;
        std::transform(arguments.begin(), arguments.end(), std::back_inserter(this->arguments),
            [](const BV& bv) -> std::string {
                std::ostringstream oss;
                oss << std::hex << bv;
                return oss.str();
            }
        );
    }
};

namespace util {

template<>
struct json_traits<EvalIdRequest> {
    typedef std::unique_ptr<EvalIdRequest> optional_ptr_t;

    static Json::Value toJson(const EvalIdRequest& val) {
        Json::Value dict;
        dict["id"] = util::toJson(val.id);
        dict["arguments"] = util::toJson(val.arguments);
        return dict;
    }
};

} // namespace util



struct EvalResponse {
    std::string status;
    /*optional*/ std::vector<BV> outputs;
    /*optional*/ std::string message;
};

namespace util {

template<>
struct json_traits<EvalResponse> {
    typedef std::unique_ptr<EvalResponse> optional_ptr_t;

    static optional_ptr_t fromJson(const Json::Value& json) {
        using borealis::util::json_object_builder;

        json_object_builder<EvalResponse, std::string> builder {
            "status"
        };

        auto* obj = builder.build(json);

        if (obj) {
            auto outputs = util::fromJson<std::vector<std::string>>(json["outputs"]);
            if (outputs) {
                std::transform(outputs->begin(), outputs->end(), std::back_inserter(obj->outputs),
                    [](const std::string& output) -> BV {
                        std::istringstream iss(output);
                        BV res;
                        iss >> std::hex >> res;
                        return res;
                    }
                );
            }
            auto message = util::fromJson<std::string>(json["message"]);
            obj->message = message ? *message : "<NONE>";
            return optional_ptr_t{ obj };
        } else {
            return optional_ptr_t{ nullptr };
        }
    }
};

} // namespace util



struct GuessRequest {
    std::string id;
    std::string program;
};

namespace util {

template<>
struct json_traits<GuessRequest> {
    typedef std::unique_ptr<GuessRequest> optional_ptr_t;

    static Json::Value toJson(const GuessRequest& val) {
        Json::Value dict;
        dict["id"] = util::toJson(val.id);
        dict["program"] = util::toJson(val.program);
        return dict;
    }
};

} // namespace util



struct GuessResponse {
    std::string status;
    /*optional*/ std::vector<BV> values;
    /*optional*/ std::string message;
    /*optional*/ bool lightning;
};

namespace util {

template<>
struct json_traits<GuessResponse> {
    typedef std::unique_ptr<GuessResponse> optional_ptr_t;

    static optional_ptr_t fromJson(const Json::Value& json) {
        using borealis::util::json_object_builder;

        json_object_builder<GuessResponse, std::string> builder {
            "status"
        };

        auto* obj = builder.build(json);

        if (obj) {
            auto values = util::fromJson<std::vector<std::string>>(json["values"]);
            if (values) {
                std::transform(values->begin(), values->end(), std::back_inserter(obj->values),
                    [](const std::string& value) -> BV {
                        std::istringstream iss(value);
                        BV res;
                        iss >> std::hex >> res;
                        return res;
                    }
                );
            }
            auto message = util::fromJson<std::string>(json["message"]);
            obj->message = message ? *message : "<NONE>";
            auto lightning = util::fromJson<bool>(json["lightning"]);
            obj->lightning = lightning ? *lightning : false;
            return optional_ptr_t{ obj };
        } else {
            return optional_ptr_t{ nullptr };
        }
    }
};

} // namespace util



} // namespace borealis

#endif /* DATA_H_ */
