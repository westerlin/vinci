#include "lcaparser.h"

char LCAStream::next() {
    char ch = buf[pos++];

    if (ch == '\n')
    {
       // Log("new line");
        line++;
        col = 0;
    }
    else
    {
       // Log("new col");
        col ++;
    }
    return ch;
}

char LCAStream::peek(){
    return buf[pos];
}

bool LCAStream::eof(){
    return pos >= std::strlen(buf);
}


void LCAStream::input(char buffer[]){
    strcpy(buf, buffer);
    /*
    Log(("LOADED:"+std::to_string(strlen(buf))).c_str());
    for (int i=0;i<strlen(buf);i++) std::cout.put(buf[i]);
    for (int i = 0; i < strlen(buf); i++)
        std::cout.put(buf[i]);
     std::cout << std::endl;   
    */
   //printBuffer();

    pos = 0;
}

void LCAStream::printBuffer(){
    std::cout << "\033[0;32m";
    for (int i = 0; i < strlen(buf); i++)
        std::cout.put(buf[i]);
    std::cout << "\033[0m" << std::endl;
    Log("----------------");
}

void LCAParser::printBuffer(){ 
    for (int i = 0; i < strlen(lcastream.buf); i++)
        std::cout.put(lcastream.buf[i]);
    std::cout << std::endl;
}

void LCAStream::croak(std::string msg)
{
    Log((msg + " (" + std::to_string(line) + ":" + std::to_string(col) +":" + std::to_string(pos) + ")").c_str());
    throw new std::exception();
}

bool isKeyword(std::string keyword){
    return false;
};

bool isWhitespace(char ch)
{
    return std::string(" \t\n").find(ch) != std::string::npos;
};

bool noLineBreak(char ch){
    return ch != '\n';
}

bool isWordStart(char ch)
{
    return std::regex_match(&ch, std::regex("[\\.\\!a-z_A-Z\\[\\]]"));
}
bool isSlash(char ch)
{
    return ch == '/';
}
bool isWord(char ch)
{
    return isWordStart(ch) || std::string("?-<>=0123456789").find(ch) != std::string::npos;
}
bool isOperator(char ch)
{
    return std::string("~+-*%/=&|<>!").find(ch) != std::string::npos;
}

bool isSeparator(char ch)
{
    return std::string(":,;").find(ch) != std::string::npos;
}
bool isBracket(char ch)
{
    return std::string("(){}").find(ch) != std::string::npos;
}

std::string* LCAParser::charToToken(char ch)
{
    token->push_back(ch);
    return token;
}

std::string *LCAParser::readwhile(bool predicate(char ch), bool adding)
    {
        while (!lcastream.eof() && predicate(lcastream.peek())){
            if (lcastream.peek() == '/') succ_backslash++; else succ_backslash = 0; 
            if (adding) token->push_back(lcastream.next());
            else lcastream.next();
        }
        //if (!predicate(lcastream.peek())) loading_state = LCA_WHITE_SPACE;
        if (lcastream.eof()) return NULL;    
        return token; 
    };

    void LCAParser::skipComment()
    {
        readwhile(noLineBreak,false);
        //if (lcastream.peek()=='\n') loading_state = LCA_WHITE_SPACE;
        //Log("__________________");
        //printBuffer();
        //std::cout << "\033[0;31m" << "[" << lcastream.peek() << "]" << "\033[0m" << "::";
        //lcastream.next();
        //std::cout << "\033[0;31m" << "[" << lcastream.peek() << "]" << "\033[0m" << "::";
    }

    std::string* LCAParser::readEscaped(std::string end, bool adding)
    {
        bool escaped = false;
        char ch;
        /*
        ch= lcastream.next();
        if (adding)
            token->push_back(ch);
    */
        while (!lcastream.eof()){
            ch = lcastream.next();
            //Log(str->c_str());
            if (escaped){
                if (adding) token->push_back(ch);
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            } else if (end.find(ch) != std::string::npos) {
                if (adding){
                    token->push_back(ch);
                    loading_state = LCA_WHITE_SPACE;
                    return token;}
                return NULL;
            } else {
                if (adding) token->push_back(ch);
            }
        }
        return NULL;
    }
 
    std::string* LCAParser::readWord(){
        return readwhile(isWord,true);
    }

    LCAConstants LCAParser::nextState() 
    {
        readwhile(isWhitespace,false);
        if (lcastream.eof()) return LCA_EOLINE;
        char ch = lcastream.peek();
        //if (isSlash(ch)) return LCA_SLASH;
        if (ch == '#') return LCA_REMARK;
        if (ch == '"') return LCA_TEXT;
        if (isWord(ch)) return LCA_WORD;
        if (isOperator(ch)) return LCA_OPERATOR;
        if (isSeparator(ch)) return LCA_SEPARATOR;
        if (isBracket(ch)) return LCA_BRACKETS;
        std::string str(1, ch);
        lcastream.printBuffer();
        lcastream.croak("Can't handle character: " + std::to_string(ch) + " [" + str + "]");
        return LCA_ERROR;
    }

    std::string* LCAParser::readnext()
    {
    //    Log(std::to_string(loading_state).c_str());
        if (loading_state == LCA_WHITE_SPACE)
            loading_state = nextState();
        //Log(std::to_string(loading_state).c_str());
        switch (loading_state)
        {
        case LCA_TEXT:
            return readEscaped("\"", true);
        break;
        case LCA_WORD:
            return readWord();
        break;
        case LCA_BRACKETS:
            return readwhile(isBracket,true);
        break;
        case LCA_OPERATOR:
            return readwhile(isOperator, true);
        break;
        case LCA_SEPARATOR:
            return readwhile(isSeparator, true);
        break;
        case LCA_REMARK:
            //skipComment();
            readEscaped("\n",false);
            if (lcastream.eof()) return NULL;
            loading_state = LCA_WHITE_SPACE;
            //std::cout << *token << "::" <<std::to_string(loading_state);
            //Log(" -------------------- RECUVISIVE ---------------------");
            readnext();
            //std::cout << *token << "::" << std::to_string(loading_state);
            //Log(" -------------------- RECUVISIVE ---------------------");
            //std::cout << *token << "::" << std::to_string(loading_state);
            return token;
            break;
        case LCA_EOLINE:
            //Log("\t EORLOINE");
            //Log(token->c_str());
            //token->push_back('#');
            loading_state=LCA_WHITE_SPACE;
            return NULL;
        break;
        default:
            return NULL;
        break;
        }
    } 
    

    std::string* LCAParser::tokenize(char input[])
    {
//        std::cout << (std::to_string(loading_state)).c_str() << "::";
//        std::cout << "\033[0;36m" << token->c_str();
        lcastream.input(input);

            //Log(("::" + std::string(lcastream.buf)).c_str());
            return readnext();
    };
    std::string* LCAParser::nextToken()
    {
        delete token;
        token = new std::string();
        loading_state = LCA_WHITE_SPACE;
        return readnext();
    };
