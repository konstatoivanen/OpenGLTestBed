#include "PrecompiledHeader.h"
#include "hlslmath.h"

ushort CGConvert::Size(ushort type)
{
	switch (type)
	{
		case CG_TYPE_FLOAT: return CG_TYPE_SIZE_FLOAT;
		case CG_TYPE_FLOAT2: return CG_TYPE_SIZE_FLOAT2;
		case CG_TYPE_FLOAT3: return CG_TYPE_SIZE_FLOAT3;
		case CG_TYPE_FLOAT4: return CG_TYPE_SIZE_FLOAT4;
		case CG_TYPE_FLOAT2X2: return CG_TYPE_SIZE_FLOAT2X2;
		case CG_TYPE_FLOAT3X3: return CG_TYPE_SIZE_FLOAT3X3;
		case CG_TYPE_FLOAT4X4: return CG_TYPE_SIZE_FLOAT4X4;
		case CG_TYPE_INT: return CG_TYPE_SIZE_INT;
		case CG_TYPE_INT2: return CG_TYPE_SIZE_INT2;
		case CG_TYPE_INT3: return CG_TYPE_SIZE_INT3;
		case CG_TYPE_INT4: return CG_TYPE_SIZE_INT4;
		case CG_TYPE_TEXTURE: return CG_TYPE_SIZE_TEXTURE;
		case CG_TYPE_CONSTANT_BUFFER: return CG_TYPE_SIZE_CONSTANT_BUFFER;
		case CG_TYPE_COMPUTE_BUFFER: return CG_TYPE_SIZE_COMPUTE_BUFFER;
	}

	return CG_TYPE_ERROR;
}

ushort CGConvert::Components(ushort type)
{
	switch (type)
	{
		case CG_TYPE_FLOAT: return CG_TYPE_COMPONENTS_FLOAT;
		case CG_TYPE_FLOAT2: return CG_TYPE_COMPONENTS_FLOAT2;
		case CG_TYPE_FLOAT3: return CG_TYPE_COMPONENTS_FLOAT3;
		case CG_TYPE_FLOAT4: return CG_TYPE_COMPONENTS_FLOAT4;
		case CG_TYPE_FLOAT2X2: return CG_TYPE_COMPONENTS_FLOAT2X2;
		case CG_TYPE_FLOAT3X3: return CG_TYPE_COMPONENTS_FLOAT3X3;
		case CG_TYPE_FLOAT4X4: return CG_TYPE_COMPONENTS_FLOAT4X4;
		case CG_TYPE_INT: return CG_TYPE_COMPONENTS_INT;
		case CG_TYPE_INT2: return CG_TYPE_COMPONENTS_INT2;
		case CG_TYPE_INT3: return CG_TYPE_COMPONENTS_INT3;
		case CG_TYPE_INT4: return CG_TYPE_COMPONENTS_INT4;
		case CG_TYPE_TEXTURE: return CG_TYPE_COMPONENTS_TEXTURE;
		case CG_TYPE_CONSTANT_BUFFER: return CG_TYPE_COMPONENTS_CONSTANT_BUFFER;
		case CG_TYPE_COMPUTE_BUFFER: return CG_TYPE_COMPONENTS_COMPUTE_BUFFER;
	}

	return CG_TYPE_ERROR;
}

ushort CGConvert::BaseType(ushort type)
{
	switch (type)
	{
		case CG_TYPE_FLOAT: return GL_FLOAT;
		case CG_TYPE_FLOAT2: return GL_FLOAT;
		case CG_TYPE_FLOAT3: return GL_FLOAT;
		case CG_TYPE_FLOAT4: return GL_FLOAT;
		case CG_TYPE_FLOAT2X2: return GL_FLOAT;
		case CG_TYPE_FLOAT3X3: return GL_FLOAT;
		case CG_TYPE_FLOAT4X4: return GL_FLOAT;
		case CG_TYPE_INT: return GL_INT;
		case CG_TYPE_INT2: return GL_INT;
		case CG_TYPE_INT3: return GL_INT;
		case CG_TYPE_INT4: return GL_INT;
		case CG_TYPE_TEXTURE: return GL_INT;
		case CG_TYPE_CONSTANT_BUFFER: return GL_INT;
		case CG_TYPE_COMPUTE_BUFFER: return GL_INT;
	}

	return CG_TYPE_ERROR;
}

