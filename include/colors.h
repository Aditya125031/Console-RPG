#pragma once
#include <string>

/*
    ANSI Escape Codes
    -----------------
    \033[  : This is the escape sequence prefix (Escape character)
    
    m      : This is the suffix (Select Graphic Rendition)
    
    Codes are separated by semicolons (e.WELCOME_MSG, "1;31m" is Bold Red)

    Formatting:
        0: Reset all styles
        1: Bold
        4: Underline

    Foreground (Text) Colors:
        30: Black
        31: Red
        32: Green
        33: Yellow
        34: Blue
        35: Magenta
        36: Cyan
        37: White
    
    Bright Foreground (Text) Colors:
        90: Gray (Bright Black)
        91: Bright Red
        92: Bright Green
        93: Bright Yellow
        94: Bright Blue
        95: Bright Magenta
        96: Bright Cyan
        97: Bright White

    Background Colors:
        40: Black BG
        41: Red BG
        42: Green BG
        43: Yellow BG
        44: Blue BG
        45: Magenta BG
        46: Cyan BG
        47: White BG

    Bright Background Colors:
        100: Gray BG (Bright Black)
        101: Bright Red BG
        102: Bright Green BG
        103: Bright Yellow BG
        104: Bright Blue BG
        105: Bright Magenta BG
        106: Bright Cyan BG
        107: Bright White BG
*/

namespace Color {

    // --- Special ---
    const std::string RESET = "\033[0m";

    // --- Formatting ---
    const std::string BOLD = "\033[1m";
    const std::string UNDERLINE = "\033[4m";

    // --- Normal Foreground (FG) Colors ---
    const std::string FG_BLACK = "\033[30m";
    const std::string FG_RED = "\033[31m";
    const std::string FG_GREEN = "\033[32m";
    const std::string FG_YELLOW = "\033[33m";
    const std::string FG_BLUE = "\033[34m";
    const std::string FG_MAGENTA = "\033[35m";
    const std::string FG_CYAN = "\033[36m";
    const std::string FG_WHITE = "\033[37m";

    // --- Bright Foreground (FG) Colors ---
    const std::string FG_GRAY = "\033[90m"; // Also Bright Black
    const std::string FG_BRIGHT_RED = "\033[91m";
    const std::string FG_BRIGHT_GREEN = "\033[92m";
    const std::string FG_BRIGHT_YELLOW = "\033[93m";
    const std::string FG_BRIGHT_BLUE = "\033[94m";
    const std::string FG_BRIGHT_MAGENTA = "\033[95m";
    const std::string FG_BRIGHT_CYAN = "\033[96m";
    const std::string FG_BRIGHT_WHITE = "\033[97m";

    // --- Normal Background (BG) Colors ---
    const std::string BG_BLACK = "\033[40m";
    const std::string BG_RED = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
    const std::string BG_YELLOW = "\033[43m";
    const std::string BG_BLUE = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN = "\033[46m";
    const std::string BG_WHITE = "\033[47m";

    // --- Bright Background (BG) Colors ---
    const std::string BG_GRAY = "\033[100m"; // Also Bright Black
    const std::string BG_BRIGHT_RED = "\033[101m";
    const std::string BG_BRIGHT_GREEN = "\033[102m";
    const std::string BG_BRIGHT_YELLOW = "\033[103m";
    const std::string BG_BRIGHT_BLUE = "\033[104m";
    const std::string BG_BRIGHT_MAGENTA = "\033[105m";
    const std::string BG_BRIGHT_CYAN = "\033[106m";
    const std::string BG_BRIGHT_WHITE = "\033[107m";

} // namespace Color