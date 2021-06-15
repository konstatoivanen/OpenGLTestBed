#pragma once
#include "Core/Application.h"
#include "Rendering/Objects/TextureXD.h"
#include <hlslmath.h>
#include <yaml-cpp/yaml.h>

namespace YAML
{
	using namespace PK::Utilities;
	using namespace PK::Rendering::Objects;
	using namespace PK::Math;

	#define DECLARE_VECTOR_CONVERTER(type, count)				\
	template<>													\
	struct convert<type##count>									\
	{															\
		static Node encode(const type##count & rhs)				\
		{														\
			Node node;											\
			for (auto i = 0; i < count; ++i)					\
			{													\
				node.push_back(rhs[i]);							\
			}													\
			node.SetStyle(EmitterStyle::Flow);					\
			return node;										\
		}														\
																\
		static bool decode(const Node& node, type##count & rhs)	\
		{														\
			if (!node.IsSequence() || node.size() != count)		\
			{													\
				return false;									\
			}													\
																\
			for (auto i = 0; i < count; ++i)					\
			{													\
				rhs[i] = node[i].as<type>();					\
			}													\
																\
			return true;										\
		}														\
	};															\

	#define DECLARE_MATRIX_CONVERTER(type, count)							\
	template<>																\
	struct convert<type##count##x##count>									\
	{																		\
		static Node encode(const type##count##x##count & rhs)				\
		{																	\
			Node node;														\
			for (auto i = 0; i < count; ++i)								\
			for (auto j = 0; j < count; ++j)								\
			{																\
				node.push_back(rhs[i][j]);									\
			}																\
			node.SetStyle(EmitterStyle::Flow);								\
			return node;													\
		}																	\
																			\
		static bool decode(const Node& node, type##count##x##count & rhs)	\
		{																	\
			if (!node.IsSequence() || node.size() != count * count)			\
			{																\
				return false;												\
			}																\
																			\
			for (auto i = 0; i < count; ++i)								\
			for (auto j = 0; j < count; ++j)								\
			{																\
				rhs[i][j] = node[i * count + j].as<type>();					\
			}																\
																			\
			return true;													\
		}																	\
	};																		\

	DECLARE_VECTOR_CONVERTER(float, 2)
	DECLARE_VECTOR_CONVERTER(float, 3)
	DECLARE_VECTOR_CONVERTER(float, 4)
	DECLARE_VECTOR_CONVERTER(int, 2)
	DECLARE_VECTOR_CONVERTER(int, 3)
	DECLARE_VECTOR_CONVERTER(int, 4)
	DECLARE_VECTOR_CONVERTER(uint, 2)
	DECLARE_VECTOR_CONVERTER(uint, 3)
	DECLARE_VECTOR_CONVERTER(uint, 4)

	DECLARE_MATRIX_CONVERTER(float, 2)
	DECLARE_MATRIX_CONVERTER(float, 3)
	DECLARE_MATRIX_CONVERTER(float, 4)

	#undef DECLARE_VECTOR_CONVERTER
	#undef DECLARE_MATRIX_CONVERTER

	template<>
	struct convert<TextureXD*>
	{
		static Node encode(const TextureXD*& rhs)
		{
			Node node;
			node.push_back(rhs->GetFileName());
			node.SetStyle(EmitterStyle::Default);
			return node;
		}

		static bool decode(const Node& node, TextureXD*& rhs)
		{
			auto path = node.as<std::string>();
			rhs = Application::GetService<AssetDatabase>()->Load<TextureXD>(path);
			return true;
		}
	};

	struct ParsableValue
	{
		virtual void Parse(const YAML::Node& parent) = 0;
		virtual void TryParse(const YAML::Node& parent) = 0;
	};

	template<typename T>
	struct BoxedValue : ParsableValue
	{
		std::string key;
		T value;
		T defaultValue;

		BoxedValue(const char* ckey, const T& initialValue) : key(ckey), value(initialValue), defaultValue(initialValue) {}

		inline void Parse(const YAML::Node& parent) final { value = parent[key.c_str()] ? parent[key.c_str()].as<T>() : defaultValue; }

		inline void TryParse(const YAML::Node& parent) final { if (parent[key.c_str()]) value = parent[key.c_str()].as<T>(); }

		operator T& () { return value; }
		operator T() const { return value; }
	};

	struct YamlValueList
	{
		std::vector<ParsableValue*> values;

		inline void Load(const std::string& filepath)
		{
			auto properties = YAML::LoadFile(filepath);
			PK_CORE_ASSERT(properties, "Failed to open config file at: %s", filepath.c_str());
			Parse(properties);
		}

		inline void Parse(const YAML::Node& parent)
		{
			for (auto& value : values)
			{
				value->Parse(parent);
			}
		}

		inline void TryParse(const YAML::Node& parent)
		{
			for (auto& value : values)
			{
				value->TryParse(parent);
			}
		}
	};
}