ushort CGConvert::NativeEnum(ushort type)
{
	switch (type)
	{
		case CG_TYPE_FLOAT: return GL_FLOAT;
		case CG_TYPE_FLOAT2: return GL_FLOAT;
		case CG_TYPE_FLOAT3: return GL_FLOAT;
		case CG_TYPE_FLOAT4: return GL_FLOAT;
		case CG_TYPE_FLOAT2X2: return GL_FLOAT;
		case CG_TYPE_FLOAT3X3: return GL_FLOAT;
		case CG_TYPE_FLOAT4X4: return GL_FLOAT;
		case CG_TYPE_INT: return GL_INT;
		case CG_TYPE_INT2: return GL_INT;
		case CG_TYPE_INT3: return GL_INT;
		case CG_TYPE_INT4: return GL_INT;
		case CG_TYPE_TEXTURE: return GL_TEXTURE;
		case CG_TYPE_CONSTANT_BUFFER: return GL_UNIFORM_BUFFER;
		case CG_TYPE_COMPUTE_BUFFER: return GL_SHADER_STORAGE_BUFFER;
	}

	return CG_TYPE_ERROR;
}

std::string CGConvert::ToString(ushort type)
{
	switch (type)
	{
		case CG_TYPE_FLOAT: return "FLOAT";
		case CG_TYPE_FLOAT2: return "FLOAT2";
		case CG_TYPE_FLOAT3: return "FLOAT3";
		case CG_TYPE_FLOAT4: return "FLOAT4";
		case CG_TYPE_FLOAT2X2: return "FLOAT2X2";
		case CG_TYPE_FLOAT3X3: return "FLOAT3X3";
		case CG_TYPE_FLOAT4X4: return "FLOAT4X4";
		case CG_TYPE_INT: return "INT";
		case CG_TYPE_INT2: return "INT2";
		case CG_TYPE_INT3: return "INT3";
		case CG_TYPE_INT4: return "INT4";
		case CG_TYPE_TEXTURE: return "TEXTURE";
		case CG_TYPE_CONSTANT_BUFFER: return "CONSTANT_BUFFER";
		case CG_TYPE_COMPUTE_BUFFER: return "COMPUTE_BUFFER";
	}

	return "INVALID";
}

ushort CGConvert::FromString(const char* string)
{
	if (strcmp(string, "FLOAT") == 0) return CG_TYPE_FLOAT;
	if (strcmp(string, "FLOAT2") == 0) return CG_TYPE_FLOAT2;
	if (strcmp(string, "FLOAT3") == 0) return CG_TYPE_FLOAT3;
	if (strcmp(string, "FLOAT4") == 0) return CG_TYPE_FLOAT4;
	if (strcmp(string, "FLOAT2X2") == 0) return CG_TYPE_FLOAT2X2;
	if (strcmp(string, "FLOAT3X3") == 0) return CG_TYPE_FLOAT3X3;
	if (strcmp(string, "FLOAT4X4") == 0) return CG_TYPE_FLOAT4X4;
	if (strcmp(string, "INT") == 0) return CG_TYPE_INT;
	if (strcmp(string, "INT2") == 0) return CG_TYPE_INT2;
	if (strcmp(string, "INT3") == 0) return CG_TYPE_INT3;
	if (strcmp(string, "INT4") == 0) return CG_TYPE_INT4;
	if (strcmp(string, "TEXTURE") == 0) return CG_TYPE_TEXTURE;
	if (strcmp(string, "CONSTANT_BUFFER") == 0) return CG_TYPE_CONSTANT_BUFFER;
	if (strcmp(string, "COMPUTE_BUFFER") == 0) return CG_TYPE_COMPUTE_BUFFER;
	return CG_TYPE_ERROR;
}

float4x4 CGMath::GetMatrixTRS(const float3& position, const quaternion& rotation, const float3& scale)
{
	return glm::translate(float4x4(1.0f), position) * glm::toMat4(rotation) * glm::scale(scale);
}

