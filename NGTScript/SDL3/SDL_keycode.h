/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 * # CategoryKeycode
 *
 * Defines constants which identify keyboard keys and modifiers.
 */

#ifndef SDL_keycode_h_
#define SDL_keycode_h_

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_scancode.h>

/**
 * The SDL virtual key representation.
 *
 * Values of this type are used to represent keyboard keys using the current
 * layout of the keyboard. These values include Unicode values representing
 * the unmodified character that would be generated by pressing the key, or an
 * `SDLK_*` constant for those keys that do not generate characters.
 *
 * A special exception is the number keys at the top of the keyboard which map
 * to SDLK_0...SDLK_9 on AZERTY layouts.
 *
 * \since This datatype is available since SDL 3.0.0.
 *
 * \sa SDL_KeyCode
 */
typedef Uint32 SDL_Keycode;

#define SDLK_SCANCODE_MASK          (1u<<30)
#define SDL_SCANCODE_TO_KEYCODE(X)  (X | SDLK_SCANCODE_MASK)
#define SDLK_UNKNOWN                0x00000000u /* 0 */
#define SDLK_RETURN                 0x0000000du /* '\r' */
#define SDLK_ESCAPE                 0x0000001bu /* '\x1B' */
#define SDLK_BACKSPACE              0x00000008u /* '\b' */
#define SDLK_TAB                    0x00000009u /* '\t' */
#define SDLK_SPACE                  0x00000020u /* ' ' */
#define SDLK_EXCLAIM                0x00000021u /* '!' */
#define SDLK_DBLAPOSTROPHE          0x00000022u /* '"' */
#define SDLK_HASH                   0x00000023u /* '#' */
#define SDLK_DOLLAR                 0x00000024u /* '$' */
#define SDLK_PERCENT                0x00000025u /* '%' */
#define SDLK_AMPERSAND              0x00000026u /* '&' */
#define SDLK_APOSTROPHE             0x00000027u /* '\'' */
#define SDLK_LEFTPAREN              0x00000028u /* '(' */
#define SDLK_RIGHTPAREN             0x00000029u /* ')' */
#define SDLK_ASTERISK               0x0000002au /* '*' */
#define SDLK_PLUS                   0x0000002bu /* '+' */
#define SDLK_COMMA                  0x0000002cu /* ',' */
#define SDLK_MINUS                  0x0000002du /* '-' */
#define SDLK_PERIOD                 0x0000002eu /* '.' */
#define SDLK_SLASH                  0x0000002fu /* '/' */
#define SDLK_0                      0x00000030u /* '0' */
#define SDLK_1                      0x00000031u /* '1' */
#define SDLK_2                      0x00000032u /* '2' */
#define SDLK_3                      0x00000033u /* '3' */
#define SDLK_4                      0x00000034u /* '4' */
#define SDLK_5                      0x00000035u /* '5' */
#define SDLK_6                      0x00000036u /* '6' */
#define SDLK_7                      0x00000037u /* '7' */
#define SDLK_8                      0x00000038u /* '8' */
#define SDLK_9                      0x00000039u /* '9' */
#define SDLK_COLON                  0x0000003au /* ':' */
#define SDLK_SEMICOLON              0x0000003bu /* ';' */
#define SDLK_LESS                   0x0000003cu /* '<' */
#define SDLK_EQUALS                 0x0000003du /* '=' */
#define SDLK_GREATER                0x0000003eu /* '>' */
#define SDLK_QUESTION               0x0000003fu /* '?' */
#define SDLK_AT                     0x00000040u /* '@' */
#define SDLK_A                      0x00000041u /* 'A' */
#define SDLK_B                      0x00000042u /* 'B' */
#define SDLK_C                      0x00000043u /* 'C' */
#define SDLK_D                      0x00000044u /* 'D' */
#define SDLK_E                      0x00000045u /* 'E' */
#define SDLK_F                      0x00000046u /* 'F' */
#define SDLK_G                      0x00000047u /* 'G' */
#define SDLK_H                      0x00000048u /* 'H' */
#define SDLK_I                      0x00000049u /* 'I' */
#define SDLK_J                      0x0000004au /* 'J' */
#define SDLK_K                      0x0000004bu /* 'K' */
#define SDLK_L                      0x0000004cu /* 'L' */
#define SDLK_M                      0x0000004du /* 'M' */
#define SDLK_N                      0x0000004eu /* 'N' */
#define SDLK_O                      0x0000004fu /* 'O' */
#define SDLK_P                      0x00000050u /* 'P' */
#define SDLK_Q                      0x00000051u /* 'Q' */
#define SDLK_R                      0x00000052u /* 'R' */
#define SDLK_S                      0x00000053u /* 'S' */
#define SDLK_T                      0x00000054u /* 'T' */
#define SDLK_U                      0x00000055u /* 'U' */
#define SDLK_V                      0x00000056u /* 'V' */
#define SDLK_W                      0x00000057u /* 'W' */
#define SDLK_X                      0x00000058u /* 'X' */
#define SDLK_Y                      0x00000059u /* 'Y' */
#define SDLK_Z                      0x0000005au /* 'Z' */
#define SDLK_LEFTBRACKET            0x0000005bu /* '[' */
#define SDLK_BACKSLASH              0x0000005cu /* '\\' */
#define SDLK_RIGHTBRACKET           0x0000005du /* ']' */
#define SDLK_CARET                  0x0000005eu /* '^' */
#define SDLK_UNDERSCORE             0x0000005fu /* '_' */
#define SDLK_GRAVE                  0x00000060u /* '`' */
#define SDLK_a                      0x00000061u /* 'a' */
#define SDLK_b                      0x00000062u /* 'b' */
#define SDLK_c                      0x00000063u /* 'c' */
#define SDLK_d                      0x00000064u /* 'd' */
#define SDLK_e                      0x00000065u /* 'e' */
#define SDLK_f                      0x00000066u /* 'f' */
#define SDLK_g                      0x00000067u /* 'g' */
#define SDLK_h                      0x00000068u /* 'h' */
#define SDLK_i                      0x00000069u /* 'i' */
#define SDLK_j                      0x0000006au /* 'j' */
#define SDLK_k                      0x0000006bu /* 'k' */
#define SDLK_l                      0x0000006cu /* 'l' */
#define SDLK_m                      0x0000006du /* 'm' */
#define SDLK_n                      0x0000006eu /* 'n' */
#define SDLK_o                      0x0000006fu /* 'o' */
#define SDLK_p                      0x00000070u /* 'p' */
#define SDLK_q                      0x00000071u /* 'q' */
#define SDLK_r                      0x00000072u /* 'r' */
#define SDLK_s                      0x00000073u /* 's' */
#define SDLK_t                      0x00000074u /* 't' */
#define SDLK_u                      0x00000075u /* 'u' */
#define SDLK_v                      0x00000076u /* 'v' */
#define SDLK_w                      0x00000077u /* 'w' */
#define SDLK_x                      0x00000078u /* 'x' */
#define SDLK_y                      0x00000079u /* 'y' */
#define SDLK_z                      0x0000007au /* 'z' */
#define SDLK_LEFTBRACE              0x0000007bu /* '{' */
#define SDLK_PIPE                   0x0000007cu /* '|' */
#define SDLK_RIGHTBRACE             0x0000007du /* '}' */
#define SDLK_TILDE                  0x0000007eu /* '~' */
#define SDLK_DELETE                 0x0000007fu /* '\x7F' */
#define SDLK_CAPSLOCK               0x40000039u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CAPSLOCK) */
#define SDLK_F1                     0x4000003au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F1) */
#define SDLK_F2                     0x4000003bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F2) */
#define SDLK_F3                     0x4000003cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F3) */
#define SDLK_F4                     0x4000003du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F4) */
#define SDLK_F5                     0x4000003eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F5) */
#define SDLK_F6                     0x4000003fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F6) */
#define SDLK_F7                     0x40000040u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F7) */
#define SDLK_F8                     0x40000041u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F8) */
#define SDLK_F9                     0x40000042u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F9) */
#define SDLK_F10                    0x40000043u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F10) */
#define SDLK_F11                    0x40000044u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F11) */
#define SDLK_F12                    0x40000045u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F12) */
#define SDLK_PRINTSCREEN            0x40000046u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN) */
#define SDLK_SCROLLLOCK             0x40000047u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK) */
#define SDLK_PAUSE                  0x40000048u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAUSE) */
#define SDLK_INSERT                 0x40000049u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_INSERT) */
#define SDLK_HOME                   0x4000004au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HOME) */
#define SDLK_PAGEUP                 0x4000004bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEUP) */
#define SDLK_END                    0x4000004du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_END) */
#define SDLK_PAGEDOWN               0x4000004eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PAGEDOWN) */
#define SDLK_RIGHT                  0x4000004fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RIGHT) */
#define SDLK_LEFT                   0x40000050u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LEFT) */
#define SDLK_DOWN                   0x40000051u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DOWN) */
#define SDLK_UP                     0x40000052u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UP) */
#define SDLK_NUMLOCKCLEAR           0x40000053u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR) */
#define SDLK_KP_DIVIDE              0x40000054u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE) */
#define SDLK_KP_MULTIPLY            0x40000055u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY) */
#define SDLK_KP_MINUS               0x40000056u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MINUS) */
#define SDLK_KP_PLUS                0x40000057u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUS) */
#define SDLK_KP_ENTER               0x40000058u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_ENTER) */
#define SDLK_KP_1                   0x40000059u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_1) */
#define SDLK_KP_2                   0x4000005au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_2) */
#define SDLK_KP_3                   0x4000005bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_3) */
#define SDLK_KP_4                   0x4000005cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_4) */
#define SDLK_KP_5                   0x4000005du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_5) */
#define SDLK_KP_6                   0x4000005eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_6) */
#define SDLK_KP_7                   0x4000005fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_7) */
#define SDLK_KP_8                   0x40000060u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_8) */
#define SDLK_KP_9                   0x40000061u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_9) */
#define SDLK_KP_0                   0x40000062u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_0) */
#define SDLK_KP_PERIOD              0x40000063u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERIOD) */
#define SDLK_APPLICATION            0x40000065u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_APPLICATION) */
#define SDLK_POWER                  0x40000066u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_POWER) */
#define SDLK_KP_EQUALS              0x40000067u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALS) */
#define SDLK_F13                    0x40000068u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F13) */
#define SDLK_F14                    0x40000069u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F14) */
#define SDLK_F15                    0x4000006au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F15) */
#define SDLK_F16                    0x4000006bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F16) */
#define SDLK_F17                    0x4000006cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F17) */
#define SDLK_F18                    0x4000006du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F18) */
#define SDLK_F19                    0x4000006eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F19) */
#define SDLK_F20                    0x4000006fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F20) */
#define SDLK_F21                    0x40000070u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F21) */
#define SDLK_F22                    0x40000071u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F22) */
#define SDLK_F23                    0x40000072u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F23) */
#define SDLK_F24                    0x40000073u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_F24) */
#define SDLK_EXECUTE                0x40000074u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXECUTE) */
#define SDLK_HELP                   0x40000075u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_HELP) */
#define SDLK_MENU                   0x40000076u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MENU) */
#define SDLK_SELECT                 0x40000077u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SELECT) */
#define SDLK_STOP                   0x40000078u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_STOP) */
#define SDLK_AGAIN                  0x40000079u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AGAIN) */
#define SDLK_UNDO                   0x4000007au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_UNDO) */
#define SDLK_CUT                    0x4000007bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CUT) */
#define SDLK_COPY                   0x4000007cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_COPY) */
#define SDLK_PASTE                  0x4000007du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PASTE) */
#define SDLK_FIND                   0x4000007eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_FIND) */
#define SDLK_MUTE                   0x4000007fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MUTE) */
#define SDLK_VOLUMEUP               0x40000080u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEUP) */
#define SDLK_VOLUMEDOWN             0x40000081u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_VOLUMEDOWN) */
#define SDLK_KP_COMMA               0x40000085u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COMMA) */
#define SDLK_KP_EQUALSAS400         0x40000086u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400) */
#define SDLK_ALTERASE               0x40000099u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ALTERASE) */
#define SDLK_SYSREQ                 0x4000009au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SYSREQ) */
#define SDLK_CANCEL                 0x4000009bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CANCEL) */
#define SDLK_CLEAR                  0x4000009cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEAR) */
#define SDLK_PRIOR                  0x4000009du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_PRIOR) */
#define SDLK_RETURN2                0x4000009eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RETURN2) */
#define SDLK_SEPARATOR              0x4000009fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SEPARATOR) */
#define SDLK_OUT                    0x400000a0u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OUT) */
#define SDLK_OPER                   0x400000a1u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_OPER) */
#define SDLK_CLEARAGAIN             0x400000a2u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN) */
#define SDLK_CRSEL                  0x400000a3u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CRSEL) */
#define SDLK_EXSEL                  0x400000a4u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_EXSEL) */
#define SDLK_KP_00                  0x400000b0u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_00) */
#define SDLK_KP_000                 0x400000b1u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_000) */
#define SDLK_THOUSANDSSEPARATOR     0x400000b2u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR) */
#define SDLK_DECIMALSEPARATOR       0x400000b3u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR) */
#define SDLK_CURRENCYUNIT           0x400000b4u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT) */
#define SDLK_CURRENCYSUBUNIT        0x400000b5u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT) */
#define SDLK_KP_LEFTPAREN           0x400000b6u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN) */
#define SDLK_KP_RIGHTPAREN          0x400000b7u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN) */
#define SDLK_KP_LEFTBRACE           0x400000b8u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE) */
#define SDLK_KP_RIGHTBRACE          0x400000b9u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE) */
#define SDLK_KP_TAB                 0x400000bau /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_TAB) */
#define SDLK_KP_BACKSPACE           0x400000bbu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE) */
#define SDLK_KP_A                   0x400000bcu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_A) */
#define SDLK_KP_B                   0x400000bdu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_B) */
#define SDLK_KP_C                   0x400000beu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_C) */
#define SDLK_KP_D                   0x400000bfu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_D) */
#define SDLK_KP_E                   0x400000c0u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_E) */
#define SDLK_KP_F                   0x400000c1u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_F) */
#define SDLK_KP_XOR                 0x400000c2u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_XOR) */
#define SDLK_KP_POWER               0x400000c3u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_POWER) */
#define SDLK_KP_PERCENT             0x400000c4u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PERCENT) */
#define SDLK_KP_LESS                0x400000c5u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_LESS) */
#define SDLK_KP_GREATER             0x400000c6u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_GREATER) */
#define SDLK_KP_AMPERSAND           0x400000c7u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND) */
#define SDLK_KP_DBLAMPERSAND        0x400000c8u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND) */
#define SDLK_KP_VERTICALBAR         0x400000c9u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR) */
#define SDLK_KP_DBLVERTICALBAR      0x400000cau /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR) */
#define SDLK_KP_COLON               0x400000cbu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_COLON) */
#define SDLK_KP_HASH                0x400000ccu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HASH) */
#define SDLK_KP_SPACE               0x400000cdu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_SPACE) */
#define SDLK_KP_AT                  0x400000ceu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_AT) */
#define SDLK_KP_EXCLAM              0x400000cfu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM) */
#define SDLK_KP_MEMSTORE            0x400000d0u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE) */
#define SDLK_KP_MEMRECALL           0x400000d1u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL) */
#define SDLK_KP_MEMCLEAR            0x400000d2u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR) */
#define SDLK_KP_MEMADD              0x400000d3u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMADD) */
#define SDLK_KP_MEMSUBTRACT         0x400000d4u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT) */
#define SDLK_KP_MEMMULTIPLY         0x400000d5u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY) */
#define SDLK_KP_MEMDIVIDE           0x400000d6u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE) */
#define SDLK_KP_PLUSMINUS           0x400000d7u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS) */
#define SDLK_KP_CLEAR               0x400000d8u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEAR) */
#define SDLK_KP_CLEARENTRY          0x400000d9u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY) */
#define SDLK_KP_BINARY              0x400000dau /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_BINARY) */
#define SDLK_KP_OCTAL               0x400000dbu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_OCTAL) */
#define SDLK_KP_DECIMAL             0x400000dcu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL) */
#define SDLK_KP_HEXADECIMAL         0x400000ddu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL) */
#define SDLK_LCTRL                  0x400000e0u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LCTRL) */
#define SDLK_LSHIFT                 0x400000e1u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LSHIFT) */
#define SDLK_LALT                   0x400000e2u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LALT) */
#define SDLK_LGUI                   0x400000e3u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_LGUI) */
#define SDLK_RCTRL                  0x400000e4u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RCTRL) */
#define SDLK_RSHIFT                 0x400000e5u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RSHIFT) */
#define SDLK_RALT                   0x400000e6u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RALT) */
#define SDLK_RGUI                   0x400000e7u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_RGUI) */
#define SDLK_MODE                   0x40000101u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MODE) */
#define SDLK_SLEEP                  0x40000102u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SLEEP) */
#define SDLK_WAKE                   0x40000103u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_WAKE) */
#define SDLK_CHANNEL_INCREMENT      0x40000104u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_INCREMENT) */
#define SDLK_CHANNEL_DECREMENT      0x40000105u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CHANNEL_DECREMENT) */
#define SDLK_MEDIA_PLAY             0x40000106u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY) */
#define SDLK_MEDIA_PAUSE            0x40000107u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PAUSE) */
#define SDLK_MEDIA_RECORD           0x40000108u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_RECORD) */
#define SDLK_MEDIA_FAST_FORWARD     0x40000109u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_FAST_FORWARD) */
#define SDLK_MEDIA_REWIND           0x4000010au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_REWIND) */
#define SDLK_MEDIA_NEXT_TRACK       0x4000010bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_NEXT_TRACK) */
#define SDLK_MEDIA_PREVIOUS_TRACK   0x4000010cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PREVIOUS_TRACK) */
#define SDLK_MEDIA_STOP             0x4000010du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_STOP) */
#define SDLK_MEDIA_EJECT            0x4000010eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_EJECT) */
#define SDLK_MEDIA_PLAY_PAUSE       0x4000010fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_PLAY_PAUSE) */
#define SDLK_MEDIA_SELECT           0x40000110u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_MEDIA_SELECT) */
#define SDLK_AC_NEW                 0x40000111u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_NEW) */
#define SDLK_AC_OPEN                0x40000112u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_OPEN) */
#define SDLK_AC_CLOSE               0x40000113u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_CLOSE) */
#define SDLK_AC_EXIT                0x40000114u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_EXIT) */
#define SDLK_AC_SAVE                0x40000115u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SAVE) */
#define SDLK_AC_PRINT               0x40000116u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PRINT) */
#define SDLK_AC_PROPERTIES          0x40000117u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_PROPERTIES) */
#define SDLK_AC_SEARCH              0x40000118u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_SEARCH) */
#define SDLK_AC_HOME                0x40000119u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_HOME) */
#define SDLK_AC_BACK                0x4000011au /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BACK) */
#define SDLK_AC_FORWARD             0x4000011bu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_FORWARD) */
#define SDLK_AC_STOP                0x4000011cu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_STOP) */
#define SDLK_AC_REFRESH             0x4000011du /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_REFRESH) */
#define SDLK_AC_BOOKMARKS           0x4000011eu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS) */
#define SDLK_SOFTLEFT               0x4000011fu /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTLEFT) */
#define SDLK_SOFTRIGHT              0x40000120u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_SOFTRIGHT) */
#define SDLK_CALL                   0x40000121u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_CALL) */
#define SDLK_ENDCALL                0x40000122u /* SDL_SCANCODE_TO_KEYCODE(SDL_SCANCODE_ENDCALL) */

