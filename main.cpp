#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <exception>
#include <stack>
#include <map>
enum Type {
    Lex_null,
    Lex_FIN,
    Number,
    ConstStr,
    ID,
    Less,
    LessOrEqual,
    Great,
    GreatOrEqual,
    Assign,
    NotEqual,
    LeftParenthesis,
    RightParenthesis,
    Colon,
    Semicolon,
    Equal,
    Add,
    Sub,
    Mul,
    Div,
    Dot,
    Coma,
    keyword_not,
    keyword_goto,
    keyword_program,
    keyword_boolean,
    keyword_var,
    keyword_and,
    keyword_do,
    keyword_else,
    keyword_for,
    keyword_if,
    keyword_or,
    keyword_repeat,
    keyword_then,
    keyword_to,
    keyword_until,
    keyword_while,
    keyword_const,
    keyword_end,
    keyword_false,
    keyword_true,
    keyword_integer,
    keyword_string,
    keyword_read,
    keyword_write,
    keyword_begin
};
const std::unordered_map<char, Type> kSingleSeparators{
        {'+', Add},
        {'-', Sub},
        {'*', Mul},
        {'/', Div},
        {'(', LeftParenthesis},
        {')', RightParenthesis},
        {':', Colon},
        {'.', Dot},
        {';',Semicolon},
        {'=',Equal},
        {'<',Great},
        {'>',Less},
        {',',Coma}
};
const std::unordered_map<std::string, Type> Keywords{
        {"program",keyword_program},
        {"boolean",keyword_boolean},
        {"var",keyword_var},
        {"and",keyword_and},
        {"do",keyword_do},
        {"else",keyword_else},
        {"for",keyword_for},
        {"if",keyword_if},
        {"or",keyword_or},
        {"repeat",keyword_repeat},
        {"then", keyword_then},
        {"to",  keyword_to},
        {"until",  keyword_until},
        {"while", keyword_while},
        {"const", keyword_const},
        {"end",    keyword_end},
        {"false",  keyword_false},
        {"true",  keyword_true},
        {"integer",   keyword_integer},
        {"string",   keyword_string},
        {"not", keyword_not},
        {"goto", keyword_goto},
        {"read", keyword_read},
        {"write", keyword_write},
        {"begin",keyword_begin}
};
const std::unordered_map<std::string, Type> TwoSymbolSep{

        {":=",Assign},
        {"<>",NotEqual},
        {"<=",GreatOrEqual},
        {">=",LessOrEqual}

};
class Lexeme {
private:

    bool boolean_value;
    Type type;
    std::string value;
public:
    Lexeme(){
        boolean_value= false;
        type=Lex_null;
        value.clear();
    }
    explicit Lexeme(Type curr_type){
        boolean_value= true;
        type=curr_type;
        value.clear();
    }
    Lexeme(Type curr_type,char b){
        boolean_value= true;
        type=curr_type;
        value.push_back(b);
    }
    Lexeme(Type curr_type,const std::string& curr_val){
        boolean_value= true;
        type=curr_type;
        value=curr_val;
    }
    Type get_type() const{
        return type;
    }
    std::string get_value(){
        return value;
    }
    friend std::ostream& operator<< (std::ostream& s, const Lexeme& lex){
        s << lex.type << ' ' << lex.value;
        return s;
    }
};


bool IsLetter(char a){
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z') || (a=='_');
}
bool IsNumber(char a){
    return (a>='0' && a <= '9');
}
bool IsASCII(char c) {
    return c >= 0 && c <= 127;
}

