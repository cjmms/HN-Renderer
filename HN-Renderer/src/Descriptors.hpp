#pragma once
#include "pch.h"
#include "Device.hpp"

namespace HN
{
    // works as a blue print for the pipeline
    // so that pipeline will be aware of what to expect
    class DescriptorSetLayout {
        public:
            class Builder {
                public:
                    Builder(Device& device) : device{ device } {}

                    // descriptorType: uniform buffer? image buffer? etc.
                    // stageFlags: vertex buffer? fragment shader? etc.
                    // count: each binding can hold an array of Descriptors
                    Builder& addBinding(
                        uint32_t binding,
                        VkDescriptorType descriptorType,
                        VkShaderStageFlags stageFlags,
                        uint32_t count = 1);

                    inline std::unique_ptr<DescriptorSetLayout> build() const { return std::make_unique<DescriptorSetLayout>(device, bindings); }

                private:
                    Device& device;
                    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
            };

            DescriptorSetLayout(
                Device& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
            ~DescriptorSetLayout();
            DescriptorSetLayout(const DescriptorSetLayout&) = delete;
            DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;

            inline VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

        private:
            Device& device;
            VkDescriptorSetLayout descriptorSetLayout;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class DescriptorWriter;
    };

    class DescriptorPool {
    public:
        class Builder {
        public:
            Builder(Device& device) : device{ device } {}

            // example:
            // create a pool that has 100 uniform descriptors
            // create a pool that has 100 samplers descriptors
            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);

            // configure the behavior
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);

            // max number of descriptors can be allocated for current pool
            Builder& setMaxSets(uint32_t count);
            inline std::unique_ptr<DescriptorPool> build() const { return std::make_unique<DescriptorPool>(device, maxSets, poolFlags, poolSizes); }

        private:
            Device& device;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        DescriptorPool(
            Device& device,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~DescriptorPool();
        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        inline void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const {
            vkFreeDescriptorSets(device.device(), descriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
        }

        inline void resetPool() { vkResetDescriptorPool(device.device(), descriptorPool, 0); }

    private:
        Device& device;
        VkDescriptorPool descriptorPool;

        friend class DescriptorWriter;
    };




    class DescriptorWriter {
    public:
        DescriptorWriter(DescriptorSetLayout& setLayout, DescriptorPool& pool);

        DescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        DescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        DescriptorSetLayout& setLayout;
        DescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}