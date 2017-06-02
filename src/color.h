#ifndef COLOR_H
#define COLOR_H

#include <string>

namespace AutoTrace {

class Color
{
public:
    Color();
    Color(unsigned char r,
          unsigned char g,
          unsigned char b);
    Color (const std::string string);
    Color (const Color &original);
    ~Color();
    bool equal(const Color *color);
    void set(unsigned char r,
             unsigned char g,
             unsigned char b);
    unsigned char luminance ();
    unsigned char getR() const;
    unsigned char getG() const;
    unsigned char getB() const;

private:
    static unsigned int hctoi (const char &c);
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

}

#endif // COLOR_H
