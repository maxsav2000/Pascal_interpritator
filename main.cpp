#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <exception>
#include <stack>
#include <map>
#include <memory>
#include <tuple>
enum Type {
    Lex_null,
    Lex_FIN,
    Lex_Number,
    Lex_ConstStr,
    ID,
    Lex_Less,
    Lex_LessOrEqual,
    Lex_Great,
    Lex_GreatOrEqual,
    Lex_Assign,
    Lex_NotEqual,
    Lex_LeftParenthesis,
    Lex_RightParenthesis,
    Lex_Colon,
    Lex_Semicolon,
    Lex_Equal,
    Lex_Add,
    Lex_Sub,
    Lex_Mul,
    Lex_Div,
    Lex_Dot,
    Lex_Coma,
    keyword_string,
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
    keyword_read,
    keyword_write,
    keyword_begin,
    POLIZ_adress,
    POLIZ_label,
    POLIZ_GO,
    POLIZ_IF_GO
};
const std::unordered_map<char, Type> kSingleSeparators{
        {'+', Lex_Add},
        {'-', Lex_Sub},
        {'*', Lex_Mul},
        {'/', Lex_Div},
        {'(', Lex_LeftParenthesis},
        {')', Lex_RightParenthesis},
        {':', Lex_Colon},
        {'.', Lex_Dot},
        {';', Lex_Semicolon},
        {'=', Lex_Equal},
        {'<', Lex_Great},
        {'>', Lex_Less},
        {',', Lex_Coma}
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
        {"begin",keyword_begin},
};
const std::unordered_map<std::string, Type> TwoSymbolSep{

        {":=", Lex_Assign},
        {"<>", Lex_NotEqual},
        {"<=", Lex_GreatOrEqual},
        {">=", Lex_LessOrEqual}

};
class Lexeme {
private:

    bool boolean_value;
    Type type;
    std::string value;
    unsigned int value_int;
public:
    Lexeme(){
        boolean_value= false;
        type=Lex_null;
        value.clear();
        value_int=0;
    }
    explicit Lexeme(Type curr_type){
        boolean_value= true;
        type=curr_type;
        value.clear();
        value_int=0;
    }
    Lexeme(Type curr_type,char b){
        boolean_value= true;
        type=curr_type;
        value.push_back(b);
        value_int=0;
    }
    Lexeme(Type curr_type,const std::string& curr_val){
        boolean_value= true;
        type=curr_type;
        value=curr_val;
        value_int=0;
    }
    explicit Lexeme(Type curr_type, unsigned int val){
        boolean_value = true;
        type = curr_type;
        value_int = val;
    }
    Type get_type() const{
        return type;
    }
    std::string get_value(){
        return value;
    }
    unsigned int get_int_value(){
        return value_int;
    }
    friend std::ostream& operator<< (std::ostream& s, const Lexeme& lex){
        s << lex.type << ' ' << lex.value;
        if (lex.value_int!=0){
            s << lex.value_int;
        }
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
    enum states {START_STATE, IDENT, NUMB, COMMENT, STATE_BEFORE_2SYMB_SEP, STRING_STATE};//Состояния автомата
    states current_state = START_STATE;
    int current_number_string=1;
    int current_number_symbol=1;
    std::string buf;
    while(true){
        input.get(a);
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
                    Lexeme lex(Lex_Number, buf);
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

                    current_state=START_STATE;
                    Lexeme lex(Lex_ConstStr, buf);
                    All_Lexeme.push_back(lex);
                    buf.clear();
                }
                break;

            }
        }

        current_number_symbol++;
    }

    return All_Lexeme;
}//Лексический анализ

namespace execution {
    class Address_var{
    private:
        std::string name;
    public:
        Address_var(std::string& str){
            name = str;
        }

        explicit Address_var(){}

        std::string Get_name() const{
            return name;
        }
    };

