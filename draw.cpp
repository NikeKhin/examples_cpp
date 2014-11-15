#include <iostream>
#include <cmath>

#include "gif.hpp"

using namespace std;

const int size = 256;

class Frame
{
public:
    uint32_t arr[size][size];
};

int main()
{
    Frame frame;

    // создание объекта (gif-файла) с заданным именем и размерами
    GifWriter gw("checking.gif",size,size);

    // диагональный градиент красным
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            frame.arr[i][j]=uint8_t((i+j)/2);
    gw.write(frame.arr);

    // горизонтальный градиент зеленым
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            frame.arr[i][j]=uint8_t(j) << 8; //RGB - каждому 8 бит, поэтому для зеленого, идущего вторым, сдвиг на 8
    gw.write(frame.arr);

    // вертикальный градиент синим
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            frame.arr[i][j]=uint8_t(i) << 16; //RGB - для синего, идущего третьим, сдвиг на 16
    gw.write(frame.arr);


    // все белое
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            frame.arr[i][j]=-1;
    gw.write(frame.arr);

    // синусоида
    for(int i=0;i<size;i++)
        frame.arr[127+int(50*sin(0.1*i))][i]=0;
    gw.write(frame.arr);


    return 0;
}
