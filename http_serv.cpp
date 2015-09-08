#include "http_serv.hpp"


namespace Http {
    const std::map<std::string, std::string> MimeType = {
        {"html", "text/html"},
        {"htm", "text/html"},
        {"shtm", "text/html"},
        {"shtml", "text/html"},
        {"css", "text/css"},
        {"js",  "application/javascript"},
        {"ico", "image/x-icon"},
        {"gif", "image/gif"},
        {"jpg", "image/jpeg"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"svg", "image/svg+xml"},
        {"txt", "text/plain"},
        {"torrent", "application/x-bittorrent"},
        {"wav", "audio/x-wav"},
        {"mp3", "audio/x-mp3"},
        {"mid", "audio/mid"},
        {"m3u", "audio/x-mpegurl"},
        {"ogg", "application/ogg"},
        {"ram", "audio/x-pn-realaudio"},
        {"xml", "text/xml"},
        {"json",  "application/json"},
        {"xslt", "application/xml"},
        {"xsl", "application/xml"},
        {"ra",  "audio/x-pn-realaudio"},
        {"doc", "application/msword"},
        {"exe", "application/octet-stream"},
        {"zip", "application/x-zip-compressed"},
        {"xls", "application/excel"},
        {"tgz", "application/x-tar-gz"},
        {"tar", "application/x-tar"},
        {"gz",  "application/x-gunzip"},
        {"arj", "application/x-arj-compressed"},
        {"rar", "application/x-rar-compressed"},
        {"rtf", "application/rtf"},
        {"pdf", "application/pdf"},
        {"swf", "application/x-shockwave-flash"},
        {"mpg", "video/mpeg"},
        {"webm", "video/webm"},
        {"mpeg", "video/mpeg"},
        {"mov", "video/quicktime"},
        {"mp4", "video/mp4"},
        {"m4v", "video/x-m4v"},
        {"asf", "video/x-ms-asf"},
        {"avi", "video/x-msvideo"},
        {"bmp", "image/bmp"},
        {"ttf", "application/x-font-ttf"}
    };

    std::string getStateByCode(const unsigned short& code) {
        switch(code) {
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 102: return "Processing";

        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 207: return "Multi-Status";
        case 208: return "Already Reported";
        case 226: return "IM Used";

        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 306: return "Switch Proxy";
        case 307: return "Temporary Redirect";
        case 308: return "Permanent Redirect";

        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Timeout";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Payload Too Large";
        case 414: return "URI Too Long";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested Range Not Satisfiable";
        case 417: return "Expectation Failed";
        case 418: return "I\'m a teapot";
        case 422: return "Unprocessable Entity";
        case 423: return "Locked";
        case 424: return "Failed Dependency";
        case 426: return "Upgrade Required";
        case 428: return "Precondition Required";
        case 429: return "Too Many Requests";
        case 431: return "Request Header Fields Too Large";
        case 451: return "Unavailable For Legal Reasons";

        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Timeout";
        case 505: return "HTTP Version Not Supported";
        case 506: return "Variant Also Negotiates";
        case 507: return "Insufficient Storage";
        case 508: return "Loop Detected";
        case 510: return "Not Extended";
        case 511: return "Network Authentication Required";

        default:  return "Server Error";
        }
    }

    std::string getMimeTypeByExt(const std::string& ext) {
        return MimeType.at(ext);
    }

    HeaderField::HeaderField(const std::string& fName,
                             const std::string& fValue,
                             const bool fIsSend) : name(fName), value(fValue), isSend(fIsSend) {}

    HeaderField& HttpHeader::findItem(const std::string& itemName) {
        auto it = std::find_if(item.begin(), item.end(), [itemName](HeaderField& elem) {
                return elem.name == itemName;
            });
        return *it;
    }

    GeneralHeader::GeneralHeader() {
        item = {
            HeaderField("Cache-Control", "", false),
            HeaderField("Cache-Control", "", false),
            HeaderField("Connection", "", false),
            HeaderField("Date", "", false),
            HeaderField("Pragma", "", false),
            HeaderField("Trailer", "", false),
            HeaderField("Transfer-Encoding", "", false),
            HeaderField("Upgrade", "", false),
            HeaderField("Via", "", false),
            HeaderField("Warning", "", false)
        };
    }

    RequestHeader::RequestHeader() {
        item = {
            HeaderField("Accept", "", false),
            HeaderField("Accept-Charset", "", false),
            HeaderField("Accept-Encoding", "", false),
            HeaderField("Accept-Language", "", false),
            HeaderField("Authorization", "", false),
            HeaderField("Expect", "", false),
            HeaderField("From", "", false),
            HeaderField("Host", "", false),
            HeaderField("If-Match", "", false),
            HeaderField("If-Modified-Since", "", false),
            HeaderField("If-None-Match", "", false),
            HeaderField("If-Range", "", false),
            HeaderField("If-Unmodified-Since", "", false),
            HeaderField("Max-Forwards", "", false),
            HeaderField("Proxy-Authorization", "", false),
            HeaderField("Range", "", false),
            HeaderField("Referer", "", false),
            HeaderField("TE", "", false),
            HeaderField("User-Agent", "", false),
        };
    }

    ResponseHeader::ResponseHeader() {
        item = {
            HeaderField("Accept-Ranges", "", false),
            HeaderField("Age", "", false),
            HeaderField("ETag", "", false),
            HeaderField("Location", "", false),
            HeaderField("Proxy-Authenticate", "", false),
            HeaderField("Retry-After", "", false),
            HeaderField("Server", "", false),
            HeaderField("Vary", "", false),
            HeaderField("WWW-Authenticate", "", false)
        };
    }
}
