// Partners: Martin + Vincent

#include "ComplexPlane.h"

int main()
{
    // Create a video mode object
    VideoMode vm(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2);

    // Create and open a window for the game
    RenderWindow window(vm, "Mandelbrot Set!!", Style::Default);

    // Create a complex plane
    ComplexPlane plane(VideoMode::getDesktopMode().width/2, VideoMode::getDesktopMode().height/2);

    // Creat a text object with font
    sf::Text text;
    sf::Font font;
    font.loadFromFile("CreamyCoffeeDemo-MV1Vp.ttf");
    text.setFont(font);
    text.setCharacterSize(10);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);

    while (window.isOpen())
    {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                // Quit the game when the window is closed
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    plane.zoomOut();
                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    plane.zoomIn();
                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            if (event.type == sf::Event::MouseMoved)
            {
                plane.setMouseLocation(Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        /*
        ****************************************
        Update
        ****************************************
        */
        plane.updateRender();
        plane.loadText(text);
        /*
        ****************************************
        Draw
        ****************************************
        */
        window.clear();
        window.draw(plane);
        window.draw(text);
        window.display();
    }
}