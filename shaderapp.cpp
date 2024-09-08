#include "./TinyVulkan/TinyVulkan.hpp"
using namespace tinyvk;

#define DEFAULT_VERTEX_SHADER "./Shaders/passthrough_vert.spv"
#define DEFAULT_FRAGMENT_SHADER "./Shaders/passthrough_frag.spv"
const std::tuple<VkShaderStageFlagBits, std::string> vertexShader = { VK_SHADER_STAGE_VERTEX_BIT, DEFAULT_VERTEX_SHADER };
const std::tuple<VkShaderStageFlagBits, std::string> fragmentShader = { VK_SHADER_STAGE_FRAGMENT_BIT, DEFAULT_FRAGMENT_SHADER };
const std::vector<std::tuple<VkShaderStageFlagBits, std::string>> defaultShaders = { vertexShader, fragmentShader };

const std::vector<VkPhysicalDeviceType> rdeviceTypes = { VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU };
const TinyVkBufferingMode bufferingMode = TinyVkBufferingMode::DOUBLE;
const TinyVkVertexDescription vertexDescription = TinyVkVertex::GetVertexDescription();
const std::vector<VkDescriptorSetLayoutBinding> pushDescriptorLayouts = {
    TinyVkGraphicsPipeline::SelectPushDescriptorLayoutBinding(0, TINYVK_DESCRIPTOR_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1)
};

int32_t TINYVULKAN_WINDOWMAIN {
    TinyVkWindow window("Sample Application", 1920, 1080, true, false);
    TinyVkVulkanDevice vkdevice("Sample Application", rdeviceTypes, &window, window.QueryRequiredExtensions(TINYVK_VALIDATION_LAYERS));
    TinyVkCommandPool commandPool(vkdevice);
    TinyVkGraphicsPipeline pipeline(vkdevice, vertexDescription, defaultShaders, pushDescriptorLayouts, {}, false);
    TinyVkSwapChainRenderer swapRenderer(vkdevice, window, pipeline, bufferingMode);

    swapRenderer.onRenderEvents.hook(TinyVkCallback<TinyVkCommandPool&>([&window, &swapRenderer, &pipeline](TinyVkCommandPool& commandPool) {
        auto commandBuffer = commandPool.LeaseBuffer();
        swapRenderer.BeginRecordCmdBuffer(commandBuffer.first);
        
        
        
        swapRenderer.EndRecordCmdBuffer(commandBuffer.first);
    }));

    std::thread mythread([&window, &swapRenderer]() { while (!window.ShouldClose()) { swapRenderer.RenderExecute(); } });
    window.WhileMain(true);
    mythread.join();
    return VK_SUCCESS;
}

/*
    Desktop Shadertoy: Shader Inputs
        0. fIndexOfFrame   - Current frame texture index.
        1. fIndexOfPass    - Current pass index (for multi-pass shaders).
        2. fTexture[0-9]   - 10 total frame textures (index: fIndexOfFrame).
              * recursive multi-pass shaders use up two textures.
        3. fWidth[0]       - width of each frame texture 0-9 (index: fIndexOfFrame).
        4. fHeight[0]      - height of each frame texture 0-9 (index: fIndexOfFrame).
        5. fMouseXY        - Mouse XY-pos from Top/Left of Frame.
        6. fMousePrevXY    - Mouse XY-pos from Top/Left of Previous Frame.
        7. fUserInput[0-9] - User-Set Inputs from GUI interface.
        8. fRunTime        - Current Time in Milliseconds.
        9. fDeltaTime      - Time between Current and Previous Frame.
*/