    enum ValueType { Str, Int, Logic, Label, Address};
    const std::string& ToString(ValueType type) {
        static const std::unordered_map<ValueType, std::string> kResults{
                {Str, "string"}, {Int, "int"}, {Logic, "bool"}, {Label, "label"},
                {Address, "address"}
        };
        return kResults.at(type);
    }

    class PolymorphicValue {
    public:
        PolymorphicValue(const char* str): type_(Str), str_(str) {}
        PolymorphicValue(const std::string& str): type_(Str), str_(str) {}
        PolymorphicValue(int integral): type_(Int), integral_(integral) {}
        PolymorphicValue(bool logic): type_(Logic), logic_(logic) {}
        PolymorphicValue(size_t place): type_(Label), place_(place){}
        PolymorphicValue(Address_var& addr_var): type_(Address), addr_var_(addr_var){}

        operator std::string() const { CheckIs(Str); return str_; }
        operator int() const { CheckIs(Int); return integral_; }
        operator bool() const { CheckIs(Logic); return logic_; }
        operator size_t() const {CheckIs(Label); return place_;}
        operator Address_var() const {CheckIs(Address); return addr_var_;}

        void ChangeInt(int Int_){
            integral_=Int_;
        }
        void ChangeStr(std::string& Str_){
            str_=Str_;
        }
        friend std::ostream& operator<< (std::ostream& s, const PolymorphicValue& PV){
            s << PV.type_ << ' ' << PV.integral_ << ' ' << PV.str_;
            return s;
        }
        PolymorphicValue &operator= (const PolymorphicValue & PV){
            if (this != & PV){//Защита от самоприсваивания;
                this->str_=PV.str_;
                this->logic_=PV.logic_;
                this->integral_=PV.integral_;
                this->place_=PV.place_;
                this->type_=PV.type_;
            }
            return *this;
        }

        Address_var addr_var_;
    private:
        ValueType type_;
        std::string str_;
        int integral_ = 0;
        size_t place_ = 0;
        bool logic_ = false;

        void CheckIs(ValueType type) const {
            if (type != type_) {
                throw std::logic_error("type mismatch expected " + ToString(type) +
                                       " != actual " + ToString(type_));
            }
        }
    };

// -x

    struct Context {
        std::size_t operation_index = 0;
        std::stack<PolymorphicValue> stack;
        std::map<std::string,std::shared_ptr<execution::PolymorphicValue>> Table_Op_shr;

    };

    struct Operation {
        virtual ~Operation() {}
        virtual void Do(Context& context) const = 0;
    };

    struct ValueOperation : Operation {
        ValueOperation(PolymorphicValue value): value_(value) {}

        void Do(Context& context) const final {
            context.stack.push(value_);
        }

    private:
        const PolymorphicValue value_;
    };


    struct TakeInfoOperation : Operation {
        TakeInfoOperation(const std::shared_ptr<PolymorphicValue>& value_shr): value_(*value_shr) {
            sh_ptr_val=value_shr;
        }

        void Do(Context& context) const final {
            context.stack.push(*sh_ptr_val);
        }

    private:
        const PolymorphicValue value_;
        std::shared_ptr<PolymorphicValue> sh_ptr_val;
    };


    template<typename T>
    struct GoToOperation : Operation {
        void Do(Context& context) const final;
    };

    template<typename T>
    void GoToOperation<T>::Do(Context& context) const {
        context.operation_index=static_cast<T>(context.stack.top());
        context.stack.pop();
    }

    template<typename T>
    struct NotOperation : Operation {
        void Do(Context& context) const final;
    };

    template<typename T>
    void NotOperation<T>::Do(Context& context) const {
        const PolymorphicValue new_value(not static_cast<T>(context.stack.top()));
        context.stack.pop();
        context.stack.push(new_value);
    }

