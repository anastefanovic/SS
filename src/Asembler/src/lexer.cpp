// D:\Helper\Ana\SS\Asembler\Asembler\src\lexer.cpp generated by reflex 2.1.4 from D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l

#define REFLEX_VERSION "2.1.4"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  OPTIONS USED                                                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#define REFLEX_OPTION_header_file         "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\h\\lexer.h"
#define REFLEX_OPTION_lex                 lex
#define REFLEX_OPTION_lexer               Lexer
#define REFLEX_OPTION_outfile             "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\src\\lexer.cpp"
#define REFLEX_OPTION_token_type          Token

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %top user code                                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 1 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"


    #include "token.h"
    using namespace std;



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  REGEX MATCHER                                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/matcher.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  ABSTRACT LEXER CLASS                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <reflex/abslexer.h>

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  LEXER CLASS                                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class Lexer : public reflex::AbstractLexer<reflex::Matcher> {
 public:
  typedef reflex::AbstractLexer<reflex::Matcher> AbstractBaseLexer;
  Lexer(
      const reflex::Input& input = reflex::Input(),
      std::ostream&        os    = std::cout)
    :
      AbstractBaseLexer(input, os)
  {
  }
  static const int INITIAL = 0;
  static const int COMMENT_STATE = 1;
  virtual Token lex(void);
  Token lex(const reflex::Input& input)
  {
    in(input);
    return lex();
  }
  Token lex(const reflex::Input& input, std::ostream *os)
  {
    in(input);
    if (os)
      out(*os);
    return lex();
  }
};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 1: %{ user code %}                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#line 8 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
// dodajemo opciju da nam je povratna vrednost token
#line 11 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
// definisemo stanje za parsiranje komentara

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  SECTION 2: rules                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Token Lexer::lex(void)
{
  static const char *REGEX_INITIAL = "(?m)((?:\\r?\\n))|((?:[\\x09\\x20]))|((?:(?:\\Q;\\E)))|((?:(?:\\Q.global\\E)))|((?:(?:\\Q.extern\\E)))|((?:(?:\\Q.section\\E)))|((?:(?:\\Q.end\\E)))|((?:(?:\\.byte)|(?:\\.word)))|((?:(?:\\Q.equ\\E)))|((?:(?:\\Q:\\E)))|((?:(?:\\Q.skip\\E)))|((?:(?:\\Q,\\E)))|((?:(?:\\Q+\\E)))|((?:(?:\\Q-\\E)))|((?:(?:\\Q*\\E)))|((?:(?:\\Q$\\E)))|((?:(?:\\Q(\\E)))|((?:(?:\\Q)\\E)))|((?:%(?:(?:r[0-7](?:l|h)?)|pc)))|((?:[A-Za-z][0-9A-Z_a-z]*))|((?:(?:0x[0-9A-Fa-f]+)|(?:0[0-7]*)|(?:[1-9][0-9]*)))|((?:.))";
  static const reflex::Pattern PATTERN_INITIAL(REGEX_INITIAL);
  static const char *REGEX_COMMENT_STATE = "(?m)((?:\\r?\\n))|((?:.))";
  static const reflex::Pattern PATTERN_COMMENT_STATE(REGEX_COMMENT_STATE);
  if (!has_matcher())
  {
    matcher(new Matcher(PATTERN_INITIAL, stdinit(), this));
  }
  while (true)
  {
    switch (start())
    {
      case INITIAL:
        matcher().pattern(PATTERN_INITIAL);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 63 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::END_OF_FILE, str()); }
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:42: {line_end} :
#line 42 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::END_OF_LINE, str()); }
            break;
          case 2: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:43: {blanks} :
#line 43 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ }
            break;
          case 3: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:44: {comment} :
#line 44 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ start(COMMENT_STATE);                        }
            break;
          case 4: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:45: {global} :
#line 45 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::GLOBAL, str());      }
            break;
          case 5: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:46: {extern} :
#line 46 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::EXTERN, str());      }
            break;
          case 6: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:47: {section} :
#line 47 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::SECTION, str());     }
            break;
          case 7: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:48: {end} :
#line 48 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::END, str());         }
            break;
          case 8: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:49: {data_def} :
#line 49 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::DATA_DEF, str());    }
            break;
          case 9: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:50: {equ} :
#line 50 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::EQU, str());         }
            break;
          case 10: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:51: {ddot} :
#line 51 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::DDOT, str());        }
            break;
          case 11: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:52: {skip} :
#line 52 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::SKIP, str());        }
            break;
          case 12: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:53: {comma} :
#line 53 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::COMMA, str());       }
            break;
          case 13: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:54: {plus} :
#line 54 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::PLUS, str());        }
            break;
          case 14: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:55: {minus} :
#line 55 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::MINUS, str());       }
            break;
          case 15: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:56: {mul} :
#line 56 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::MUL, str());         }
            break;
          case 16: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:57: {dollar} :
#line 57 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::DOLLAR, str());      }
            break;
          case 17: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:58: {lparen} :
#line 58 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::LPAREN, str());      }
            break;
          case 18: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:59: {rparen} :
#line 59 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::RPAREN, str());      }
            break;
          case 19: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:60: {register} :
#line 60 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::REGISTER, str());    }
            break;
          case 20: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:61: {symbol} :
#line 61 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::SYMBOL, str());      }
            break;
          case 21: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:62: {literal} :
#line 62 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::LITERAL, str());     }
            break;
          case 22: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:64: {anything} :
#line 64 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ }


            break;
        }
        break;
      case COMMENT_STATE:
        matcher().pattern(PATTERN_COMMENT_STATE);
        switch (matcher().scan())
        {
          case 0:
            if (matcher().at_end())
            {
#line 63 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ return Token(TokenType::END_OF_FILE, str()); }
            }
            else
            {
              out().put(matcher().input());
            }
            break;
          case 1: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:67: {line_end} :
#line 67 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ start(INITIAL); }
            break;
          case 2: // rule D:\Helper\Ana\SS\Asembler\Asembler\spec\test.l:68: {anything} :
#line 68 "D:\\Helper\\Ana\\SS\\Asembler\\Asembler\\spec\\test.l"
{ }

            break;
        }
        break;
      default:
        start(0);
    }
  }
}
