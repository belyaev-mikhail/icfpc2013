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

#include "BV/BV.h"
#include "Logging/logger.hpp"
#include "REST/Data.h"
#include "Term/Term.h"
#include "Util/util.h"

namespace borealis {

class REST {

    const std::string url = "http://icfpc2013.cloudapp.net/";
    const std::string authKey = "02202IyLDrkqBSOKrBETKmIgSohseDuWPELwNvXi" "vpsH1H";

    const bool verbose;

public:

    std::vector<Problem> getProblems() const {
        using namespace curlpp;

        auto path = "myproblems?auth=";

        auto response = util::uniq(new std::stringstream());

        Easy curl;
        curl.setOpt(new options::Url(url + path + authKey));
        curl.setOpt(new options::Verbose(verbose));
        curl.setOpt(new options::PostFields("{}"));
        curl.setOpt(new options::WriteStream(response.get()));
        curl.perform();

        std::vector<Problem> res;
        *response >> util::jsonify(res);
        return res;
    }

    EvalResponse getEvalByProgram(Term::Ptr lambda, const std::vector<BV>& args) const {
        using namespace curlpp;

        auto path = "eval?auth=";

        auto request = util::uniq(new std::stringstream());

        EvalProgramRequest req(lambda->toString(), args);
        *request << util::jsonify(req);

        auto response = util::uniq(new std::stringstream());

        Easy curl;
        curl.setOpt(new options::Url(url + path + authKey));
        curl.setOpt(new options::Verbose(verbose));
        curl.setOpt(new options::PostFields(request->str()));
        curl.setOpt(new options::PostFieldSize(request->str().size()));
        curl.setOpt(new options::WriteStream(response.get()));
        curl.perform();

        EvalResponse res;
        *response >> util::jsonify(res);
        return res;
    }

    EvalResponse getEvalById(const std::string& id, const std::vector<BV>& args) const {
        using namespace curlpp;

        auto path = "eval?auth=";

        auto request = util::uniq(new std::stringstream());

        EvalIdRequest req(id, args);
        *request << util::jsonify(req);

        auto response = util::uniq(new std::stringstream());

        Easy curl;
        curl.setOpt(new options::Url(url + path + authKey));
        curl.setOpt(new options::Verbose(verbose));
        curl.setOpt(new options::PostFields(request->str()));
        curl.setOpt(new options::PostFieldSize(request->str().size()));
        curl.setOpt(new options::WriteStream(response.get()));
        curl.perform();

        EvalResponse res;
        *response >> util::jsonify(res);
        return res;
    }

    GuessResponse tryGuess(const std::string& id, Term::Ptr lambda) const {
        using namespace curlpp;

        auto path = "guess?auth=";

        auto request = util::uniq(new std::stringstream());

        GuessRequest req{ id, lambda->toString() };
        *request << util::jsonify(req);

        auto response = util::uniq(new std::stringstream());

        Easy curl;
        curl.setOpt(new options::Url(url + path + authKey));
        curl.setOpt(new options::Verbose(verbose));
        curl.setOpt(new options::PostFields(request->str()));
        curl.setOpt(new options::PostFieldSize(request->str().size()));
        curl.setOpt(new options::WriteStream(response.get()));
        curl.perform();

        GuessResponse res;
        *response >> util::jsonify(res);
        return res;
    }

    REST(bool verbose = true) : verbose(verbose) {}
    virtual ~REST();

};

} /* namespace borealis */

#endif /* REST_H_ */
