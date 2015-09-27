#include "StaticFileHandler.hpp"


namespace Http {
    StaticFileHandler::StaticFileHandler(const std::string& dir, const Request& req) : dir(dir), req(req) {}

    StaticFileHandler::~StaticFileHandler() {
    }

    Response StaticFileHandler::get() const {
        return View();
    }
}
