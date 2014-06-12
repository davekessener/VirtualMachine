#define CURSE_BANNER_MAIN
#include "Banner.h"
#undef CURSE_BANNER_MAIN

namespace ncurses
{
    Banner::Banner(const std::string& _msg, int _x, int _y, int _len, int _speed, int _dist) : 
        msg(_msg), x(_x), y(_y), len(_len), speed(_speed), dist(_dist), pos(0), t(0)
    {
        if(speed <= 0) speed = STD_SPEED;
        if(dist < 0) dist = STD_DIST;
        if(len <= 0) len = msg.length() + dist;

        if(dist)
        {
            char buf[dist + 1];
            memset(buf, ' ', dist);
            buf[dist] = '\0';

            msg += buf;
        }
    }

    Banner::~Banner(void)
    {
    }

    void Banner::update(int ms)
    {
        t += ms;
        while(t > speed)
        {
            t -= speed;
            if(++pos == msg.length()) pos = 0;
        }
    }

    void Banner::draw(Curse& curse)
    {
        int _x, _y;
        curse.getCursorPos(_x, _y);
        curse.setCursorPos(x, y);

        std::string s = msg;
        while(s.length() < len + pos) s += msg;

        curse.printf(s.substr(pos, len).c_str());

        curse.setCursorPos(_x, _y);
    }

    void Banner::setMsg(const std::string& s, int l)
    {
        if(s.length() == 0) return;

        if(l == 0) l = s.length() + dist;
        if(l > 0) len = l;

        msg = s + msg.substr(0, dist);
        pos = 0;
    }
}