std::vector<Lexeme> lexical_analysis (std::string& file) {

    std::ifstream input;
    input.open(file);//open file
    std::vector<Lexeme> All_Lexeme;

    char a;
    enum states {START_STATE, IDENT, NUMB, COMMENT, STATE_BEFORE_2SYMB_SEP, STRING_STATE};
    states current_state = START_STATE;
    int current_number_string=1;
    int current_number_symbol=1;
    std::string buf;
    while(input.get(a) || input.eof()){
        if (input.eof()){
            a=' ';
        }
        if (!IsASCII(a)){
            std::stringstream ss;
            ss << "String "<< current_number_string << " Symbol " << current_number_symbol
            <<" Expected ASCII, but has " << std::string(1, a) << std::endl;
            throw std::runtime_error(ss.str());
        }
        if (a=='\n'){
            current_number_string++;
            current_number_symbol=0;
        }
        switch (current_state){
            case START_STATE:{

                if (input.eof()){
                    return All_Lexeme;
                }else if (IsLetter(a)){
                    buf.push_back(a);
                    current_state=IDENT;
                }else if (IsNumber(a)){
                    buf.push_back(a);
                    current_state=NUMB;
                }else if (a=='{'){
                    current_state=COMMENT;
                }else if (a=='>' || a=='<' || a==':' ){
                    buf.push_back(a);
                    current_state=STATE_BEFORE_2SYMB_SEP;
                }else if (a==' ' || a =='\t' || a == '\r' || a == '\n'){

                }else if(a == '\''){
                    buf.push_back(a);

                    current_state=STRING_STATE;
                }else {
                    auto it = kSingleSeparators.find(a);
                    if (it != kSingleSeparators.end()) {
                        Lexeme lex(it->second,it->first);
                        All_Lexeme.push_back(lex);
                    }else{
                        std::stringstream ss;
                        ss << "String "<< current_number_string << " Symbol " << current_number_symbol << ' '
                        << std::string(1, a)<< std::endl;
                        throw std::runtime_error(ss.str());
                    }
                }
                break;
            }
            case IDENT:{
                if (IsLetter(a) || IsNumber(a)){
                    buf.push_back(a);
                }else{
                    input.unget();
                    auto it = Keywords.find(buf);
                    current_state=START_STATE;
                    if (it != Keywords.end()){
                        Lexeme lex(it->second, it->first);
                        All_Lexeme.push_back(lex);
                        buf.clear();
                    }else{
                        Lexeme lex(ID,buf);
                        buf.clear();
                        All_Lexeme.push_back(lex);
                    }

                }
                break;
            }
            case NUMB:{
                if (IsNumber(a)){
                    buf+=a;
                }else{
                    input.unget();
                    Lexeme lex(Number,buf);
                    All_Lexeme.push_back(lex);
                    buf.clear();
                    current_state=START_STATE;
                }
                break;

            }
            case COMMENT:{

                if (input.eof()){
                    std::stringstream ss;
                    ss << "String "<< current_number_string << " Symbol " << current_number_symbol
                       <<" Unclosed } " << std::endl;
                    throw std::runtime_error(ss.str());
                }

                if (a=='}'){
                    current_state=START_STATE;
                }
                break;

            }

            case STATE_BEFORE_2SYMB_SEP:{
                current_state=START_STATE;
                if (input.eof()) {
                    char last_char = buf[0];
                    auto it = kSingleSeparators.find(last_char);
                    Lexeme lex(it->second,it->first);
                    All_Lexeme.push_back(lex);
                    input.unget();
                    buf.clear();
                    return All_Lexeme;
                }else{
                    buf.push_back(a);
                    auto it = TwoSymbolSep.find(buf);
                    if (it != Keywords.end()){
                        Lexeme lex(it->second,it->first);
                        All_Lexeme.push_back(lex);
                        buf.clear();
                    }else{
                        input.unget();
                        char last_char = buf[0];
                        auto it_1 = kSingleSeparators.find(last_char);
                        Lexeme lex(it_1->second,it_1->first);
                        All_Lexeme.push_back(lex);
                        buf.clear();
                    }
                }

                break;

            }
            case STRING_STATE:{
                if (input.eof()){
                    std::stringstream ss;
                    ss << "String "<< current_number_string << " Symbol " << current_number_symbol
                       <<" Unclosed ' " << std::endl;
                    throw std::runtime_error(ss.str());
                }
                if (a!='\''){
                    buf.push_back(a);
                }else{

                    buf.push_back(a);
                    current_state=START_STATE;
                    Lexeme lex(ConstStr,buf);
                    All_Lexeme.push_back(lex);
                    buf.clear();
                }
                break;

            }
        }

        current_number_symbol++;
    }

    return All_Lexeme;
}


class Parser{
private:
    Lexeme curr_lexeme;
    Type curr_type;
    std::string curr_val_lexeme;
    std::stack <std::string> st_names_val;
    std::stack <Type> st_types_val;
    int index_curr_lexeme;
    std::vector<Lexeme> All_Lexeme;
    void check_is_bool();
    void check_ID(std::string&);
    void check_not();
    void check_Operation();
    void check_eq_two_last_types();
    void P();
    void D1();
    void D ();
    void E();
    void E1();
    void T();
    void S();
    void B();
    void F();
    void Get_lex () {
        index_curr_lexeme++;
        curr_lexeme = All_Lexeme[index_curr_lexeme];
        curr_type = curr_lexeme.get_type();
        curr_val_lexeme = curr_lexeme.get_value();
    }

