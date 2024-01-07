#include "App.h"

void writeThemes();
void readThemes();

int main()
{
    App app{"Partial Commander"};
    readThemes();
    app.init();
    app.run();
    writeThemes();
    return 0;
}