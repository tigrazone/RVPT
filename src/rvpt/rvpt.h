#pragma once

#include <string>
#include <vector>
#include <optional>
#include <iostream>

#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <VkBootstrap.h>
#include "window.h"

#include "vk_util.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

class RVPT
{
   public:
    explicit RVPT(window& window);
    ~RVPT();

    RVPT(RVPT const& other) = delete;
    RVPT operator=(RVPT const& other) = delete;
    RVPT(RVPT&& other) = delete;
    RVPT operator=(RVPT&& other) = delete;

    // Create a Vulkan context and do all the one time initialization
    bool initialize();

    bool update();

    enum class draw_return
    {
        success,
        swapchain_out_of_date
    };

    draw_return draw();

    void shutdown();

   private:
    window& window_ref;

    // from a callback
    bool framebuffer_resized = false;

    struct Context
    {
        VkSurfaceKHR surf{};
        vkb::Instance inst{};
        vkb::Device device{};
    } context;
    VkDevice vk_device{};

    // safe to assume its always available
    std::optional<VK::Queue> graphics_queue;
    std::optional<VK::Queue> present_queue;

    // not safe to assume, not all hardware has a dedicated compute queue
    std::optional<VK::Queue> compute_queue;

    VK::PipelineBuilder pipeline_builder;
    VK::Memory memory_allocator;

    vkb::Swapchain vkb_swapchain;
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;

    uint32_t current_frame_index = 0;
    std::vector<VK::FrameResources> frame_resources;
    std::vector<VkFence> frames_inflight_fences;

    VkRenderPass fullscreen_tri_render_pass;
    VK::Pipeline fullscreen_triangle_pipeline;

    std::vector<VK::Framebuffer> framebuffers;

    std::optional<VK::Image> sampled_image;
    std::optional<VK::DescriptorPool> sampled_image_pool;
    std::optional<VK::DescriptorSet> sampled_image_descriptor_set;

    // helper functions
    bool context_init();
    bool swapchain_init();
    bool swapchain_reinit();
    bool swapchain_get_images();
    void create_framebuffers();
};