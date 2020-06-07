#include <iostream>
#include <string>

#include "GL_Core.hpp"

#include "imgui.h"
#include "examples/imgui_impl_glfw.cpp"
#include "examples/imgui_impl_opengl3.cpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "main.h"

// Use nvidia card if available.
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

using namespace glc;

unsigned int images_saved = 0;

void saveImageTexture(Texture& tex) {
	// Save textue to incremented png file
	tex.bind();

	unsigned char* pixels;
	pixels = new unsigned char[4 * tex.getWidth() * tex.getHeight()];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	stbi_flip_vertically_on_write(true);

	std::string filename = "image_" + std::to_string(images_saved) + ".png";
	if (!(stbi_write_png(filename.c_str(), tex.getWidth(), tex.getHeight(), 4, pixels, 4 * tex.getWidth()))){
		std::cout << "Could not write image." << std::endl;
	}
	else {
		images_saved++;
	}

	delete[] pixels;
}

int getPixelFromTexture(Texture &tex, int x, int y) {
	// Return uid value from texure at coords.
	tex.bind();
	
	float* pixels;
	pixels = new float[4 * tex.getWidth() * tex.getHeight()];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);

	int uid = -1;

	if ( 0 <= x && x < tex.getWidth() && 0 <= y && y < tex.getHeight()) {
		y = tex.getHeight() - y;
		uid = pixels[4 * (y * tex.getWidth() + x)];
	}

	delete[] pixels;

	return uid;
}

// Renderer Parameters
struct Parameters {
	float absorbtion = 0.2;
	int bounces = 30;
	float gamma = 1.0;

} params;

struct TextureMap {
	Texture* normal;
	Texture* diffuse;
};

struct GLDeviceInfo {
	char vendor[64];
	char renderer[64];
	GLint work_group_count[3];
};