    template<typename T>
    struct PrintOperation : Operation {
        void Do(Context& context) const final {
            const PolymorphicValue op = context.stack.top();
            context.stack.pop();
            std::cout << static_cast<T>(op) << std::endl;
        }
    };
    template<typename T>
    struct ReadOperation : Operation {
        void Do(Context& context) const final {
            const PolymorphicValue op = context.stack.top();
            context.stack.pop();
            std::string str;
            std::cin >> str;
            bool Is_number_flag = true;
            for (auto a:str){
                if (!IsNumber(a)){
                    Is_number_flag= false;
                }
            }
            std::string name = op.addr_var_.Get_name();
            if (Is_number_flag){//Ввели число
                int Int_=std::stoi(str);
                context.Table_Op_shr.at(name).get()->ChangeInt(Int_);
            }else{//Ввели строку
                context.Table_Op_shr.at(name).get()->ChangeStr(str);
            }
            //TODO
        }
    };

    struct ChengeOperation : Operation {
        void Do(Context& context) const final {
            // извлекаем второй операнд
            PolymorphicValue op2 = context.stack.top();
            context.stack.pop();
            // извлекаем первый операнд
            PolymorphicValue op1 = context.stack.top();
            context.stack.pop();
            //context.Table_Op_shr.get().at(op.addr_var_.Get_name())=;

            std::string name = op1.addr_var_.Get_name();
            context.Table_Op_shr.at(name).get()->operator=(op2);
            //TODO
        }
        virtual void DoChenge(PolymorphicValue op1,
                              PolymorphicValue op2) const = 0;
    };
    template<typename T1, typename T2>
    struct AssignOperation : ChengeOperation {
        void DoChenge(PolymorphicValue op1,
                      PolymorphicValue op2) const final {}
    };


    struct MoveOperation : Operation {
        void Do(Context& context) const final {
            // извлекаем второй операнд
            PolymorphicValue op2 = context.stack.top();
            context.stack.pop();
            // извлекаем первый операнд
            PolymorphicValue op1 = context.stack.top();
            context.stack.pop();
            if (not op1){
                context.operation_index=op2;
            }
        }
        virtual void DoMove(PolymorphicValue op1,
                            PolymorphicValue op2) const = 0;
    };
    //IfNotGoToOperation
    template<typename T1, typename T2>
    struct IfNotGoToOperation: MoveOperation {
        void DoMove(PolymorphicValue op1,
                    PolymorphicValue op2) const final {}
    };

    struct MathOperation : Operation {
        void Do(Context& context) const final {
            // извлекаем второй операнд
            PolymorphicValue op2 = context.stack.top();
            context.stack.pop();
            // извлекаем первый операнд
            PolymorphicValue op1 = context.stack.top();
            context.stack.pop();
            // записываем результат операции
            context.stack.push(DoMath(op1, op2));
        }

        virtual PolymorphicValue DoMath(PolymorphicValue op1,
                                        PolymorphicValue op2) const = 0;
    };