float4x4 CGMath::GetMatrixInvTRS(const float3& position, const quaternion& rotation, const float3& scale)
{
	return glm::inverse(glm::translate(float4x4(1.0f), position) * glm::toMat4(rotation) * glm::scale(scale));
}

float4x4 CGMath::GetMatrixTR(const float3& position, const quaternion& rotation)
{
	return glm::translate(float4x4(1.0f), position) * glm::toMat4(rotation);
}

float4x4 CGMath::GetPerspective(float fov, float aspect, float nearClip, float farClip)
{
	auto proj = glm::perspective(fov * CG_FLOAT_DEG2RAD, aspect, nearClip, farClip);
	proj[2][2] *= -1;
	proj[2][3] *= -1;
	return proj;
}

float4x4 CGMath::GetOffsetPerspective(float left, float right, float bottom, float top, float fovy, float aspect, float zNear, float zFar)
{
	const float tanHalfFovy = tan((fovy * CG_FLOAT_DEG2RAD) / 2.0f);
	const float rcpRL = 1.0f / (right - left);
	const float rcpTB = 1.0f / (top - bottom);
	const float rcpFN = 1.0f / (zFar - zNear);

	float4x4 Result(0);
	Result[0][0] = (2.0f * rcpRL) / (aspect * tanHalfFovy);
	Result[1][1] = (2.0f * rcpTB) / tanHalfFovy;
	Result[2][0] = -(right + left) * rcpRL;
	Result[2][1] = -(top + bottom) * rcpTB;
	Result[2][2] = -(zFar + zNear) * rcpFN;
	Result[2][3] = 1.0f;
	Result[3][2] = (2.0f * zFar * zNear) * rcpFN;

	return Result;
}

float4x4 CGMath::GetPerspectiveSubdivision(int index, const int3& gridSize, float fovy, float aspect, float znear, float zfar)
{
	int3 coord;
	coord.x = index % gridSize.x;
	coord.y = (index / gridSize.x) % gridSize.y;
	coord.z = index / (gridSize.x * gridSize.y);

	float ix = 2.0f / gridSize.x;
	float iy = 2.0f / gridSize.y;
	float iz = 1.0f / gridSize.z;

	float x = -1.0f + (2.0f * (float)coord.x) / gridSize.x;
	float y = -1.0f + (2.0f * (float)coord.y) / gridSize.y;
	float z = (float)coord.z / gridSize.z;

	float zrange = zfar - znear;

	return CGMath::GetOffsetPerspective(x, x + ix, y, y + iy, fovy, aspect, znear + zrange * z, znear + zrange * (z + iz));
}

size_t CGMath::GetNextExponentialSize(size_t start, size_t min)
{
	if (start < 1)
	{
		start = 1;
	}

	while (start < min)
	{
		start <<= 1;
	}

	return start;
}

color CGMath::HueToRGB(float hue)
{
	float R = abs(hue * 6 - 3) - 1;
	float G = 2 - abs(hue * 6 - 2);
	float B = 2 - abs(hue * 6 - 4);
	return float4(glm::clamp(float3(R, G, B), CG_FLOAT3_ZERO, CG_FLOAT3_ONE), 1.0f);
}

void CGMath::NormalizePlane(float4* plane)
{
	float mag;
	mag = sqrt(plane->x * plane->x + plane->y * plane->y + plane->z * plane->z);
	plane->x = plane->x / mag;
	plane->y = plane->y / mag;
	plane->z = plane->z / mag;
	plane->w = plane->w / mag;
}

