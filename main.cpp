#include <hv/HttpServer.h>
#include "server/handler.h"
#include "Inference.h"

hv::HttpServer  g_http_server;
hv::HttpService g_http_service;

int main() {
    int port = 34568;

    g_http_service.POST("/getInferResult",Handler::getInferResult);

    g_http_server.port = port;
    g_http_server.registerHttpService(&g_http_service);

    std::cout << "Server running on port " << port << std::endl;
    g_http_server.run();

    return 0;
}