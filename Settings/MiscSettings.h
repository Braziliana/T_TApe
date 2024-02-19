#pragma once
#include "../imgui/imgui.h"
#include "../Drawings/Renderer.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

struct MiscSettings
{
private:
    bool superGlideEnabled;
    bool wallJumpEnabled;

public:
    MiscSettings()
    {
        superGlideEnabled = true;
        wallJumpEnabled = true;
    }

    inline bool IsSuperGlideEnabled() const {
        return superGlideEnabled;
    }

    inline bool IsWallJumpEnabled() const {
        return wallJumpEnabled;
    }

    void Render() {
        ImGui::Checkbox("Super glide enabled##Misc", &superGlideEnabled);
        ImGui::Checkbox("Wall jump enabled##Misc", &wallJumpEnabled);
    }

    void Load(const json& j)
    {
        superGlideEnabled = j.at("superGlideEnabled").get<bool>();
        wallJumpEnabled = j.at("wallJumpEnabled").get<bool>();
    }

    json ToJson() const {
        return {
            {"superGlideEnabled", superGlideEnabled},
            {"wallJumpEnabled", wallJumpEnabled},
        };
    }
};

typedef MiscSettings* PMiscSettings;