void Dockspace() {
	bool* p_open = NULL;

	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("GLRT", p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	else
	{
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window Options"))
		{

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
			ImGui::Separator();
			if (ImGui::MenuItem("Close DockSpace", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

GLDeviceInfo getGLDeviceInfo() {

	GLDeviceInfo info;
	
	// Query Device Info
	strcpy(info.vendor,(char*)glGetString(GL_VENDOR));
	strcpy(info.renderer,(char*)glGetString(GL_RENDERER));

	// Query Work Groups
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &info.work_group_count[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &info.work_group_count[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &info.work_group_count[2]);

	return info;
}

void initImGui(Window &window) {

    IMGUI_CHECKVERSION(); ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

}

enum MaterialType {
	LAMBERT = 0,
	METAL = 1,
	DIELECTRIC = 2,
};
struct Material {
	//enum MaterialType type;
	int type;
	float rough;
	float refractive_index;
	glm::vec3 color;
	int textureID;

};

int main(void)
{

	glm::ivec2 tex_size(1024, 1024);
	glm::ivec2 uid_tex_size(1024, 1024);

    Window window = Window("GLRT", 1920, 1080);

    Camera camera = Camera();
    Camera rt_camera = Camera();
    FpsController controller = FpsController(&window, &rt_camera);

	Cube skybox = Cube();

	GLDeviceInfo info = getGLDeviceInfo();

	std::cout << "VENDOR: " << info.vendor << "\nDEVICE: " << info.renderer << std::endl;
	std::cout << "MAX WORK GROUP COUNTS: x: " << info.work_group_count[0] << " y: " << info.work_group_count[1] << " z: " << info.work_group_count[2] << std::endl;

	initImGui(window);


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f,0.2f,0.2f,1.0f);

    Quad quad = Quad();

	Texture uid_texture = Texture(1024, 1024, 3);

	Texture texture = Texture(1024, 1024, 3);
	

	//fb.attachTexture(texture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Shader shader = Shader("shaders/quad_vertex.glsl", "shaders/quad_fragment.glsl");
    //Shader cubemap_shader = Shader("shaders/cubemap_vert.glsl", "shaders/cubemap_frag.glsl");

	ComputeShader computeShader("shaders/compute.glsl");
	


	std::vector<std::string> cube_maps;
	for (auto & i : std::experimental::filesystem::directory_iterator("./assets/cubemaps/")) {
		std::string cb = i.path().stem().u8string();
		bool found = false;
		for (auto & w : cube_maps){
			if (w == cb){
				found = true;
			}
		}
		if (!found)
			cube_maps.push_back(cb);
		 
	}
		 
	std::vector<std::string> texture_paths;
	for (auto & i : std::experimental::filesystem::directory_iterator("./assets/textures/")) {
		std::string cb = i.path().stem().u8string();
		bool found = false;
		for (auto & w : texture_paths) {
			if (w == cb) {
				found = true;
			}
		}
		if (!found)
			texture_paths.push_back(cb);
	}
	/*
	std::vector<TextureMap> texture_maps;
	for (auto & tp : texture_paths) {
		int pos = tp.find("-");
		std::string file_name = tp.substr(0, pos);
		TextureMap tm;
		Texture diffuse = Texture("./assets/textures/" + tp + "/" + file_name + "_Color.jpg");
		Texture normal = Texture("./assets/textures/" + tp + "/" + file_name + "_Normal.jpg");
		tm.diffuse = &diffuse;
		tm.normal = &normal;
		texture_maps.push_back(tm);
	}
	*/

	std::string selected_cubemap = "LancellottiChapel";

    glm::mat4 model = glm::mat4(1.0f);

    double last = window.getTime();

	//std::string tex_folder = "Tiles012_2K";
	//std::string tex_folder = "Rocks012_2K";
	//std::string tex_folder = "Tiles060_2K";
	//std::string tex_folder = "Tiles031_2K";
	//std::string tex_folder = "SurfaceImperfections013_2K";
	//std::string tex_folder = "RoofingTiles002_2K";
	//std::string tex_folder = "Metal003_2K";
	std::string tex_folder = "Tiles071_2K";
	//std::string tex_folder = "Chip004_2K";
	//std::string tex_folder = "Metal007_2K";
	//std::string tex_folder = "Rock030_4K";
	//std::string tex_folder = "Rock030_4K";

	Texture rock_normal = Texture("assets/textures/" + tex_folder + "-JPG/" +  tex_folder + "_Normal.jpg");
	Texture rock_color = Texture("assets/textures/" + tex_folder + "-JPG/" +  tex_folder + "_Color.jpg");
	//Texture rock_color = Texture("assets/Globe.jpg");
	Texture rock_displacement = Texture("assets/textures/" + tex_folder + "-JPG/" +  tex_folder + "_Displacement.jpg");


	CubeMap cubemap = CubeMap("assets/cubemaps/" + selected_cubemap);
	cubemap.bind();
	
	Material b1,b2,b3,b4,p1;
	b1.type = METAL;
	b1.color = glm::vec3(1.0, 1.0, 1.0);
	b1.rough = 1.0;
	b1.refractive_index = 1.0;
	b1.textureID = 0;

	b2.type = METAL;
	b2.color = glm::vec3(1.0, 1.0, 1.0);
	b2.rough = 0.0;
	b2.refractive_index = 1.0;
	b2.textureID = -1;

	b3.type = METAL;
	b3.color = glm::vec3(1.0, 0.76, 0.14);
	b3.rough = 0.3;
	b3.refractive_index = 1.0;
	b3.textureID = -1;

	b4.type = METAL;
	b4.color = glm::vec3(1.0, 1.0, 1.0);
	b4.rough = 0.04;
	b4.refractive_index = 1.0;
	b4.textureID = -1;

	p1.type = METAL;
	p1.color = glm::vec3(1.0, 1.0, 1.0);
	p1.rough = 0.0;
	p1.refractive_index = 1.0;
	p1.textureID = 0;

	std::vector<Material*> materials;
	materials.push_back(&b1);
	materials.push_back(&b2);
	materials.push_back(&b3);
	materials.push_back(&b4);
	materials.push_back(&p1);

	int selected_material = 0;

	int iterations = 0;
    while (!window.shouldClose())
    {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        camera.UpdateProjection(90.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1, 100.0);
        double time = window.getTime();
        double delta_time = time - last;
        last = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Dockspace();
		{
			{
				ImGui::Begin("UID Buffer");
				glm::ivec2 window_size = glm::ivec2(ImGui::GetWindowSize().x * 1, ImGui::GetWindowSize().y * 0.96);

				if (window_size != uid_tex_size) {
					//iterations = 0;
				}
				ImGui::Image((void*)(intptr_t)uid_texture.id, ImVec2(window_size.x, window_size.y), ImVec2(0, 1), ImVec2(1, 0));
				ImGui::End();
			}
			{
				ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar);
				ImGuiIO& io = ImGui::GetIO();
				ImVec2 cursor_pos = ImGui::GetCursorScreenPos();


				glm::ivec2 window_size = glm::ivec2(ImGui::GetWindowSize().x * 1, ImGui::GetWindowSize().y * 0.96);

				if (window_size != tex_size) {
					texture.resize(window_size.x, window_size.y);
					tex_size = window_size;
					uid_tex_size = window_size;
					uid_texture.resize(window_size.x, window_size.y);
					iterations = 0;
				}

				ImGui::Image((void*)(intptr_t)texture.id, ImVec2(window_size.x, window_size.y), ImVec2(0, 1), ImVec2(1, 0));

				if (ImGui::IsMouseClicked(0)) {

					int mouseX = (int)io.MousePos.x - cursor_pos.x;
					int mouseY = (int)io.MousePos.y - cursor_pos.y;

					int uid = getPixelFromTexture(uid_texture, mouseX, mouseY);
					if (uid > -1) {
						selected_material = uid;
					}

				if (ImGui::IsMousePosValid())
					ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
				if (ImGui::Button("Save")) {


					}
				}
				ImGui::End();
			}
			{
				ImGui::Begin("Help");
				ImGui::TextWrapped("Viewport Controls: (holding left alt)\nw,a,s,d for movement\nmouse for camera\nclick objects to select them");
				if (ImGui::Button("save png")) {
					saveImageTexture(texture);
				}
				if (ImGui::Button("reload_shader")) {
					computeShader.reload();
					iterations = 0;
				}
				ImGui::End();
			}
		}

		{
			computeShader.bind();
			computeShader.setVec3f("camera.position", rt_camera.position);
			computeShader.setVec3f("camera.front", rt_camera.front);
			computeShader.setVec3f("camera.right", rt_camera.right);
			computeShader.setMat4f("camera.view", rt_camera.GetViewMatrix());
			computeShader.setInt("iterations", iterations);
			
			// Set Material Uniforms
			for (int i = 0; i < materials.size(); i++) {
				Material *mat = materials[i];
				std::string mat_name = "material_" + std::to_string(i + 1);
				computeShader.setInt(mat_name + ".type", mat->type);
				computeShader.setVec3f(mat_name + ".color", mat->color);
				computeShader.setFloat(mat_name + ".rough", mat->rough);
				computeShader.setFloat(mat_name + ".refractive_index", mat->refractive_index);
				computeShader.setInt(mat_name + ".type", mat->type);
				computeShader.setInt(mat_name + ".textureID", mat->textureID);
				computeShader.setInt(mat_name + ".mat_id", i);

			}


			computeShader.setFloat("time", window.getTime());
			computeShader.setFloat("time", window.getTime());
			computeShader.setFloat("refl", 1.0 - params.absorbtion);
			computeShader.setFloat("gamma", params.gamma);
			computeShader.setInt("bounces", params.bounces);
	
			uid_texture.bind(1);
			texture.bind(0);

			glActiveTexture(GL_TEXTURE0);
			cubemap.bind();
		
			glActiveTexture(GL_TEXTURE1);
			rock_color.bind();
			glActiveTexture(GL_TEXTURE2);
			rock_normal.bind();

			computeShader.setInt("texture_1_diffuse", 1);
			computeShader.setInt("texture_1_normal", 2);
			computeShader.setInt("cubemap", 0);

			// Run compute shader.
			glDispatchCompute((GLuint)texture.getWidth(), texture.getHeight(), 1);

		}

		// Gaurantee texture is not accessed until compute shader returns.
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);


#ifdef _DEBUG
		ImGui::ShowDemoWindow();
#endif

		{
			ImGui::Begin("Renderer Parameters");
			if (ImGui::SliderFloat("gamma", &params.gamma, 0.0001, 5.0, "%.3f", 1.0))
				iterations = 0;
			if (ImGui::SliderFloat("absorbtion", &params.absorbtion, 0.0, 1.0, "%.3f"))
				iterations = 0;
			if (ImGui::SliderInt("bounces", &params.bounces, 0, 60))
				iterations = 0;
			ImGui::LabelText("samples", "%d", iterations);

			if (ImGui::BeginCombo("Environment", selected_cubemap.c_str(), 0))
			{
				for (int n = 0; n < cube_maps.size(); n++) {
					bool is_selected = (selected_cubemap == cube_maps[n]);
					if (ImGui::Selectable(cube_maps[n].c_str(), is_selected)) {
						selected_cubemap = cube_maps[n];
						cubemap.LoadFromString("assets/cubemaps/" + selected_cubemap);
						iterations = 0;
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();

				}

				ImGui::EndCombo();
			}

			ImGui::End();
		}

		{
			ImGui::Begin("Material Editor");
			if (ImGui::SliderInt("Selected Material", &selected_material, 0, materials.size() - 1, "%d"))
				iterations = 0;
			Material* m= materials[selected_material];
			if (ImGui::SliderInt("TextureID", &m->textureID, -1, 1, "%d"))
				iterations = 0;
			if (ImGui::Combo("Type", &m->type, "lambert\0metal\0dielectric\0"))
				iterations = 0;
			if (ImGui::ColorPicker4("Albedo", (float*)&m->color, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview))
				iterations = 0;
			if (ImGui::SliderFloat("roughness", &m->rough, 0.0, 1.0, "%.3f", 1.0f))
				iterations = 0;
			if (ImGui::SliderFloat("refractive index", &m->refractive_index, 0.0, 5.0, "%.3f", 1.0f))
				iterations = 0;
			ImGui::End();
		}

		// Render ui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO(); 

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}ImGui::UpdatePlatformWindows();


		if (controller.HandleInput(delta_time)) {
			iterations = 0;
		}
		else {
			iterations += 1;
		}
        camera.Update();
        rt_camera.Update();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}