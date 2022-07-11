#include "./GUI/GUI_setup.h"
#include <iostream>
using namespace std;

int main(int, char**){
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = GL_init();

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(940, 640, "soChat", NULL, NULL);
    if (window == NULL)
        return 1;

    // Set the Context of the created window as the main Context of the current thread
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Set the window icon
    GLFWimage icon;
    int channels = 3;
    icon.pixels = stbi_load("./images/icon.png", &icon.width, &icon.height, &channels, 4);
    glfwSetWindowIcon(window, 1, &icon);

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
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.664f, 0.797f, 0.953f, 1.00f);

    int display_w = 940, display_h = 640;

    // initialize soTalk
    // initialize Messages and Groups List
    bool Messages_selected = true, Groups_selected = false, Messages_pressed = false, Groups_pressed = false;
	// initialize Chat area
    bool chat_p_open = true;

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

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;


            ImGui::Begin("Hello, world!");                       // Create a window called "Hello, world!" and append into it.


            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
		
        

        // Menu
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGuiWindowFlags MGL_window_flags = 0;
        MGL_window_flags |= ImGuiWindowFlags_NoTitleBar;
        MGL_window_flags |= ImGuiWindowFlags_NoMove;
        MGL_window_flags |= ImGuiWindowFlags_NoResize;
        MGL_window_flags |= ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Messages and Groups List", nullptr, MGL_window_flags);
        ImGui::SetWindowSize(ImVec2(280, static_cast<float>(display_h)), ImGuiCond_Always);
        ImGui::BeginGroup();

		if (Messages_pressed) {
            Messages_selected = true;
			Groups_selected = false;
        }
        else if (Groups_pressed) {
            Messages_selected = false;
            Groups_selected = true;
		}
        if (Messages_selected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.59f, 0.98f, 1.0f));
            Messages_pressed = ImGui::Button("Messages", ImVec2(106.5, 35));
            ImGui::PopStyleColor();
            ImGui::SameLine();
            Groups_pressed = ImGui::Button("Groups", ImVec2(106.5, 35));
        }
        else {
            Messages_pressed = ImGui::Button("Messages", ImVec2(106.5, 35));
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.260f, 0.59f, 0.98f, 1.0f));
            Groups_pressed = ImGui::Button("Groups", ImVec2(106.5, 35));
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();
        ImGui::Button("+", ImVec2(35, 35));
        
        ImGui::EndGroup();
        ImGui::Separator();
        ImGui::Text("Friend are following..");
        const char* text = "wow";
        ImGui::CollapsingHeader(text);
        ImGui::End();

        // Chat area
        if (chat_p_open) {
            ImGui::SetNextWindowPos(ImVec2(280, 0), ImGuiCond_Always);
            ImGuiWindowFlags Chat_window_flags = 0;
            //Chat_window_flags |= ImGuiWindowFlags_NoMove;
            //Chat_window_flags |= ImGuiWindowFlags_NoResize;
            Chat_window_flags |= ImGuiWindowFlags_NoCollapse;
            ImGui::Begin("chat area", &chat_p_open, Chat_window_flags);
            ImGui::SetWindowSize(ImVec2(static_cast<float>(display_w) - 280, static_cast<float>(display_h) - 200), ImGuiCond_Always);
            ImGui::End();

            ImGui::SetNextWindowPos(ImVec2(280, static_cast<float>(display_h) - 200), ImGuiCond_Always);
            ImGuiWindowFlags Input_window_flags = 0;
            Input_window_flags |= ImGuiWindowFlags_NoTitleBar;
            //Input_window_flags |= ImGuiWindowFlags_NoMove;
            //Input_window_flags |= ImGuiWindowFlags_NoResize;
            Input_window_flags |= ImGuiWindowFlags_NoCollapse;
            ImGui::Begin("input area", &chat_p_open, Input_window_flags);
            ImGui::SetWindowSize(ImVec2(static_cast<float>(display_w) - 280, 200), ImGuiCond_Always);
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