    template<typename T1, typename T2>
    struct PlusOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) + static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct MinusOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) - static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct MulOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) * static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct DivOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) / static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct LessOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) > static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct LessOrEqualOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) >= static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct EqualOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) == static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct NotEqualOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) != static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct GreatOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) < static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct GreatOrEqualOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) <= static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct OrOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) or static_cast<T2>(op2));
        }
    };

    template<typename T1, typename T2>
    struct AndOperation : MathOperation {
        PolymorphicValue DoMath(PolymorphicValue op1,
                                PolymorphicValue op2) const final {
            return PolymorphicValue(static_cast<T1>(op1) and static_cast<T2>(op2));
        }
    };

    using Operations = std::vector<std::shared_ptr<Operation>>;

    enum OperationType { Plus, Less, Print, Minus, Div, Mul, LessOrEqual, Great, GreatOrEqual, Equal,
                         NotEqual, And, Or, Not, GoTo, IfNotGoTo, Read, Assign};

    template<typename T>
    Operation* MakeOp() {
        return new T();
    }

    using OperationBuilder = Operation* (*)();

    using UnaryKey = std::tuple<OperationType, ValueType>;

    static const std::map<UnaryKey, std::shared_ptr<Operation>> kUnaries{
            {{Print, Str}, std::shared_ptr<Operation>(new PrintOperation<std::string>)},
            {{Print, Int}, std::shared_ptr<Operation>(new PrintOperation<int>)},
            {{Print, Logic}, std::shared_ptr<Operation>(new PrintOperation<bool>)},

            {{Read, Address}, std::shared_ptr<Operation>(new ReadOperation<Address_var>) },

            {{Not, Logic}, std::shared_ptr<Operation>(new NotOperation<bool>)},

            {{GoTo, Label}, std::shared_ptr<Operation>(new GoToOperation<size_t>)},
    };

    using BinaryKey = std::tuple<OperationType, ValueType, ValueType>;

    static const std::map<BinaryKey, OperationBuilder> kBinaries{
            {{Assign, Address, Logic}, &MakeOp<AssignOperation<Address_var,bool>>},
            {{Assign, Address, Int}, &MakeOp<AssignOperation<Address_var,int>>},
            {{Assign, Address, Str}, &MakeOp<AssignOperation<Address_var, std::string>>},

            {{IfNotGoTo, Logic, Label}, &MakeOp<IfNotGoToOperation<bool, size_t>>},

            {{Plus, Str, Str}, &MakeOp<PlusOperation<std::string, std::string>>},
            {{Plus, Int, Int}, &MakeOp<PlusOperation<int, int>>},

            {{Minus, Int, Int}, &MakeOp<MinusOperation<int,int>>},
            {{Div, Int, Int}, &MakeOp<DivOperation<int,int>>},
            {{Mul, Int, Int}, &MakeOp<MulOperation<int,int>>},

            {{Or, Logic, Logic}, &MakeOp<OrOperation<bool, bool>>},
            {{And, Logic, Logic},&MakeOp<AndOperation<bool, bool>>},

            {{NotEqual, Str, Str}, &MakeOp<NotEqualOperation<std::string, std::string>>},
            {{NotEqual, Int, Int}, &MakeOp<NotEqualOperation<int, int>>},
            {{NotEqual, Logic, Logic}, &MakeOp<NotEqualOperation<bool, bool>>},

            {{Equal, Str, Str}, &MakeOp<EqualOperation<std::string, std::string>>},
            {{Equal, Int, Int}, &MakeOp<EqualOperation<int, int>>},
            {{Equal, Logic, Logic}, &MakeOp<EqualOperation<bool, bool>>},

            {{GreatOrEqual, Str, Str}, &MakeOp<GreatOrEqualOperation<std::string, std::string>>},
            {{GreatOrEqual, Int, Int}, &MakeOp<GreatOrEqualOperation<int, int>>},
            {{GreatOrEqual, Logic, Logic}, &MakeOp<GreatOrEqualOperation<bool, bool>>},

            {{Great, Str, Str}, &MakeOp<GreatOperation<std::string, std::string>>},
            {{Great, Int, Int}, &MakeOp<GreatOperation<int, int>>},
            {{Great, Logic, Logic}, &MakeOp<GreatOperation<bool, bool>>},

            {{LessOrEqual, Str, Str}, &MakeOp<LessOrEqualOperation<std::string, std::string>>},
            {{LessOrEqual, Int, Int}, &MakeOp<LessOrEqualOperation<int, int>>},
            {{LessOrEqual, Logic, Logic}, &MakeOp<LessOrEqualOperation<bool, bool>>},

            {{Less, Str, Str}, &MakeOp<LessOperation<std::string, std::string>>},
            {{Less, Int, Int}, &MakeOp<LessOperation<int, int>>},
            {{Less, Logic, Logic}, &MakeOp<LessOperation<bool, bool>>},
    };


}//namespace executions

