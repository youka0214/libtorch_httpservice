#include "handler.h"

#include "../Inference.h"

int Handler::getInferResult(const HttpContextPtr &ctx) {
	hv::Json resp;

	if (ctx -> is(MULTIPART_FORM_DATA)) {
		std::string image_buffer;
		auto form = ctx -> form();
		if ( form.find("image") != form.end()) {
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
		resp["message"] = "Inference complete";
		resp["result"] = result;
		return ctx->send(resp.dump(), APPLICATION_JSON);
	}
	ctx->setStatus(HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE);
	resp["message"] = "Unsupported media";
	return ctx->send(resp.dump(), APPLICATION_JSON);
}

