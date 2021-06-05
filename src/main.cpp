/*
 * ANANI TRANSPILER
 *
 * Version: 0.31
 * Transpiles to: c++
 *
 * Note: anani sikeyim faraj
 *
 * This project is licensed under the terms of MIT License.
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <fstream>
using namespace std;

class candidate{
public:
    string name;
    string origin;

    candidate(string, string);
};

string ltrim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
                               not1(ptr_fun<int, int>(isspace))));
    return s;
}

string rtrim(string s) {
    s.erase(find_if(s.rbegin(), s.rend(),
                    not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

string trim(string s) {
    return ltrim(rtrim(s));
}

vector<string> ReadAllLines(string path){
    fstream fileStream;
    fileStream.open(path, ios::in);

    vector<string> lines;

    if (fileStream.is_open()){
        string lc;

        while (getline(fileStream, lc)){
            lines.push_back(lc);
        }

        fileStream.close();
    }

    return lines;
}

bool WriteAllText(string path, string text){
    std::fstream fileStream;
    fileStream.open(path, ios::out);

    if (fileStream.is_open()){
        fileStream << text;
        fileStream.close();
    }

    return true;
}

candidate::candidate(string a, string b) {
    name = a;
    origin = b;
}

int main(int argc, char **argv) {
    string code;
    for(string ln : ReadAllLines(argv[1])){
        code += trim(ln);
    }

    vector<string> splcode;
    string str = "";
    bool ins = false;
    for(int i = 0; i < code.size(); i++){
        char c = code[i];

        if(c == '\"'){
            if(ins){
                str += c;
                ins = false;
                splcode.push_back(str);
                str.clear();
            }
            else{
                ins = true;
                splcode.push_back(str);
                str.clear();
                str += c;
            }
        }
        else{
            str += c;
        }

        if(i == code.size() - 1 && str != ""){
            splcode.push_back(str);
            str.clear();
        }
    }

    string endline = "\n";

    // i% #include <bits/stdc++.h>\nusing namespace std;\n\n
    // m int main()
    // ( {
    // ) }
    // [ (
    // ] )
    // * ;
    // @pf printf

    vector<candidate> cds = {
            candidate("i%", "#include <bits/stdc++.h>" + endline + "using namespace std;" + endline),
            candidate("-m", "int main"),
            candidate("(", "{"),
            candidate(")", "}"),
            candidate("[", "("),
            candidate("]", ")"),
            candidate("*", ";"),
            candidate("@pf", "printf"),
            candidate("@sf", "scanf"),
            candidate("-i.", "int"),
            candidate("-c.", "char"),
            candidate("-d.", "double"),
            candidate("-f.", "float"),
            candidate("@r", "return"),
            candidate("-/f", "for"),
            candidate("-/i", "if"),
            candidate("-/w", "while"),
            candidate("-/s", "switch"),
            candidate("-/c", "case"),
            candidate("-c", "class"),
            candidate("-s", "struct"),
            candidate("@b", "break")
    };

    string cppcode;

    for(string s : splcode){
        if(s[0] != '"' && s.back() != '"'){
            string tmp = s;
            for(candidate cddt : cds){
                boost::replace_all(tmp, cddt.name, cddt.origin);
            }
            cppcode += tmp;
        }
        else{
            cppcode += s;
        }
    }

    WriteAllText("anani.tmp.gen.cpp", cppcode);

    string out = "gen.out";

    if(argc > 2){
        out = argv[2];
    }

    string compiler = "g++";

    string cmd = compiler + " anani.tmp.gen.cpp -o " + out;

    int syscode = std::system(cmd.c_str());

    if(syscode != 0){
        return syscode;
    }
    else{
        cout << ":: Success." << endl;
    }

    return 0;
}
