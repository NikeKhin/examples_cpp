#include <iostream>
#include <cmath>

#include "gif.hpp"

using namespace std;

int main()
{

    const int size = 256;

    uint32_t arr[size][size];

    // �������� ������� (gif-�����) � �������� ������ � ���������
    GifWriter gw("checking.gif",size,size);

    // ������������ �������� �������
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=uint8_t((i+j)/2);
    gw.write(arr);

    // �������������� �������� �������
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=uint8_t(j) << 8; //RGB - ������� 8 ���, ������� ��� ��������, ������� ������, ����� �� 8
    gw.write(arr);

    // ������������ �������� �����
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=uint8_t(i) << 16; //RGB - ��� ������, ������� �������, ����� �� 16
    gw.write(arr);


    // ��� �����
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            arr[i][j]=-1;
    gw.write(arr);

    // ���������
    for(int i=0;i<size;i++)
        arr[127+int(50*sin(0.1*i))][i]=0;
    gw.write(arr);


    return 0;
}
