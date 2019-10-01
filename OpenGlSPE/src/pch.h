#pragma once
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "vendor\imgui\imgui.h"
#include "vendor\imgui\imgui_impl_opengl3.h"
#include "vendor\imgui\imgui_impl_glfw.h"

#include "Graphics\IndexBuffer.h"
#include "Graphics\VertexBuffer.h"
#include "Graphics\VertexArray.h"
#include "Graphics\VertexBufferLayout.h"
#include "Graphics\Shader.h"
#include "Graphics\View.h"

#include "FrameTimer.h"
#include "Log.h"
