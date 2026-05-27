#include "Editor.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h> // required for DockBuilder API
#include <glm/gtc/type_ptr.hpp>

void Editor::onInit() {
    //m_isEditor = true;

    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->AddFontFromFileTTF("../assets/fonts/Inter_18pt-Regular.ttf", 18.0f);

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;
    style.TabRounding = 0.0f;
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.DockingSeparatorSize = 1.0f;
    style.TabBarBorderSize = 0.0f;
    style.TabMinWidthBase = 0.0f;
    style.TabBarOverlineSize = 0.0f;
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(14.0f / 255.0f, 14.0f / 255.0f, 14.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(29.0f / 255.0f, 29.0f / 255.0f, 29.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(29.0f / 255.0f, 29.0f / 255.0f, 29.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(29.0f / 255.0f, 29.0f / 255.0f, 29.0f / 255.0f, 1.0f);

    style.Colors[ImGuiCol_TabHovered] = ImVec4(55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(55.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1.0f);

    style.Colors[ImGuiCol_TabDimmedSelected] = ImVec4(29.0f / 255.0f, 29.0f / 255.0f, 29.0f / 255.0f, 1.0f);
    ImGui_ImplGlfw_InitForOpenGL(Kita::Engine::getEngine()->getWindow().getGLFWwindow(), true);
    ImGui_ImplOpenGL3_Init(Kita::Window::GLSL_VERSION.data());

    m_sandbox = std::make_shared<Sandbox>();
    m_sandbox->onInit();
}

void Editor::onUpdate() {
    m_sandbox->onUpdate();
}

void Editor::buildInitialLayout(ImGuiID dockspaceId) {
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

    ImGuiID dockLeft;
    ImGuiID dockCenter = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.20f, &dockLeft, &dockspaceId);
    ImGuiID dockRight;
    ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Right, 0.25f, &dockRight, &dockspaceId);
    ImGuiID dockLeftTop, dockLeftBot;
    ImGui::DockBuilderSplitNode(dockLeft, ImGuiDir_Up, 0.60f, &dockLeftTop, &dockLeftBot);
    ImGuiID dockCenterTop, dockCenterBot;
    ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Up, 0.70f, &dockCenterTop, &dockCenterBot);

    ImGui::DockBuilderDockWindow("Scene", dockLeftTop);
    ImGui::DockBuilderDockWindow("File Browser", dockLeftBot);
    ImGui::DockBuilderDockWindow("Viewport", dockCenterTop);
    ImGui::DockBuilderDockWindow("Console", dockCenterBot);
    ImGui::DockBuilderDockWindow("Properties", dockRight);
    ImGui::DockBuilderDockWindow("Properties Second", dockRight);

    ImGui::DockBuilderFinish(dockspaceId);
    m_layoutInitialized = true;
}

void Editor::onRender() {
    m_sandbox->onRender();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //static bool yes = true;
    //ImGui::ShowDemoWindow(&yes);

    ImGui::Begin("Debug Properties");
    Kita::Entity light = Kita::Entity(&m_sandbox->getScene(), m_sandbox->getScene().view<Kita::LightComponent>().front());
    Kita::Entity postProcess = Kita::Entity(&m_sandbox->getScene(), m_sandbox->getScene().view<Kita::PostProcessingComponent>().front());
    Kita::Entity scene = Kita::Entity(&m_sandbox->getScene(), m_sandbox->getScene().view<Kita::SceneComponent>().front());
    auto& lightComponent = light.getComponent<Kita::LightComponent>();
    auto& postProcessComponent = postProcess.getComponent<Kita::PostProcessingComponent>();
    auto& sceneComponent = scene.getComponent<Kita::SceneComponent>();
    ImGui::DragFloat3("Direction", glm::value_ptr(lightComponent.properties.direction), 0.01f, -1.0f, 1.0f);
    ImGui::DragFloat3("Diffuse", glm::value_ptr(lightComponent.properties.diffuse), 0.05f, 0.0f, 20.0f);
    ImGui::DragFloat("PostProcess Exposure", &postProcessComponent.properties.exposure, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("IBL Intensity", &sceneComponent.properties.iblIntensity, 0.01f, 0.0f, 10.0f);
    ImGui::End();

    /*
    const ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::SetNextWindowViewport(vp->ID);

    ImGuiWindowFlags host_flags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_MenuBar; // so the menu bar sits inside the host

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
    ImGui::Begin("##DockspaceHost", nullptr, host_flags);
    ImGui::PopStyleVar(3);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) { ImGui::EndMenu(); }
        if (ImGui::BeginMenu("Edit")) { ImGui::EndMenu(); }
        if (ImGui::BeginMenu("View")) { ImGui::EndMenu(); }
        ImGui::EndMenuBar();
    }

    ImGuiID dockspaceId = ImGui::GetID("MainDockspace");
    ImGui::DockSpace(dockspaceId, {0.0f, 0.0f}, ImGuiDockNodeFlags_None);

    if (!m_layoutInitialized)
        buildInitialLayout(dockspaceId);

    ImGui::End();

    const ImGuiWindowFlags panel_flags =
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("Scene", nullptr, panel_flags);
    ImGui::End();

    ImGui::Begin("File Browser", nullptr, panel_flags);
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
    ImGui::Begin("Viewport", nullptr, panel_flags);
    ImGui::PopStyleVar();
    ImVec2 vpSize = ImGui::GetContentRegionAvail();
    ImGui::Image(
        (ImTextureID)(intptr_t)Kita::Engine::getEngine()
                               ->getRenderer().getOutputFramebuffer()
                               .getColorTexture()->getTexture(),
        vpSize, {0.0f, 1.0f}, {1.0f, 0.0f});
    ImGui::End();

    ImGui::Begin("Console", nullptr, panel_flags);
    ImGui::End();

    ImGui::Begin("Properties", nullptr, panel_flags);
    ImGui::End();

    ImGui::Begin("Properties Second", nullptr, panel_flags);
    ImGui::End();*/

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::onExit() {
    m_sandbox->onExit();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
