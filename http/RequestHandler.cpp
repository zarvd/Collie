#include "RequestHandler.hpp"


namespace Http {
    RequestHandler::~RequestHandler() {}

    Response RequestHandler::get() {
        throw std::domain_error("Uninitialized GET method");
    }

    Response RequestHandler::post() {
        throw std::domain_error("Uninitialized POST method");
    }

    Response RequestHandler::put() {
        throw std::domain_error("Uninitialized PUT method");
    }

    Response RequestHandler::head() {
        throw std::domain_error("Uninitialized HEAD method");
    }
}
