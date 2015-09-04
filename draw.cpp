#include <iostream>
#include <cmath>
#include <random>
#include <complex>
#include <typeinfo>

#include "gif.hpp"

using namespace std;

const int size = 512;

class Pixel
{
public:
    void red(uint8_t color)
    {
        px &= 0xFFFFFF00;
        px |= static_cast<uint32_t>(color);
    }
    void green(uint8_t color)
    {
        px &= 0xFFFF00FF;
        px |= static_cast<uint32_t>(color) << 8;
    }
    void blue(uint8_t color)
    {
        px &= 0xFF00FFFF;
        px |= static_cast<uint32_t>(color) << 16;
    }
    Pixel& operator=(uint32_t color)
    {
        px=color;
        return *this;
    }
private:
    uint32_t px {0};
};


class Frame
{
    using FrameRow = Pixel(&)[size];
public:
    FrameRow operator[](int idx)
    {
        return arr[idx];
    }
    operator void*()
    {
        return arr;
    }
private:
    Pixel arr[size][size];
};

class Shape
{
    int x1,y1, x2,y2;
public:
    Shape(int xx1,int yy1,int xx2,int yy2)
    {
        x1 = xx1;
        y1 = yy1;
        x2 = xx2;
        y2 = yy2;
    }
    void paint(Frame &f)
    {
        for(int y=y1; y<=y2; y++) // rows
            for(int x=x1; x<=x2; x++) // cols
            {
                double w = x2-x1;
                double h = y2-y1;
                if( (x-x1)/w + (y-y1)/h < 1.0 )
                    f[y][x].red(255);

            }

    }
};


int main()
{
    setlocale(LC_ALL, "");
    //SetConsoleOutputCP(1251);
    //SetConsoleCP(1251);


    // Pixel 2D frame
    Frame frame;

    // Construct the animated GIF. The last parameter is animation delay
    GifWriter gw("julia.gif", size, size, 250);


    // Julia set (https://en.wikipedia.org/wiki/Julia_set, http://elementy.ru/posters/fractals)
    for(int cc=-10; cc<=10; cc++)
    {
        wcout << L"current step (шаг) =" << cc << endl;
        complex<double> c= {(-0.2 + cc*0.01 ),(0.7 - cc*0.01)};
        for(int i=0; i<size; i++) // rows
        {
            for(int j=0; j<size; j++) // columns
            {
                frame[i][j] = 0;
                complex<double> x = {static_cast<double>(j)/size-0.5, static_cast<double>(i)/size-0.5};
                x *= 3;
                for(int n=0; n<50; n++)
                {
                    x = x*x+c; // iterate x[k+1]=x[k]*x[k]+c;
                    if(std::abs(x) > 10.0 )
                    {
                        frame[i][j].red(n*255/20);
                        frame[i][j].green(n*255/20);
                        frame[i][j].blue(n*255/20);
                        break;
                    }
                }
            }
        }
        gw.write(frame);
    }

    //Newton fractal(https://en.wikipedia.org/wiki/Newton_fractal, http://elementy.ru/posters/fractals/Newton)
    // Множество Жюлиа (http://elementy.ru/posters/fractals)
    for(int cc=-10; cc<=10; cc++)
    {
        wcout << L"current step=" << cc << endl;
        complex<double> c= {(1 + cc*0.01),(0 - cc*0.01)};
        for(int i=0; i<size; i++) // по строчкам
        {
            for(int j=0; j<size; j++) // по столбцам
            {
                frame[i][j] = 0;
                complex<double> x = {static_cast<double>(j)/size-0.5, static_cast<double>(i)/size-0.5};
                complex<double> d1 = {size, 1};
                complex<double> d2 = {size, size};
                complex<double> d3 = {1, size};
                complex<double> o = {1,0};
                complex<double> a = {3,0};
                complex<double> b = {2,0};
                complex<double> x1 = {0, 0};
                complex<double> res1 = {0,0};
                double eps = 0.0000001;
                //double eps2 = 0.0001;
                //double eps3 = 0.1;


                x *= 30;
                for(int n=0; n<50; n++)
                {
                    x1 = x;
                    x = c * (o+b*x*x*x)/(a*x*x);
                    //res = (std::abs(x) - std::abs(x1)) / std::abs(x);
                    if ((std::abs(x) - std::abs(x1)) > eps )
                    {
                        res1 = (d1-x)/x;
                        frame[i][j].red(n*res1.real());
                        res1 = (d2-x)/x;
                        frame[i][j].green(n*res1.real());
                        res1 = (d3-x)/x;
                        frame[i][j].blue(n*res1.real());
                        break;
                    }
                }
            }
        }
        gw.write(frame);
    }



    //https://en.wikipedia.org/wiki/Mandelbrot_set
    //https://en.wikipedia.org/wiki/Collatz_conjecture#Collatz_fractal
    //https://en.wikibooks.org/wiki/Fractals/The_Newton-Raphson_fractal, http://elementy.ru/posters/fractals/Halley
    //http://formulas.ultrafractal.com/


    return 0;
}
