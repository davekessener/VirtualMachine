#include <iostream>
#include "Curse.h"
#include "Editor.h"

int main(int argc, char *argv[])
{
    Editor e;

    ncurses::Curse::play(e);

    return 0;
}

