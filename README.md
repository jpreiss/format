format
======

Curly brace string formatting for C++, like Python and C#.

Doesn't handle floating-point format specifiers or implicit argument indices yet,
but handles escaped braces `"{{"` correctly.


    // char buf[32];
    // sprintf(buf, "(%d, %f, %f)", vec.x, vec.y, vec.z); // spot the mistakes!
    
    // std::stringstream ss;
    // ss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")"; // ugly!
    
    std::string s = format("({0}, {1}, {2})", vec.x, vec.y, vec.z); // yes!
