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

int main()
{
    // ������ ��������
    Frame frame;

    // �������� ������� (gif-�����) � �������� ������ � ���������
    GifWriter gw("checking.gif",size,size,250);
    /*
        // ������������ ��������
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
            {
                frame[i][j].red( (i+j)/2 );    // ������� �������
                frame[i][j].green( (i+j)/4 );  // ��������� ������� ��������
                frame[i][j].blue( (i+j)/3 );   // � ������
            }
        //gw.write(frame);

        // ��� ����������
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                frame[i][j]=0x00AAFFAA; // 0x00BBGGRR - �������� ������� ������
        //gw.write(frame);

        // ���������
        for(int i=0;i<size;i++)
            frame[size/2+int(size/4*sin(0.1*i))][i] = 0;
        //gw.write(frame);

        // ��������� �������
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0.0, 0.5); // ���������� ������������� �� ������� 127 � ��� 32
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
            {
                frame[i][j].red  ( distribution(generator) );    // ������� �������
                frame[i][j].green( distribution(generator) );    // ������� �������
                frame[i][j].blue ( distribution(generator) );    // ������� �����
            }
        //gw.write(frame);
    */

    // ��������� ����� (http://elementy.ru/posters/fractals)

    for(int cc=-10; cc<=10; cc++)
    {
        cout << "current step=" << cc << endl;
        complex<double> c= {(-0.2 + cc*0.01 ),(0.7 - cc*0.01)};
        for(int i=0; i<size; i++) // �� ��������
        {
            for(int j=0; j<size; j++) // �� ��������
            {
                frame[i][j] = 0;
                complex<double> x = {static_cast<double>(j)/size-0.5, static_cast<double>(i)/size-0.5};
                x *= 3;
                for(int n=0; n<50; n++)
                {
                    x = x*x+c; // �������� x[k+1]=x[k]*x[k]+c;
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


    return 0;
}
