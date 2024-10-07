#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils/logger.hpp"

struct Map
{
    sf::Image map_color;
    sf::Image map_material_mask;
    unsigned int width = 0;
    unsigned int height = 0;
    bool validated = false;
    // TODO: add calculated hash for security
};

struct MapStorage
{
    Map TEST_MAP;

    MapStorage()
    {
        loadMap(TEST_MAP, "assets/maps/test_map");
    };

private:
    void loadMap(Map &map, const std::string &path)
    {
        Logger::log(Logger::LogType::Info, "loadMap", "Loading: " + path);

        const std::string map_color_path = path + "/map_color.png";
        const std::string map_material_mask_path = path + "/map_material_mask.png";

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

        map.map_color = map_color_temp;
        map.map_material_mask = map_material_mask_temp;

        map.width = map_color_size.x;
        map.height = map_color_size.y;

        map.validated = true;

        Logger::log(Logger::LogType::Info, "loadMap", "Successfully compiled: " + path);
    }
};