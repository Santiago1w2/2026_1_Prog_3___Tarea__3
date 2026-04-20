#include <iostream>
#include <map>
#include <list>
#include <string>
#include <functional>
using namespace std;
class Entity {
    int pos_x;
    int pos_y;
    int energia;
    int vida;
    string nombre;
    int nivel;
    string recursos;
    public:
    Entity() {
        pos_x = 0;
        pos_y = 0;
        energia = 100;
        vida = 100;
        nivel = 1;
        nombre = "";
        recursos = "";
    }
    Entity(int x, int y, int Energia, int vida, string nombre, string recursos) {
        this->pos_x = x;
        this->pos_y = y;
        this->energia = Energia;
        this->vida = vida;
        this->nombre = nombre;
        this->recursos = recursos;
        this->nivel = 1;
    }

    int getPos_x() const {
        return pos_x;
    }

    void setPos_x(int x) {
        this->pos_x = x;
    }

    int getPos_y() const {
        return pos_y;
    }

    void setPos_y(int y) {
        this->pos_y = y;
    }

    int getEnergia() const {
        return energia;
    }

    void setEnergia(int energia) {
        if (energia < 0)
            this->energia = 0;
        else this->energia = energia;
    }

    int getVida() const {
        return vida;
    }

    void setVida(int vida) {
        if (vida < 0)
            this->vida = 0;
        else this->vida = vida;
    }

    string getNombre() const {
        return nombre;
    }

    void setNombre(const string &nombre) {
        this->nombre = nombre;
    }

    int getNivel() const {
        return nivel;
    }

    void setNivel(int nivel) {
        this->nivel = nivel;
    }

    string getRecursos() const {
        return recursos;
    }

    void setRecursos(const string &recursos) {
        this->recursos = recursos;
    }
    string status() const {
        return "Nombre =" + nombre + ",\n"+
               "Pos = (" + to_string(pos_x) + "," + to_string(pos_y) + ")" + ",\n"+
               "Vida = " + to_string(vida) + ",\n"+
               "Energia = " + to_string(energia) + ",\n"+
               "Nivel = " + to_string(nivel);
    }

};

using Command = function<void(const list<string>&)>;

class ComandCenter {
    map<string, Command> commands;
    list<string> history;
    map<string, list<pair<string,list<string>>>> macros;

    Entity& entity;
public:
    ComandCenter(Entity& entity):entity(entity) {}

    void registerCommand(const string &name, Command cmd) {
        commands[name] = cmd;
    }

    void execute(const string &name, const list<string>& args) {
        map<string,Command>::iterator it = commands.find(name);
        if (it == commands.end()) {
            cout << "Error el siguiente comando no se encontro: "<< name << endl;
            return;
        }

        string before = entity.status();

        it->second(args);

        string after = entity.status();

        string Str = "";
        list<string>::const_iterator i;
        for (i = args.begin(); i != args.end(); ++i) {
            Str+=*i+" ";
        }

        string record = "Command: " + name + " " + Str + "\n";
        record+= "BEFORE: \n"+ before + "\n";
        record+= "AFTER: \n"+ after + "\n";
        record+= "---------------------";

        history.push_back(record);
    }

    void removeCommand(const string &name) {
        map<string, Command>::iterator i = commands.find(name);

        if (i != commands.end()) {
            commands.erase(i);
            cout << "El siguiente comando fue eliminado: " << name << endl;
        } else {
            cout << "El siguiente comando no existe: " << name << endl;
        }
    }

    void printHistory() {
        list<string>::iterator i;
        for (i = history.begin(); i != history.end(); ++i) {
            cout << *i << endl;
        }
    }

    void registerMacro(const string& name, const list<pair<string,list<string>>>& instrucciones) {
        macros[name] = instrucciones;
    }

