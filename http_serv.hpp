#ifndef HTTP_SERV_H
#define HTTP_SERV_H

#include "httpd.hpp"
#include <map>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>


namespace Http {

    extern const std::map<std::string, std::string> MimeType;

    struct HeaderField {
        std::string name;
        std::string value;
        bool isSend;

        HeaderField() = delete;
        HeaderField(const std::string&, const std::string& = "", const bool = false);
    };

    class HttpHeader {
    protected:
        std::vector<HeaderField> item;
        HeaderField& findItem(const std::string&);
        const HeaderField& findConstItem(const std::string&) const;

    public:
        virtual ~HttpHeader() = default;
        virtual Status init() = 0;
        std::string toString() const;
        std::string getItemValue(const std::string&) const;
        Status setItemValue(const std::string&, const std::string&);
        Status assignItem(const std::string&, const bool);
    };

    class GeneralHeader final : public HttpHeader {
    public:
        GeneralHeader();
        ~GeneralHeader() = default;
        Status init() override;
        std::string getCurrentDate() const;
    };

    class RequestHeader final : public HttpHeader {
    public:
        RequestHeader();
        ~RequestHeader() = default;
        Status init() override;
    };

    class ResponseHeader final : public HttpHeader {
    public:
        ResponseHeader();
        ~ResponseHeader() = default;
        Status init() override;
    };

    class EntityHeader final : public HttpHeader {
    public:
        EntityHeader();
        ~EntityHeader() = default;
        Status init() override;
    };


    inline std::string getMimeTypeByExt(const std::string& ext) {
        return MimeType.at(ext);
    }

    std::string getStateByCode(const unsigned short&);
    std::string generateHeader();
    void start();
}

#endif /* HTTP_SERV_H */
