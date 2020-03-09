#pragma once
#include "hd/Core/StringHash.hpp"
#include "hd/Core/Color.hpp"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

namespace hg {

using JSONObject = nlohmann::json;

}

namespace hd {

void from_json(const hg::JSONObject &json, StringHash &data);
void from_json(const hg::JSONObject &json, Color4 &data);

void to_json(hg::JSONObject &json, const StringHash &data);
void to_json(hg::JSONObject &json, const Color4 &data);

}

namespace glm {

void to_json(hg::JSONObject &json, const vec2 &data);
void to_json(hg::JSONObject &json, const ivec2 &data);
void to_json(hg::JSONObject &json, const vec3 &data);
void to_json(hg::JSONObject &json, const ivec3 &data);
void to_json(hg::JSONObject &json, const vec4 &data);
void to_json(hg::JSONObject &json, const ivec4 &data);

void from_json(const hg::JSONObject &json, vec2 &data);
void from_json(const hg::JSONObject &json, ivec2 &data);
void from_json(const hg::JSONObject &json, vec3 &data);
void from_json(const hg::JSONObject &json, ivec3 &data);
void from_json(const hg::JSONObject &json, vec4 &data);
void from_json(const hg::JSONObject &json, ivec4 &data);

}