    void executeMacro(const string& name) {
        map<string, list<pair<string, list<string>>>>::iterator i = macros.find(name);

        if (i == macros.end()) {
            cout << "La siguiente Macro no fue encontrada: " << name << endl;
            return;
        }
        list<pair<string, list<string>>>::iterator inst;

        for (inst = i->second.begin(); inst != i->second.end(); ++inst) {
            string cmdName = inst->first;
            list<string> args = inst->second;

            map<string, Command>::iterator cmdI = commands.find(cmdName);

            if (cmdI == commands.end()) {
                cout << "El siguiente comando dentro de la Macro no fue encontrada: " << cmdName << endl;
                return;
            }

            execute(cmdName, args);
        }
    }


};
//funcion libre
void FuncDanño(Entity& m, const list<string>& args) {
    if (args.size() != 1) {
        cout << "El daño necesita al menos 1 argumento" << endl;
        return;
    }
    int n = stoi(args.front());
    m.setVida(m.getVida() - n);
}


//functor
class LimitadorHeal {
    Entity& entity;
    int contador;
public:
    LimitadorHeal(Entity& entity):entity(entity) {
        contador = 0;
    }

    void operator()(const list<string>& args) {
        if (args.size() != 1) {
            cout << "El heal necesita al menos 1 argumento"<<endl;
            return;
        }
        if (contador >= 3) {
            cout << "Se alcanzo el limite de heals" << endl;
            return;
        }
        int n = stoi(args.front());
        entity.setVida(entity.getVida() + n);
        contador++;
    }
};


int main() {
    Entity entity;
    entity.setNombre("PLayer1");
    ComandCenter center(entity);


    // función libre
    center.registerCommand("damage", [&](const list<string>& args){
        FuncDanño(entity, args);
    });

    // functor
    LimitadorHeal heal(entity);
    center.registerCommand("heal", heal);

    center.registerCommand("move", [&entity](const list<string>& args) {
        cout << "Move..." << endl;
        cout << "---------------------------" << endl;
        if (args.size() != 2) {
        cout << "move necesita 2 argumentos" << endl;
        return;
        }

        try {
        list<string>::const_iterator it = args.begin();
        int x = stoi(*it++);
        int y = stoi(*it);

        entity.setPos_x(x);
        entity.setPos_y(y);
        } catch (...) {
        cout << "Argumentos invalidos en move" << endl;
        }
        });

    center.registerCommand("status", [&entity](const list<string>& args) {
        cout << "---------------------------" << endl;
        cout << "Register.... " << endl;
        if (!args.empty()) {
        cout << "status no recibe argumentos" << endl;
        return;
        }

        cout << entity.status() << endl;
        });

    center.registerCommand("reset", [&entity](const list<string>& args) {
        cout << "Delete.. " << endl;
        cout << "---------------------------" << endl;
        entity = Entity(); // reinicia todo
        });

    //pruebas
    list<string> args1;
    args1.push_back("10");
    center.execute("damage", args1);

    list<string> args2;
    args2.push_back("5");
    center.execute("heal", args2);

    list<string> args3;
    args3.push_back("3");
    args3.push_back("4");
    center.execute("move", args3);

    list<string> empty;
    center.execute("status", empty);

    //el comando invalido
    center.execute("volar", empty);


    //prueba macro 1
    list<pair<string, list<string>>> macro1;

    list<string> m1_args1;
    m1_args1.push_back("10");
    macro1.push_back(make_pair("heal", m1_args1));

    macro1.push_back(make_pair("status", empty));

    center.registerMacro("heal_and_status", macro1);
    center.executeMacro("heal_and_status");


    //prueba macro 2
    list<pair<string, list<string>>> macro2;

    list<string> m2_args1;
    m2_args1.push_back("2");
    m2_args1.push_back("2");

    macro2.push_back(make_pair("move", m2_args1));
    macro2.push_back(make_pair("status", empty));

    center.registerMacro("move_and_status", macro2);
    center.executeMacro("move_and_status");


    // prueba macro 3
    list<pair<string, list<string>>> macro3;

    macro3.push_back(make_pair("reset", empty));
    macro3.push_back(make_pair("status", empty));

    center.registerMacro("reset_all", macro3);
    center.executeMacro("reset_all");


    // mostrar hisotiral
    cout << "\n===== HISTORIAL =====\n";
    center.printHistory();
}