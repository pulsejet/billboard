#include "painter.hpp"
#include "scene/event.hpp"

Painter::Painter(sf::RenderWindow * window) {
    _window = window;
    scene = new EventScene();
    scene->create(window);
}

Painter::~Painter() {
    delete scene;
}

void Painter::paint() {
    scene->paint();
}
