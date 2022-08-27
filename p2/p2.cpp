#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno {
  char codigo [5];
  char nombre [11];
  char apellidos [20];
  char carrera [15];
  int ciclo;
  float mensualidad;
  int nextdel;
};

class FixedRecord {
private:
    string filename;
public:
    FixedRecord(string filename) {
        this->filename = filename;
        ifstream file(filename, ios::binary);
        file.seekg(0, ios::beg);
        if(is_empty(file)) {
            cout << "XD";
            ofstream filex(filename, ios::app | ios::binary);
            int posfdel = -1;
            filex.write((char*) &posfdel, sizeof(int));
        }
    }

    bool is_empty(ifstream& pFile ){
        return pFile.peek() == ifstream::traits_type::eof();
    }

    vector<Alumno> load() {
        ifstream file(filename, ios::binary);
        vector<Alumno> result;
        Alumno a;
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        file.seekg(sizeof(int), ios::beg);
        while(!file.eof()) {
            a = Alumno();
            file.read((char*) &a, sizeof(Alumno));
            result.push_back(a);
        }
        file.close();
        return result;
    }

    void add(Alumno record) {
        record.nextdel = -100;
        ifstream filef(filename, ios::binary);
        try {
            if(!filef.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        int holepos;
        filef.seekg(0, ios::beg);
        filef.read((char*) &holepos, sizeof(int));
        if(holepos == -1) {
            ofstream file(filename, ios::app | ios::binary);
            file.write((char*) &record, sizeof(Alumno));
            file.close();
            filef.close();
        }
        else {
            int newhead;
            ofstream file(filename, ios::in | ios::binary);
            filef.seekg((holepos + 1) * sizeof(Alumno), ios::beg);
            filef.read((char*) &newhead, sizeof(int));
            file.seekp((holepos) * sizeof(Alumno) + sizeof(int), ios::beg);
            file.write((char*) &record, sizeof(Alumno));
            file.seekp(0, ios::beg);
            file.write((char*) &newhead, sizeof(int));
            file.close();
            filef.close();
        }
        
    }

    Alumno readRecord(int pos) {
        ifstream file(filename, ios::binary);
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        Alumno record;
        file.seekg(pos * sizeof(Alumno) + sizeof(int), ios::beg);
        file.read((char*) &record, sizeof(Alumno));
        file.close();
        return record;
    }

    bool deleteRecord(int pos) {
        ifstream filer(filename, ios::binary);
        try {
            if(!filer.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        filer.seekg(0, ios::beg);
        int nextdel;
        filer.read((char*) &nextdel, sizeof(int));
        int nextdelpos;
        filer.seekg((pos+1) * sizeof(Alumno), ios::beg);
        filer.read((char*) &nextdelpos, sizeof(int));
        if(nextdelpos != -100) {
            cout << "Record with pos: " << pos << " has already been deleted.";
            return false;
        }
        filer.close();
        ofstream filew(filename, ios::in | ios::binary);
        filew.seekp((pos+1) * sizeof(Alumno), ios::beg);
        filew.write((char*) &nextdel, sizeof(int));
        filew.seekp(0, ios::beg);
        filew.write((char*) &pos, sizeof(int));
        filew.close();
        filer.seekg(0, ios::beg);
        filer.read((char*) &nextdel, sizeof(int));
        return true;
  /*       cout << nextdel << " xd ";
        int inpos;
        filer.seekg((pos + 1) * sizeof(Alumno), ios::beg);
        filer.read((char*) &inpos, sizeof(int));
        filer.close();
        cout << inpos << endl; */
    }

    void printHeader() {
        int result;
        ifstream file(filename, ios::binary);
        file.seekg(0, ios::beg);
        file.read((char*) &result, sizeof(int));
        cout << result << endl;
        file.close();
    }
};

void printAlumno(Alumno alumno) {
    for(int j = 0; j < 5; j++) {
        cout << alumno.codigo[j];
    }
    cout << " ";
    for(int j = 0; j < 11; j++) {
        cout << alumno.nombre[j];
    }
    cout << " ";
    for(int j = 0; j < 20; j++) {
        cout << alumno.apellidos[j];
    }
    cout << " ";
    for(int j = 0; j < 15; j++) {
        cout << alumno.carrera[j];
    }
    cout << " " << alumno.ciclo << " " << alumno.mensualidad << " " << alumno.nextdel << endl;
}

void printAlumnos(vector<Alumno> alumnos) {
    for(int i = 0; i < alumnos.size() - 1; i++) {
        printAlumno(alumnos[i]);
    }
}

int main() {
    FixedRecord fr("datos2.bin");
    cout << endl;
    Alumno a;
    strcpy(a.codigo, "0008 ");
    strcpy(a.nombre, "Ignacio    ");
    strcpy(a.apellidos, "Rubio Montiel       ");
    strcpy(a.carrera, "Computacion    ");
    a.ciclo = 8;
    a.mensualidad = 4000.05f;
    printAlumnos(fr.load());
    cout << endl;
    fr.add(a);
    fr.printHeader();
    printAlumnos(fr.load());

}


