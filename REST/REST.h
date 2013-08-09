/*
 * REST.h
 *
 *  Created on: Aug 9, 2013
 *      Author: ice-phoenix
 */

#ifndef REST_H_
#define REST_H_

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "Logging/logger.hpp"
#include "REST/Data.h"
#include "Util/util.h"

namespace borealis {

class REST {

    const std::string url = "http://icfpc2013.cloudapp.net/";
    const std::string authKey = "02202IyLDrkqBSOKrBETKmIgSohseDuWPELwNvXi" "vpsH1H";

public:

    std::vector<Problem> getProblems() const {
        using namespace curlpp;

        auto path = "myproblems?auth=";

        auto response = util::uniq(new std::stringstream());

        Easy curl;
        curl.setOpt(new options::Url(url + path + authKey));
        curl.setOpt(new options::Verbose(true));
        curl.setOpt(new options::PostFields("{}"));
        curl.setOpt(new options::WriteStream(response.get()));
        curl.perform();

        std::vector<Problem> res;
        *response >> util::jsonify(res);
        return res;
    }

    virtual ~REST();

};

} /* namespace borealis */

#endif /* REST_H_ */
