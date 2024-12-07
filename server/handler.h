#ifndef HANDLER_H
#define HANDLER_H

#include <hv/HttpService.h>

class Handler {
public:
	static int getInferResult(const HttpContextPtr& ctx);
private:
	static int response_status(HttpResponse* resp, int code = 200, const char* message = NULL) {
		if (message == NULL) message = http_status_str((enum http_status)code);
		resp->Set("code", code);
		resp->Set("message", message);
		return code;
	}
	static int response_status(const HttpResponseWriterPtr& writer, int code = 200, const char* message = NULL) {
		response_status(writer->response.get(), code, message);
		writer->End();
		return code;
	}
	static int response_status(const HttpContextPtr& ctx, int code = 200, const char* message = NULL) {
		response_status(ctx->response.get(), code, message);
		ctx->send();
		return code;
	}
};

#endif //HANDLER_H
