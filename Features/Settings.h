#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <string>
using json = nlohmann::json;

#include "../Drawings/Renderer.h"
#include "../imgui/imgui.h"
#include "../Settings/AimbotSettings.h"
#include "../Settings/RcsSettings.h"
#include "../Settings/GlowSettings.h"
#include "../Settings/EspSettings.h"
#include "../Settings/MiscSettings.h"

namespace Features {

    class Settings {
    private:
        int selectedTab = 0;
        bool _isOpen;
        GLuint logo;
        int logoWidth;
        int logoHeight;
        ImFont* logoFont;

        float screenWidth = 3440;
        float screenHeight = 1440;

        AimbotSettings aimbotSettings;
        RcsSettings rcsSettings;

        GlowSettings glowSettings;
        EspSettings espSettings;

        MiscSettings miscSettings;

        Settings() {
            _isOpen = false;
            int w, h;
            logo = Renderer::LoadTextureFromFile("Assets/logo.png", logoWidth, logoHeight);

            ImGuiIO& io = ImGui::GetIO();
            logoFont = io.Fonts->AddFontFromFileTTF("Assets/Roboto-Bold.ttf", 30);
            LoadConfigs();
        }
        ~Settings() = default;

    public:
        static Settings& getInstance() {
            static Settings instance;
            return instance;
        }

        Settings(const Settings&) = delete;
        Settings& operator=(const Settings&) = delete;

        void open() {
            _isOpen = true;
        }

        void close() {
            _isOpen = false;
        }

        [[nodiscard]] bool isOpen() const {
            return _isOpen;
        }

        [[nodiscard]] inline float GetScreenWidth() const {
            return screenWidth;
        }

        [[nodiscard]] inline float GetScreenHeight() const {
            return screenHeight;
        }

