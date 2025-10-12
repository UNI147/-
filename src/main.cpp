#include "core/Application.h"
#include <iostream>

int main() {
    Revolt::Application app;
    
    if (!app.Initialize()) {
        std::cerr << "Failed to initialize Revolt Engine" << std::endl;
        return -1;
    }
    
    app.Run();
    app.Shutdown();
    
    return 0;
}