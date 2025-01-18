#pragma once
#ifndef CONOS_DEFINE
#define CONOS_DEFINE 1

#include <iostream>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <chrono>
#include <condition_variable>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <map>
#include <commdlg.h>

#define USE_ASSERT true

#if defined(NDEBUG) && (USE_ASSERT == true)
#undef NDEBUG
#endif

#include <cassert>

#ifdef __cpp_lib_experimental_filesystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem::v1;
#elif __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#error "no filesystem support ='("
#endif

#pragma warning(disable : 4996)

#pragma comment(lib, "sfml-audio.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")

#include "resource.h"
#include "Util.h"

using namespace std;

#endif