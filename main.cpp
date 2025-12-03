#include <iostream>

#include "App.h"   
// #include "FileIO.h"   

int main() {
    try {
    
        App gymApp;
        
        gymApp.run();
        
    } catch (const std::exception& e) {
        // Phong truong hop co loi nghiem trong xay ra
        std::cerr << "Da xay ra loi ngoai le khong xac dinh: " << e.what() << std::endl;
        return 1; // Tra ve ma loi
    }
    
    return 0; // Thoat chuong trinh thanh cong
}   