#pragma once
#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>
#include <Core/Type.h>


// Emitter macro
#define YAML_KEY_VALUE(emitter, key, value) emitter<<YAML::Key<<key<<YAML::Value<<value

// YAML Implement
namespace YAML {
	Emitter& operator<<(Emitter& emitter, glm::vec2 v) {
		return emitter << BeginSeq << v.x << v.y << EndSeq;
	}

	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	Emitter& operator<<(Emitter& emitter, glm::vec3 v) {
		return emitter << BeginSeq << v.x << v.y << v.z << EndSeq;
	}

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	Emitter& operator<<(Emitter& emitter, glm::vec4 v) {
		return emitter << BeginSeq << v.x << v.y << v.z << v.w << EndSeq;
	}

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	Emitter& operator<<(Emitter& emitter, UUIDType v) {
		return emitter << to_string(v);
	}

	template<>
	struct convert<UUIDType> {
		static Node encode(const UUIDType& rhs) {
			Node node;
			node.push_back(to_string(rhs));
			return node;
		}
		static bool decode(const Node& node, UUIDType& rhs) {
			if (node.IsScalar() == false) return false;
			rhs = UUIDType::from_string(node.as<std::string>()).value();
			return true;
		}
	};
}