// https://www.gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf
void CGMath::ExtractFrustrumPlanes(const float4x4 viewprojection, FrustrumPlanes* frustrum, bool normalize)
{
	float4* planes = frustrum->planes;
	// Left clipping plane
	planes[0].x = viewprojection[0][3] + viewprojection[0][0];
	planes[0].y = viewprojection[1][3] + viewprojection[1][0];
	planes[0].z = viewprojection[2][3] + viewprojection[2][0];
	planes[0].w = viewprojection[3][3] + viewprojection[3][0];
	// Right clipping plane
	planes[1].x = viewprojection[0][3] - viewprojection[0][0];
	planes[1].y = viewprojection[1][3] - viewprojection[1][0];
	planes[1].z = viewprojection[2][3] - viewprojection[2][0];
	planes[1].w = viewprojection[3][3] - viewprojection[3][0];
	// Top clipping plane
	planes[2].x = viewprojection[0][3] - viewprojection[0][1];
	planes[2].y = viewprojection[1][3] - viewprojection[1][1];
	planes[2].z = viewprojection[2][3] - viewprojection[2][1];
	planes[2].w = viewprojection[3][3] - viewprojection[3][1];
	// Bottom clipping plane
	planes[3].x = viewprojection[0][3] + viewprojection[0][1];
	planes[3].y = viewprojection[1][3] + viewprojection[1][1];
	planes[3].z = viewprojection[2][3] + viewprojection[2][1];
	planes[3].w = viewprojection[3][3] + viewprojection[3][1];
	// Near clipping plane
	planes[4].x = viewprojection[0][3] + viewprojection[0][2];
	planes[4].y = viewprojection[1][3] + viewprojection[1][2];
	planes[4].z = viewprojection[2][3] + viewprojection[2][2];
	planes[4].w = viewprojection[3][3] + viewprojection[3][2];
	// Far clipping plane
	planes[5].x = viewprojection[0][3] - viewprojection[0][2];
	planes[5].y = viewprojection[1][3] - viewprojection[1][2];
	planes[5].z = viewprojection[2][3] - viewprojection[2][2];
	planes[5].w = viewprojection[3][3] - viewprojection[3][2];

	// Normalize the plane equations, if requested
	if (normalize == true)
	{
		NormalizePlane(&planes[0]);
		NormalizePlane(&planes[1]);
		NormalizePlane(&planes[2]);
		NormalizePlane(&planes[3]);
		NormalizePlane(&planes[4]);
		NormalizePlane(&planes[5]);
	}
}

bool CGMath::IntersectPlanesAABB(const float4* planes, int planeCount, const BoundingBox& aabb)
{
	for (auto i = 0; i < planeCount; ++i)
	{
		auto& plane = planes[i];

		auto bx = plane.x > 0 ? aabb.max.x : aabb.min.x;
		auto by = plane.y > 0 ? aabb.max.y : aabb.min.y;
		auto bz = plane.z > 0 ? aabb.max.z : aabb.min.z;

		if (plane.x * bx + plane.y * by + plane.z * bz < -plane.w)
		{
			return false;
		}
	}

	return true;
}

void CGMath::BoundsEncapsulate(BoundingBox* bounds, const BoundingBox& other)
{
	for (auto i = 0; i < 3; ++i)
	{
		if (other.min[i] < bounds->min[i])
		{
			bounds->min[i] = other.min[i];
		}

		if (other.max[i] > bounds->max[i])
		{
			bounds->max[i] = other.max[i];
		}
	}
}

int CGMath::BoundsLongestAxis(const BoundingBox& bounds)
{
	auto ext = bounds.GetExtents();
	auto length = ext[0];
	auto axis = 0;

	for (auto i = 1; i < 3; ++i)
	{
		if (ext[i] > length)
		{
			axis = i;
			length = ext[i];
		}
	}

	return axis;
}

int CGMath::BoundsShortestAxis(const BoundingBox& bounds)
{
	auto ext = bounds.GetExtents();
	auto length = ext[0];
	auto axis = 0;

	for (auto i = 1; i < 3; ++i)
	{
		if (ext[i] < length)
		{
			axis = i;
			length = ext[i];
		}
	}

	return axis;
}

void CGMath::BoundsSplit(const BoundingBox& bounds, int axis, BoundingBox* out0, BoundingBox* out1)
{
	*out0 = bounds;
	*out1 = bounds;
	out0->max[axis] = bounds.min[axis] + (bounds.max[axis] - bounds.min[axis]) * 0.5f;
	out1->min[axis] = bounds.min[axis] + (bounds.max[axis] - bounds.min[axis]) * 0.5f;
}

bool CGMath::BoundsContains(const BoundingBox& bounds, const float3& point)
{
	for (auto i = 0; i < 3; ++i)
	{
		if (bounds.min[i] > point[i] || bounds.max[i] < point[i])
		{
			return false;
		}
	}

    return true;
}
