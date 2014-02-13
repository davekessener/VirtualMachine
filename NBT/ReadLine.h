#ifndef CURSE_READLINE_H
#define CURSE_READLINE_H

#include <iostream>
#include "Curse.h"

#ifdef CURSE_READLINE_MAIN
#include <cstring>

#define BUF_SIZE 256
#endif

namespace ncurses
{
    class ReadLine
    {
        typedef std::function<void(const std::string&)> onDoneFn;
        typedef std::function<void(void)> onFailureFn;

        public:
            ReadLine(Curse&, onDoneFn, onFailureFn = onFailureFn());
            ~ReadLine( );
            bool operator()(int);
        private:
            void resize( );
            bool isSuitable(int);
            Curse &curse;
            onDoneFn onDone;
            onFailureFn onFailure;
            char *buf;
            int l, i, idx;
            int x, y;
    };
}

#endif

