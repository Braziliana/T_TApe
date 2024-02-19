//
// Created by koelion on 05.02.24.
//

#ifndef APEX_RENDERER_H
#define APEX_RENDERER_H
#include <GL/gl.h>
#include <algorithm>
#include <cstring>
#include <string>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <vector>
#include <cmath>

#include "../imgui/imgui.h"
#include "../Math/Color.h"
#include "../Math/Vector2D.h"


class Renderer
{
private:

public:

    static void drawRectangleOutline(Math::Vector2D position, Math::Vector2D size, Math::Color color, float lineWidth) {
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(position.x-size.x/2, position.y),
            ImVec2(position.x+size.x/2, position.y + size.y),
            ImColor(color.r, color.g, color.b, 1.0f),
            0.0f,
            ImDrawFlags_None,
            lineWidth);
    }

    static void drawLine(Math::Vector2D start, Math::Vector2D end, Math::Color color, float width) {
        ImGui::GetWindowDrawList()->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImColor(color.r, color.g, color.b, 1.0f), width);
    }

    static void drawText(std::string msg, Math::Vector2D position,  Math::Color color) {
        ImGui::GetWindowDrawList()->AddText(ImVec2(position.x, position.y), ImColor(color.r, color.g, color.b, 1.0f), msg.c_str());
    }

    static void drawFilledRectagle(Math::Vector2D position, Math::Vector2D size, Math::Color color) {
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(position.x - size.x/2, position.y),
            ImVec2(position.x + size.x/2, position.y + size.y),
            ImColor(color.r, color.g, color.b, 1.0f));
    }

    static void drawHealthBar(Math::Vector2D position, Math::Vector2D size, Math::Color color, float fill) {

        ImVec2 startPosition = ImVec2(position.x - size.x/2, position.y);
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(startPosition.x, position.y),
            ImVec2(startPosition.x + size.x*fill, position.y + size.y),
            ImColor(color.r, color.g, color.b, 1.0f));
        
        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(position.x-size.x/2, position.y),
            ImVec2(position.x+size.x/2, position.y + size.y),
            ImColor(1.0f, 1.0f, 1.0f, 1.0f),
            0.0f,
            ImDrawFlags_None,
            2);
    }

    static void drawSheildBar(Math::Vector2D position, Math::Vector2D size, Math::Color color, float fill, int segments, int maxSegments){

        ImVec2 startPosition = ImVec2(position.x - size.x/2, position.y);
        ImGui::GetWindowDrawList()->AddRectFilled(
            ImVec2(startPosition.x, position.y),
            ImVec2(startPosition.x + size.x*fill, position.y + size.y),
            ImColor(color.r, color.g, color.b, 1.0f));

        auto segmentSize = size.x/segments;

        for(auto i=0; i<segments; i++) 
        {
            auto x = startPosition.x + i*segmentSize;
            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(x , position.y),
                ImVec2(x + segmentSize, position.y + size.y),
                ImColor(1.0f, 1.0f, 1.0f, 1.0f),
                0.0f,
                ImDrawFlags_None,
                2);
        }
    }

    static void drawBorderedFillRectangle(Math::Vector2D position, Math::Vector2D size, Math::Color fillColor, Math::Color borderColor, float lineWidth, float fill) {
        drawFilledRectagle(position, Math::Vector2D(size.x, size.y), Math::Color(0, 0, 0));
        drawFilledRectagle(position, Math::Vector2D(size.x * fill, size.y), fillColor);
        drawRectangleOutline(position, size, borderColor, lineWidth);
    }

    static void DrawCircle(Math::Vector2D position, float radius, Math::Color color, int numSegments = 64)
    {
        auto drawList = ImGui::GetWindowDrawList();
        const float segmentAngle = 2.0f * 3.1415926f / numSegments;
            for (int i = 0; i < numSegments; ++i) {
                float angle0 = segmentAngle * i;
                float angle1 = segmentAngle * (i + 1);
                drawList->AddLine(ImVec2(position.x + radius * cosf(angle0), position.y + radius * sinf(angle0)),
                                ImVec2(position.x + radius * cosf(angle1), position.y + radius * sinf(angle1)),
                                ImColor(color.r, color.g, color.b, 1.0f), 2.0f);
            }
    }

    static bool renderImguiFloatValue(std::string label, std::string id, float* value, float min, float max, float slowStep, float fastStep) {
        const char* format = "%.3f";

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

    static void renderImguiColorValue(std::string label, std::string id, Math::Color& color) {

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

    static void DrawHorizontalTab(std::string tabName, ImVec2 childSize, std::vector<std::string> tabs, int& selectedTab)
    {
        auto style = &ImGui::GetStyle();

        ImGui::BeginChild(tabName.c_str(), ImVec2(childSize.x * tabs.size() + tabs.size() * 5, childSize.y), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        for (int i = 0; i < tabs.size(); i++) {
            std::string it = tabs[i];
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.5));
            ImGui::PushStyleColor(ImGuiCol_Button, selectedTab == i ? style->Colors[ImGuiCol_ButtonActive] : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, selectedTab == i ? ImVec4(1, 1, 1, 1) : style->Colors[ImGuiCol_Text]);
            if (ImGui::Button(it.c_str(), childSize)) selectedTab = i;
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
            ImGui::SameLine();
        }

        ImGui::EndChild();
    }


    static void DrawVerticalTab(std::string tabName, ImVec2 childSize, std::vector<std::string> tabs, int& selectedTab)
    {
        auto style = &ImGui::GetStyle();
        

        ImGui::BeginChild(tabName.c_str(), ImVec2(childSize.x, childSize.y * tabs.size() + tabs.size() * 5), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        for (int i = 0; i < tabs.size(); i++) {
            std::string it = tabs[i];
            ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.5));
            ImGui::PushStyleColor(ImGuiCol_Button, selectedTab == i ? style->Colors[ImGuiCol_ButtonActive] : ImVec4(0, 0, 0, 0));
            ImGui::PushStyleColor(ImGuiCol_Text, selectedTab == i ? ImVec4(1, 1, 1, 1) : style->Colors[ImGuiCol_Text]);
            if (ImGui::Button(it.c_str(), childSize)) selectedTab = i;
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(2);
        }

        ImGui::EndChild();
    }

    static GLuint LoadTextureFromFile(const char* filename, int& width, int& height) {
        int channels;
        unsigned char* image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGBA);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(image);

        return textureID;
    }

};

#endif //APEX_RENDERER_H
