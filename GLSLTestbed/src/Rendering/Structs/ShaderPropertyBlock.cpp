#include "PrecompiledHeader.h"
#include "Rendering/Structs/ShaderPropertyBlock.h"

namespace PK::Rendering::Structs
{
	void ShaderPropertyBlock::SetKeyword(uint32_t hashId, bool value)
	{
		auto iterator = std::find(m_keywords.begin(), m_keywords.end(), hashId);
	
		if (iterator == m_keywords.end())
		{
			if (value)
			{
				m_keywords.push_back(hashId);
			}
	
			return;
		}
	
		if (!value)
		{
			m_keywords.erase(iterator);
		}
	}

    void ShaderPropertyBlock::SetKeywords(std::initializer_list<uint32_t> hashIds)
    {
		m_keywords.resize(hashIds.size());
		memcpy(m_keywords.data(), hashIds.begin(), sizeof(uint32_t) * hashIds.size());
    }

	void ShaderPropertyBlock::CopyBufferLayout(const BufferLayout& layout, char* destination)
	{
		for (auto& element : layout)
		{
			if (m_properties.count(element.NameHashId) < 1)
			{
				continue;
			}

			auto& prop = m_properties.at(element.NameHashId);
			auto valueptr = GetElementPtr<void>(prop);

			if (prop.type == CG_TYPE::TEXTURE)
			{
				PK_CORE_ASSERT(element.Type == CG_TYPE::SAMPLER, "Trying to map an incompatible type!");

				auto graphicsId = *reinterpret_cast<const GraphicsID*>(valueptr);
				auto handle = glGetTextureHandleARB(graphicsId);

				if (!glIsTextureHandleResidentARB(handle))
				{
					glMakeTextureHandleResidentARB(handle);
				}

				memcpy(destination + element.Offset, &handle, sizeof(GLuint64));
				continue;
			}

			PK_CORE_ASSERT(element.Type == prop.type, "Trying to map an incompatible type!");
			PK_CORE_ASSERT(element.Size == prop.size, "Trying to map a property of differing size!");
			memcpy(destination + element.Offset, valueptr, element.Size);
		}
	}
	
	void ShaderPropertyBlock::Clear()
	{
		PropertyBlock::Clear();
		m_keywords.clear();
	}
}