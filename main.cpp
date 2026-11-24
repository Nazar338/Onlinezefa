#include "Files/define.h"
#include "Files/Login.h"
#include "Files/patch/AntiCrack.h"
#include "Files/patch/ResetGuest.h"

float aimDistance = 200.0f;
bool aimPred = true;
float aimSmooth = 4.2f;
bool aimRecoilComparison = true;
bool g_Lag = false;

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (glWidth <= 0 || glHeight <= 0)
        return orig_eglSwapBuffers(dpy, surface);

    if (!g_App)
        return orig_eglSwapBuffers(dpy, surface);

    egl.screenWidth = ANativeWindow_getWidth(g_App->window);
    egl.screenHeight = ANativeWindow_getHeight(g_App->window);
    density = AConfiguration_getDensity(g_App->config);

    if (g_Lag && !isLogin) {
        for (int i = 0; i < 10000000; i++) {
            asm volatile("");
        }
    }

    if (!initImGui) {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;
        // Deep blue glass theme with bright cyan controls, matching the supplied clip.
        colors[ImGuiCol_Text] = ImVec4(0.91f, 0.97f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.48f, 0.63f, 0.72f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.025f, 0.075f, 0.125f, 0.97f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.035f, 0.105f, 0.170f, 0.96f);
        colors[ImGuiCol_Border] = ImVec4(0.20f, 0.82f, 1.00f, 0.95f);
        colors[ImGuiCol_Button] = ImVec4(0.055f, 0.22f, 0.34f, 0.96f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.58f, 0.78f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.10f, 0.70f, 0.88f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.055f, 0.19f, 0.29f, 0.98f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.08f, 0.34f, 0.48f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.43f, 0.58f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.32f, 0.91f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.22f, 0.78f, 0.96f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.91f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.02f, 0.09f, 0.15f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.035f, 0.22f, 0.34f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.02f, 0.09f, 0.15f, 0.80f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.10f, 0.42f, 0.56f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.68f, 0.82f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.28f, 0.82f, 0.96f, 1.00f);
        style.WindowRounding = 22.0f;
        style.ChildRounding = 22.0f;
        style.FrameRounding = 22.0f;
        style.GrabRounding = 22.0f;
        style.PopupRounding = 10.0f;
        style.ScrollbarRounding = 30.0f;
        style.TabRounding = 22.0f;
        style.WindowBorderSize = 2.0f;
        style.FrameBorderSize = 2.0f;
        style.PopupBorderSize = 1.0f;
        style.ChildBorderSize = 0.0f;
        style.WindowPadding = ImVec2(33, 32);
        style.FramePadding = ImVec2(18, 13.8);
        style.ItemSpacing = ImVec2(15, 8);
        style.ItemInnerSpacing = ImVec2(14, 10);
        style.GrabRounding = 10.0f;
        style.GrabMinSize = 52.0f;
        style.IndentSpacing = 0.0f;
        style.ScrollbarSize = 60.0f;
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);

        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false;
        font_cfg.OversampleH = 3;
        font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
        ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)RobotoFont, 146004, 23.1576f, &font_cfg);
        ZoloMainFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)RobotoFont, 146004, 22.908f, &font_cfg);
        ZoloFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)RobotoFont, 146004, 48, &font_cfg);
        ZoloEFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)RobotoFont, 146004, 33, &font_cfg);
        ImFontConfig pixelFontCfg;
        pixelFontCfg.SizePixels = 23.24f;
        ZoloPixelFont = ImGui::GetIO().Fonts->AddFontDefault(&pixelFontCfg);
        ImGui::GetIO().FontDefault = ZoloPixelFont;
        ZoloEspPixelFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)ProggyCleanTTF, ProggyCleanTTF_size, 11.288f);
        ZoloEsp2PixelFont = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)ProggyCleanTTF, ProggyCleanTTF_size, 16.6f);
        ImGui::GetIO().FontGlobalScale = 1.0f;
        g_CurrentFPS = ImGui::GetIO().Framerate;
        style.ScaleAllSizes(0.664);
        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");
        initImGui = true;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    
    if (!sdkReset) {
        ImGui::SetNextWindowSizeConstraints(ImVec2(140 * 0.664, 140 * 0.664), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::SetNextWindowSize(ImVec2((float)glWidth * 0.3185f, (float)glHeight * 0.468f), ImGuiCond_FirstUseEver);
        bool open = true;
        ImGui::Begin(OBFUSCATE("ZoloCheat-PUBG MOBILE-VIP-4.7-ESP+AIM2.0 :)"), 0);

        // Animated cyan/blue light traveling around the outside of the menu.
        // This is visual-only: it does not change widgets, settings, or game logic.
        {
            const ImVec2 wp = ImGui::GetWindowPos();
            const ImVec2 ws = ImGui::GetWindowSize();
            const float t = (float)ImGui::GetTime();
            ImDrawList* glow = ImGui::GetBackgroundDrawList();
            const ImVec2 mn(wp.x - 3.0f, wp.y - 3.0f);
            const ImVec2 mx(wp.x + ws.x + 3.0f, wp.y + ws.y + 3.0f);

            glow->AddRect(mn, mx, IM_COL32(20, 175, 255, 35), 22.0f, 0, 8.0f);
            glow->AddRect(ImVec2(mn.x + 1.0f, mn.y + 1.0f),
                          ImVec2(mx.x - 1.0f, mx.y - 1.0f),
                          IM_COL32(55, 215, 255, 65), 21.0f, 0, 4.0f);

            const float w = ws.x + 6.0f;
            const float h = ws.y + 6.0f;
            const float perimeter = 2.0f * (w + h);
            const auto pointOnEdge = [&](float distance) -> ImVec2 {
                float d = fmodf(distance, perimeter);
                if (d < 0.0f) d += perimeter;
                if (d < w) return ImVec2(mn.x + d, mn.y);
                d -= w;
                if (d < h) return ImVec2(mx.x, mn.y + d);
                d -= h;
                if (d < w) return ImVec2(mx.x - d, mx.y);
                d -= w;
                return ImVec2(mn.x, mx.y - d);
            };

            for (int trail = 0; trail < 3; ++trail) {
                const float head = fmodf(t * (115.0f + trail * 12.0f) +
                                         perimeter * (float)trail / 3.0f, perimeter);
                const float span = perimeter * 0.105f;
                ImVec2 prev = pointOnEdge(head);
                for (int j = 1; j <= 24; ++j) {
                    const float d = head + span * (float)j / 24.0f;
                    const ImVec2 next = pointOnEdge(d);
                    const float hue = fmodf(0.52f + t * 0.055f + trail * 0.16f +
                                            (float)j / 24.0f * 0.10f, 1.0f);
                    const ImU32 col = (ImU32)ImColor::HSV(hue, 0.62f, 1.0f, 0.92f);
                    glow->AddLine(prev, next, col, 3.0f);
                    prev = next;
                }
            }
        }

        if (!isLogin) {
            ImGui::PushFont(ZoloMainFont);
            ImGui::Text(OBFUSCATE("Detecting your key..."));
            ImGui::Spacing();

            static char buffer[64] = {0};
            static bool initialized = false;
            static std::string lastClipboardText;
            static std::string lastError = "";
            static bool autoCheckDone = false;

            {
                std::lock_guard<std::mutex> lock(loginMutex);
                if (!initialized) {
                    if (!s.empty()) {
                        strncpy(buffer, s.c_str(), sizeof(buffer) - 1);
                        buffer[sizeof(buffer) - 1] = '\0';
                    }
                    initialized = true;
                }
            }

            std::string clip = getClipboardText();
            bool clipboardChanged = !clip.empty() && clip != lastClipboardText;
            if (clipboardChanged) {
                strncpy(buffer, clip.c_str(), sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
                {
                    std::lock_guard<std::mutex> lock(loginMutex);
                    s = buffer;
                }
                lastClipboardText = clip;
                autoCheckDone = false;
            }

            if (ImGui::InputText("##KeyInput", buffer, sizeof(buffer))) {
                std::lock_guard<std::mutex> lock(loginMutex);
                s = buffer;
                autoCheckDone = false;
            }

            if (!autoCheckDone && strlen(buffer) > 0) {
                std::string loginCopy = buffer;
                std::string result = Login(loginCopy.c_str());

                if (result == "OK" && bValid && g_Auth == g_Token && !g_Auth.empty()) {
                    isLogin = true;
                    firstLoginDone = true;
                    lastError.clear();
                    g_Lag = false;
                } else {
                    if (result.empty()) {
                        lastError = "Error 404";
                    } else {
                        lastError = result;
                    }
                    g_Lag = true;
                }
                autoCheckDone = true;
            }

            if (!lastError.empty() && !isLogin) {
                ImGui::Spacing();
                ImGui::Text("%s", lastError.c_str());
            }

            ImGui::PopFont();
        }

        if (isLogin) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGui::SetCursorPosX(0);
            ImGui::SetCursorPosY(75 * 0.664);
            ImGui::BeginChild("Bankai", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(48 / 255.0f, 55 / 255.0f, 61 / 255.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(82 / 255.0f, 113 / 255.0f, 134 / 255.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(48 / 255.0f, 55 / 255.0f, 61 / 255.0f, 1.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(13 * 0.664, 17 * 0.664));
            ImGui::SetCursorPosY(20 * 0.664);
            ImGui::SetCursorPosX(20 * 0.664);
            ImGui::BeginChild(OBFUSCATE("Tabs"), ImVec2(0, 0), true);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5 * 0.664, 16 * 0.600));
            const char* tabs[] = {
                OBFUSCATE(" VISUAL"),
                OBFUSCATE(" ITEMS "),
                OBFUSCATE("VEHICLE"),
                OBFUSCATE("AIM MENU"),
                OBFUSCATE(" Extras")
            };
            for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
                ImGui::SetCursorPosX(5 * 0.664);
                if (ImGui::Button(tabs[i], ImVec2(175 * 0.664, 60 * 0.664))) {
                    current_tab = i + 1;
                }
            }
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);
            ImGui::EndChild();

            if (current_tab == 1) {
                ImGui::SetCursorPosY(3.3 * 0.664);
                ImGui::SetCursorPosX(202.9 * 0.664);
                ImGui::BeginChild("ZoloCheat", ImVec2(0, 0), true);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5 * 0.664, 16 * 0.664));
                ImGui::SetCursorPosY(9.8 * 0.664);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Text(OBFUSCATE("ZoloCheat    FPS : %.1f"), io.Framerate);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Hide Record"), &sdkReset); 
                ImGui::SameLine(340 * 0.664);
                ImGui::Checkbox(OBFUSCATE("ESP"), &enableAllEsp);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Name "), &espName);
                ImGui::SameLine(235 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Line"), &espLine);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Team ID "), &espTeamID);
                ImGui::SameLine(235 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Distance"), &espDistance);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Health"), &espHealth);
                ImGui::SameLine(235 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Skeleton"), &espSkeleton);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(48 / 255.0f, 55 / 255.0f, 61 / 255.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(80 / 255.0f, 91 / 255.0f, 97 / 255.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(80 / 255.0f, 91 / 255.0f, 97 / 255.0f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(14 * 0.664, 8 * 0.664));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10 * 0.664, 13.8 * 0.664));
                ImGui::PopStyleVar(2 * 0.664);
                ImGui::PopStyleColor(3 * 0.664);
                ImGui::Separator();
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("Extras"));
                ImGui::Separator();
                ImGui::Checkbox(OBFUSCATE("LootBox"), &espLootbox);
                ImGui::SameLine(226 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Grenade"), &espGrenade);
                ImGui::PopStyleVar();
                ImGui::EndChild();
            } else if (current_tab == 2) {
                ImGui::SetCursorPosY(3.3 * 0.664);
                ImGui::SetCursorPosX(202.9 * 0.664);
                ImGui::BeginChild(OBFUSCATE("Functions"), ImVec2(0, 0), true);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5 * 0.664, 16 * 0.664));
                ImGui::SetCursorPosY(9.8 * 0.664);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("Items Menu"));
                ImGui::SetCursorPosY(80 * 0.664);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("coming soon"));
                ImGui::PopStyleVar();
                ImGui::EndChild();
            } else if (current_tab == 3) {
                ImGui::SetCursorPosY(3.3 * 0.664);
                ImGui::SetCursorPosX(202.9 * 0.664);
                ImGui::BeginChild(OBFUSCATE("Functions"), ImVec2(0, 0), true);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5 * 0.664, 16 * 0.664));
                ImGui::SetCursorPosY(9.8 * 0.664);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("Vehicle"));
                ImGui::Separator();
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Enable"), &espVehicle);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Speed"), &espVehicleSpeed);
                ImGui::PopStyleVar();
                ImGui::EndChild();
            } else if (current_tab == 4) {
                ImGui::SetCursorPosY(3.3 * 0.664);
                ImGui::SetCursorPosX(202.9 * 0.664);
                ImGui::BeginChild(OBFUSCATE("Functions"), ImVec2(0, 0), true);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5 * 0.664, 16 * 0.664));
                ImGui::SetCursorPosY(9.8 * 0.664);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Enable Aim"), &aimEnable);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Ignore knocked"), &aimKnock);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Ignore bot"), &aimBot);
                float windowWidth = ImGui::GetWindowSize().x;
                float padding = windowWidth * 0.35f;
                ImGui::PushItemWidth(windowWidth - padding);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18 * 0.664, 12 * 0.664));
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::SliderFloat(OBFUSCATE("FOV"), &fov, 0, 400);
                ImGui::PopStyleVar();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(48 / 255.0f, 55 / 255.0f, 61 / 255.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(80 / 255.0f, 91 / 255.0f, 97 / 255.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(80 / 255.0f, 91 / 255.0f, 97 / 255.0f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(14 * 0.664, 8 * 0.664));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10 * 0.664, 13.8 * 0.664));
                ImGui::PushItemWidth(windowWidth - padding);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Combo(OBFUSCATE("Visibility check"), &VisibilityCheck, VisiblityCheckOptions, IM_ARRAYSIZE(VisiblityCheckOptions));
                ImGui::PushItemWidth(windowWidth - padding);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Combo(OBFUSCATE("Trigger"), &Trigger, TriggerOptions, IM_ARRAYSIZE(TriggerOptions));
                ImGui::PushItemWidth(windowWidth - padding);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Combo(OBFUSCATE("Target mode"), &TargetMode, TargetModeOptions, IM_ARRAYSIZE(TargetModeOptions));
                ImGui::PushItemWidth(windowWidth - padding);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Combo(OBFUSCATE("Target bone"), &TargetBone, TargetBoneOptions, IM_ARRAYSIZE(TargetBoneOptions));
                ImGui::PopStyleVar(2);
                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar();
                ImGui::EndChild();
            } else if (current_tab == 5) {
                ImGui::SetCursorPosY(3.3 * 0.664);
                ImGui::SetCursorPosX(202.9 * 0.664);
                ImGui::BeginChild(OBFUSCATE("Functions"), ImVec2(0, 0), true);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5 * 0.664, 16.5 * 0.664));
                ImGui::SetCursorPosY(9.8 * 0.664);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("Change PLayer View Angle.."));
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Enable Wide View"), &sdkView);
                float windowWidth = ImGui::GetWindowSize().x;
                float padding = windowWidth * 0.35f;
                ImGui::PushItemWidth(windowWidth - padding);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(18 * 0.664, 12 * 0.664));
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::SliderFloat(OBFUSCATE("View"), &sdkViewFloat, 50, 170);
                ImGui::PopStyleVar();
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("ESP Extras"));
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Checkbox(OBFUSCATE("Hide Bot"), &espNoBot);
                ImGui::SetCursorPosX(15 * 0.664);
                ImGui::Text(OBFUSCATE("Tools...."));
                ImGui::SetCursorPosX(15 * 0.664);
                if (ImGui::Button(OBFUSCATE("Reset Guest"))) {
                    resetGuest();
                }
                ImGui::PopStyleVar();
                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    if (isLogin && !sdkReset) {
        DrawESP(ImGui::GetBackgroundDrawList());

        // HUD labels transferred from huey.cpp
        ImDrawList* draw = ImGui::GetForegroundDrawList();
        const ImU32 hudColor = IM_COL32(0, 255, 255, 255);
        const float hudSize = (float)density / 15.0f;
        draw->AddText(NULL, hudSize,
            ImVec2((float)glWidth / 13.0f + (float)glWidth / 30.0f, 15.0f),
            hudColor, OBFUSCATE("Feedbacks: @Feedback_cheatbot"));
        draw->AddText(NULL, hudSize,
            ImVec2((float)glWidth / 150.0f + (float)glWidth / 40.0f, 650.0f),
            hudColor, OBFUSCATE("Telegram - @Zolo_cheatss"));
    }

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return orig_eglSwapBuffers(dpy, surface);
}