        void RenderLogo() {
            ImGui::BeginGroup();
            { 
                ImGui::BeginChild("Logo", ImVec2(200, 190), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                ImGui::SetCursorPosX(25);
                ImGui::Image((void*)(intptr_t)logo, ImVec2(150, 150));

                ImGui::PushFont(logoFont);
                ImVec2 textSize = ImGui::CalcTextSize("T_T Ape");
                float textPosition = 100 - textSize.x * 0.5;

                ImGui::SetCursorPosX(textPosition);
                ImGui::TextUnformatted("T_T Ape");
                ImGui::PopFont();

		        ImGui::EndChild();
            }
            ImGui::EndGroup();
        }

        void RenderTabs() 
        {
            std::vector<std::string> tabNames = { "Main", "Aim", "Visuals", "Misc", "Configs" };
            Renderer::DrawVerticalTab("MainTabs", ImVec2(200, 30), tabNames, selectedTab);
        }


        int aimTab = 0;
        void RenderAimTabs() 
        {
            std::vector<std::string> tabNames = { "Aimbot", "TriggerBot", "Rcs" };
            Renderer::DrawHorizontalTab("AimTabs", ImVec2(200, 30), tabNames, aimTab);
            
	        ImGui::Spacing();

            switch (aimTab)
            {
            case 0:
                aimbotSettings.Render();
                break;
            case 1:
                //aimbotSettings.Render();
                break;
            case 2:
                rcsSettings.Render();
                break;
            
            default:
                break;
            }
        }

        int visualsTab = 0;
        void RenderVisualsTab()
        {
            //glowSettings
            std::vector<std::string> tabNames = { "Glow", "Esp" };
            Renderer::DrawHorizontalTab("VisualsTabs", ImVec2(200, 30), tabNames, visualsTab);
            
	        ImGui::Spacing();

            switch (visualsTab)
            {
            case 0:
                glowSettings.Render();
                break;
            case 1:
                espSettings.Render();
                break;
            
            default:
                break;
            }
        }

        int miscTab = 0;
        void RenderMiscTab()
        {
            std::vector<std::string> tabNames = { "Misc" };
            Renderer::DrawHorizontalTab("MiscTabs", ImVec2(200, 30), tabNames, miscTab);
            
	        ImGui::Spacing();

        switch (miscTab)
            {
            case 0:

                miscSettings.Render();
                break;
            
            default:
                break;
            }
        }

        std::vector<std::string> jsonFiles;
        void LoadConfigs(){
            std::string configDirectory = "Configs";
            jsonFiles.clear();
            for (const auto& entry : std::filesystem::directory_iterator(configDirectory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    jsonFiles.push_back(entry.path().stem().string());
                }
            }
        }


        char configName[64] = {0};
        int selectedConfig = 0;
        void RenderConfigs()
        {
            ImGui::InputText("Config", configName, 64);

            ImGui::SameLine();
            if(ImGui::Button("Save")){
                std::string configFile = "Configs/" + std::string(configName) + ".json";
                json j = ToJson();
                std::ofstream file(configFile.c_str());
                file << std::setw(4) << j.dump(4) << std::endl;
                file.close();
                LoadConfigs();
            }
            
            ImGui::SameLine();
            
            if(ImGui::Button("Load")){
                std::string configFile = "Configs/" + std::string(configName) + ".json";
                std::ifstream file(configFile);
                if (file.is_open()) {
                    json j;
                    file >> j;
                    file.close();
                    Load(j);
                }
            }

            ImGui::BeginListBox("##Configs");
            int n=0;
            for(auto config : jsonFiles){
                const bool is_selected = (selectedConfig == n);
                if (ImGui::Selectable(config.c_str(), is_selected)) { 
                    selectedConfig = n; 
                    strcpy(configName, config.c_str());
                }

                if (is_selected) { ImGui::SetItemDefaultFocus(); }
                n++;
            }
            ImGui::EndListBox();
        }

        void Render() {
            if(!_isOpen){
                return;
            }

            ImGui::Begin("Settings");
            
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnOffset(1, 220);

            RenderLogo();
	        ImGui::Spacing();
            RenderTabs();

		    ImGui::NextColumn();
            
            switch (selectedTab)
            {
            case 1:
                RenderAimTabs();
                break;
            case 2:
                RenderVisualsTab();
                break;
            case 3:
                RenderMiscTab();
                break;
            case 4:
                RenderConfigs();
                break;
            
            default:
                break;
            }

            ImGui::End();
        }

        void SetScreenSize(int setScreenWidth, int setScreenHeight) {
            screenWidth = static_cast<float>(setScreenWidth);
            screenHeight = static_cast<float>(setScreenHeight);
        }

        PAimbotSettings GetAimbotSettings(){
            return &aimbotSettings;
        }

        PRcsSettings GetRcsSettings() {
            return &rcsSettings;
        }

        PGlowSettings GetGlowSettings() {
            return &glowSettings;
        }

        PEspSettings GetEspSettings() {
            return &espSettings;
        }

        PMiscSettings GetMiscSettings() {
            return &miscSettings;
        }

        Math::Vector2D GetScreenSize() {
            return {screenWidth, screenHeight};
        }

        Math::Vector2D GetScreenMid() {
            return {screenWidth/2.0f, screenHeight/2.0f};
        }

        void Load(const json& j)
        {
            aimbotSettings.Load(j.at("aimbotSettings"));
            rcsSettings.Load(j.at("rcsSettings"));
            glowSettings.Load(j.at("glowSettings"));
            espSettings.Load(j.at("espSettings"));
            miscSettings.Load(j.at("miscSettings"));
        }

        json ToJson() const 
        {
            return {
                {"aimbotSettings", aimbotSettings.ToJson()},
                {"rcsSettings", rcsSettings.ToJson()},
                {"glowSettings", glowSettings.ToJson()},
                {"espSettings", espSettings.ToJson()},
                {"miscSettings", miscSettings.ToJson()}
            };
        }
    };

}