// main.cpp
#include "App.h"      // File App chinh
#include "FileIO.h"   // De App co the goi load/save
#include <iostream>

/*
 * Day la diem khoi dau (entry point) cua toan bo chuong trinh.
 */
int main() {
    try {
        // 1. Khoi tao doi tuong App
        // (Ham dung cua App se tu dong goi loadAssets, loadAllData,
        // va tao man hinh LoginScreen)
        App gymApp;
        
        // 2. Chay vong lap chinh cua ung dung
        gymApp.run();
        
        // 3. Khi vong lap ket thuc (dong cua so), ham run() ket thuc
        // Destructor (~App) se tu dong duoc goi
        // (Destructor cua App se tu dong goi saveAllData neu can)
        
    } catch (const std::exception& e) {
        // Phong truong hop co loi nghiem trong xay ra
        std::cerr << "Da xay ra loi ngoai le khong xac dinh: " << e.what() << std::endl;
        return 1; // Tra ve ma loi
    }
    
    return 0; // Thoat chuong trinh thanh cong
}   