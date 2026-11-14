#ifndef lex_h
#define lex_h

#include <stddef.h>

typedef enum {
	/* keywords (37) */
	TK_AUTO,
	TK_BREAK,
	TK_CASE,
	TK_CHAR,
	TK_CONST,
	TK_CONTINUE,
	TK_DEFAULT,
	TK_DO,
	TK_DOUBLE,
	TK_ELSE,
	TK_ENUM,
	TK_EXTERN,
	TK_FLOAT,
	TK_FOR,
	TK_GOTO,
	TK_IF,
	TK_INLINE,
	TK_INT,
	TK_LONG,
	TK_REGISTER,
	TK_RESTRICT,
	TK_RETURN,
	TK_SHORT,
	TK_SIGNED,
	TK_SIZEOF,
	TK_STATIC,
	TK_STRUCT,
	TK_SWITCH,
	TK_TYPEDEF,
	TK_UNION,
	TK_UNSIGNED,
	TK_VOID,
	TK_VOLATILE,
	TK_WHILE,
	TK__BOOL,
	TK__COMPLEX,
	TK__IMAGINARY,

	/* punctuators (54) */
	TK_LBRACKET, // [
	TK_RBRACKET, // ]
	TK_LPAREN, // (
	TK_RPAREN, // )
	TK_LBRACE, // {
	TK_RBRACE, // }
	TK_DOT, // .
	TK_ARROW, // ->
	TK_PLUS_PLUS, // ++
	TK_MINUS_MINUS, // --
	TK_AND, // &
	TK_STAR, // *
	TK_PLUS, // +
	TK_MINUS, // -
	TK_TILDE, // ~
	TK_NOT, // !
	TK_SLASH, // /
	TK_PERCENT, // %
	TK_LESS_LESS, // <<
	TK_GREATER_GREATER, // >>
	TK_LESS, // <
	TK_GREATER, // >
	TK_LESS_EQ, // <=
	TK_GREATER_EQ, // >=
	TK_EQ_EQ, // ==
	TK_NOT_EQ, // !=
	TK_CARET, // ^
	TK_PIPE, // |
	TK_AND_AND, // &&
	TK_PIPE_PIPE, // ||
	TK_QUES, // ?
	TK_COLON, // :
	TK_SEMICOLON, // ;
	TK_ELLIPSIS, // ...
	TK_EQ, // =
	TK_STAR_EQ, // *=
	TK_SLASH_EQ, // /=
	TK_PERCENT_EQ, // %=
	TK_PLUS_EQ, // +=
	TK_MINUS_EQ, // -=
	TK_LESS_LESS_EQ, // <<=
	TK_GREATER_GREATER_EQ, // >>=
	TK_AND_EQ, // &=
	TK_CARET_EQ, // ^=
	TK_PIPE_EQ, // |=
	TK_COMMA, // ,
	TK_HASH, // #
	TK_HASH_HASH, // ##
	TK_LESS_COLON, // <:
	TK_COLON_GREATER, // :>
	TK_LESS_PERCENT, // <%
	TK_PERCENT_GREATER, // %>
	TK_PERCENT_COLON, // %:
	TK_PERCENT_COLONX2, // %:%:

	/* others */
	TK_IDENTIFIER,
	TK_INTEGER_CONSTANT,
	TK_FLOATING_CONSTANT,
	TK_CHAR_CONSTANT,
	TK_STRING_LITERAL,
	TK_PP_NUMBER, // Preprocessing number

	TK_ERROR,
	TK_EOF,
	TK_COUNT
} TokenKind;

typedef struct {
	TokenKind kind;
	size_t lexeme_id;
} Token;

typedef struct {
	const char *source;

	Token token;

	size_t line;
	size_t column;
} Lexer;

#endif