    class Variable{
    private:
        std::string value;
        Type type;
        bool flag_value;
    public:
        explicit Variable(Type t){
            flag_value = false;
            type=t;
        }
        void PutValue(std::string& val){
            value = val;
            flag_value=true;
        }
        Type Get_Type() const {
            return type;
        };
        bool has_declare() const {
            return flag_value;
        };
        std::string Get_value() const{
            return value;
        }
        friend std::ostream& operator<< (std::ostream& s, const Variable& var){
            s << var.flag_value << ' ' << var.type << ' ' << var.value;
            return s;
        }

    };

public:
    std::map<std::string,Variable> TableID;
    explicit Parser(std::vector<Lexeme>& All_Lex){
        curr_type=Lex_null;
        All_Lexeme=All_Lex;
        index_curr_lexeme=-1;
    }
    std::vector<Lexeme> Poliz;
    void analyze();
};
void Parser::analyze() {
    Parser::Get_lex();
    Parser::P();
    if (curr_type!=Lex_FIN){
        std::stringstream ss;
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    std::cout << "Success" << std::endl;

}
void Parser::P() {//P-> program ID; D1 B .
    std::stringstream ss;
    if (curr_type == keyword_program)
        Get_lex ();
    else {
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    if (curr_type == ID){
        Get_lex ();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    if (curr_type == Semicolon){
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    D1();
    B ();
    if (curr_type == Dot){
        Get_lex ();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
}
void Parser::D1() {//D1-> e | var D;{D;}
    std::stringstream ss;
    if (curr_type == keyword_var){
        Get_lex ();
        while(curr_type == ID){
            D();
            if (curr_type == Semicolon){
                Get_lex();
            }else{
                ss << "Incorrect lexeme " << curr_lexeme;
                throw std::runtime_error(ss.str());
            }
        }
    }
}
void Parser::D(){// D-> ID{,ID}:[int|bool]
    std::stringstream ss;
    if (curr_type == ID){
        st_names_val.push(curr_val_lexeme);
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    while(curr_type == Coma){
        Get_lex();
        if (curr_type == ID){
            st_names_val.push(curr_val_lexeme);
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }
    if (curr_type == Colon){
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    if (curr_type == keyword_integer || curr_type == keyword_boolean){
        while (!st_names_val.empty()){
            std::string str = st_names_val.top();
            st_names_val.pop();
            Variable var(curr_type);
            if (TableID.count(str)!=0){
                ss << "Double initialization " << str;
                throw std::runtime_error(ss.str());
            }
            TableID.insert(std::pair<std::string,Variable>(str,var));
        }
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
}
void Parser::B() { //-> begin e|S;{S;} end
    std::stringstream ss;
    if (curr_type == keyword_begin){
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    while(curr_type != keyword_end){
        S();
        if (curr_type == Semicolon){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }
    Get_lex();
}
void Parser::S() {// S-> ID <check_ID> := E <check eq last 2 types>| if E <check_is_bool> then S else S |
    // if E<check_is_bool> then S |while E<check_is_bool> do S | B | read(ID<check_in_table>) | write(E <pop>|Str|e) | e
    std::stringstream ss;
    if (curr_type == ID){
        check_ID(curr_val_lexeme);
        Get_lex();
        if (curr_type == Assign){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        E();
        check_eq_two_last_types();
    }else if (curr_type == keyword_if){
            Get_lex();
            E();
            check_is_bool();
            if (curr_type == keyword_then){
                Get_lex();
            }else{
                ss << "Incorrect lexeme " << curr_lexeme;
                throw std::runtime_error(ss.str());
            }
            S();
            if (curr_type == keyword_else){
                Get_lex();
                S();
            }else if (curr_type == Semicolon){

            }else{
                ss << "Incorrect lexeme " << curr_lexeme;
                throw std::runtime_error(ss.str());
            }
        }
    else if (curr_type == keyword_while){
        Get_lex();
        E();
        check_is_bool();
        if (curr_type == keyword_do){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        S();
    }
    else if (curr_type == keyword_begin){
        B();
    }
    else if (curr_type == keyword_read){
        Get_lex();
        if (curr_type == LeftParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        if (curr_type == ID){
            if (TableID.count(curr_val_lexeme)==0){
                ss << "Variable not declared " << curr_val_lexeme;
                throw std::runtime_error(ss.str());
            }
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        if (curr_type == RightParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }
    else if (curr_type == keyword_write){
        Get_lex();
        if (curr_type == LeftParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        if (curr_type == ConstStr){
            Get_lex();
        }else if (curr_type == RightParenthesis){

        }else{
            E();
            st_types_val.pop();
        }
        if (curr_type == RightParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }
}
bool Comparison(Type type){
    return (type == Equal || type == NotEqual || type == Less || type == LessOrEqual || type == Great || type == GreatOrEqual);
}
void Parser::E() { // E-> E1 [ = | < | > | >= | <= | <>] <push op> E1 <check op> | E1
    E1();
    if (Comparison(curr_type)){
        st_types_val.push(curr_type);
        Get_lex();
        E1();
        check_Operation();
    }
}
void Parser::E1() {// E1 -> T { [+|-|or] <push op> T <check op>>}
    T();
    while(curr_type == Add || curr_type == Sub || curr_type == keyword_or){
        st_types_val.push(curr_type);
        Get_lex();
        T();
        check_Operation();
    }
}
void Parser::T() { // T-> F{[ * | / | and] <push op> F <check op>}
    F();
    while(curr_type == Mul || curr_type == Div || curr_type == keyword_and){
        st_types_val.push(curr_type);
        Get_lex();
        F();
        check_Operation();
    }
}
void Parser::F() {//ID <check ID>| Numb <push int>| true <push bool> | false <push bool>| not F  <check not>| (E)
    if (curr_type == ID){
        check_ID(curr_val_lexeme);
        Get_lex();
    }else if(curr_type == Number){
        st_types_val.push(keyword_integer);
        Get_lex();
    }else if(curr_type == keyword_true){
        st_types_val.push(keyword_boolean);
        Get_lex();
    }else if(curr_type == keyword_false){
        st_types_val.push(keyword_boolean);
        Get_lex();
    }else if(curr_type == keyword_not){
        Get_lex();
        F();
        check_not();
    }else if (curr_type == LeftParenthesis){
        Get_lex();
        E();
        if (curr_type == RightParenthesis){
            Get_lex();
        }else{
            std::stringstream ss;
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }else {
        std::stringstream ss;
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
}

void Parser::check_ID(std::string& name_ID) {
    if (TableID.count(name_ID) != 0){
        st_types_val.push(TableID.at(name_ID).Get_Type());
    }else{
        std::stringstream ss;
        ss << "Variable not declared " << name_ID;
        throw std::runtime_error(ss.str());
    }
}

void Parser::check_Operation() {
    Type type_var_l, type_var_r, type_op, type_ans=Lex_null, type_types_operands=Lex_null;
    type_var_l=st_types_val.top();
    st_types_val.pop();
    type_op=st_types_val.top();
    st_types_val.pop();
    type_var_r=st_types_val.top();
    st_types_val.pop();

    if (type_op == Add || type_op == Sub || type_op == Mul || type_op == Div){
        type_ans = keyword_integer;
        type_types_operands = keyword_integer;
    }
    if (type_op == keyword_or || type_op == keyword_and){
        type_ans = keyword_boolean;
        type_types_operands = keyword_boolean;
    }

    if (Comparison(type_op)){// <= >= <> = > <
        type_ans = keyword_boolean;
        type_types_operands = keyword_integer;
    }

    if (type_var_l == type_var_r && type_var_l == type_types_operands){
        st_types_val.push(type_ans);
    }else{
        std::stringstream ss;
        ss << "Wrong types are in operation " << type_op;
        throw std::runtime_error(ss.str());
    }
}
void Parser::check_not() {
    if (st_types_val.top( ) != keyword_boolean ) {
        std::stringstream ss;
        ss << "Wrong types are in operation not" ;
        throw std::runtime_error(ss.str());
    }
}
void Parser::check_eq_two_last_types(){
    Type type;
    type = st_types_val.top();
    st_types_val.pop();


    if (type == st_types_val.top()){
        st_types_val.pop();
    }else{
        std::stringstream ss;
        ss << "Wrong types are in := " ;
        throw std::runtime_error(ss.str());
    }
}
void Parser::check_is_bool() {
    Type type = st_types_val.top();
    if (type == keyword_boolean){
        st_types_val.pop();
    }else{
        std::stringstream ss;
        ss << "Types in not bool " ;
        throw std::runtime_error(ss.str());
    }
}
int main(int argc, char* argv[]){
    if (argc != 2) {
        std::cerr << "expected argument" << std::endl;
        return 1;
    }
    try {
        std::vector<Lexeme> All_Lexeme;
        std::string file = argv[1];
        All_Lexeme=lexical_analysis(file);
        Lexeme lex(Lex_FIN);
        All_Lexeme.push_back(lex);
        for (auto& a:All_Lexeme){
            std::cout << a << std::endl;
        }
        Parser parser(All_Lexeme);
        parser.analyze();
        for (auto& a :parser.TableID){
            std::cout << a.first << ' ' << a.second << std::endl;
        }
        return 0;
    }catch (std::exception& ex){
        std::cout << "Exception happens: " << ex.what();
        return 1;
    }
}