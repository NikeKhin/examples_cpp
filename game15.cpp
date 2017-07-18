#include<iostream>
#include <stdlib.h>
#include <conio.h>
using namespace std;
int field[4][4];
int empty_x, empty_y;

void create_field()
{
	int i;
	bool null_cell[15];
	int	cells[15];
	int rand_num;
	bool ok;
	
	for (i = 0; i < 15; i++)
		null_cell[i] = true;
	for (i = 0; i < 15; i++)
	{
		ok = false;
		while (!ok)
		{
			rand_num = rand() %15 +1;
			if (null_cell[rand_num - 1])
			{
				null_cell[rand_num - 1] =false;
				cells[i] = rand_num;
				ok = true;
			}
		}
	}
	for (int i = 0; i < 15; i++)
	{
  		field[i % 4][i / 4] = cells[i];
  	}
 	field[3][3] = 0;
 	empty_x = 3;
	empty_y = 3;
}
 
void print_field() 
{
	system("cls");
 	for (int i = 0; i < 4; i++)
 	{
 		cout << "|---||---||---||---|" << endl;
  		for (int j = 0; j < 4; j++)
  		{
  			cout << "|" ;
  			cout.width(3);
  			if (field[i][j] != 0)
  				cout << field[i][j];
  			else
  				cout << " ";
  			cout << "|";
  		}
 		cout << endl << "|---||---||---||---|" << endl;
  	
	}
   
}
void move(int dir)
{
	switch (dir)
	{
		case 1:
   		{
   			if (empty_x < 3)
    		{
    			field[empty_x][empty_y] = field[empty_x + 1][empty_y];
    			field[empty_x + 1][empty_y] = 0;
    			empty_x++;
    		}
   		} break;
  		case 2:
   		{
	   		if (empty_y < 3)
	    	{
	    		field[empty_x][empty_y] = field[empty_x][empty_y + 1];
	    		field[empty_x][empty_y + 1] = 0;
	    		empty_y++;
	    	}
   		} break;
  		case 3:
   		{
	   		if (empty_x > 0)
	    	{
			    field[empty_x][empty_y] = field[empty_x - 1][empty_y];
			    field[empty_x - 1][empty_y] = 0;
			    empty_x--;
	    	}
   		} break;
  		case 4:
   		{
   			if (empty_y > 0)
		    {
			    field[empty_x][empty_y] = field[empty_x][empty_y - 1];
			    field[empty_x][empty_y - 1] = 0;
			    empty_y--;
		    }
   		} break;
  	}
 }
bool is_correct()
{
	for (int i = 0; i < 15; i++)
  		if (field[i % 4][i / 4] != i + 1)
   			return false;
 	return true;
}

int main ()
{
	create_field();
	print_field();
	char key;
	while (!is_correct()) 
	{
		key = getch();		
		switch((int)key) //В зависимости от нажатой клавиши (от ее целочисленного кода) двигаем костяшки
		{
			case 56: move(1); break; //Нажата клавиша "Вверх"
			case 52: move(2); break; //Нажата клавиша "Влево"
			case 50: move(3); break; //Нажата клавиша "Вниз"
			case 54: move(4); break; //Нажата клавиша "Вправо"
			case 27: return 0; //Нажата клавиша "Escape"
		}
		print_field();
		cout << (int)key <<endl;
	}
	return 0;
}
