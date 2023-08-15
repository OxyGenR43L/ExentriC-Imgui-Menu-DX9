// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3dx9.h>
#pragma comment (lib, "d3dx9.lib")
#include "imgui_internal.h"
#include "font.h"
#include "image.h"

#include "notification.h"

POINT mouse;
HWND hwnd;
RECT rc;

namespace image
{
    IDirect3DTexture9* logo = nullptr;
    IDirect3DTexture9* user = nullptr;
    IDirect3DTexture9* background = nullptr;
    IDirect3DTexture9* aimbot = nullptr;
    IDirect3DTexture9* visuals = nullptr;
    IDirect3DTexture9* skins = nullptr;
    IDirect3DTexture9* misc = nullptr;
    IDirect3DTexture9* settings = nullptr;

}

namespace font
{
    ImFont* inter_bold = nullptr;
    ImFont* inter_child = nullptr;
    ImFont* manrope_widgets = nullptr;

    ImFont* icon_font = nullptr;
}

namespace var
{
    namespace checkbox
    {
        bool keybind = false;
    }

    namespace keybind_checkbox
    {
        bool keybind = false;
        int key, m;
    }

    namespace keybind_pickerbox
    {
        float color_picker[4] = { 179 / 255.f, 80 / 255.f, 220 / 255.f, 1.f };
        bool checkbox = false;
        int key, m;
    }

    namespace picker_box
    {
        bool accent = false;
        float color[4] = { 179 / 255.f, 80 / 255.f, 220 / 255.f, 1.f };
    }

    namespace combo
    {
        int select = 0;
        const char* items[3]{ "Selected", "Selected ", "Selected  " };
    }

    namespace multi_combo
    {
        bool multi_num[10] = { true, true, true, true, true, false, false, false, false, false };
        const char* multi_items[10] = { "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten" };
    }

    namespace input_text
    {
        char input[16] = { "" };
    }

    namespace keybind
    {
        int key, m;
    }

    namespace slider
    {
        int slider_int = 50;
        float slider_float = 1.f;
    }

}

namespace addition
{
    const char* cheat_name = "ExentriC";
    const char* user_name = "Admin";
    ImVec2 logo_size = ImVec2(30, 30);
    ImVec2 user_circle_size = ImVec2(26, 26);

    float menu_opticaly = 1.f;
    bool menu_show = true;
}

DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    font::manrope_widgets = io.Fonts->AddFontFromMemoryTTF(manrope_medium, sizeof(manrope_medium), 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_child = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 17.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 14.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    font::icon_font = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 17.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    if (image::background == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &background, sizeof(background), 1920, 1080, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::background);
    if (image::logo == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &cheat_logo, sizeof(cheat_logo), 20, 20, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::logo);
    if (image::user == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &user_panel, sizeof(user_panel), 26, 26, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::user);

    if (image::aimbot == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &aimbot, sizeof(aimbot), 26, 26, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::aimbot);
    if (image::visuals == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &visuals, sizeof(visuals), 26, 26, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::visuals);
    if (image::skins == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &skins, sizeof(skins), 26, 26, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::skins);
    if (image::misc == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &misc, sizeof(misc), 26, 26, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::misc);
    if (image::settings == nullptr) D3DXCreateTextureFromFileInMemoryEx(g_pd3dDevice, &settings, sizeof(settings), 26, 26, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &image::settings);


    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

            c::accent_color = { var::picker_box::color[0], var::picker_box::color[1], var::picker_box::color[2], 1.f };
            c::accent_color_low = { var::picker_box::color[0], var::picker_box::color[1], var::picker_box::color[2], 0.5f };

            ImGui::GetBackgroundDrawList()->AddImage(image::background, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

            ImGuiStyle* style = &ImGui::GetStyle();

            style->WindowPadding = ImVec2(0, 0);
            style->ItemSpacing = ImVec2(20, 20);
            style->ScrollbarSize = 8.f;

            ImGui::SetNextWindowSizeConstraints(ImVec2(c::bg::size), ImGui::GetIO().DisplaySize);

            if (GetAsyncKeyState(VK_INSERT) & 0x01) addition::menu_show = !addition::menu_show;

            addition::menu_opticaly = ImLerp(addition::menu_opticaly, addition::menu_show ? 1.f : 0.f, ImGui::GetIO().DeltaTime * 10.f);

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, addition::menu_opticaly);

            if (addition::menu_opticaly > 0.01f) {
                ImGui::Begin("NULL", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
                {
                    const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(160, region.y), ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(160, 48), ImGui::GetColorU32(c::bg::Platinum), c::bg::rounding, ImDrawCornerFlags_Top);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 48), pos + ImVec2(160, region.y), ImGui::GetColorU32(c::bg::Platinum), c::bg::rounding, ImDrawCornerFlags_Bot);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(170, 0), pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);
                    ImGui::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(170, 0), pos + ImVec2(region.x, 48), ImGui::GetColorU32(c::bg::Platinum), c::bg::rounding, ImDrawCornerFlags_Top);

                    ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(170, 48), pos + ImVec2(region.x, 48), ImGui::GetColorU32(c::accent_color), 1.f);
                    ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(0, region.y - 48), pos + ImVec2(160, region.y - 48), ImGui::GetColorU32(c::accent_color), 1.f);
                    ImGui::GetBackgroundDrawList()->AddLine(pos + ImVec2(0, 48), pos + ImVec2(160, 48), ImGui::GetColorU32(c::accent_color), 1.f);

                    ImGui::GetBackgroundDrawList()->AddImage(image::logo, pos + (ImVec2(160, 48) - addition::logo_size) / 2, pos + (ImVec2(160, 48) + addition::logo_size) / 2, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::accent_color));
                    ImGui::GetBackgroundDrawList()->AddImage(image::user, pos + ImVec2(region.x - ((spacing.x / 2) + 26), (48 - addition::user_circle_size.x) / 2), pos + ImVec2(region.x - spacing.x / 2, (48 + addition::user_circle_size.x) / 2), ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(c::text::text_active));

                    ImGui::PushFont(font::inter_bold);
                    ImGui::GetBackgroundDrawList()->AddText(pos + ImVec2((160 - ImGui::CalcTextSize(addition::cheat_name).x) / 2, region.y - (48 + ImGui::CalcTextSize(addition::cheat_name).y) / 2), ImGui::GetColorU32(c::text::text), addition::cheat_name);

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(c::text::text));

                    ImGui::RenderTextClipped(pos + ImVec2(170, 0), pos + ImVec2(region.x - spacing.x - 26, 48), addition::user_name, NULL, NULL, ImVec2(1.0f, 0.5f), NULL);

                    ImGui::PopStyleColor();
                    ImGui::PopFont();

                    static int tabs = 0;
                    static int subtabs = 0;

                    ImGui::SetCursorPos(ImVec2(spacing.x, 48 + spacing.y));
                    ImGui::BeginGroup();
                    {
                        const char* tabs_array[] = { "Aimbot", "Visuals", "Skins", "Misc", "Settings" };
                        IDirect3DTexture9* image_array[5] = { image::aimbot, image::visuals, image::skins, image::misc, image::settings };

                        for (int i = 0; i < 5; i++) if (ImGui::Tabs(tabs == i, image_array[i], tabs_array[i], ImVec2(140 - spacing.x, 30))) tabs = i;

                    }
                    ImGui::EndGroup();

                    ImGui::SetCursorPos(ImVec2(170 + spacing.x, 0));
                    ImGui::BeginGroup();
                    {
                        const char* subtabs_array[] = { "Enemy", "Team", "Others" };

                        for (int i = 0; i < 3; i++) {
                            if (ImGui::Subtab(subtabs == i, subtabs_array[i], ImVec2(60, 48))) subtabs = i;
                            ImGui::SameLine(0, 0);
                        }
                    }
                    ImGui::EndGroup();

                    static float tab_alpha = 0.f; /* */ static float tab_add; /* */ static int active_tab = 0;

                    tab_alpha = ImClamp(tab_alpha + (4.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
                    tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

                    if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;

                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * style->Alpha);

                    ImGui::SetCursorPos(ImVec2(170, 48) + style->ItemSpacing);
                    ImGui::BeginChild("ChildBar", ImVec2(region) - ImVec2(170, 48) - style->ItemSpacing);
                    {
                        if (active_tab == 0) {

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("GENERAL", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 4)) / 3));
                                {

                                    ImGui::Keybindbox("Enabled Aimbot", &var::keybind_checkbox::keybind, &var::keybind_checkbox::key, &var::keybind_checkbox::m);

                                    ImGui::KeybindPickerbox("Silent Aim", &var::keybind_pickerbox::checkbox, &var::keybind_pickerbox::key, &var::keybind_pickerbox::m, var::keybind_pickerbox::color_picker, picker_flags);

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("OTHER", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 4)) / 3));
                                {

                                    ImGui::Combo("Combo", &var::combo::select, var::combo::items, IM_ARRAYSIZE(var::combo::items), 3);

                                    ImGui::MultiCombo("Multi-Combo", var::multi_combo::multi_num, var::multi_combo::multi_items, 10);

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("MISC", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 4)) / 3));
                                {

                                    if (ImGui::Button("Button", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 25)));

                                    ImGui::InputTextEx("##NULL", "Enter your text here", var::input_text::input, 16, ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 25), NULL);

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();

                            ImGui::SameLine();

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("AIMBOT", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                    if (ImGui::Button("Succesfully", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Success, 2000, "Successfully!" });
                                    ImGui::SameLine();
                                    if (ImGui::Button("Warning", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Warning, 2000, "Warning!" });

                                    if (ImGui::Button("Error", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Error, 2000, "Error!" });
                                    ImGui::SameLine();
                                    if (ImGui::Button("Info", ImVec2((ImGui::GetContentRegionMax().x - style->WindowPadding.x - spacing.x / 2) / 2, 25))) ImGui::Notification({ ImGuiToastType_Info, 2000, "Notification" });

                                    if (ImGui::Button("Config", ImVec2(ImGui::GetContentRegionMax().x - style->WindowPadding.x, 25))) ImGui::Notification({ ImGuiToastType_Config, 2000, "Config saved successfully!" });

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("SETTINGS", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                    ImGui::Keybind("Keybind", &var::keybind::key, &var::keybind::m);

                                    ImGui::Pickerbox("Accent Color", &var::picker_box::accent, var::picker_box::color, picker_flags);

                                    ImGui::SliderInt("Slider Integer", &var::slider::slider_int, 1, 1000);

                                    ImGui::SliderFloat("Slider Float", &var::slider::slider_float, -1.f, 10.f, "%.3f");

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();
                        }
                        else if (active_tab == 1) {

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("SETTINGS", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 2))));
                                {

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();

                            ImGui::SameLine();

                            ImGui::BeginGroup();
                            {
                                ImGui::CustomBeginChild("MISC", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                }
                                ImGui::CustomEndChild();

                                ImGui::CustomBeginChild("JUST NAME", ImVec2((region.x / 2) - spacing.x - (spacing.x / 2) - (170 / 2), (region.y - 48 - (spacing.y * 3)) / 2));
                                {

                                }
                                ImGui::CustomEndChild();
                            }
                            ImGui::EndGroup();
                        }

                    }
                    ImGui::EndChild();

                    ImGui::PopStyleVar();

                    ImGui::RenderNotifications();
                }
                ImGui::End();
            }

            ImGui::PopStyleVar();

            ImGui::SetNextWindowSizeConstraints(ImVec2(320, 500), ImGui::GetIO().DisplaySize);

            ImGui::Begin("NULL ONE", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
            {
                const ImVec2& pos = ImGui::GetWindowPos(), spacing = style->ItemSpacing, region = ImGui::GetContentRegionMax();

                ImGui::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(region), ImGui::GetColorU32(c::bg::background), c::bg::rounding);

                ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(pos + ImVec2(spacing) * 2, pos + ImVec2(region.x - (spacing.x * 2), spacing.y * 2 + 2), ImColor(255, 0, 0, 255), ImColor(0, 255, 0, 255), ImColor(0, 255, 0, 255), ImColor(255, 0, 0, 255), c::bg::rounding);

                ImGui::GetWindowDrawList()->AddRect(pos + ImVec2(spacing), pos + ImVec2(region) - spacing, GetColorU32(c::accent_color));

            }
            ImGui::End();

        }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
