#pragma once

#include <config.hpp>

#include <filesystem>

Config ParseConfig(std::filesystem::path config_path);
