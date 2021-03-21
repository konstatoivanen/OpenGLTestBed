#pragma once
#include "Rendering/Objects/GraphicsObject.h"
#include "Rendering/Structs/PropertyBlock.h"
#include <hlslmath.h>

namespace PK::Rendering::Structs
{
	using namespace Objects;

	class ShaderPropertyBlock : public PropertyBlock
	{
	    public:
			void SetTexture(uint32_t hashId, const GraphicsID* textureIds, uint32_t count = 1) { SetValue(hashId, CG_TYPE::TEXTURE, textureIds, count); }
			void SetConstantBuffer(uint32_t hashId, const GraphicsID* bufferIds, uint32_t count = 1) { SetValue(hashId, CG_TYPE::CONSTANT_BUFFER, bufferIds, count); }
			void SetComputeBuffer(uint32_t hashId, const GraphicsID* bufferIds, uint32_t count = 1) { SetValue(hashId, CG_TYPE::COMPUTE_BUFFER, bufferIds, count); }
	
			void SetTexture(uint32_t hashId, GraphicsID textureId) { SetValue(hashId, CG_TYPE::TEXTURE, &textureId); }
			void SetConstantBuffer(uint32_t hashId, GraphicsID bufferId) { SetValue(hashId, CG_TYPE::CONSTANT_BUFFER, &bufferId); }
			void SetComputeBuffer(uint32_t hashId, GraphicsID bufferId) { SetValue(hashId, CG_TYPE::COMPUTE_BUFFER, &bufferId); }
	
			void SetKeyword(uint32_t hashId, bool value);
			void SetKeywords(std::initializer_list<uint32_t> hashIds);
	
	        void Clear() override;
	
			const std::vector<uint32_t>& GetKeywords() const { return m_keywords; }
	    private:
			std::vector<uint32_t> m_keywords;
	};
}