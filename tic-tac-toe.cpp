#include <iostream>
#include <array>
#include <map>

using namespace std;

enum class Marks:char {none=' ',x='X',o='0'};

class Field{
    friend class Player;
    array< array<Marks,3>, 3> cells{}; // int8_t cells[3][3]={0}
public:
    Field(){
        cells[0].fill( Marks::none);
        cells.fill( cells[0] );
    }
    void put(int row, int col, Marks ch){
        cells[row][col] = ch;
    }
    bool available(int row, int col){
        return cells[row][col] == Marks::none;
    }

    void out(){
        for (int i = 0; i < cells.size(); i++){
            for (int j = 0; j < cells[0].size(); j++)
                cout << f(i,j);
            cout << endl;

        }
        cout << "---" << endl;
    }
private:
    string f(int row, int col){
        switch(cells[row][col])
        {
            case Marks::x:
                return "X";
            case Marks::o:
                return "0";
        }
        return " ";
    }
};

class Player{
    Field &field;
    Marks mark;
public:
    Player(Field &f, Marks m): field{f}, mark{m}{
    }
    void move(){
        using position = pair<int,int>;
        multimap<int, position> moves;
        // поиск наилучшего хода перебором по всем возможным
        for (int i = 0; i < field.cells.size(); i++){
            for (int j = 0; j < field.cells[0].size(); j++)
                if ( field.available(i,j) ){
                    position move;
                    move.first = i;
                    move.second = j;
                    // проверка хода противника
                    moves.insert( pair<int,position>(100,move));
                    field.put(i,j,mark);
                    return;
                }
        }
    }

};


int main()
{
    Field f;
    Player p1(f,Marks::x);
    Player p2(f,Marks::o);

    f.out();

    p1.move();
    p2.move();
    f.out();

    return 0;
}