class Parser{
private:
    Lexeme curr_lexeme;
    Type curr_type;
    std::string curr_val_lexeme;
    int index_curr_lexeme;
    std::stack <std::string> st_names_val; // Стек для имен переменных для заноса в таблицу переменных
    std::stack <Type> st_types_val; // Стек типов для семантического анализа
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
    const std::map<Type,execution::OperationType> Type_to_op{
            {Lex_Add,execution::Plus},
            {Lex_Sub,execution::Minus},
            {Lex_Div,execution::Div},
            {Lex_Mul,execution::Mul},
            {Lex_Equal,execution::Equal},
            {Lex_NotEqual,execution::NotEqual},
            {Lex_GreatOrEqual,execution::GreatOrEqual},
            {Lex_Great,execution::Great},
            {Lex_Less,execution::Less},
            {Lex_LessOrEqual,execution::LessOrEqual},
            {keyword_write,execution::Print},
            {keyword_read,execution::Read},
            {keyword_not,execution::Not},
            {keyword_or,execution::Or},
            {keyword_and,execution::And},
            {POLIZ_GO,execution::GoTo},
            {POLIZ_IF_GO,execution::IfNotGoTo}
    };
    const std::map<Type,execution::ValueType > Type_to_typeop{
            {keyword_string,execution::Str},
            {keyword_boolean,execution::Logic},
            {keyword_integer, execution::Int}
    };


public:
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
    std::vector<std::shared_ptr<execution::Operation>> operations; //Полиз
    std::map<std::string,std::shared_ptr<execution::PolymorphicValue>> Table_op; // Таблица переменных для выполнения
    std::map<std::string,Variable> TableID; //Таблица переменных для контроля семантических условий
    explicit Parser(std::vector<Lexeme>& All_Lex){
        curr_type=Lex_null;
        All_Lexeme=All_Lex;
        index_curr_lexeme=-1;
    }
    void analyze();
};//Parser
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
void Parser::P() {
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
    if (curr_type == Lex_Semicolon){
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    D1();
    B ();
    if (curr_type == Lex_Dot){
        Get_lex ();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
}//P-> program ID; D1 B .
void Parser::D1() {
    std::stringstream ss;
    if (curr_type == keyword_var){
        Get_lex ();
        while(curr_type == ID){
            D();
            if (curr_type == Lex_Semicolon){
                Get_lex();
            }else{
                ss << "Incorrect lexeme " << curr_lexeme;
                throw std::runtime_error(ss.str());
            }
        }
    }
}//D1-> e | var D;{D;}
void Parser::D(){
    std::stringstream ss;
    if (curr_type == ID){
        st_names_val.push(curr_val_lexeme);
        //Считываем имена переменных и заносим их в стек, для того чтобы заполнить таблицу переменных
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    while(curr_type == Lex_Coma){
        Get_lex();
        if (curr_type == ID){
            st_names_val.push(curr_val_lexeme);
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }
    if (curr_type == Lex_Colon){
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    if (curr_type == keyword_integer || curr_type == keyword_boolean || curr_type == keyword_string){
        while (!st_names_val.empty()){
            //Все накопелнные имена переменных закосим в таблицу переменных с значениями по умолчанию
            std::string str = st_names_val.top();
            st_names_val.pop();
            Variable var(curr_type);
            if (TableID.count(str)!=0){
                ss << "Double initialization " << str;
                throw std::runtime_error(ss.str());
            }
            TableID.insert(std::pair<std::string,Variable>(str,var));

            std::shared_ptr<execution::PolymorphicValue> shptr;
            if (curr_type == keyword_integer){
                shptr = std::make_shared<execution::PolymorphicValue>(0);
            }
            if (curr_type == keyword_string){
                shptr = std::make_shared<execution::PolymorphicValue>("");
            }
            if (curr_type == keyword_boolean){
                shptr = std::make_shared<execution::PolymorphicValue>(false);
            }
            Table_op.insert(std::pair<std::string,std::shared_ptr<execution::PolymorphicValue>>(str, shptr));

        }
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
}// D-> ID{,ID}:[int | bool | string ]
void Parser::B() {
    std::stringstream ss;
    if (curr_type == keyword_begin){
        Get_lex();
    }else{
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
    while(curr_type != keyword_end){
        S();
        if (curr_type == Lex_Semicolon){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }
    Get_lex();
}//-> begin e|S;{S;} end
void Parser::S() {// S-> ID <check_ID> := E <check eq last 2 types>| if E <check_is_bool> then S else S |
    // if E<check_is_bool> then S |while E<check_is_bool> do S | B | read(ID<check_in_table>) | write(E <pop>|e) | e
    std::stringstream ss;
    if (curr_type == ID){
        check_ID(curr_val_lexeme);
        execution::Address_var adr_(curr_val_lexeme);
        operations.emplace_back(new execution::ValueOperation(adr_));
        Get_lex();
        if (curr_type == Lex_Assign){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        E();
        operations.emplace_back(execution::kBinaries.at(std::make_tuple(execution::Assign, execution::Address,
                Type_to_typeop.at(st_types_val.top())))());
        check_eq_two_last_types();
    }else if (curr_type == keyword_if){
            Get_lex();
            E();
            check_is_bool();

            size_t place_label_goto_end_ifelse_block_ = operations.size();
            // запоминаем место куда будем записывать адрес
            // перехода место после окончания блока then
            operations.emplace_back(new execution::ValueOperation(static_cast<size_t>(0)));// кладем пустую лексему
            operations.emplace_back(execution::kBinaries.at(std::make_tuple(execution::IfNotGoTo,
                    execution::Logic, execution::Label))());

            if (curr_type == keyword_then){
                Get_lex();
            }else{
                ss << "Incorrect lexeme " << curr_lexeme;
                throw std::runtime_error(ss.str());
            }
            S();

            size_t place_label_goto_start_elseblock_ = operations.size();
            operations.emplace_back(new execution::ValueOperation(static_cast<size_t>(0)));

            operations.push_back(execution::kUnaries.at(std::make_tuple(execution::GoTo, execution::Label)));

            operations[place_label_goto_end_ifelse_block_].reset(new execution::ValueOperation(operations.size()));
            // теперь мы заполняем адрес куда переходить если if блок не срабоатал

            if (curr_type == keyword_else){
                Get_lex();
                S();
                operations[place_label_goto_start_elseblock_].reset(new execution::ValueOperation(operations.size()));
                // Заполняем адрес перехода в конец if then else блока

            }else if (curr_type == Lex_Semicolon){
                operations[place_label_goto_start_elseblock_].reset(new execution::ValueOperation(operations.size()));
                //если else блока нет то заполняем метку концом then блока
            }else{
                ss << "Incorrect lexeme " << curr_lexeme;
                throw std::runtime_error(ss.str());
            }
        }//Parser::S
    else if (curr_type == keyword_while){
        // Место куда будем переходить после окончание очередного блока цикла
        size_t place_label_start_while_ = operations.size();
        Get_lex();
        E();
        check_is_bool();
        size_t place_label_goto_end_while_ = operations.size();
        //  Место для метки перехода по условию если условие цикла не сработало
        operations.emplace_back(new execution::ValueOperation(static_cast<size_t>(0)));
        // Резервируем место под метку
        operations.emplace_back(execution::kBinaries.at(std::make_tuple(execution::IfNotGoTo,
                                                                        execution::Logic, execution::Label))());

        if (curr_type == keyword_do){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
        S();
        operations.emplace_back(new execution::ValueOperation(place_label_start_while_));
        operations.push_back(execution::kUnaries.at(std::make_tuple(execution::GoTo, execution::Label)));

        operations[place_label_goto_end_while_].reset(new execution::ValueOperation(operations.size()));
        // Заполняем адрес для перехода если условие цикла не выполнено

    }
    else if (curr_type == keyword_begin){
        B();
    }
    else if (curr_type == keyword_read){
        Get_lex();

        if (curr_type == Lex_LeftParenthesis){
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

            execution::Address_var adr_(curr_val_lexeme);
            operations.emplace_back(new execution::ValueOperation(adr_));

            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }

        if (curr_type == Lex_RightParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }

        operations.push_back(execution::kUnaries.at(std::make_tuple(execution::Read, execution::Address)));
    }
    else if (curr_type == keyword_write){
        Get_lex();
        if (curr_type == Lex_LeftParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }

        if (curr_type != Lex_RightParenthesis){
            E();

            operations.push_back(execution::kUnaries.at(std::make_tuple(Type_to_op.at(keyword_write),
                    Type_to_typeop.at(st_types_val.top()))));

            st_types_val.pop();
        }

        if (curr_type == Lex_RightParenthesis){
            Get_lex();
        }else{
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }

    }
}
bool Comparison(Type type){
    return (type == Lex_Equal || type == Lex_NotEqual || type == Lex_Less ||
            type == Lex_LessOrEqual || type == Lex_Great || type == Lex_GreatOrEqual);
}
void Parser::E() {
    E1();
    if (Comparison(curr_type)){
        st_types_val.push(curr_type);
        Get_lex();
        E1();
        check_Operation();
    }
}// E-> E1 [ = | < | > | >= | <= | <>] <push op> E1 <check op> | E1
void Parser::E1() {
    T();
    while(curr_type == Lex_Add || curr_type == Lex_Sub || curr_type == keyword_or){
        st_types_val.push(curr_type);
        Get_lex();
        T();
        check_Operation();
    }
}// E1 -> T { [+|-|or] <push op> T <check op>> }
void Parser::T() {
    F();
    while(curr_type == Lex_Mul || curr_type == Lex_Div || curr_type == keyword_and){
        st_types_val.push(curr_type);
        Get_lex();
        F();
        check_Operation();
    }
}// T-> F {[ * | / | and] <push op> F <check op>}
void Parser::F() {
    if (curr_type == ID){
        check_ID(curr_val_lexeme);
        operations.emplace_back(new execution::TakeInfoOperation(Table_op.at(curr_val_lexeme)));
        //operations.emplace_back(Table_op.at(curr_val_lexeme));
        Get_lex();
    }else if(curr_type == Lex_Number){
        st_types_val.push(keyword_integer);

        int val_int = std::stoi(curr_val_lexeme);
        operations.emplace_back(new execution::ValueOperation(val_int));
        Get_lex();
    }else if(curr_type == keyword_true){
        st_types_val.push(keyword_boolean);
        bool val_bool=true;
        operations.emplace_back(new execution::ValueOperation(val_bool));
        Get_lex();
    }else if(curr_type == keyword_false){
        st_types_val.push(keyword_boolean);
        bool val_bool=false;
        operations.emplace_back(new execution::ValueOperation(val_bool));
        Get_lex();
    }else if(curr_type == keyword_not){
        Get_lex();
        F();
        check_not();
    }else if (curr_type == Lex_LeftParenthesis){
        Get_lex();
        E();
        if (curr_type == Lex_RightParenthesis){
            Get_lex();
        }else{
            std::stringstream ss;
            ss << "Incorrect lexeme " << curr_lexeme;
            throw std::runtime_error(ss.str());
        }
    }else if (curr_type == Lex_ConstStr){
        st_types_val.push(keyword_string);

        const std::string val_str = curr_val_lexeme;
        operations.emplace_back(new execution::ValueOperation(val_str));
        Get_lex();
    }else{
        std::stringstream ss;
        ss << "Incorrect lexeme " << curr_lexeme;
        throw std::runtime_error(ss.str());
    }
}//ID <check ID>| Numb <push int>| true <push bool> | false <push bool>| not F  <check not>| Const_str <push str>|(E)

void Parser::check_ID(std::string& name_ID) {
    if (TableID.count(name_ID) != 0){
        st_types_val.push(TableID.at(name_ID).Get_Type());
    }else{
        std::stringstream ss;
        ss << "Variable not declared " << name_ID;
        throw std::runtime_error(ss.str());
    }
}//Проверяет индефикатор на наличие в таблице переменных

void Parser::check_Operation() {
    Type type_var_l, type_var_r, type_op, type_ans=Lex_null, type_types_operands=Lex_null;
    type_var_l=st_types_val.top();
    st_types_val.pop();
    type_op=st_types_val.top();
    st_types_val.pop();
    type_var_r=st_types_val.top();
    st_types_val.pop();

    if (type_op == Lex_Add || type_op == Lex_Sub || type_op == Lex_Mul || type_op == Lex_Div){
        type_ans = keyword_integer;
        type_types_operands = keyword_integer;
    }

    if (type_op == Lex_Add ){
        if (type_var_l == keyword_string && type_var_r == keyword_string){
            type_ans = keyword_string;
            type_types_operands = keyword_string;
        }
    }

    if (type_op == keyword_or || type_op == keyword_and){
        type_ans = keyword_boolean;
        type_types_operands = keyword_boolean;
    }

    if (Comparison(type_op)){// <= >= <> = > <
        if (type_var_l == keyword_string && type_var_r == keyword_string){
            type_ans = keyword_boolean;
            type_types_operands = keyword_string;
        }else if (type_var_l == keyword_integer && type_var_r == keyword_integer){
            type_ans = keyword_boolean;
            type_types_operands = keyword_integer;
        }else{
            type_ans = keyword_boolean;
            type_types_operands = keyword_boolean;
        }
    }

    if (type_var_l == type_var_r && type_var_l == type_types_operands){
        st_types_val.push(type_ans);
    }else{
        std::stringstream ss;
        ss << "Wrong types are in operation " << type_op;
        throw std::runtime_error(ss.str());
    }

    operations.emplace_back(execution::kBinaries.at(std::make_tuple(Type_to_op.at(type_op),
                                                       Type_to_typeop.at(type_var_l), Type_to_typeop.at(type_var_r)))());
} // Провряет валидность бинарной операции и заносит результат в стек типов, и заносит операцию в полиз
void Parser::check_not() {
    if (st_types_val.top( ) != keyword_boolean ) {
        std::stringstream ss;
        ss << "Wrong types are in operation not" ;
        throw std::runtime_error(ss.str());
    }else{
        operations.push_back(execution::kUnaries.at(std::make_tuple(execution::Not, execution::Logic)));
    }
}//Проверяет верхний элемент на bool и заносит в полиз операцию not
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
}//забирает два верних элемента из стека и проверяет на равенство
void Parser::check_is_bool() {
    Type type = st_types_val.top();
    if (type == keyword_boolean){
        st_types_val.pop();
    }else{
        std::stringstream ss;
        ss << "Types in not bool " ;
        throw std::runtime_error(ss.str());
    }
}//забирает последний элемент и проверяет он bool

namespace execution{
    class Executor{
    private:
        std::vector<std::shared_ptr<execution::Operation>> operations;
        Context context;

    public:
        Executor(std::map<std::string,std::shared_ptr<PolymorphicValue>>&,
                 std::vector<std::shared_ptr<Operation>>&);
        void Perform(){
            while (context.operation_index < operations.size()) {
                const auto& operation = operations[context.operation_index];
                ++context.operation_index;
                operation->Do(context);
            }
        }
        std::map<std::string,std::shared_ptr<Operation>>TableOP;
    };//Executor

    Executor::Executor(std::map<std::string,std::shared_ptr<PolymorphicValue>>& TableOP_,
                       std::vector<std::shared_ptr<Operation>>& ops){
        operations = ops;
        context.Table_Op_shr = TableOP_;
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
        All_Lexeme.emplace_back(Lex_FIN);
        //Лексический анализ
        Parser parser(All_Lexeme);
        parser.analyze();
        //Синтаксический семантический анализы, генерация полиза
        //parser.Table_op - таблица переменных, parser.operations полиз анализируемой программы программы
        execution::Executor ex(parser.Table_op,parser.operations);
        ex.Perform();
        //Выполнение
        return 0;
    }catch (std::exception& ex){
        std::cout << "Exception happens: " << ex.what();
        return 1;
    }
}