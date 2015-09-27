#include "Response.hpp"
#include "Header.hpp"


namespace Http {
    Response::~Response() {};

    void Response::setStatus(const unsigned& code) {
        const std::string status = getStateByCode(code);
        this->status = {code, status};
    }

    void Response::setHeader(const std::string& key, const std::string& value) {
        auto it = Header::HEADER.find(key);
        if(it == Header::HEADER.end()) {
            Log(logLevel::Warn) << "Unknown header filed setted "
                                << key << " : "
                                << value;
        } else {
            HeaderType type = it->second;
            if(type != HeaderType::RES) {
                Log(logLevel::Warn) << "Non response header setted "
                                    << key << " : "
                                    << value;
            }
        }
        this->header[key] = value;
    }
}
