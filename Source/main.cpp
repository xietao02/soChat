#define STB_IMAGE_IMPLEMENTATION
#include "GUI_setup.h"
#include <iostream>
using namespace std;



int main(int, char**){
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = GL_init();

    // Create window with graphics context
    int display_w = 940, display_h = 640;
    GLFWwindow* window = glfwCreateWindow(display_w, display_h, "soChat", NULL, NULL);
    if (window == NULL)
        return 1;

    // Set the Context of the created window as the main Context of the current thread
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Set the window icon
    GLFWimage logo;
    int channels = 3;
    logo.pixels = stbi_load("./images/icons/logo.png", &logo.width, &logo.height, &channels, STBI_rgb_alpha);
    glfwSetWindowIcon(window, 1, &logo);
    //ImTextureID

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    io.Fonts->AddFontFromFileTTF("./fonts/DroidSans.ttf", 21.0f);

    // status
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);


    // initialize soTalk
	bool chat_selected = true, chat_pressed = false, contact_selected = false, contact_pressed = false;
    bool Friends_selected = true, Groups_selected = false, Friends_pressed = false, Groups_pressed = false;
	// initialize Chat area
    bool chat_p_open = true;


    //Load texture
    int texture_width = 0;
    int texture_height = 0;
    GLuint avatar_texture = 0;
    bool ret = LoadTextureFromFile("./images/avatar/12_Penguin.png", &avatar_texture, &texture_width, &texture_height);
    IM_ASSERT(ret);
    GLuint background = 0;
    LoadTextureFromFile("./images/background/Online_discussion.png", &background, &texture_width, &texture_height);
    GLuint chat_0_texture = 0;
    LoadTextureFromFile("./images/icons/chat_0.png", &chat_0_texture, &texture_width, &texture_height);
    GLuint chat_1_texture = 0;
    LoadTextureFromFile("./images/icons/chat_1.png", &chat_1_texture, &texture_width, &texture_height);
    GLuint contact_0_texture = 0;
    LoadTextureFromFile("./images/icons/contact_0.png", &contact_0_texture, &texture_width, &texture_height);
    GLuint contact_1_texture = 0;
    LoadTextureFromFile("./images/icons/contact_1.png", &contact_1_texture, &texture_width, &texture_height);
    GLuint setting_texture = 0;
    LoadTextureFromFile("./images/icons/setting.png", &setting_texture, &texture_width, &texture_height);

    // Main loop
    while (!glfwWindowShouldClose(window)){
        // Poll and handle events (inpu+ts, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // start init widgets

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            ImGui::Begin("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            ImGui::End();
        }

        ImGuiWindowFlags tips_window_flags = 0;
        tips_window_flags |= ImGuiWindowFlags_NoTitleBar;
        tips_window_flags |= ImGuiWindowFlags_NoScrollbar;
        tips_window_flags |= ImGuiWindowFlags_NoMove;
        tips_window_flags |= ImGuiWindowFlags_NoResize;
        tips_window_flags |= ImGuiWindowFlags_NoCollapse;
        tips_window_flags |= ImGuiWindowFlags_NoBackground;
        tips_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::SetNextWindowPos(ImVec2(320, 0), ImGuiCond_Once);
        ImGui::Begin("background", nullptr, tips_window_flags);
        ImGui::Separator();
        ImGui::SetWindowSize(ImVec2((float)display_w - 320, (float)display_h), ImGuiCond_Always);
        ImVec2 background_size = ImGui::GetWindowSize();
        ImGui::SetCursorPos(ImVec2(background_size.x / 2 - 200, background_size.y / 2 - 200));
        ImGui::Image((void*)(intptr_t)background, ImVec2(400, 323));
        ImGui::SetCursorPos(ImVec2(background_size.x / 2 - 110, background_size.y / 2 + 100));
        ImGui::Text("so...Chat with your friends!~");
        ImGui::End();

        ImGui::ShowDemoWindow(&show_demo_window);
   

        // Menu
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGuiWindowFlags M_window_flags = 0;
        M_window_flags |= ImGuiWindowFlags_NoTitleBar;
        M_window_flags |= ImGuiWindowFlags_NoMove;
        M_window_flags |= ImGuiWindowFlags_NoResize;
        M_window_flags |= ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Menu", nullptr, M_window_flags);
        ImGui::SetWindowSize(ImVec2(61, (float)display_h), ImGuiCond_Always);
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.941f, 0.941f, 0.941f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.729f, 0.835f, 0.953f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.62f, 0.78f, 0.961f, 0.8f));
		// personal infomation
        if (ImGui::ImageButton((void*)(intptr_t)avatar_texture, ImVec2(37, 37))) {
            
        }
        ImGui::Separator();
        ImGui::SetCursorPos(ImVec2(8, (float)display_h - 60));
		// settings
        if (ImGui::ImageButton((void*)(intptr_t)setting_texture, ImVec2(37, 37))) {
			
        }
        if (chat_pressed) {
            chat_selected = true;
            contact_selected = false;
        }
        else if(contact_pressed) {
			chat_selected = false;
			contact_selected = true;
		}
        if (chat_selected) {
            ImGui::SetCursorPos(ImVec2(8, 80));
            chat_pressed = ImGui::ImageButton((void*)(intptr_t)chat_1_texture, ImVec2(37, 37));
            ImGui::SetCursorPos(ImVec2(8, 140));
            contact_pressed = ImGui::ImageButton((void*)(intptr_t)contact_0_texture, ImVec2(37, 37));
        }
        else {
            ImGui::SetCursorPos(ImVec2(8, 80));
            chat_pressed = ImGui::ImageButton((void*)(intptr_t)chat_0_texture, ImVec2(37, 37));
            ImGui::SetCursorPos(ImVec2(8, 140));
            contact_pressed = ImGui::ImageButton((void*)(intptr_t)contact_1_texture, ImVec2(37, 37));
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::EndGroup();
        ImGui::End();

        // List
        ImGui::SetNextWindowPos(ImVec2(60, 0), ImGuiCond_Once);
        ImGuiWindowFlags MGL_window_flags = 0;
        MGL_window_flags |= ImGuiWindowFlags_NoTitleBar;
        MGL_window_flags |= ImGuiWindowFlags_NoMove;
        MGL_window_flags |= ImGuiWindowFlags_NoResize;
        MGL_window_flags |= ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Messages and Groups List", nullptr, MGL_window_flags);
        ImGui::SetWindowSize(ImVec2(260, (float)display_h), ImGuiCond_Always);
        ImGui::BeginGroup();

        if (chat_selected) {
            static char search_buf[32] = "";
            ImGui::InputText("##", search_buf, 32);
            ImGui::SameLine();
            ImGui::Button("Search", ImVec2(68, 27));
        }
        else {
            if (Friends_pressed) {
                Friends_selected = true;
                Groups_selected = false;
            }
            else if (Groups_pressed) {
                Friends_selected = false;
                Groups_selected = true;
            }
            if (Friends_selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.629f, 0.98f, 1.0f));
                Friends_pressed = ImGui::Button("Friends", ImVec2(98, 27));
                ImGui::PopStyleColor();
                ImGui::SameLine();
                Groups_pressed = ImGui::Button("Groups", ImVec2(98, 27));
            }
            else {
                Friends_pressed = ImGui::Button("Friends", ImVec2(98, 27));
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.629f, 0.98f, 1.0f));
                Groups_pressed = ImGui::Button("Groups", ImVec2(98, 27));
                ImGui::PopStyleColor();
            }
            ImGui::SameLine();
            ImGui::Button("+", ImVec2(27, 27));
        }
		
        ImGui::EndGroup();
        ImGui::Separator();
        ImGui::Text("Friend are following..");
        const char* text = "Classmates";
        ImGui::CollapsingHeader(text);
        ImGui::End();


        // Chat area
        if (chat_p_open) {
            ImGui::SetNextWindowPos(ImVec2(320, 0), ImGuiCond_Always);
            ImGuiWindowFlags Chat_window_flags = 0;
            Chat_window_flags |= ImGuiWindowFlags_NoMove;
            Chat_window_flags |= ImGuiWindowFlags_NoResize;
            Chat_window_flags |= ImGuiWindowFlags_NoCollapse;
            ImGui::Begin("chat area", &chat_p_open, Chat_window_flags);
            ImGui::SetWindowSize(ImVec2((float)display_w - 320, (float)display_h - 199), ImGuiCond_Always);
            ImGui::End();

            ImGui::SetNextWindowPos(ImVec2(320, (float)display_h - 200), ImGuiCond_Always);
            ImGuiWindowFlags Input_window_flags = 0;
            Input_window_flags |= ImGuiWindowFlags_NoTitleBar;
            Input_window_flags |= ImGuiWindowFlags_NoMove;
            Input_window_flags |= ImGuiWindowFlags_NoResize;
            Input_window_flags |= ImGuiWindowFlags_NoCollapse;
            ImGui::Begin("input area", &chat_p_open, Input_window_flags);
            ImGui::SetWindowSize(ImVec2((float)display_w - 320, 200), ImGuiCond_Always);
            ImGui::End();
        }


        // Rendering
        ImGui::Render();
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
	
    return 0;
}
