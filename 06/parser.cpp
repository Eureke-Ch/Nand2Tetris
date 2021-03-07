#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
using namespace std;

class Parser{
    public:
        fstream file_asm;
        string current_command = "";

        map<int,string> command_map;

        Parser(string file_name){
            file_asm.open(file_name);
            command_map[0] = "A_COMMAND";
            command_map[1] = "C_COMMAND";
            command_map[2] = "L_COMMAND";
        }
        ~Parser(){
            file_asm.close();
        }
        bool hasMoreCommands(){
            if(file_asm.eof()){
                return false;
            }else{
                return true;
            }
        };
        void advance(){
            getline(file_asm, current_command);
            int len = current_command.length();
            int flag = 0;
            int j = 0;
            int i = 0;
            while(i < len-j){
                if(current_command[i] == ' ' && flag == 0){
                    current_command.erase(i,1);
                    ++j;
                }else if(current_command[i] == ' ' && flag == 1){
                    current_command.erase(i,current_command.length()-i);
                    break;
                }
                else{
                    flag = 1;
                    ++i;
                }
            }
        };
        int commandType(){
            if(current_command[0] == '@'){
                return 0;
            }
            else if(current_command[0] == '('){
                return 2;
            }
            return 1;
        };
        string symbol(){
            string ans;
            int l = current_command.length();
            for(int i = 1; i < l-1; ++i){
                ans += current_command[i];
            }
            if(commandType() == 0){
                return ans+current_command[l-1];
            }else if(commandType() == 2){
                return ans;
            }
            return "";
        };
        string dest(){
            if(commandType() == 1){
                string ans;
                for(auto i : current_command){
                    if(i == '='){
                        return ans;
                    }else{
                        ans += i;
                    }
                }
            }
            return "";
        };
        string comp(){
            if(commandType() == 1){
                string ans = "";
                bool flag = 0;
                for(auto i : current_command){
                    if(dest() != ""){
                        if(i == '='){
                            flag = 1;
                        }else if(i == ';'){
                            return ans;
                        }else if(flag == 1){
                            ans += i;
                        }
                    }else{
                        if(i == ';'){
                            return ans;
                        }else{
                            ans += i;
                        }
                    }
                }
                return ans;
            }
            return "";
        };
        string jump(){
            if(commandType() == 1){
                string ans = "";
                bool flag = 0;
                for(auto i : current_command){
                    if(i == ';'){
                        flag = 1;
                    }else if(flag == 1){
                        ans += i;
                    }
                }
                return ans;
            }
            return "";
        }
};
class Code_parser{
    public:
    map<string, string> comp_map;

    map<string, string> jump_map;

    map<string, string> dest_map;

    Code_parser(){
        comp_map["0"] = "0101010";
        comp_map["1"] = "0111111";
        comp_map["-1"] = "0111010";
        comp_map["D"] = "0001100";
        comp_map["A"] = "0110000";
        comp_map["!D"] = "0001101";
        comp_map["!A"] = "0110001";
        comp_map["-D"] = "0001111";
        comp_map["-A"] = "0110011";
        comp_map["D+1"] = "0011111";
        comp_map["A+1"] = "0110111";
        comp_map["D-1"] = "0001110";
        comp_map["A-1"] = "0110010";
        comp_map["D+A"] = "0000010";
        comp_map["D-A"] = "0010011";
        comp_map["A-D"] = "0000111";
        comp_map["D&A"] = "0000000";
        comp_map["D|A"] = "0010101";
        comp_map["M"] = "1110000";
        comp_map["!M"] = "1110001";
        comp_map["-M"] = "1110011";
        comp_map["M+1"] = "1110111";
        comp_map["M-1"] = "1110010";
        comp_map["D+M"] = "1000010";
        comp_map["D-M"] = "1010011";
        comp_map["M-D"] = "1000111";
        comp_map["D&M"] = "1000000";
        comp_map["D|M"] = "1010101";

        jump_map["JGT"] = "001";
        jump_map["JEQ"] = "010";
        jump_map["JGE"] = "011";
        jump_map["JLT"] = "100";
        jump_map["JNE"] = "101";
        jump_map["JLE"] = "110";
        jump_map["JMP"] = "111";

        dest_map["M"] = "001";
        dest_map["D"] = "010";
        dest_map["MD"] = "011";
        dest_map["A"] = "100";
        dest_map["AM"] = "101";
        dest_map["AD"] = "110";
        dest_map["AMD"] = "111";
    };
    string dest(string d){
        if(d == ""){
            return "000";
        }  
        return dest_map[d];
    };
    string comp(string c){
        if(c == ""){
            return "000000";
        }
        return comp_map[c];
    };
    string jump(string j){
        if(j == ""){
            return "000";
        }
        return jump_map[j];
    };
};
class SymbolTable{
    public:
    map<string, int> symbol_map;
    int address_symbol;
    SymbolTable(){
        address_symbol = 15;
        symbol_map["SP"] = 0;
        symbol_map["LCL"] = 1;
        symbol_map["ARG"] = 2;
        symbol_map["THIS"] = 3;
        symbol_map["THAT"] = 4;
        symbol_map["R0"] = 0;
        symbol_map["R1"] = 1;
        symbol_map["R2"] = 2;
        symbol_map["R3"] = 3;
        symbol_map["R4"] = 4;
        symbol_map["R5"] = 5;
        symbol_map["R6"] = 6;
        symbol_map["R7"] = 7;
        symbol_map["R8"] = 8;
        symbol_map["R9"] = 9;
        symbol_map["R10"] = 10;
        symbol_map["R11"] = 11;
        symbol_map["R12"] = 12;
        symbol_map["R13"] = 13;
        symbol_map["R14"] = 14;
        symbol_map["R16"] = 16;
        symbol_map["SCREEN"] = 16384;
        symbol_map["KBD"] = 24576;

    }
    void addEntry(string symbol, int address){
        symbol_map[symbol] = address;
    }
    bool contains(string symbol){
        auto it = symbol_map.find(symbol);
        if(it == symbol_map.end()){
            return false;
        }else{
            return true;
        }
    }
    int GetAddress(string symbol){
        return symbol_map[symbol];
    }
};

