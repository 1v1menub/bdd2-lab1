#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>
#include <iomanip>
#include <limits>

using namespace std;

struct Matricula
{
  string codigo;
  int ciclo;
  float mensualidad;
  string observaciones;
};


class VariableRecord {
private:
    string filename;
    string metadataFilename;
public:
    VariableRecord(string filename, string metadataFilename) {
        this->filename = filename;
        this->metadataFilename = metadataFilename;
    }

    vector<Matricula> load() {
        ifstream file(filename, ios::binary);
        ifstream metaFile(metadataFilename, ios::binary);
        vector<Matricula> result;
        Matricula m;
        try {
            if(!file.is_open() || !metaFile.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
            return;
        }
        file.seekg(sizeof(int), ios::beg);
        while(!file.eof()) {
            int pos = 1;
            m = Matricula();
            int sizetoreadpos;
            int sizetoread;
            metaFile.seekg((sizeof(int) * pos * 2) - 2, ios::beg);
            metaFile.read(reinterpret_cast<char*>(&sizetoreadpos), sizeof(int));
            metaFile.seekg((sizeof(int) * pos * 2) - 1, ios::beg);
            metaFile.read(reinterpret_cast<char*>(&sizetoread), sizeof(int));
            file.seekg(sizetoreadpos, ios::beg);
            file.read((char*) &m, sizeof(sizetoread));
            result.push_back(m);
        }
        file.close();
        return result;
    }

    void add(Matricula record) {
        fstream file(filename, ios::binary | ios::app);
        fstream metaf(metadataFilename, ios::binary | ios::app);
        try {
            if(!file.is_open() || !metaf.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        int position;
        int positionw;
        int sizerw;
        int sizer = record.codigo.size() + record.observaciones.size() + sizeof(int) + sizeof(float) + 3*sizeof(int);
        if(metaf.eof()){
            int posicion = 0;
            metaf.write(reinterpret_cast<char*>(&position), sizeof(int));
            metaf.write(reinterpret_cast<char*>(&sizer), sizeof(int));            
        }
        else {
            metaf.seekg(-sizeof(int), ios::end);
            metaf.read(reinterpret_cast<char*>(&sizerw), sizeof(int));
            metaf.seekg(-sizeof(int)*2, ios::end);
            metaf.read(reinterpret_cast<char*>(&positionw), sizeof(int));
            position = positionw + sizerw;
            metaf.seekp(0, ios::beg);
            metaf.write(reinterpret_cast<char*>(&position), sizeof(int));
            metaf.write(reinterpret_cast<char*>(&sizer), sizeof(int));
        }
        int codesize = record.codigo.size();
        int ciclosize = sizeof(int);
        int mensualidadsize = sizeof(float);
        int obsersize = record.observaciones.size();
        file.write(reinterpret_cast<char*>(&record.codigo), record.codigo.size());
        file.write(reinterpret_cast<char*>(&codesize), sizeof(int));
        file.write(reinterpret_cast<char*>(&record.ciclo), sizeof(int));
        file.write(reinterpret_cast<char*>(&ciclosize), sizeof(int));
        file.write(reinterpret_cast<char*>(&record.mensualidad), sizeof(float));
        file.write(reinterpret_cast<char*>(&mensualidadsize), sizeof(int));
        file.write(reinterpret_cast<char*>(&record.observaciones), record.observaciones.size());
        file.write(reinterpret_cast<char*>(&obsersize), sizeof(int));
        file.close();
        metaf.close();
    }

    Matricula readRecord(int pos) {
        ifstream file(filename, ios::binary);
        ifstream metaFile(metadataFilename, ios::binary);
        try {
            if(!file.is_open() ||!metaFile.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        Matricula record;
        int sizetoreadpos;
        int sizetoread;
        metaFile.seekg((sizeof(int) * pos * 2) - 2, ios::beg);
        metaFile.read(reinterpret_cast<char*>(&sizetoreadpos), sizeof(int));
        metaFile.seekg((sizeof(int) * pos * 2) - 1, ios::beg);
        metaFile.read(reinterpret_cast<char*>(&sizetoread), sizeof(int));
        file.seekg(sizetoreadpos, ios::beg);
        file.read(reinterpret_cast<char*>(&record), sizetoread);
        file.close();
        metaFile.close();
        return record;
    }
};

void printMatricula(Matricula matricula) {
    cout << matricula.codigo << "|" << matricula.ciclo << "|" << matricula.mensualidad << "|" << matricula.observaciones << endl;
}

void printAlumnos(vector<Matricula> matriculas) {
    for(int i = 0; i < matriculas.size(); i++) {
        printMatricula(matriculas[i]);
    }
}

int main() {
    VariableRecord record("matriculas.bin", "metadata.bin");
    Matricula m;
    m.codigo = "123";
    m.ciclo = 1;
    m.mensualidad = 100;
    m.observaciones = "NO asistio";
    record.add(m);
    Matricula m2;
    m2.codigo = "456";
    m2.ciclo = 2;
    m2.mensualidad = 200;
    m2.observaciones = "Asistio";
    record.add(m2);
    Matricula m3;
    m3.codigo = "789";
    m3.ciclo = 3;
    m3.mensualidad = 300;
    m3.observaciones = "Asistio";
    record.add(m3);
}

