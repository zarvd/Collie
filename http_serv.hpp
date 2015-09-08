#ifndef HTTP_SERV_H
#define HTTP_SERV_H

#include "httpd.hpp"
#include <map>
#include <vector>
#include <algorithm>


namespace Http {

    extern const std::map<std::string, std::string> MimeType;

    struct HeaderField {
        std::string name;
        std::string value;
        bool isSend;

        HeaderField() = delete;
        HeaderField(const std::string&, const std::string&, const bool);
    };

    class HttpHeader {
    protected:
        std::vector<HeaderField> item;
        HeaderField& findItem(const std::string&);

    public:
        virtual ~HttpHeader() = 0;
        Status setItemValue(const std::string&, const std::string&);
        Status assignItem(const std::string&, const bool&);
    };

    class GeneralHeader final : public HttpHeader {
    public:
        GeneralHeader();
    };

    class RequestHeader final : public HttpHeader {
    public:
        RequestHeader();
    };

    class ResponseHeader final : public HttpHeader {
    public:
        ResponseHeader();
    };

    class EntityHeader final : public HttpHeader {
    public:
        EntityHeader();
    };

    std::string getStateByCode(const unsigned short&);
    std::string getMimeTypeByExt(const std::string&);
    std::string createHeader(GeneralHeader&, RequestHeader&,
                             ResponseHeader&, EntityHeader&);
    void start();
}

#endif /* HTTP_SERV_H */