std::chrono::steady_clock::time_point lastChangeTime;

void *maps_thread(void *) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        static int lastState = -1;
        int currentState = (g_LocalController == nullptr) ? 0 : 1;
        if (currentState != lastState) {
            lastState = currentState;
            if (currentState == 1) {
                lastChangeTime = std::chrono::steady_clock::now();
            } else {
                InGame = false;
            }
        }
        if (lastState == 1 && std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - lastChangeTime).count() >= 5) {
            InGame = true;
        }
    }
}

void *itsmyhubs_thread(void *) {

PATCH_LIB("libUE4.so","0x7504B5C","C0 03 5F D6");
PATCH_LIB("libUE4.so","0x85F03F8","C0 03 5F D6");// RPC_ClientCoronaLab

PATCH_LIB("libanogs.so", "0x2190A0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22EF80", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x3AE4DC", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x340968", "C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4DE660", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x340968", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x3853CC", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x3AE4DC", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x63ACAE8", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2F43B0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x3A8C18", "C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x22EF80", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22F374", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x80F0DB8", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2191B0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x3379F0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x239B40", "C0 03 5F D6");



PATCH_LIB("libUE4.so", "0x65984e4", "00 00 80 D2 C0 03 5F D6"); //4.7.0 Small + less recoil

PATCH_LIB("libanogs.so", "0x2E7840", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4D73B4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4D7644", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4D73B4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4D76E4", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x477830", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F85D0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x479694", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x23A010", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x495160", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F486C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F5478", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F486C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F4C78", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2342C4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x23644C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x232720", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2A06F4", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x2A57B8", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x1D2E20", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x218AC4", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4AF418", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4B1B24", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x2191B0", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4F5478", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1B24", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4DBBDC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4DC38C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F8930", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1A74", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x239B40", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");


PATCH_LIB("libUE4.so", "0x65984e4", "00 00 80 D2 C0 03 5F D6"); //4.7.0 Small + less recoil

PATCH_LIB("libanogs.so", "0x4F6AEC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F486C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F4C78", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4DD694", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4DD674", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F6674", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B2890", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4DD624", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x244ACC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1DD4", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x24847C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7504", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F61FC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x245350", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FC158", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4B1B60", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7084", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F5A38", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FB3D8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FA2F8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F9E84", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7980", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4FB3D8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1C90FC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x20E040", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x20E098", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x210758", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x21088C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2105EC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2107B0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F97CC", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4FB858", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2485B8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7DF4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FA1DC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x477830", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x47D1A0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FBA98", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F68B0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2A5990", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22FFFC", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4FABF8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F946C", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x230A50", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FAAD8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7BBC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2A06F4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1DD4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x240550", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x239B40", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1C6B94", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x218AC4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x218F28", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1B24", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1CAF18", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4AF418", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2A6F44", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2A7B10", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C4C58", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C3F14", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C3F50", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C4C44", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C3F3C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C3BE0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C4C30", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1CB01C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x239EBC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x245658", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x245920", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x225A3C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x225BD4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22EE80", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22F178", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22EE80", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22FCA4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22EE80", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22F374", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22EE80", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22FA78", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x246308", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x20034C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4DD6F4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C3F28", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C4C6C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2159FC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x215E6C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F486C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F4D78", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x24614C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4D193C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4D1B08", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F8150", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x240550", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x246314", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1CB120", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F71A4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1A74", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F85D0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1A74", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F98EC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1A74", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FC6F4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x239B40", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7F10", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4AE6F4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FB738", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4AE904", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FBF18", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x47C034", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x480DA4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F7F10", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4AE6F4", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4AE904", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4FB738", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1A74", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1D12A0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22FFFC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x23006C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x23032C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1E1E90", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1E2A5C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x20BBE8", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1E34C4", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x2E470C", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x2398AC", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x20BBA0", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x20BB54", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F6AEC", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x239B40", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F54A4", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4F486C", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1E1E90", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x1E29D0", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F486C", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4F4C78", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4F5478", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x232720", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x232934", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4D1100", "00 00 80 D2 C0 03 5F D6");

PATCH_LIB("libanogs.so", "0x4D1738", "00 00 80 D2 C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x246D44", "00 00 80 D2 C0 03 5F D6");

MemoryPatch::createWithHex("libanogs.so",0x22F374,"C0 03 5F D6").Modify();//Case 37
MemoryPatch::createWithHex("libanogs.so",0x2F43B0,"C0 03 5F D6").Modify();//10y
MemoryPatch::createWithHex("libanogs.so",0x21813C,"C0 03 5F D6").Modify();//crash
MemoryPatch::createWithHex("libanogs.so",0x2191B0,"C0 03 5F D6").Modify();//Mid Game Crash fix
PATCH_LIB("libanogs.so","0x328BB8","00 00 80 D2 C0 03 5F D6");//Obb stopper
PATCH_LIB("libanogs.so","0x3AE4DC","00 00 80 D2 C0 03 5F D6");//Vtable Check
PATCH_LIB("libUE4.so","0x7E67494","00 00 80 D2 C0 03 5F D6");//Android so
PATCH_LIB("libanogs.so","0x2707EC","00 00 80 D2 C0 03 5F D6");//Flag Fix

PATCH_LIB("libUE4.so", "0x7504B5C0", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x85F03F", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x80F0DB", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x7AD66F", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x5EB3A6", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x6D1B72", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x7428BF", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x73E30E", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0xA12B70", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x698482", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x6982E3", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x698320", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x62074E", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x67B8C0", "C0 03 5F D6");
PATCH_LIB("libUE4.so", "0x8BFCE8", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x214098", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22BD98", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2F7A9C", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B5D00", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2940D0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4B1DF8", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x1ECD44", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x22F374", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2F43B0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x21813C", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2191B0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C5BC0", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x4C5B64", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x328BB8", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x3AE4DC", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x2707EC", "C0 03 5F D6");
PATCH_LIB("libanogs.so", "0x40ACF0", "C0 03 5F D6");
PATCH_LIB("libhdmpve.so", "0x11A348", "C0 03 5F D6");
PATCH_LIB("libhdmpve.so", "0x11A608", "C0 03 5F D6");

MemoryPatch::createWithHex("libhdmpve.so",0x89CF8,"00 00 00 00").Modify();//on
MemoryPatch::createWithHex("libhdmpve.so",0xA2DF4,"00 00 00 00").Modify();//on
MemoryPatch::createWithHex("libhdmpve.so",0x89CF8,"2D E9 F0 4F").Modify();//off
MemoryPatch::createWithHex("libhdmpve.so",0xA2DF4,"2D E9 F0 4F").Modify();//off
PATCH_LIB("libanogs.so","0x218EE8","C0 03 5F D6");//LOBBY CRASH FIXER
return nullptr;
}

void *main_thread(void *) {
    UE4 = Tools::GetBaseAddress("libUE4.so");
    while (!UE4) {
        UE4 = Tools::GetBaseAddress("libUE4.so");
        sleep(1);
    }
    sleep(4);
    while (!g_App) {
        g_App = *(android_app **)(UE4 + GNativeAndroidApp_Offset);
        sleep(1);
    }

    // ========== вызов ==========
    ZoloAntiCrack1();
    ZoloAntiCrack2();
    ZoloAntiCrack3();
    // =========================

    FName::GNames = GetGNames();
    while (!FName::GNames) {
        FName::GNames = GetGNames();
        sleep(1);
    }

    UObject::GUObjectArray = (FUObjectArray *)(UE4 + GUObject_Offset);
    void *libAndroid = dlopen_ex(OBFUSCATE("libandroid.so"), 4);
    if (libAndroid) {
        void *symEvent = dlsym_ex(libAndroid, OBFUSCATE("AInputQueue_getEvent"));
        protectFunc(symEvent);
        DobbyHook(symEvent, (void *)hooked_AInputQueue_getEvent, (void **)&orig_AInputQueue_getEvent);
        dlclose_ex(libAndroid);
    }
    void *egl = dlopen_ex("libEGL.so", 4);
    while (!egl) {
        egl = dlopen_ex("libEGL.so", 4);
        sleep(1);
    }
    DobbyHook((void *) dlsym_ex(egl, "eglSwapBuffers"), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
    pthread_t t;
    pthread_create(&t, 0, maps_thread, 0);
    return nullptr;
}

__attribute__((constructor)) void _init()
{
    pthread_t t;
    pthread_create(&t, 0, main_thread, 0);
    pthread_create(&t, NULL, itsmyhubs_thread, NULL);
}
