#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"

enum class MapID
{
    Test,
    Beach,

    None
};

struct Map
{
    void load(const std::string &path, MapID mapID)
    {
        Logger::log(Logger::LogType::Info, "loadMap", "Loading: " + path);

        const std::string map_color_path = path + "/map_color.png";
        const std::string map_material_mask_path = path + "/map_material.png";
        const std::string map_background_path = path + "/map_background.png";

        sf::Image map_color_temp;
        sf::Image map_material_mask_temp;

        if (!map_color_temp.loadFromFile(map_color_path))
        {
            Logger::log(Logger::LogType::Error, "loadMap", "Error loading: " + map_color_path + " - Terminating map compilation.");
            return;
        }

        if (!map_material_mask_temp.loadFromFile(map_material_mask_path))
        {
            Logger::log(Logger::LogType::Error, "loadMap", "Error loading: " + map_material_mask_path + " - Terminating map compilation.");
            return;
        }

        sf::Vector2u map_color_size = map_color_temp.getSize();
        sf::Vector2u map_material_mask_size = map_material_mask_temp.getSize();

        if (map_color_size.x != map_material_mask_size.x ||
            map_color_size.y != map_material_mask_size.y)
        {
            Logger::log(Logger::LogType::Error, "loadMap", "Map color and material mask size mismatch! Terminating map compilation.");
            return;
        }

        if (!map_background.loadFromFile(map_background_path))
        {
            Logger::log(Logger::LogType::Warning, "loadMap", "Unable to load: " + map_background_path + " - Continuing map compilation without background image.");
            hasBackground = false;
        }

        this->mapID = mapID;

        map_color = map_color_temp;
        map_material_mask = map_material_mask_temp;

        width = map_color_size.x;
        height = map_color_size.y;

        validated = true;

        Logger::log(Logger::LogType::Info, "loadMap", "Successfully compiled: " + path);
    }

    MapID mapID;
    sf::Image map_color;
    sf::Image map_material_mask;
    sf::Texture map_background;
    unsigned int width = 0;
    unsigned int height = 0;
    bool hasBackground = true;
    bool validated = false;
};

struct MapStorage
{
    Map TEST;
    Map BEACH;

    MapStorage()
    {
        TEST.load("assets/maps/test_map", MapID::Test);
        BEACH.load("assets/maps/beach", MapID::Beach);
    };
};