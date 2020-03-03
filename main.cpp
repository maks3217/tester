#include <iostream>
#include <windows.h>
#include <list>
#include <iterator>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>
#include <stdlib.h>
#include <sstream>

using namespace std;

bool checkPattern(string fileName, string patternOntheEnd) {

    string patternOntheEndWithTxtEnd = patternOntheEnd + ".txt";

    if(fileName.size() > patternOntheEnd.size())
    {
        if(fileName.substr(fileName.size() - patternOntheEnd.length()) == patternOntheEnd)
            return true;
    }
    if(fileName.size() > patternOntheEndWithTxtEnd.size())
    {
        if(fileName.substr(fileName.size() - patternOntheEndWithTxtEnd.length()) == patternOntheEndWithTxtEnd)
            return true;
    }

   return false;
}

string getValuefromFile(string fileName) {
    string line;
    string result = "";
    const char * constFileName = fileName.c_str();
    ifstream file (constFileName);

    if (file.is_open())
    {
        while ( getline (file,line) )
        {
        result += line + '\n';
        }
        file.close();
    }
    return result;
}

list<string> generateList(char* folderName, string endPattern) {
    string basicPath = "C:/Users/Maks/Desktop/Testy/";
    string path = basicPath + folderName;
    const char * constPath = path.c_str();
    list<string> resultList;
    struct dirent *entry;
    DIR *dir = opendir(constPath);

   if (dir == NULL) {
      cout << "Bad directory path";
   }
   string entryName;
   while ((entry = readdir(dir)) != NULL) {
    entryName = entry->d_name;
    if(checkPattern(entryName, endPattern))
        resultList.push_back(getValuefromFile(path + "/" + entry->d_name));
   }

   closedir(dir);
   return resultList;
}

map<string, string> generateMap(list<string> InputList, list<string> OutList) {
    map<string, string> mapa;
    if(InputList.size() != OutList.size())
        return mapa;
    int mapSize = OutList.size();
    string inputArray[mapSize];
    string outArray[mapSize];
    copy(InputList.begin(), InputList.end(), inputArray);
    copy(OutList.begin(), OutList.end(), outArray);
    for(int i = 0; i < mapSize; i++)
    {
        mapa.insert(pair<string, string>(inputArray[i], outArray[i]));
    }
    return mapa;
}

void showlist(list <string> g)
{
    list <string> :: iterator it;
    for(it = g.begin(); it != g.end(); ++it)
        cout << *it << endl;
    cout << '\n';
}

void buildProgram(string path, string program)
{
    string systemCommand = "g++ " + path + program + ".cpp " + "-o " +path + program;
    const char * constSystemCommand = systemCommand.c_str();
    system(constSystemCommand);
}

string getOutputFromFile(string fileName)
{
    ifstream inFile;
    inFile.open(fileName.c_str()); //open the input file

    stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    string str = strStream.str();
    return str;
}

void clearTmpFile(string fileName) {
    fstream ofs;
    ofs.open(fileName.c_str(), ios::out | ios::trunc);
    ofs.close();
}

int main(int arg, char**argv)
{
    string result;
    string program = argv[2];
    string OutputFileName = "tmpfile.txt";
    string programPath = "C:/Users/Maks/Desktop/Potyczki/";
    string programExePath = programPath + program + " > " +  OutputFileName;
    const char * constProgramExePath = programExePath.c_str();
    buildProgram(programPath, program);
    map<string,string>::iterator it;
    list<string> inputList = generateList(argv[1], "in");
    list<string> outputList = generateList(argv[1], "out");
    map<string,string> inOutMap = generateMap(inputList, outputList);
    cout << "Liczba testow = " << inOutMap.size() << endl;
    FILE *in;
    char buffer[1024];
    string input;
    string output;
    for(it = inOutMap.begin(); it != inOutMap.end(); it++)
    {
        in = popen(constProgramExePath, "w");
        input = it->first;
        strcpy(buffer, input.c_str());
        fwrite(buffer, sizeof(char), sizeof(buffer), in);
        fflush(in);
        Sleep(100);
        output = getOutputFromFile(OutputFileName);
        Sleep(100);
        clearTmpFile(OutputFileName);
        Sleep(100);
        if(output.compare(it->second) == 0)
        {
            cout << "ok" << endl;
        }
        else
        {
            cout << "Bad" << endl;
        }
    }

    return 0;
}

