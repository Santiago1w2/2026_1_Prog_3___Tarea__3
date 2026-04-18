#include <iostream>
using namespace std;
class Entity {
    int pos_x;
    int pos_y;
    int Energia;
    int vida;
    string nombre;
    int nivel;
    string recursos;
    public:
    Entity() {};
    Entity(int x, int y, int Energia, int vida, string nombre, string recursos) {
        this->pos_x = x;
        this->pos_y = y;
        this->Energia = Energia;
        this->vida = vida;
        this->nombre = nombre;
        this->recursos = recursos;
    }

    int GetPos_x() const {
        return pos_x;
    }

    void SetPos_x(int x) {
        this->pos_x = x;
    }

    int GetPos_Y() const {
        return pos_y;
    }

    void SetPos_y(int y) {
        this->pos_y = y;
    }

    int GetEnergia() const {
        return Energia;
    }

    void SetEnergia(int energia) {
        Energia = energia;
    }

    int GetVida() const {
        return vida;
    }

    void SetVida(int vida) {
        this->vida = vida;
    }

    string GetNombre() const {
        return nombre;
    }

    void SetNombre(const string &nombre) {
        this->nombre = nombre;
    }

    int GetNivel() const {
        return nivel;
    }

    void SetNivel(int nivel) {
        this->nivel = nivel;
    }

    string GetRecursos() const {
        return recursos;
    }

    void SetRecursos(const string &recursos) {
        this->recursos = recursos;
    }
};