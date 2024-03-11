#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "gpupixel.h"
using namespace gpupixel;


std::shared_ptr<BeautyFaceFilter> beauty_face_filter_;
std::shared_ptr<FaceReshapeFilter> face_reshape_filter_;
std::shared_ptr<gpupixel::FaceMakeupFilter> lipstick_filter_;
std::shared_ptr<gpupixel::FaceMakeupFilter> blusher_filter_;
std::shared_ptr<SourceImage> gpuSourceImage;
std::shared_ptr<SourceRawDataInput> source_raw_input_;
std::shared_ptr<TargetRawDataOutput> target_raw_output_;
std::shared_ptr<TargetView> target_view;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

 void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}

int GUIDemo(char *image)
{
     GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // create filter
    // ----
 #if 0
    auto mouth = SourceImage::create("mouth.png");
    lipstick_filter_ = FaceMakeupFilter::create();
    lipstick_filter_->setImageTexture(mouth);
    lipstick_filter_->setTextureBounds(FrameBounds{502.5, 710, 262.5, 167.5});

    auto blusher = SourceImage::create("blusher.png");
    blusher_filter_ = FaceMakeupFilter::create();
    blusher_filter_->setImageTexture(blusher);
    blusher_filter_->setTextureBounds(FrameBounds{395, 520, 489, 209});
 #endif
    beauty_face_filter_ = BeautyFaceFilter::create();
 
    //  filter pipline
    // ----
    gpuSourceImage = SourceImage::create(image);
    target_view = std::make_shared<TargetView>();
    
    gpuSourceImage->addTarget(beauty_face_filter_)
                  ->addTarget(target_view); 
   
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    // 
    target_view->onSizeChanged(1280, 720);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // 
        // -----
        gpuSourceImage->proceed();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

int GUIDemo1(char *image)
{
	int width   = 564;
    int height  = 846;
	int ch		= 4;
    int stride  = width*ch;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = stride * height;
    unsigned char *raw_input	= NULL;

    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.rgba", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);

     GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // create filter
    // ----
 #if 0
    auto mouth = SourceImage::create("mouth.png");
    lipstick_filter_ = FaceMakeupFilter::create();
    lipstick_filter_->setImageTexture(mouth);
    lipstick_filter_->setTextureBounds(FrameBounds{502.5, 710, 262.5, 167.5});

    auto blusher = SourceImage::create("blusher.png");
    blusher_filter_ = FaceMakeupFilter::create();
    blusher_filter_->setImageTexture(blusher);
    blusher_filter_->setTextureBounds(FrameBounds{395, 520, 489, 209});
 #endif
    beauty_face_filter_ = BeautyFaceFilter::create();
 
    //  filter pipline
    // ----
    //gpuSourceImage = SourceImage::create(image);
    gpuSourceImage = SourceImage::create_from_memory(width, height, ch, raw_input);
    target_view = std::make_shared<TargetView>();
    
    gpuSourceImage->addTarget(beauty_face_filter_)
                  ->addTarget(target_view); 
   
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    // 
    target_view->onSizeChanged(1280, 720);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // 
        // -----
        gpuSourceImage->proceed();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

int GUIDemo2(char *image)
{
	int width   = 564;
    int height  = 846;
	int ch		= 4;
    int stride  = width*ch;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = stride * height;
    unsigned char *raw_input	= NULL;

    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.rgba", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);

     GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // create filter
    // ----
 #if 0
    auto mouth = SourceImage::create("mouth.png");
    lipstick_filter_ = FaceMakeupFilter::create();
    lipstick_filter_->setImageTexture(mouth);
    lipstick_filter_->setTextureBounds(FrameBounds{502.5, 710, 262.5, 167.5});

    auto blusher = SourceImage::create("blusher.png");
    blusher_filter_ = FaceMakeupFilter::create();
    blusher_filter_->setImageTexture(blusher);
    blusher_filter_->setTextureBounds(FrameBounds{395, 520, 489, 209});
 #endif
    beauty_face_filter_ = BeautyFaceFilter::create();
 
    //  filter pipline
    // ----
    //gpuSourceImage = SourceImage::create(image);
    //gpuSourceImage = SourceImage::create_from_memory(width, height, ch, raw_input);
    source_raw_input_	= SourceRawDataInput::create();
    source_raw_input_->uploadBytes(raw_input, width, height, stride);
    target_view = std::make_shared<TargetView>();
    
    source_raw_input_->addTarget(beauty_face_filter_)
                  ->addTarget(target_view); 
   
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    // 
    target_view->onSizeChanged(1280, 720);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // 
        // -----
        source_raw_input_->proceed();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    printf("width:%d, height:%d, \n", width, height);
    glViewport(0, 0, width, height);
    
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

int RawDemo(char *image)
{
    int width   = 564;
    int height  = 846;
    int stride  = width*4;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = stride * height;
    unsigned char *raw_input	= NULL;

    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.rgba", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);
    GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //  filter pipline
    // ----
    source_raw_input_	= SourceRawDataInput::create();
    target_raw_output_	= TargetRawDataOutput::create();
    //target_raw_output_	= std::make_shared<TargetRawDataOutput>();
    beauty_face_filter_ = BeautyFaceFilter::create();

    source_raw_input_->addTarget(beauty_face_filter_)
                    ->addTarget(target_raw_output_);

    //target_raw_output_->onSizeChanged(width, height);
    //target_raw_output_->setPixelsCallbck(i420_output_cb);
    target_raw_output_->setI420Callbck(i420_output_cb);
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    source_raw_input_->uploadBytes(raw_input, width, height, stride);
    
        
    source_raw_input_->proceed();

    glfwTerminate();
    return 0;
}

int RawDemo2(char *image)
{
    int width   = 564;
    int height  = 846;
    int stride  = width*3;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = stride * height;
    unsigned char *raw_input	= NULL;

    printf("RawDemo2.\n");
    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.rgb", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);
    GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //  filter pipline
    // ----
    gpuSourceImage = SourceImage::create_from_memory(width, height, 3, raw_input);
    target_raw_output_	= TargetRawDataOutput::create();
    //target_raw_output_	= std::make_shared<TargetRawDataOutput>();
    beauty_face_filter_ = BeautyFaceFilter::create();

    gpuSourceImage->addTarget(beauty_face_filter_)
                    ->addTarget(target_raw_output_);

    //target_raw_output_->onSizeChanged(width, height);
    //target_raw_output_->setPixelsCallbck(output_cb);
    target_raw_output_->setI420Callbck(i420_output_cb);
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    gpuSourceImage->proceed();

    glfwTerminate();
    return 0;
}

int RawDemo3(char *image)
{
	int width   = 564;
    int height  = 846;
	int ch		= 3;
    int stride  = width*ch;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = stride * height;
    unsigned char *raw_input	= NULL;

	{
    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.rgb", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);

     GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // create filter
    // ----
 #if 0
    auto mouth = SourceImage::create("mouth.png");
    lipstick_filter_ = FaceMakeupFilter::create();
    lipstick_filter_->setImageTexture(mouth);
    lipstick_filter_->setTextureBounds(FrameBounds{502.5, 710, 262.5, 167.5});

    auto blusher = SourceImage::create("blusher.png");
    blusher_filter_ = FaceMakeupFilter::create();
    blusher_filter_->setImageTexture(blusher);
    blusher_filter_->setTextureBounds(FrameBounds{395, 520, 489, 209});
 #endif
    beauty_face_filter_ = BeautyFaceFilter::create();
 
    //  filter pipline
    // ----
    //gpuSourceImage = SourceImage::create(image);
    gpuSourceImage = SourceImage::create_from_memory(width, height, ch, raw_input);
    //target_view = std::make_shared<TargetView>();
    target_raw_output_	= std::make_shared<TargetRawDataOutput>();
    
    gpuSourceImage->addTarget(beauty_face_filter_)
                  ->addTarget(target_raw_output_); 
                  //->addTarget(target_view); 
   
    target_raw_output_->setI420Callbck(i420_output_cb);
    //target_raw_output_->setPixelsCallbck(rgba_output_cb);
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    // 
    //target_view->onSizeChanged(1280, 720);
    //target_raw_output_->onSizeChanged(1280, 720);
    
    // render loop
    // -----------
    //while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // 
        // -----
        gpuSourceImage->proceed();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        //glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
	}

    return 0;
}

int RawDemoRGBA(char *image)
{
	int width   = 564;
    int height  = 846;
	int ch		= 4;
    int stride  = width*ch;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = stride * height;
    unsigned char *raw_input	= NULL;

	{
    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.rgba", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);

     GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // create filter
    // ----
 #if 0
    auto mouth = SourceImage::create("mouth.png");
    lipstick_filter_ = FaceMakeupFilter::create();
    lipstick_filter_->setImageTexture(mouth);
    lipstick_filter_->setTextureBounds(FrameBounds{502.5, 710, 262.5, 167.5});

    auto blusher = SourceImage::create("blusher.png");
    blusher_filter_ = FaceMakeupFilter::create();
    blusher_filter_->setImageTexture(blusher);
    blusher_filter_->setTextureBounds(FrameBounds{395, 520, 489, 209});
 #endif
    beauty_face_filter_ = BeautyFaceFilter::create();
 
    //  filter pipline
    // ----
    //gpuSourceImage = SourceImage::create(image);
    //gpuSourceImage = SourceImage::create_from_memory(width, height, ch, raw_input);
    source_raw_input_	= SourceRawDataInput::create();
    //target_view = std::make_shared<TargetView>();
    target_raw_output_	= std::make_shared<TargetRawDataOutput>();
    
    source_raw_input_->addTarget(beauty_face_filter_)
                  ->addTarget(target_raw_output_); 
                  //->addTarget(target_view); 
   
    target_raw_output_->setI420Callbck(i420_output_cb);
    //target_raw_output_->setPixelsCallbck(rgba_output_cb);
    // set default value
    // ----
    beauty_face_filter_->setBlurAlpha(9.0f/10);
    beauty_face_filter_->setWhite(9.0f/20);

    // 
    //target_view->onSizeChanged(1280, 720);
    //target_raw_output_->onSizeChanged(1280, 720);
    
    // render loop
    // -----------
    //while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        
        // 
        // -----
        //gpuSourceImage->proceed();
		source_raw_input_->uploadBytes(raw_input, width, height, stride);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        //glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
	}

    return 0;
}

int RawDemoI420(char *image)
{
	int width   = 564;
    int height  = 846;
    FILE *fp    = NULL;
    int nrd     = 0;
    size_t size = width * height * 3/2;
    unsigned char *raw_input	= NULL;
    unsigned char *dataY		= NULL;
	int	yStride					= width;
    unsigned char *dataU		= NULL;
	int	uStride					= width/2;
    unsigned char *dataV		= NULL;
	int	vStride					= width/2;

	{
	std::shared_ptr<SourceRawDataInput> source_raw_input;
	std::shared_ptr<BeautyFaceFilter> beauty_face_filter;
	std::shared_ptr<TargetRawDataOutput> target_raw_output;

    raw_input = (unsigned char *)malloc(size);
    if (raw_input == NULL)
		exit(1);

    fp = fopen("demo.yuv", "r");
    if (fp == NULL)
		exit(1);

    nrd = fread(raw_input, 1, size, fp);
    if (nrd != size)
		exit(1);

	dataY = raw_input;
	dataU = dataY + width * height;
	dataV = dataU + width/2 * height/2;

     GLFWwindow* window = GPUPixelContext::getInstance()->GetGLContext();
  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //glfwShowWindow(window);

    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // create filter
    // ----
 #if 0
    auto mouth = SourceImage::create("mouth.png");
    lipstick_filter_ = FaceMakeupFilter::create();
    lipstick_filter_->setImageTexture(mouth);
    lipstick_filter_->setTextureBounds(FrameBounds{502.5, 710, 262.5, 167.5});

    auto blusher = SourceImage::create("blusher.png");
    blusher_filter_ = FaceMakeupFilter::create();
    blusher_filter_->setImageTexture(blusher);
    blusher_filter_->setTextureBounds(FrameBounds{395, 520, 489, 209});
 #endif
    beauty_face_filter = BeautyFaceFilter::create();
 
    //  filter pipline
    // ----
    //gpuSourceImage = SourceImage::create(image);
    //gpuSourceImage = SourceImage::create_from_memory(width, height, ch, raw_input);
    source_raw_input	= SourceRawDataInput::create();
    //target_view = std::make_shared<TargetView>();
    target_raw_output	= std::make_shared<TargetRawDataOutput>();
    
    source_raw_input->addTarget(beauty_face_filter)
                  ->addTarget(target_raw_output); 
                  //->addTarget(target_view); 
   
    //target_raw_output->setI420Callbck(i420_output_cb);
    target_raw_output->setPixelsCallbck(rgba_output_cb);
    // set default value
    // ----
    beauty_face_filter->setBlurAlpha(9.0f/10);
    beauty_face_filter->setWhite(9.0f/20);

    // 
    //target_view->onSizeChanged(1280, 720);
    //target_raw_output_->onSizeChanged(1280, 720);
    
    // render loop
    // -----------
    //while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        //processInput(window);
        
        // 
        // -----
        //gpuSourceImage->proceed();
		source_raw_input->uploadBytes(width, height,
				dataY, yStride,
				dataU, uStride,
				dataV, vStride);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //glfwSwapBuffers(window);
        //glfwPollEvents();
    }

    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	}
    glfwTerminate();

    return 0;
}



static char image_dlf[] = "demo.jpg";
int main(int argc, char **argv)
{
	char *image = image_dlf;
	if (argc == 2)
		image = argv[1];
	//GUIDemo(image);
	//GUIDemo1(image);
	//GUIDemo2(image);
	//RawDemo(image);
	//RawDemo2(image);
	//RawDemo3(image);

	//RawDemoRGBA(image);
	RawDemoI420(image);
}