int main(){
    string str;
    string file_name_in = "a.asm";

    string file_name_out = "a.hack";
    fstream file_out(file_name_out);

    Parser parser_asm(file_name_in);
    Code_parser hack_parser;
    SymbolTable symbol_parser;
    Parser parser_symbol(file_name_in);
    int PC = 0;
    while(parser_symbol.hasMoreCommands()){
        parser_symbol.advance();
        if(parser_symbol.current_command[0] == '/' || parser_symbol.current_command == ""){
            continue;
        }else{
            if(parser_symbol.commandType() == 2){
                symbol_parser.addEntry(parser_symbol.symbol(), PC);
            }else{
                ++PC;
            }
        }
    }

    while(parser_asm.hasMoreCommands()){
        parser_asm.advance();
        if(parser_asm.current_command[0] == '/' || parser_asm.current_command == ""){
            continue;
        }else{
            if(parser_asm.commandType() == 0){
                char first_char = parser_asm.symbol()[0];
                int num;
                if(first_char <= '9' && first_char >= '0'){
                    num = 0;
                    for(auto i : parser_asm.symbol()){
                        num = num*10 + int(i)-48;
                    }
                }else{
                    if(!symbol_parser.contains(parser_asm.symbol())){
                        symbol_parser.addEntry(parser_asm.symbol(), symbol_parser.address_symbol+1);
                        symbol_parser.address_symbol++;
                    }
                    num = symbol_parser.GetAddress(parser_asm.symbol());
                }
                string ans = "";
                string s = "";
                while(num){
                    s.clear();
                    s += char(num%2 + 48);
                    ans.insert(0,s);
                    num = num/2;
                }
                while(ans.length() < 16){
                    ans.insert(0,"0");
                }
                file_out<<ans<<endl;
            }else if(parser_asm.commandType() == 1){
                string ans = "111";
                string dest = hack_parser.dest(parser_asm.dest());
                string comp = hack_parser.comp(parser_asm.comp());
                string jump = hack_parser.jump(parser_asm.jump());
                ans += comp;
                ans += dest;
                ans += jump;
                file_out<<ans<<endl;
            }
        }
    }
    file_out.close();





    // map<int,string> command_map;
    // command_map[0] = "A_COMMAND";
    // command_map[1] = "C_COMMAND";
    // command_map[2] = "L_COMMAND";

    // parser_asm.advance();
    // cout<<parser_asm.current_command<<endl;
    // cout<<command_map[parser_asm.commandType()]<<endl;
    // cout<<parser_asm.symbol()<<endl;

    // parser_asm.advance();
    // cout<<parser_asm.current_command<<endl;
    // cout<<command_map[parser_asm.commandType()]<<endl;
    // cout<<"dest:  "<<parser_asm.dest()<<endl;
    // cout<<"comp:  "<<parser_asm.comp()<<endl;
    // cout<<"jump:  "<<parser_asm.jump()<<endl;

    // parser_asm.advance();
    // cout<<parser_asm.current_command<<endl;
    // cout<<command_map[parser_asm.commandType()]<<endl;
    // cout<<"dest:  "<<parser_asm.dest()<<endl;
    // cout<<"comp:  "<<parser_asm.comp()<<endl;
    // cout<<"jump:  "<<parser_asm.jump()<<endl;
}