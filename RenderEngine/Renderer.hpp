#pragma once
#include <GL/gl.h>
#include "../imgui/imgui.h"
#include <algorithm>

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

};