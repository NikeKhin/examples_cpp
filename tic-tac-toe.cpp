#include <forward_list>
#include <exception>
#include <iostream>
#include <array>
#include <map>

using namespace std;

enum class Marks:char {none=' ',x='X',o='0'};

class win_exception: public exception
{
    Marks mark;
public:
    win_exception(Marks m):mark{m}{}
    virtual const char* what() const throw()
    {
        string str = "Выиграл ";
        str += static_cast<char>(mark);
        return str.c_str();
    }
};


class Field{
    friend class MachinePlayer;
    friend class LivePlayer;
    friend class Game;
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
        if(row < 0 || col < 0 || row > 2 || col > 2)
            return false;
        return cells[row][col] == Marks::none;
    }
    array<Marks,3> operator[](int i){
        return cells[i];
    }
    void out(){
        for (int i = 0; i < cells.size(); i++){
            cout << "+---+---+---+" << endl;
            for (int j = 0; j < cells[0].size(); j++)
                cout << "| " << static_cast<char>(f(i,j)) << " ";
            cout << "|" << endl;
        }
        cout << "+---+---+---+" << endl << endl;
    }
private:
    Marks f(int row, int col){
        return cells[row][col];
    }
};


class Player{
protected:
    Field &field;
    Marks mark;
public:
    Player(Field &f, Marks m): field{f},mark{m}{}
    virtual void move()=0;
};

class MachinePlayer: public Player{
private:
    bool check_win(int i, int j){
        if(field[i][(j+1)%3] == mark && field[i][(j+2)%3] == mark )
            return true;
        if(field[(i+1)%3][j] == mark && field[(i+2)%3][j] == mark )
            return true;
        if( i == j )
        {
            if(field[(i+1)%3][(j+1)%3] == mark && field[(i+2)%3][(j+2)%3] == mark)
                return true;
        }
        else if( i == (3-j-1))
        {
            if(field[(i+1)%3][3-(j+1)%3-1] == mark && field[(i+2)%3][3-(j+2)%3-1] == mark)
                return true;
        }
        return false;
    }
    bool check_loss(int i, int j){
        auto check = [this](int i, int j){
            return field[i][j] != mark && field[i][j] != Marks::none;
        };
        if(check(i,(j+1)%3) && check(i,(j+2)%3) )
            return true;
        if(check((i+1)%3,j) && check((i+2)%3,j) )
            return true;
        if( i == j)
        {
            if(check((i+1)%3,(j+1)%3) && check((i+2)%3,(j+2)%3) )
                return true;
        }
        else if( i == (3-j-1))
        {
            if(check((i+1)%3,3-(j+1)%3-1) && check((i+2)%3,3-(j+2)%3-1) )
                return true;
        }

        return false;
    }

    int score_the_move(int i, int j){
        int move_score = 0;
        // проверка выигрышного хода
        if( check_win(i,j) ){
            move_score = 100;
        }
        // проверка выигрышного хода противника
        else if( check_loss(i,j) ){
            move_score = 99;
        }
        // оценка хода
        else{
            array< array<int,3>, 3> scores{30,20,30,
                                          20,40,20,
                                          30,20,30};
            move_score = scores[i][j];
        }

        return move_score;
    }

public:
    MachinePlayer(Field &f, Marks m): Player(f,m){
    }
    virtual void move(){
        using position = pair<int,int>;
        position best_move;
        int move_score = 0;
        cout << "--- Ход машины ---" << endl;
        // поиск наилучшего хода перебором по всем возможным и оценкой ранга каждого хода
        for (int i = 0; i < field.cells.size(); i++){
            for (int j = 0; j < field.cells[0].size(); j++)
                if ( field.available(i,j) ){
                    position move;
                    move.first = i;
                    move.second = j;
                    int score = score_the_move(i,j);
                    if( score > move_score ){
                        best_move = move;
                        move_score = score;
                    }
                }
        }
        cout << "номер строки (1, 2 or 3) = " << best_move.first+1 << endl;
        cout << "номер столбца (1, 2 or 3) = " << best_move.second+1 << endl;
        // осуществление наилучшего хода
        field.put(best_move.first, best_move.second, mark);
    }
};

class LivePlayer: public Player{
public:
    LivePlayer(Field &f, Marks m): Player(f,m){
    }

    virtual void move(){
        using position = pair<int,int>;
        position move;
        cout << "--- Ваш ход ---" << endl;
        do{
            cout << "введите номер строки  (1, 2 or 3) = ";
            cin >> move.first;
            cout << "введите номер столбца (1, 2 or 3) = ";
            cin >> move.second;
            move.first--;
            move.second--;
        }while( !field.available(move.first, move.second) );
        field.put(move.first, move.second, mark);
    }

};

class Game{
    Field &field;
    forward_list<Player*> players;
    forward_list<Player*>::iterator it;

    bool check_win(int i, int j){
        Marks mark = field[i][j];
        if(field[i][0] == mark && field[i][1] == mark && field[i][2] == mark)
            return true;
        if(field[0][j] == mark && field[1][j] == mark && field[2][j] == mark   )
            return true;
        if( i == j || i == (3-j-1))
        {
            if(field[0][0] == mark && field[1][1] == mark && field[2][2] == mark   )
                return true;
            if(field[0][2] == mark && field[1][1] == mark && field[2][0] == mark   )
                return true;
        }
        return false;
    }

public:
    Game(Field &f): field{f}{
    }
    void add(Player &p){
        players.push_front(&p);
        it=players.begin();
    }
    Player* current_player(){
        forward_list<Player*>::iterator ret = it;
        ++it;
        if( it==players.end() )
            it = players.begin();
        return *ret;
    }
    bool next(){
        bool available = false;
        for (int i = 0; i < field.cells.size(); i++)
            for (int j = 0; j < field.cells[0].size(); j++)
                if ( !field.available(i,j) ){
                    if( check_win(i,j) ){
                        throw win_exception( field.f(i,j) );
                        return false;
                    }
                }
                else{
                    available = true;
                }
        return available;
    }
};

int main()
{
    setlocale(LC_ALL, "");
    Field f;
    MachinePlayer p1(f,Marks::x);
    LivePlayer p2(f,Marks::o);
    Game game(f);

    game.add(p1);
    game.add(p2);

    try{
        do{
            Player *p = game.current_player();
            p->move();
            f.out();
        }while( game.next() );
        cout << "Игра окончена. Ничья!" << endl;
    }
    catch( win_exception &e){
        cout << "Игра окончена. " << e.what() << endl;
    }
    return 0;
}
