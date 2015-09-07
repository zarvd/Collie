#ifndef HTTP_SERV_H
#define HTTP_SERV_H

#include "httpd.hpp"
#include <map>



namespace Http {

    extern const std::map<std::string, std::string> MimeType;

    class HttpHeader {
    protected:
        std::map<std::string, bool> selectItem;
        std::map<std::string, std::string> item;
    public:
        HttpHeader() = delete;
        virtual Status setItemValue(const std::string&, const std::string&);
        virtual Status assignItem(const std::string&, const bool&);
    };

    class GeneralHeader final : public HttpHeader {};

    class RequestHeader final : public HttpHeader {};

    class ResponseHeader final : public HttpHeader {};

    class EntityHeader final : public HttpHeader {};

    std::string getStateByCode(const unsigned short&);
    std::string getMimeTypeByExt(const std::string&);
    std::string createHeader(GeneralHeader&, RequestHeader&, ResponseHeader&, EntityHeader&);
    void start();
}

#endif /* HTTP_SERV_H */
