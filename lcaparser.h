#include <string>
#include "logica.h"

bool isWhitespace(char ch);
static const int LCA_PARSER_BUFFER_SIZE = 4096;

class LCAStream {



public:
    int line = 1;
    int col = 0;
    int pos = 0;
    char buf[LCA_PARSER_BUFFER_SIZE];

    char next();
    void croak(std::string msg);
    bool eof();
    char peek();
    void input(char buffer[LCA_PARSER_BUFFER_SIZE]);
    void printBuffer();
};

enum LCAConstants
{
    LCA_WHITE_SPACE = 1,
    LCA_REMARK = 2,
    LCA_COMMENT = 3,
    LCA_SEPARATOR = 4,
    LCA_BRACKETS = 5,
    LCA_WORD = 6,
    LCA_TEXT = 7,
    LCA_OPERATOR = 8,
    LCA_EOLINE = 9,
    LCA_SLASH = 10,
    LCA_ERROR=11
};

enum LCACommands {

    LCA_OPEN=0,
    LCA_CONTEXT_BEGIN=1,
    LCA_CONTEXT_READ=2,
    LCA_RULE_BEGIN=3,
    LCA_RULE_READ=4,
};

enum LCARuleCommands {

    LCA_RULE_READ_OPEN = 0,
    LCA_RULE_READ_BEGIN = 1,
    LCA_RULE_READ_COLLECT=2
};

class LCAParser
{
    public:
      LCAConstants loading_state = LCA_WHITE_SPACE;
      std::string* token = new std::string();
      int parenthesis_normal = 0;
      int parenthesis_curly = 0;
      int parenthesis_bracket = 0;
      bool commented_out = false;
      int succ_backslash = 0;

      LCAStream lcastream = LCAStream();

      std::string* readEscaped(std::string end, bool adding);
      std::string* readwhile(bool predicate(char ch), bool adding);
      std::string* tokenize(char input[LCA_PARSER_BUFFER_SIZE]);
      LCAConstants nextState();
      std::string* readnext();
      std::string* readWord();
      std::string* charToToken(char ch);
      std::string* nextToken();
      void printBuffer();
      void skipComment();
};

