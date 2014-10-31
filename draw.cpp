#include <iostream>
#include <cmath>

#include "gif.hpp"

using namespace std;

int main()
{

    const int size = 256;

    uint32_t arr[size][size];

    // создание объекта (gif-файла) с заданным именем и размерами
    GifWriter gw("checking.gif",size,size);

    // диагональный градиент красным
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=uint8_t((i+j)/2);
    gw.write(arr);

    // горизонтальный градиент зеленым
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=uint8_t(j) << 8; //RGB - каждому 8 бит, поэтому для зеленого, идущего вторым, сдвиг на 8
    gw.write(arr);

    // вертикальный градиент синим
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=uint8_t(i) << 16; //RGB - для синего, идущего третьим, сдвиг на 16
    gw.write(arr);


    // все белое
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=-1;
    gw.write(arr);

    // синусоида
    for(int i=0;i<size;i++)
        arr[127+int(50*sin(0.1*i))][i]=0;
    gw.write(arr);


    return 0;
}
