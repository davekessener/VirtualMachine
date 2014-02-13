#ifndef CURSE_BANNER_H
#define CURSE_BANNER_H

#include <iostream>
#include <cstring>
#include "Curse.h"

#ifdef CURSE_BANNER_MAIN
#endif

namespace ncurses
{
    class Banner
    {
        public:
            Banner(const std::string&, int, int, int = 0, int = STD_SPEED, int = STD_DIST);
            ~Banner( );
            void update(int);
            void draw(Curse&);
            void setMsg(const std::string&, int = -1);
        private:
            std::string msg;
            int x, y;
            int speed, pos, len, dist, t;

            static const int STD_DIST = 4;
            static const int STD_SPEED = 200;
    };
}

#endif

