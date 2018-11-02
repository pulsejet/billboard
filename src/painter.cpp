#include "painter.hpp"
#include "scene/event.hpp"

Painter::Painter(Config * config, sf::RenderWindow * window) {
    _window = window;
    cfg = config;
    scene = new EventScene();
    scene->create(cfg, window);
}

Painter::~Painter() {
    delete scene;
}

void Painter::paint() {
    scene->paint();
}
