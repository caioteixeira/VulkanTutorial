#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
//Avoid OpenGL depth range
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Vertex.h"
#include <stdexcept>
#include <vector>
#include "VkDebugProxies.h"
#include "VDeleter.h"
#include <chrono>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData
);

class Renderer
{
public:
	void runRenderer();

private:
	void initWindow();
	void initVulkan();
	void renderLoop();

	static void onWindowResized(GLFWwindow * window, int width, int height);

	void recreateSwapChain();
	void setupDebugCallback();

	void createInstance();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandPool();
	void createDepthResources();
	void createTextureImage();
	void createTextureImageView();
	void createTextureSampler();
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffer();
	void createDescriptorPool();
	void createDescriptorSet();
	void createCommandBuffers();
	void createSemaphores();

	void updateUniformBuffer();

	void drawFrame();

	GLFWwindow* window;

	VDeleter<VkInstance> mInstance{ vkDestroyInstance };
	VDeleter<VkDebugReportCallbackEXT> mDebugCallback{ mInstance, DestroyDebugReportCallbackEXT };
	VDeleter<VkSurfaceKHR> mSurface{ mInstance, vkDestroySurfaceKHR };

	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VDeleter<VkDevice> mDevice{ vkDestroyDevice };

	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;

	VDeleter<VkSwapchainKHR> mSwapChain{ mDevice, vkDestroySwapchainKHR };
	std::vector<VkImage> mSwapChainImages;
	VkFormat mSwapChainImageFormat;
	VkExtent2D mSwapChainExtent;
	std::vector<VDeleter<VkImageView>> mSwapChainImageViews;

	VDeleter<VkRenderPass> mRenderPass{ mDevice, vkDestroyRenderPass };
	VDeleter<VkDescriptorSetLayout> mDescriptorSetLayout{ mDevice, vkDestroyDescriptorSetLayout };
	VDeleter<VkPipelineLayout> mPipelineLayout{ mDevice, vkDestroyPipelineLayout };
	VDeleter<VkPipeline> mGraphicsPipeline{ mDevice, vkDestroyPipeline };
	std::vector<VDeleter<VkFramebuffer>> mSwapChainFramebuffers;
	VDeleter<VkCommandPool> mCommandPool{ mDevice, vkDestroyCommandPool };
	std::vector<VkCommandBuffer> mCommandBuffers;
	VDeleter<VkSemaphore> mImageAvailableSemaphore{ mDevice, vkDestroySemaphore };
	VDeleter<VkSemaphore> mRenderFinishedSemaphore{ mDevice, vkDestroySemaphore };

	VDeleter<VkDescriptorPool> mDescriptorPool{ mDevice, vkDestroyDescriptorPool };
	VkDescriptorSet mDescriptorSet;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	const int WIDTH = 800;
	const int HEIGHT = 600;

	const std::string MODEL_PATH = "models/chalet.obj";
	const std::string TEXTURE_PATH = "textures/chalet.jpg";

	std::vector<Vertex> mVertices;
	std::vector<uint32_t> mIndices;

	VDeleter<VkImage> mTextureImage{ mDevice, vkDestroyImage };
	VDeleter<VkImageView> mTextureImageView{ mDevice, vkDestroyImageView };
	VDeleter<VkDeviceMemory> mTextureImageMemory{ mDevice, vkFreeMemory };
	VDeleter<VkSampler> mTextureSampler{ mDevice, vkDestroySampler };

	VDeleter<VkImage> mDepthImage{ mDevice, vkDestroyImage };
	VDeleter<VkDeviceMemory> mDepthImageMemory{ mDevice, vkFreeMemory };
	VDeleter<VkImageView> mDepthImageView{ mDevice, vkDestroyImageView };

	VDeleter<VkBuffer> mVertexBuffer{ mDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> mVertexBufferMemory{ mDevice, vkFreeMemory };
	VDeleter<VkBuffer> mIndexBuffer{ mDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> mIndexBufferMemory{ mDevice, vkFreeMemory };
	VDeleter<VkBuffer> mUniformStagingBuffer{ mDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> mUniformStagingBufferMemory{ mDevice, vkFreeMemory };
	VDeleter<VkBuffer> mUniformBuffer{ mDevice, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> mUniformBufferMemory{ mDevice, vkFreeMemory };

	//Helper functions
	//TODO: Move some functions to a utils file
	bool checkValidationLayerSupport() const;

	static std::vector<const char*> getRequiredExtensions();

	bool isDeviceSuitable(VkPhysicalDevice device) const;

	bool checkDeviceExtensionSupport(VkPhysicalDevice device) const;

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;

	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);

	//Swap extent is the resolution of the swap chain images
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

	VkCommandBuffer beginSingleTimeCommands() const;

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule) const;

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
	                  VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory) const;

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
	                 VkMemoryPropertyFlags properties, VDeleter<VkImage>& image, VDeleter<VkDeviceMemory>& imageMemory) const;

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) const;

	void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height) const;

	void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VDeleter<VkImageView>& imageView) const;

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

	VkFormat findDepthFormat() const;

	static bool hasStencilComponent(VkFormat format);

	void loadModel();
};
