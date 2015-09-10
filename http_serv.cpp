#include "http_serv.hpp"


namespace Http {

    Header::Header(const std::string& field, const HeaderType& type) : field(field), type(type) {}

    bool Header::operator<(const Header& that) const {
        return field < that.field;
    }

    bool Header::operator==(const Header& that) const {
        return field == that.field;
    }

    const std::map<std::string, Header> HTTPHeader = {

        {"Cache-Control", Header("Cache-Control", HeaderType::GENERAL)},
        {"Connection", Header("Connection", HeaderType::GENERAL)},
        {"Date", Header("Date", HeaderType::GENERAL)},
        {"Pragma", Header("Pragma", HeaderType::GENERAL)},
        {"Trailer", Header("Trailer", HeaderType::GENERAL)},
        {"Transfer-Encoding", Header("Transfer-Encoding", HeaderType::GENERAL)},
        {"Upgrade", Header("Upgrade", HeaderType::GENERAL)},
        {"Via", Header("Via", HeaderType::GENERAL)},
        {"Warning", Header("Warning", HeaderType::GENERAL)},

        {"Accept", Header("Accept", HeaderType::REQ)},
        {"Accept-Charset", Header("Accept-Charset", HeaderType::REQ)},
        {"Accept-Encoding", Header("Accept-Encoding", HeaderType::REQ)},
        {"Accept-Language", Header("Accept-Language", HeaderType::REQ)},
        {"Authorization", Header("Authorization", HeaderType::REQ)},
        {"Expect", Header("Expect", HeaderType::REQ)},
        {"From", Header("From", HeaderType::REQ)},
        {"Host", Header("Host", HeaderType::REQ)},
        {"If-Match", Header("If-Match", HeaderType::REQ)},
        {"If-Modified-Since", Header("If-Modified-Since", HeaderType::REQ)},
        {"If-None-Match", Header("If-None-Match", HeaderType::REQ)},
        {"If-Range", Header("If-Range", HeaderType::REQ)},
        {"If-Unmodified-Since", Header("If-Unmodified-Since", HeaderType::REQ)},
        {"Max-Forwards", Header("Max-Forwards", HeaderType::REQ)},
        {"Proxy-Authorization", Header("Proxy-Authorization", HeaderType::REQ)},
        {"Range", Header("Range", HeaderType::REQ)},
        {"Referer", Header("Referer", HeaderType::REQ)},
        {"TE", Header("TE", HeaderType::REQ)},
        {"User-Agent", Header("User-Agent", HeaderType::REQ)},

        {"Accept-Ranges", Header("Accept-Ranges", HeaderType::RES)},
        {"Age", Header("Age", HeaderType::RES)},
        {"ETag", Header("ETag", HeaderType::RES)},
        {"Location", Header("Location", HeaderType::RES)},
        {"Proxy-Authenticate", Header("Proxy-Authenticate", HeaderType::RES)},
        {"Retry-After", Header("Retry-After", HeaderType::RES)},
        {"Server", Header("Server", HeaderType::RES)},
        {"Vary", Header("Vary", HeaderType::RES)},
        {"WWW-Authenticate", Header("WWW-Authenticate", HeaderType::RES)},

        {"Allow", Header("Allow", HeaderType::ENTITY)},
        {"Content-Encoding", Header("Content-Encoding", HeaderType::ENTITY)},
        {"Content-Language", Header("Content-Language", HeaderType::ENTITY)},
        {"Content-Length", Header("Content-Length", HeaderType::ENTITY)},
        {"Content-Location", Header("Content-Location", HeaderType::ENTITY)},
        {"Content-Range", Header("Content-Range", HeaderType::ENTITY)},
        {"Content-Type", Header("Content-Type", HeaderType::ENTITY)},
        {"Expires", Header("Expires", HeaderType::ENTITY)},
        {"Last-Modified", Header("Last-Modified", HeaderType::ENTITY)}
    };

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

    HttpHandler::HttpHandler() {
        LoggingHandler.init();
        logInfo("HTTP handler created");

        // init tcp handler
        tcpHandler.handler = [](const int connFd) -> Status {
            // FIXME parse header and set route
            char greeting[] = "Hello, world";
            write(connFd, greeting, sizeof(greeting));
            return Status::Success;
        };
    }

    Status HttpHandler::init(const unsigned int& port) {
        // Socket::TcpSocket socket;
        tcpSocket.init(port, IP::IPv4);
        tcpSocket.setHandler(std::make_shared<Socket::TcpHandler>(tcpHandler));
        logInfo("HTTP handler init");
        return Status::Success;
    }

    Status HttpHandler::run() {
        tcpSocket.run();
        logInfo("HTTP handler running");
        return Status::Success;
    }

    std::set<Header> HttpHandler::parseHeader(const std::string& str) const {
        std::stringstream in(str);
        std::set<Header> headers;
        std::string line;
        while(std::getline(in, line)) {
            const std::size_t foundPos = line.find(':');
            if(foundPos != std::string::npos) {
                std::string field = line.substr(0, foundPos);
                Header header;
                try {
                    header = HTTPHeader.at(field);
                } catch(std::exception& err) {
                    logError("Unknow header field: " + field);
                }
                std::string content = trim(line.substr(foundPos + 1));
                header.content = content;
                headers.insert(header);
            }
        }
        return headers;
    }

    std::string HttpHandler::generateHeader() const {
    }
}
