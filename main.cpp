#include <hv/HttpServer.h>

#include "Inference.h"
#include "opencv2/opencv.hpp"

int main() {
    HttpService router;

    router.POST("/recognition", [](const HttpContextPtr& ctx) {
        hv::Json resp;

        if (ctx->is(MULTIPART_FORM_DATA)) {
            auto form = ctx->form();
            std::string image_buffer;
            if (form.find("image") != form.end()) {
                image_buffer = form["image"].content;
            }
            else {
                ctx->setStatus(HTTP_STATUS_BAD_REQUEST);
                resp["message"] = "No image found in the request";
                return ctx->send(resp.dump(), APPLICATION_JSON);
            }

            std::string model_path = "/data/model/convnext_large.pt";
            Inference inference(model_path);

            inference.setInput(image_buffer);
            int result = inference.infer_int();

            ctx->setStatus(HTTP_STATUS_OK);
            resp["message"] = "Image received and processed";
            resp["result"] = result;
            return ctx->send(resp.dump(), APPLICATION_JSON);
        }
        ctx->setStatus(HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE);
        resp["message"] = "Unsupported media";
        return ctx->send(resp.dump(), APPLICATION_JSON);
    });

    hv::HttpServer http_server;
    http_server.setPort(34568);
    http_server.registerHttpService(&router);
    std::cout << "Server running on port 34568" << std::endl;
    http_server.run();

    return 0;
}