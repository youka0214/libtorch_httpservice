#ifndef INFERENCE_H
#define INFERENCE_H

#include <torch/torch.h>
#include <torch/script.h>

class Inference {
	std::string model_path;
	torch::jit::script::Module module;
	torch::DeviceType device;
	torch::Tensor input_tensor;
public:
	explicit Inference(const std::string& model_path);

	// 从图像缓冲区读取和预处理图像
	void setInput(const std::string& image_buffer);
	// 推理函数，返回值为最大值索引
	int infer_int();
};

#endif //INFERENCE_H
