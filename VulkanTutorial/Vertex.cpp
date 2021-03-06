﻿#include "Vertex.h"
#include <glm/gtx/hash.hpp>

VkVertexInputBindingDescription Vertex::getBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription = {};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributteDescription()
{
	std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

	//pos
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, pos);

	//color
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	//texcoord
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

	return attributeDescriptions;
}

size_t std::hash<Vertex>::operator()(Vertex const& vertex) const
{
	return ((hash<glm::vec3>()(vertex.pos) ^
			(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
		(hash<glm::vec2>()(vertex.texCoord) << 1);
}
