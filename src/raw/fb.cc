#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "gpupixel.h"
using namespace gpupixel;

static GLFWwindow *fb_context = NULL;
extern "C" void
fb_init(void)
{
	fb_context = GPUPixelContext::getInstance()->GetGLContext();

	if (fb_context == NULL) {
		std::cout << "Failed to create FB context." << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(fb_context);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		fb_context = NULL;
		return;
	}
}

extern "C" void
fb_exit(void)
{
	glfwTerminate();
}

void i420_output_cb(const unsigned char *data, int width, int height, long ts)
{
	FILE *fp    = NULL;

	printf("width:%d, height:%d, \n", width, height);
	fp = fopen("out_demo.yuv", "w");
	if (fp == NULL) {
		printf("open file failed.\n");
		exit(1);
	}

	fwrite(data, 1, width*height*3/2, fp);
	fclose(fp);
	printf("output_cb end.\n");
}

void rgba_output_cb(const unsigned char *data, int width, int height, long ts)
{
	FILE *fp    = NULL;

	printf("width:%d, height:%d, \n", width, height);
	fp = fopen("out_demo.rgba", "w");
	if (fp == NULL) {
		printf("open file failed.\n");
		exit(1);
	}

	fwrite(data, 1, width*height*4, fp);
	fclose(fp);
	printf("output_cb end.\n");
}

int
fb_handle_frame(unsigned char *frame, int width, int height, int fmt)
{
	unsigned char *dataY	= frame;
	int	yStride				= width;
	unsigned char *dataU	= dataY + width * height;
	int	uStride				= width/2;
	unsigned char *dataV	= dataU + width/2 * height/2;
	int	vStride				= width/2;

	std::shared_ptr<SourceRawDataInput> source_raw_input;
	std::shared_ptr<BeautyFaceFilter> beauty_face_filter;
	std::shared_ptr<TargetRawDataOutput> target_raw_output;

	beauty_face_filter	= BeautyFaceFilter::create();
	source_raw_input	= SourceRawDataInput::create();
	target_raw_output	= std::make_shared<TargetRawDataOutput>();

	source_raw_input->addTarget(beauty_face_filter)
				  ->addTarget(target_raw_output); 

	//target_raw_output->setI420Callbck(i420_output_cb);
	target_raw_output->setPixelsCallbck(rgba_output_cb);

	// set default value
	beauty_face_filter->setBlurAlpha(9.0f/10);
	beauty_face_filter->setWhite(9.0f/20);

	source_raw_input->uploadBytes(width, height,
				dataY, yStride,
				dataU, uStride,
				dataV, vStride);

	return 0;
}

extern "C" int
fb_process(unsigned char *frame, int width, int height, int fmt)
{
	if (fb_context == NULL) {
		std::cout << "Failed to create FB context." << std::endl;
		glfwTerminate();
		return -1;
	}

	return fb_handle_frame(frame, width, height, fmt);
}