/**
 * Valid key modifiers (possibly OR'd together).
 *
 * \since This datatype is available since SDL 3.0.0.
 */
typedef Uint16 SDL_Keymod;

#define SDL_KMOD_NONE   0x0000u /**< no modifier is applicable. */
#define SDL_KMOD_LSHIFT 0x0001u /**< the left Shift key is down. */
#define SDL_KMOD_RSHIFT 0x0002u /**< the right Shift key is down. */
#define SDL_KMOD_LCTRL  0x0040u /**< the left Ctrl (Control) key is down. */
#define SDL_KMOD_RCTRL  0x0080u /**< the right Ctrl (Control) key is down. */
#define SDL_KMOD_LALT   0x0100u /**< the left Alt key is down. */
#define SDL_KMOD_RALT   0x0200u /**< the right Alt key is down. */
#define SDL_KMOD_LGUI   0x0400u /**< the left GUI key (often the Windows key) is down. */
#define SDL_KMOD_RGUI   0x0800u /**< the right GUI key (often the Windows key) is down. */
#define SDL_KMOD_NUM    0x1000u /**< the Num Lock key (may be located on an extended keypad) is down. */
#define SDL_KMOD_CAPS   0x2000u /**< the Caps Lock key is down. */
#define SDL_KMOD_MODE   0x4000u /**< the !AltGr key is down. */
#define SDL_KMOD_SCROLL 0x8000u /**< the Scroll Lock key is down. */
#define SDL_KMOD_CTRL   (SDL_KMOD_LCTRL | SDL_KMOD_RCTRL)   /**< Any Ctrl key is down. */
#define SDL_KMOD_SHIFT  (SDL_KMOD_LSHIFT | SDL_KMOD_RSHIFT) /**< Any Shift key is down. */
#define SDL_KMOD_ALT    (SDL_KMOD_LALT | SDL_KMOD_RALT)     /**< Any Alt key is down. */
#define SDL_KMOD_GUI    (SDL_KMOD_LGUI | SDL_KMOD_RGUI)     /**< Any GUI key is down. */

#endif /* SDL_keycode_h_ */
