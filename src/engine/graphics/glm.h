#ifndef GFXMATH_H
#define GFXMATH_H

#undef abs
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace glm {

	inline glm::vec4 rgba(const glm::vec4& in) { return glm::vec4(in.r/255.0, in.g/255.0, in.b/255.0, in.a/255.0); }

	inline glm::vec3 rgb(const glm::vec3& in) { return glm::vec3(in.r/255.0, in.g/255.0, in.b/255.0); }

}

#endif