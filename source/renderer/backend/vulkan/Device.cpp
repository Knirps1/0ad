/* Copyright (C) 2023 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"

#include "Device.h"

#include "lib/external_libraries/libsdl.h"
#include "scriptinterface/JSON.h"
#include "scriptinterface/Object.h"
#include "scriptinterface/ScriptInterface.h"
#include "scriptinterface/ScriptRequest.h"

#if SDL_VERSION_ATLEAST(2, 0, 8)
#include <SDL_vulkan.h>
#endif

namespace Renderer
{

namespace Backend
{

namespace Vulkan
{

// static
std::unique_ptr<CDevice> CDevice::Create(SDL_Window* UNUSED(window))
{
	std::unique_ptr<CDevice> device(new CDevice());
	return device;
}

CDevice::CDevice() = default;

CDevice::~CDevice() = default;

void CDevice::Report(const ScriptRequest& rq, JS::HandleValue settings)
{
	Script::SetProperty(rq, settings, "name", "vulkan");

	std::string vulkanSupport = "unsupported";
	// According to http://wiki.libsdl.org/SDL_Vulkan_LoadLibrary the following
	// functionality is supported since SDL 2.0.8.
#if SDL_VERSION_ATLEAST(2, 0, 8)
	if (!SDL_Vulkan_LoadLibrary(nullptr))
	{
		void* vkGetInstanceProcAddr = SDL_Vulkan_GetVkGetInstanceProcAddr();
		if (vkGetInstanceProcAddr)
			vulkanSupport = "supported";
		else
			vulkanSupport = "noprocaddr";
		SDL_Vulkan_UnloadLibrary();
	}
	else
	{
		vulkanSupport = "cantload";
	}
#endif
	Script::SetProperty(rq, settings, "status", vulkanSupport);
}

std::unique_ptr<IDeviceCommandContext> CDevice::CreateCommandContext()
{
	return nullptr;
}

std::unique_ptr<IGraphicsPipelineState> CDevice::CreateGraphicsPipelineState(
	const SGraphicsPipelineStateDesc& pipelineStateDesc)
{
	UNUSED2(pipelineStateDesc);
	return nullptr;
}

std::unique_ptr<IVertexInputLayout> CDevice::CreateVertexInputLayout(
	const PS::span<const SVertexAttributeFormat> attributes)
{
	UNUSED2(attributes);
	return nullptr;
}

std::unique_ptr<ITexture> CDevice::CreateTexture(
	const char* name, const ITexture::Type type, const uint32_t usage,
	const Format format, const uint32_t width, const uint32_t height,
	const Sampler::Desc& defaultSamplerDesc, const uint32_t MIPLevelCount, const uint32_t sampleCount)
{
	UNUSED2(name);
	UNUSED2(type);
	UNUSED2(usage);
	UNUSED2(format);
	UNUSED2(width);
	UNUSED2(height);
	UNUSED2(defaultSamplerDesc);
	UNUSED2(MIPLevelCount);
	UNUSED2(sampleCount);
	return nullptr;
}

std::unique_ptr<ITexture> CDevice::CreateTexture2D(
	const char* name, const uint32_t usage,
	const Format format, const uint32_t width, const uint32_t height,
	const Sampler::Desc& defaultSamplerDesc, const uint32_t MIPLevelCount, const uint32_t sampleCount)
{
	return CreateTexture(
		name, ITexture::Type::TEXTURE_2D, usage, format,
		width, height, defaultSamplerDesc, MIPLevelCount, sampleCount);
}

std::unique_ptr<IFramebuffer> CDevice::CreateFramebuffer(
	const char* name, SColorAttachment* colorAttachment,
	SDepthStencilAttachment* depthStencilAttachment)
{
	UNUSED2(name);
	UNUSED2(colorAttachment);
	UNUSED2(depthStencilAttachment);
	return nullptr;
}

std::unique_ptr<IBuffer> CDevice::CreateBuffer(
	const char* name, const IBuffer::Type type, const uint32_t size, const bool dynamic)
{
	UNUSED2(name);
	UNUSED2(type);
	UNUSED2(size);
	UNUSED2(dynamic);
	return nullptr;
}

std::unique_ptr<IShaderProgram> CDevice::CreateShaderProgram(
	const CStr& name, const CShaderDefines& defines)
{
	UNUSED2(name);
	UNUSED2(defines);
	return nullptr;
}

bool CDevice::AcquireNextBackbuffer()
{
	return false;
}

IFramebuffer* CDevice::GetCurrentBackbuffer(
	const AttachmentLoadOp colorAttachmentLoadOp,
	const AttachmentStoreOp colorAttachmentStoreOp,
	const AttachmentLoadOp depthStencilAttachmentLoadOp,
	const AttachmentStoreOp depthStencilAttachmentStoreOp)
{
	UNUSED2(colorAttachmentLoadOp);
	UNUSED2(colorAttachmentStoreOp);
	UNUSED2(depthStencilAttachmentLoadOp);
	UNUSED2(depthStencilAttachmentStoreOp);
	return nullptr;
}

void CDevice::Present()
{
}

void CDevice::OnWindowResize(const uint32_t width, const uint32_t height)
{
	UNUSED2(width);
	UNUSED2(height);
}

bool CDevice::IsTextureFormatSupported(const Format format) const
{
	UNUSED2(format);
	return false;
}

bool CDevice::IsFramebufferFormatSupported(const Format format) const
{
	UNUSED2(format);
	return false;
}

Format CDevice::GetPreferredDepthStencilFormat(
	const uint32_t usage, const bool depth, const bool stencil) const
{
	UNUSED2(usage);
	UNUSED2(depth);
	UNUSED2(stencil);
	return Format::UNDEFINED;
}

std::unique_ptr<IDevice> CreateDevice(SDL_Window* window)
{
	return Vulkan::CDevice::Create(window);
}

} // namespace Vulkan

} // namespace Backend

} // namespace Renderer
