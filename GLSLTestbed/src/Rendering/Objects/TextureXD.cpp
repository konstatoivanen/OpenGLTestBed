#include "PrecompiledHeader.h"
#include "Utilities/Log.h"
#include "Rendering/Objects/TextureXD.h"
#include <stb_image.h>
#include <KTX/ktx.h>

namespace PK::Rendering::Objects
{
	using namespace PK::Utilities;
	using namespace PK::Math;

	TextureXD::TextureXD() : Texture(TextureDescriptor())
	{
	}
	
	TextureXD::TextureXD(const TextureDescriptor& descriptor) : Texture(descriptor)
	{
		CreateTextureStorage(m_graphicsId, descriptor);
	}
	
	TextureXD::~TextureXD()
	{
		glDeleteTextures(1, &m_graphicsId);
	}
	
	void TextureXD::SetData(void* data, uint32_t size, uint32_t miplevel)
	{
		PK_CORE_ASSERT(size == GetSize(), "Texture data size miss match");
		auto dataType = Texture::GetFormatDataType(m_descriptor.colorFormat);
		auto resolution = m_descriptor.resolution >> miplevel;
		
		switch (m_descriptor.dimension)
		{
			case GL_TEXTURE_1D:
				glTextureSubImage1D(m_graphicsId, miplevel, 0, resolution.x, m_channels, dataType, data);
				break;
			case GL_TEXTURE_1D_ARRAY:
			case GL_TEXTURE_RECTANGLE:
			case GL_TEXTURE_CUBE_MAP:
			case GL_TEXTURE_2D:
				glTextureSubImage2D(m_graphicsId, miplevel, 0, 0, resolution.x, resolution.y, m_channels, dataType, data);
				break;
			case GL_TEXTURE_CUBE_MAP_ARRAY:
			case GL_TEXTURE_2D_ARRAY:
			case GL_TEXTURE_3D:
				glTextureSubImage3D(m_graphicsId, miplevel, 0, 0, 0, resolution.x, resolution.y, resolution.z, m_channels, dataType, data);
				break;
			case GL_TEXTURE_2D_MULTISAMPLE:
			case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
				PK_CORE_ERROR("Accessing multisample storage is not allowed!");
				return;
		}
	}
	
	void TextureXD::SetMipLevel(const Ref<TextureXD>& texture, uint32_t miplevel)
	{
		auto srcDescriptor = texture->GetDescriptor();
		auto src = texture->GetGraphicsID();
		auto dst = m_graphicsId;
		glCopyImageSubData(src, srcDescriptor.dimension, 0, 0, 0, 0, dst, m_descriptor.dimension, miplevel, 0, 0, 0, srcDescriptor.resolution.x, srcDescriptor.resolution.y, srcDescriptor.resolution.z);
	}
	
}

template<>
void PK::Core::AssetImporters::Import(const std::string& filepath, Utilities::Ref<PK::Rendering::Objects::TextureXD>& texture)
{
	if (texture->m_graphicsId != 0)
	{
		glDeleteTextures(1, &texture->m_graphicsId);
	}

	auto wrapmode = PK::Rendering::Objects::Texture::GetWrapmodeFromString(filepath.c_str());

	if (filepath.find(".ktx") != std::string::npos)
	{
		ktxTexture* kTexture;
		KTX_error_code result;
		GLenum target, glerror;

		result = ktxTexture_CreateFromNamedFile(filepath.c_str(), KTX_TEXTURE_CREATE_NO_FLAGS, &kTexture);

		PK_CORE_ASSERT(result == KTX_SUCCESS, "Failed to load ktx!");

		PK::Rendering::Objects::Texture::GetDescirptorFromKTX(kTexture, &texture->m_descriptor, &texture->m_channels);

		glGenTextures(1, &texture->m_graphicsId);

		result = ktxTexture_GLUpload(kTexture, &texture->m_graphicsId, &target, &glerror);
		
		PK_CORE_ASSERT(result == KTX_SUCCESS, "Failed to upload ktx!");

		glTextureParameteri(texture->m_graphicsId, GL_TEXTURE_MIN_FILTER, texture->m_descriptor.filtermin);
		glTextureParameteri(texture->m_graphicsId, GL_TEXTURE_MAG_FILTER, texture->m_descriptor.filtermag);
		texture->SetWrapMode(wrapmode, wrapmode, wrapmode);

		ktxTexture_Destroy(kTexture);
		return;
	}

	texture->m_descriptor.wrapmodex = texture->m_descriptor.wrapmodey = texture->m_descriptor.wrapmodez = wrapmode;
	auto desiredChannelCount = PK::Rendering::Objects::Texture::GetChannelCount(texture->m_channels);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);

	stbi_uc* data = nullptr;
	{
		data = stbi_load(filepath.c_str(), &width, &height, &channels, desiredChannelCount);
	}

	PK_CORE_ASSERT(data, "Failed to load image!");
	PK_CORE_ASSERT(channels == desiredChannelCount, "Failed to load image with the desired channel count!");

	texture->m_descriptor.resolution = { (uint32_t)width, (uint32_t)width, 0 };

	PK::Rendering::Objects::Texture::CreateTextureStorage(texture->m_graphicsId, texture->m_descriptor);
	texture->SetData(data, texture->GetSize(), 0);

	stbi_image_free(data);
}