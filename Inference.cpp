#include "Inference.h"
#include <opencv2/opencv.hpp>

Inference::Inference(const std::string &model_path): device(torch::kCPU) {
	// 选择设备
	device = torch::cuda::is_available() ? torch::kCUDA : torch::kCPU;

	try {
		// 加载模型并放至设备上
		module = torch::jit::load(model_path);
		module.to(device);
	} catch (const c10::Error& e) {
		std::cerr << "模型加载失败: " << e.what() << '\n';
		throw;
	}
}

void Inference::setInput(const std::string& image_buffer) {
	// 使用std::string的data()成员函数获取指向数据的指针
	const unsigned char* buffer = reinterpret_cast<const unsigned char*>(image_buffer.data());
	int length = image_buffer.size(); // 获取数据的长度

	// 使用imdecode解码图像数据
	cv::Mat image = cv::imdecode(cv::Mat(length, 1, CV_8UC1, const_cast<unsigned char*>(buffer)), cv::IMREAD_COLOR);
	if (image.empty()) {
		throw std::runtime_error("无法从缓冲区解码图像");
	}

	// 将图像缩放到224x224
	cv::Mat resized_image;
	cv::resize(image, resized_image, cv::Size(224, 224), 0, 0, cv::INTER_LINEAR);

	// 将图像转换为浮点数并归一化
	cv::Mat image_float;
	image.convertTo(image_float, CV_32F, 1.0 / 255.0);

	// 转换为CHW格式
	std::vector<cv::Mat> image_channels(3);
	cv::split(image_float, image_channels);
	std::swap(image_channels[0], image_channels[2]);

	// 转换为torch::Tensor
	torch::Tensor image_tensor = torch::from_blob(image_channels[0].data,
									{image_float.rows, image_float.cols, 3})
				   .permute({2, 0, 1})
				   .to(torch::kFloat32);

	// 添加批次维度
	image_tensor = image_tensor.unsqueeze(0);

	// 移动至设备
	input_tensor = image_tensor.to(device);
}

int Inference::infer_int() {
	// 前向推理
	std::vector<torch::jit::IValue> inputs;
	inputs.emplace_back(input_tensor);
	torch::jit::IValue output = module.forward(inputs);

	// 获取最大值索引
	auto ouputTensor = output.toTensor();
	torch::Tensor output_max = ouputTensor.argmax(1);
	int index = output_max.item().toInt();

	return index;
}


