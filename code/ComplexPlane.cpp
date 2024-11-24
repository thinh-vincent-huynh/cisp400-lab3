// Partners: Martin + Vincent

#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = Vector2i(pixelWidth, pixelHeight);
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_center = {0,0};
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio};
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}
void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {x, y};
    m_state = State::CALCULATING;
}
void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {x, y};
    m_state = State::CALCULATING;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = ComplexPlane::mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = ComplexPlane::mapPixelToCoords(mousePixel);
}
void ComplexPlane::loadText(Text& text)
{
    stringstream str;
    str << "Mandelbrot Set by Martin Torrefiel and Vincent Huynh" << endl
        << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl
        << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl
        << "Left-click to Zoom in" << endl
        << "Right-click to Zoom out";
    
    text.setString(str.str());
}
void ComplexPlane::updateRender()
{
    if (m_state == State::CALCULATING)
    {
        for (int j = 0; j < m_pixel_size.x; j++)
        {
            for (int i = 0; i < m_pixel_size.y; i++)
            {
                m_vArray[j + i * m_pixel_size.x].position = { (float)j,(float)i };
                Uint8 r, g, b;
                ComplexPlane::iterationsToRGB(ComplexPlane::countIterations(ComplexPlane::mapPixelToCoords(Vector2i(j, i))), r, b, g);
                m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
            }
        }
        m_state = State::DISPLAYING;
    }
}
size_t ComplexPlane::countIterations(Vector2f coord)
{
    complex<double> c(coord.x, coord.y);
    complex<double> z(0.0, 0.0);
    for (size_t i = 0; i < MAX_ITER; i++) {
        z = z * z + c;
        if (abs(z) > ESC_RAD) { return i; }
    }
    return MAX_ITER;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER) {
        r = g = b = 0;
        return;
    }

    float t = static_cast<float>(count) / MAX_ITER;
    float h = 360.0f * t;
    float s = 0.6f;
    float v = 1.0f - t;

    float c = v * s;
    float x = c * (1.0f - fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r1, g1, b1;
    if (h >= 0 && h < 60) {
        r1 = c; g1 = x; b1 = 0;
    } else if (h >= 60 && h < 120) {
        r1 = x; g1 = c; b1 = 0;
    } else if (h >= 120 && h < 180) {
        r1 = 0; g1 = c; b1 = x;
    } else if (h >= 180 && h < 240) {
        r1 = 0; g1 = x; b1 = c;
    } else if (h >= 240 && h < 300) {
        r1 = x; g1 = 0; b1 = c;
    } else {
        r1 = c; g1 = 0; b1 = x;
    }

    r = static_cast<Uint8>((r1 + m) * 255);
    g = static_cast<Uint8>((g1 + m) * 255);
    b = static_cast<Uint8>((b1 + m) * 255);
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float real_min = m_plane_center.x - m_plane_size.x / 2.0f;
    float real_max = m_plane_center.x + m_plane_size.x / 2.0f;

    float imag_min = m_plane_center.y - m_plane_size.y / 2.0f;
    float imag_max = m_plane_center.y + m_plane_size.y / 2.0f;

    float real = ((mousePixel.x - 0) / static_cast<float>(m_pixel_size.x - 0)) * (real_max - real_min) + real_min;

    float imag = ((mousePixel.y - m_pixel_size.y) / static_cast<float>(0 - m_pixel_size.y)) * (imag_max - imag_min) + imag_min;

    return Vector2f(real, imag);
}