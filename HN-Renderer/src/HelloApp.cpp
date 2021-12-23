#include "pch.h"
#include "HelloApp.h"

namespace HN
{
    HelloTriangleApplication::HelloTriangleApplication()
    {
        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffers();
    }

    HelloTriangleApplication::~HelloTriangleApplication()
    {
        vkDestroyPipelineLayout(Device.device(), pipelineLayout, nullptr);
    }

    void HelloTriangleApplication::run()
    {
        while (!Window.ShouldClose())
        {
            glfwPollEvents();
        }
    }

    void HelloTriangleApplication::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
            throw std::runtime_error("Failed to create Pipeline layout.");
    }


    void HelloTriangleApplication::CreatePipeline()
    {
        auto pipelineConfig = Pipeline::DefaultPipelineConfigInfo(SwapChain.width(), SwapChain.height());
        pipelineConfig.renderPass = SwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;

        pipeline = std::make_unique<Pipeline>(
            Device, 
            "src/Shaders/simple_shader.vert.spv",
            "src/Shaders/simple_shader.frag.spv",
            pipelineConfig);
    }


    void HelloTriangleApplication::CreateCommandBuffers()
    {

    }


    void HelloTriangleApplication::DrawFrame()
    {

    }

    /*
    void HelloTriangleApplication::initVulkan()
    {
        createInstance();
    }


    void HelloTriangleApplication::createInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);    // get number of extensions

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create vulkan instance!");
        }
    }


    void HelloTriangleApplication::mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }


    void HelloTriangleApplication::initWindow()
    {
        glfwInit();

        // Telling glfw that context is not created for OpenGL
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // disable window resize
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan window", nullptr, nullptr);
    }


    void HelloTriangleApplication::cleanup()
    {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);
        glfwTerminate();
    }



    void HelloTriangleApplication::checkExtensions()
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "Available extensions:\n";

        for (const auto& extension : extensions)
            std::cout << "\t" << extension.extensionName << "\n";
    }
    */
}