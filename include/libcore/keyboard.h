#ifndef _LIBCORE_KEY_H
#define _LIBCORE_KEY_H

typedef enum {
    _NONE_KEY             = 0,
    _A_KEY                = 4,   _B_KEY             = 5,   _C_KEY                 = 6,   _D_KEY                 = 7,    _E_KEY                = 8,
    _F_KEY                = 9,   _G_KEY             = 10,   _H_KEY                = 11,   _I_KEY                = 12,   _J_KEY                = 13,
    _K_KEY                = 14,   _L_KEY            = 15,   _M_KEY                = 16,   _N_KEY                = 17,   _O_KEY                = 18,
    _P_KEY                = 19,   _Q_KEY            = 20,   _R_KEY                = 21,   _S_KEY                = 22,   _T_KEY                = 23,
    _U_KEY                = 24,   _V_KEY            = 25,   _W_KEY                = 26,   _X_KEY                = 27,   _Y_KEY                = 28,
    _Z_KEY                = 29,   _1_KEY            = 30,   _2_KEY                = 31,   _3_KEY                = 32,   _4_KEY                = 33,
    _5_KEY                = 34,   _6_KEY            = 35,   _7_KEY                = 36,   _8_KEY                = 37,   _9_KEY                = 38,
    _0_KEY                = 39,   _ESCAPE_KEY       = 41,   _DELETE_KEY           = 42,   _TAB_KEY              = 43,   _SPACE_KEY            = 44,
    _MINUS_KEY            = 45,   _EQUALS_KEY       = 46,   _LEFT_BRACKET_KEY     = 47,   _RIGHT_BRACKET_KEY    = 48,   _BACKSLASH_KEY        = 49,
    _SEMICOLON_KEY        = 51,   _QUOTE_KEY        = 52,   _GRAVE_KEY            = 53,   _COMMA_KEY            = 54,   _PERIOD_KEY           = 55,
    _SLASH_KEY            = 56,   _CAPSLOCK_KEY     = 57,   _F1_KEY               = 58,   _F2_KEY               = 59,   _F3_KEY               = 60,
    _F4_KEY               = 61,   _F5_KEY           = 62,   _F6_KEY               = 63,   _F7_KEY               = 64,   _F8_KEY               = 65,
    _F9_KEY               = 66,   _F10_KEY          = 67,   _F11_KEY              = 68,   _F12_KEY              = 69,   _PRINT_SCREEN_KEY     = 70,
    _SCROLL_LOCK_KEY      = 71,   _PAUSE_KEY        = 72,   _INSERT_KEY           = 73,   _HOME_KEY             = 74,   _PAGE_UP_KEY          = 75,
    _DELETE_FORWARD_KEY   = 76,   _END_KEY          = 77,   _PAGE_DOWN_KEY        = 78,   _RIGHT_KEY            = 79,   _LEFT_KEY             = 80,
    _DOWN_KEY             = 81,   _UP_KEY           = 82,   _NUMLOCK_NUMPAD_KEY   = 83,   _DIVIDE_NUMPAD_KEY    = 84,   _MULTIPLY_NUMPAD_KEY  = 85,
    _SUBTRACT_NUMPAD_KEY  = 86,   _ADD_NUMPAD_KEY   = 87,   _ENTER_NUMPAD_KEY     = 88,   _1_NUMPAD_KEY         = 89,   _2_NUMPAD_KEY         = 90,
    _3_NUMPAD_KEY         = 91,   _4_NUMPAD_KEY     = 92,   _5_NUMPAD_KEY         = 93,   _6_NUMPAD_KEY         = 94,   _7_NUMPAD_KEY         = 95,
    _8_NUMPAD_KEY         = 96,   _9_NUMPAD_KEY     = 97,   _0_NUMPAD_KEY         = 98,   _POINT_NUMPAD_KEY     = 99,   _NON_US_BACKSLASH_KEY = 100,
    _EQUALS_NUMPAD_KEY    = 103,   _F13_KEY         = 104,   _F14_KEY             = 105,   _F15_KEY             = 106,   _F16_KEY             = 107,
    _F17_KEY              = 108,   _F18_KEY         = 109,   _F19_KEY             = 110,   _F20_KEY             = 111,   _F21_KEY             = 112,
    _F22_KEY              = 113,   _F23_KEY         = 114,   _F24_KEY             = 115,   _HELP_KEY            = 117,   _MENU_KEY            = 118,
    _MUTE_KEY             = 127,   _SYSREQ_KEY      = 154,   _RETURN_KEY          = 158,   _CLEAR_NUMPAD_KEY    = 216,   _DECIMAL_NUMPAD_KEY  = 220,
    _LEFT_CONTROL_KEY     = 224,   _LEFT_SHIFT_KEY  = 225,   _LEFT_ALT_KEY        = 226,   _LEFT_GUI_KEY        = 227,   _RIGHT_CONTROL_KEY   = 228,
    _RIGHT_SHIFT_KEY      = 229,   _RIGHT_ALT_KEY   = 230,   _RIGHT_GUI_KEY       = 231
} _KEY;

#endif /* _LIBCORE_KEY_H */
