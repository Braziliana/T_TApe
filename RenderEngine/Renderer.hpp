#pragma once
#include <GL/gl.h>
#include "../imgui/imgui.h"
#include <algorithm>
#include "../Math/Color.hpp"

class Renderer
{
private:

public:

    static void drawRectangleOutline(float x, float y, float width, float height, float r, float g, float b, float lineWidth) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glLineWidth(lineWidth);
        glColor3f(r, g, b);

        glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + width, y);
            glVertex2f(x + width, y + height);
            glVertex2f(x, y + height);
        glEnd();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    static bool renderImguiFloatValue(std::string label, std::string id, float* value, float min, float max, float slowStep, float fastStep) {
        const char* format = "%.2f";
        
        std::string labelId = label + "##" + id;
        bool sliderResult = ImGui::SliderFloat(labelId.c_str(), value, min, max, format);

        ImGui::SameLine();
        
        ImGui::PushItemWidth(100);
        std::string inputId = "##_" + id + "_" + id + label;
        bool inputResult = ImGui::InputFloat(inputId.c_str(), value, slowStep, fastStep, format);
        ImGui::PopItemWidth(); 

        if(inputResult) {
            *value = std::clamp(*value, min, max);
        }

        return sliderResult || inputResult;
    }

    static bool renderImguiIntValue(std::string label, std::string id, int* value, int min, int max, int slowStep, int fastStep) {
        
        std::string labelId = label + "##" + id;
        bool sliderResult = ImGui::SliderInt(labelId.c_str(), value, min, max);

        ImGui::SameLine();
        
        ImGui::PushItemWidth(100); 
        std::string inputId = "##_" + id + "_" + id + label;
        bool inputResult = ImGui::InputInt(inputId.c_str(), value, slowStep, fastStep);
        ImGui::PopItemWidth(); 

        if(inputResult) {
            *value = std::clamp(*value, min, max);
        }

        return sliderResult || inputResult;
    }

    static void renderImguiColorValue(std::string label, std::string id, Color& color) {

        ImVec4 imColor = ImVec4(color.r, color.g, color.b, 1.0f);
        std::string labelId = label + "##" + id;
        std::string popoutId = label + "##popout_" + id;

        ImGui::Text("%s", label.c_str());
        ImGui::SameLine();
        if (ImGui::ColorButton(labelId.c_str(), imColor)) {
            ImGui::OpenPopup(popoutId.c_str());
        }

        if (ImGui::BeginPopup(popoutId.c_str())) {
            std::string pickerId = "##" + label + "_picker_" + id;
            
            if(ImGui::ColorPicker3(pickerId.c_str(), (float*)&imColor)) {
                color.r = imColor.x;
                color.g = imColor.y;
                color.b = imColor.z;
                color.clamp();
            }
            ImGui::EndPopup();
        }
    }

};