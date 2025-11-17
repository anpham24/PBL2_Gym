// #include "SearchService.h"
// #include "QuanLy.h"
// #include <algorithm>
// #include <cctype>
// #include <ctime>

// // ============================================================================
// // SEARCH IMPLEMENTATIONS
// // ============================================================================

// MyVector<HoiVien*> SearchService::searchHoiVien(const MyVector<HoiVien*>& data, 
//                                                  const string& keyword) {
//     MyVector<HoiVien*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HoiVien* hv = data[i];
//         if (hv == nullptr) continue;
        
//         string lowerID = toLowerCase(hv->getID());
//         string lowerName = toLowerCase(hv->getHoTen());
//         string lowerSDT = toLowerCase(hv->getSDT());
        
//         if (contains(lowerID, lowerKeyword) ||
//             contains(lowerName, lowerKeyword) ||
//             contains(lowerSDT, lowerKeyword)) {
//             result.push_back(hv);
//         }
//     }
    
//     return result;
// }

// MyVector<HLV*> SearchService::searchHLV(const MyVector<HLV*>& data, 
//                                         const string& keyword) {
//     MyVector<HLV*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HLV* hlv = data[i];
//         if (hlv == nullptr) continue;
        
//         string lowerID = toLowerCase(hlv->getID());
//         string lowerName = toLowerCase(hlv->getHoTen());
//         string lowerSDT = toLowerCase(hlv->getSDT());
        
//         if (contains(lowerID, lowerKeyword) ||
//             contains(lowerName, lowerKeyword) ||
//             contains(lowerSDT, lowerKeyword)) {
//             result.push_back(hlv);
//         }
//     }
    
//     return result;
// }

// MyVector<NhanVien*> SearchService::searchNhanVien(const MyVector<NhanVien*>& data, 
//                                                    const string& keyword) {
//     MyVector<NhanVien*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         NhanVien* nv = data[i];
//         if (nv == nullptr) continue;
        
//         string lowerID = toLowerCase(nv->getID());
//         string lowerName = toLowerCase(nv->getHoTen());
//         string lowerSDT = toLowerCase(nv->getSDT());
        
//         if (contains(lowerID, lowerKeyword) ||
//             contains(lowerName, lowerKeyword) ||
//             contains(lowerSDT, lowerKeyword)) {
//             result.push_back(nv);
//         }
//     }
    
//     return result;
// }

// MyVector<GoiTap*> SearchService::searchGoiTap(const MyVector<GoiTap*>& data, 
//                                                const string& keyword) {
//     MyVector<GoiTap*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         GoiTap* gt = data[i];
//         if (gt == nullptr) continue;
        
//         string lowerName = toLowerCase(gt->getTenGoi());
        
//         if (contains(lowerName, lowerKeyword)) {
//             result.push_back(gt);
//         }
//     }
    
//     return result;
// }

// MyVector<MonTap*> SearchService::searchMonTap(const MyVector<MonTap*>& data, 
//                                                const string& keyword) {
//     MyVector<MonTap*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         MonTap* mt = data[i];
//         if (mt == nullptr) continue;
        
//         string lowerName = toLowerCase(mt->getTenMon());
        
//         if (contains(lowerName, lowerKeyword)) {
//             result.push_back(mt);
//         }
//     }
    
//     return result;
// }

// MyVector<LopHoc*> SearchService::searchLopHoc(const MyVector<LopHoc*>& data, 
//                                                const string& keyword) {
//     MyVector<LopHoc*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         LopHoc* lh = data[i];
//         if (lh == nullptr) continue;
        
//         string lowerName = toLowerCase(lh->getTenLop());
        
//         if (contains(lowerName, lowerKeyword)) {
//             result.push_back(lh);
//         }
//     }
    
//     return result;
// }

// MyVector<HangHoa*> SearchService::searchHangHoa(const MyVector<HangHoa*>& data, 
//                                                  const string& keyword) {
//     MyVector<HangHoa*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HangHoa* hh = data[i];
//         if (hh == nullptr) continue;
        
//         string lowerName = toLowerCase(hh->getTenHH());
        
//         if (contains(lowerName, lowerKeyword)) {
//             result.push_back(hh);
//         }
//     }
    
//     return result;
// }

// MyVector<HoaDon*> SearchService::searchHoaDon(const MyVector<HoaDon*>& data, 
//                                                const string& keyword) {
//     MyVector<HoaDon*> result;
//     if (keyword.empty()) return result;
    
//     string lowerKeyword = toLowerCase(keyword);
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HoaDon* hd = data[i];
//         if (hd == nullptr) continue;
        
//         string lowerID = toLowerCase(hd->getID());
        
//         bool match = contains(lowerID, lowerKeyword);
        
//         // Tìm theo tên hoặc SDT hội viên
//         const HoiVien* hv = hd->getHoiVien();
//         if (hv != nullptr) {
//             string lowerHVName = toLowerCase(hv->getHoTen());
//             string lowerHVSDT = toLowerCase(hv->getSDT());
            
//             if (contains(lowerHVName, lowerKeyword) || 
//                 contains(lowerHVSDT, lowerKeyword)) {
//                 match = true;
//             }
//         }
        
//         if (match) {
//             result.push_back(hd);
//         }
//     }
    
//     return result;
// }

// // ============================================================================
// // FILTER IMPLEMENTATIONS
// // ============================================================================

// MyVector<HoiVien*> SearchService::filterHoiVien(const MyVector<HoiVien*>& data, 
//                                                  const HoiVienFilter& filter) {
//     MyVector<HoiVien*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HoiVien* hv = data[i];
//         if (hv == nullptr) continue;
        
//         bool passFilter = true;
        
//         // Filter by rank
//         if (filter.ranks.size() > 0) {
//             Config::Points::Rank hvRank = Config::Points::getRankFromPoints(hv->getPoint());
//             bool rankMatch = false;
//             for (size_t j = 0; j < filter.ranks.size(); ++j) {
//                 if (hvRank == filter.ranks[j]) {
//                     rankMatch = true;
//                     break;
//                 }
//             }
//             if (!rankMatch) passFilter = false;
//         }
        
//         // Filter by giới tính
//         if (!filter.gioiTinh.empty() && filter.gioiTinh != "Tat ca") {
//             if (hv->getGioiTinh() != filter.gioiTinh) {
//                 passFilter = false;
//             }
//         }
        
//         // Filter by trạng thái
//         if (filter.trangThai != -1) {
//             bool isActive = hv->getIsActive();
//             if ((filter.trangThai == 1 && !isActive) || 
//                 (filter.trangThai == 0 && isActive)) {
//                 passFilter = false;
//             }
//         }
        
//         if (passFilter) {
//             result.push_back(hv);
//         }
//     }
    
//     return result;
// }

// MyVector<HLV*> SearchService::filterHLV(const MyVector<HLV*>& data, 
//                                         const HLVFilter& filter) {
//     MyVector<HLV*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HLV* hlv = data[i];
//         if (hlv == nullptr) continue;
        
//         bool passFilter = true;
        
//         // Filter by giới tính
//         if (!filter.gioiTinh.empty() && filter.gioiTinh != "Tat ca") {
//             if (hlv->getGioiTinh() != filter.gioiTinh) {
//                 passFilter = false;
//             }
//         }
        
//         // Filter by trạng thái
//         if (filter.trangThai != -1) {
//             bool isActive = hlv->getIsActive();
//             if ((filter.trangThai == 1 && !isActive) || 
//                 (filter.trangThai == 0 && isActive)) {
//                 passFilter = false;
//             }
//         }
        
//         if (passFilter) {
//             result.push_back(hlv);
//         }
//     }
    
//     return result;
// }

// MyVector<NhanVien*> SearchService::filterNhanVien(const MyVector<NhanVien*>& data, 
//                                                    const NhanVienFilter& filter) {
//     MyVector<NhanVien*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         NhanVien* nv = data[i];
//         if (nv == nullptr) continue;
        
//         bool passFilter = true;
        
//         // Filter by giới tính
//         if (!filter.gioiTinh.empty() && filter.gioiTinh != "Tat ca") {
//             if (nv->getGioiTinh() != filter.gioiTinh) {
//                 passFilter = false;
//             }
//         }
        
//         // Filter by trạng thái
//         if (filter.trangThai != -1) {
//             bool isActive = nv->getIsActive();
//             if ((filter.trangThai == 1 && !isActive) || 
//                 (filter.trangThai == 0 && isActive)) {
//                 passFilter = false;
//             }
//         }
        
//         if (passFilter) {
//             result.push_back(nv);
//         }
//     }
    
//     return result;
// }

// MyVector<GoiTap*> SearchService::filterGoiTap(const MyVector<GoiTap*>& data, 
//                                                const GoiTapFilter& filter) {
//     MyVector<GoiTap*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         GoiTap* gt = data[i];
//         if (gt == nullptr) continue;
        
//         bool passFilter = true;
        
//         // Filter by thời gian
//         if (filter.thoiGian.size() > 0) {
//             bool timeMatch = false;
//             for (size_t j = 0; j < filter.thoiGian.size(); ++j) {
//                 if (gt->getThoiGian() == filter.thoiGian[j]) {
//                     timeMatch = true;
//                     break;
//                 }
//             }
//             if (!timeMatch) passFilter = false;
//         }
        
//         // Filter by môn tập
//         if (filter.maMonTap.size() > 0) {
//             const MyVector<MonTap*>& dsMonTap = gt->getDsMonTap();
//             bool monMatch = false;
            
//             for (size_t j = 0; j < filter.maMonTap.size(); ++j) {
//                 for (size_t k = 0; k < dsMonTap.size(); ++k) {
//                     if (dsMonTap[k]->getID() == filter.maMonTap[j]) {
//                         monMatch = true;
//                         break;
//                     }
//                 }
//                 if (monMatch) break;
//             }
            
//             if (!monMatch) passFilter = false;
//         }
        
//         if (passFilter) {
//             result.push_back(gt);
//         }
//     }
    
//     return result;
// }

// MyVector<LopHoc*> SearchService::filterLopHoc(const MyVector<LopHoc*>& data, 
//                                                const LopHocFilter& filter) {
//     MyVector<LopHoc*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         LopHoc* lh = data[i];
//         if (lh == nullptr) continue;
        
//         bool passFilter = true;
        
//         // Filter by môn tập
//         if (filter.maMonTap.size() > 0) {
//             MonTap* mt = lh->getMonTap();
//             if (mt == nullptr) {
//                 passFilter = false;
//             } else {
//                 bool monMatch = false;
//                 for (size_t j = 0; j < filter.maMonTap.size(); ++j) {
//                     if (mt->getID() == filter.maMonTap[j]) {
//                         monMatch = true;
//                         break;
//                     }
//                 }
//                 if (!monMatch) passFilter = false;
//             }
//         }
        
//         // Filter by lịch học
//         if (filter.lichHoc.size() > 0) {
//             string day = extractDayFromSchedule(lh->getLichTap());
//             bool dayMatch = false;
//             for (size_t j = 0; j < filter.lichHoc.size(); ++j) {
//                 if (day == filter.lichHoc[j]) {
//                     dayMatch = true;
//                     break;
//                 }
//             }
//             if (!dayMatch) passFilter = false;
//         }
        
//         if (passFilter) {
//             result.push_back(lh);
//         }
//     }
    
//     return result;
// }

// MyVector<HangHoa*> SearchService::filterHangHoa(const MyVector<HangHoa*>& data, 
//                                                  const HangHoaFilter& filter) {
//     MyVector<HangHoa*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HangHoa* hh = data[i];
//         if (hh == nullptr) continue;
        
//         bool passFilter = true;
        
//         // Filter by loại
//         if (filter.loaiHH.size() > 0) {
//             bool loaiMatch = false;
//             for (size_t j = 0; j < filter.loaiHH.size(); ++j) {
//                 if (hh->getLoaiHH() == filter.loaiHH[j]) {
//                     loaiMatch = true;
//                     break;
//                 }
//             }
//             if (!loaiMatch) passFilter = false;
//         }
        
//         if (passFilter) {
//             result.push_back(hh);
//         }
//     }
    
//     return result;
// }

// MyVector<HoaDon*> SearchService::filterHoaDonByDateRange(const MyVector<HoaDon*>& data,
//                                                           const string& fromDate,
//                                                           const string& toDate) {
//     MyVector<HoaDon*> result;
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HoaDon* hd = data[i];
//         if (hd == nullptr) continue;
        
//         if (isDateInRange(hd->getNgayLap(), fromDate, toDate)) {
//             result.push_back(hd);
//         }
//     }
    
//     return result;
// }

// MyVector<HoaDon*> SearchService::filterHoaDonByNhanVien(const MyVector<HoaDon*>& data,
//                                                          const string& maNV) {
//     MyVector<HoaDon*> result;
    
//     if (maNV.empty()) return data; // Không filter
    
//     for (size_t i = 0; i < data.size(); ++i) {
//         HoaDon* hd = data[i];
//         if (hd == nullptr) continue;
        
//         const NhanVien* nv = hd->getNhanVien();
//         if (nv != nullptr && nv->getID() == maNV) {
//             result.push_back(hd);
//         }
//     }
    
//     return result;
// }

// // ============================================================================
// // SORT IMPLEMENTATIONS
// // ============================================================================

// void SearchService::sortGoiTapByGia(MyVector<GoiTap*>& data, SortOrder order) {
//     // Bubble sort (đơn giản và đủ dùng cho số lượng nhỏ)
//     for (size_t i = 0; i < data.size(); ++i) {
//         for (size_t j = i + 1; j < data.size(); ++j) {
//             bool swap = false;
            
//             if (order == SortOrder::ASCENDING) {
//                 swap = data[i]->getGia() > data[j]->getGia();
//             } else {
//                 swap = data[i]->getGia() < data[j]->getGia();
//             }
            
//             if (swap) {
//                 GoiTap* temp = data[i];
//                 data[i] = data[j];
//                 data[j] = temp;
//             }
//         }
//     }
// }

// void SearchService::sortHangHoaByGia(MyVector<HangHoa*>& data, SortOrder order) {
//     for (size_t i = 0; i < data.size(); ++i) {
//         for (size_t j = i + 1; j < data.size(); ++j) {
//             bool swap = false;
            
//             if (order == SortOrder::ASCENDING) {
//                 swap = data[i]->getGia() > data[j]->getGia();
//             } else {
//                 swap = data[i]->getGia() < data[j]->getGia();
//             }
            
//             if (swap) {
//                 HangHoa* temp = data[i];
//                 data[i] = data[j];
//                 data[j] = temp;
//             }
//         }
//     }
// }

// void SearchService::sortHangHoaBySoLuong(MyVector<HangHoa*>& data, SortOrder order) {
//     for (size_t i = 0; i < data.size(); ++i) {
//         for (size_t j = i + 1; j < data.size(); ++j) {
//             bool swap = false;
            
//             if (order == SortOrder::ASCENDING) {
//                 swap = data[i]->getSoLuongCon() > data[j]->getSoLuongCon();
//             } else {
//                 swap = data[i]->getSoLuongCon() < data[j]->getSoLuongCon();
//             }
            
//             if (swap) {
//                 HangHoa* temp = data[i];
//                 data[i] = data[j];
//                 data[j] = temp;
//             }
//         }
//     }
// }

// // ============================================================================
// // UTILITIES IMPLEMENTATIONS
// // ============================================================================

// string SearchService::toLowerCase(const string& str) {
//     string result = str;
//     for (size_t i = 0; i < result.length(); ++i) {
//         result[i] = tolower(result[i]);
//     }
//     return result;
// }

// bool SearchService::contains(const string& str, const string& keyword) {
//     return str.find(keyword) != string::npos;
// }

// int SearchService::compareDates(const string& date1, const string& date2) {
//     // Format: DD/MM/YYYY
//     int day1 = stoi(date1.substr(0, 2));
//     int month1 = stoi(date1.substr(3, 2));
//     int year1 = stoi(date1.substr(6, 4));
    
//     int day2 = stoi(date2.substr(0, 2));
//     int month2 = stoi(date2.substr(3, 2));
//     int year2 = stoi(date2.substr(6, 4));
    
//     if (year1 != year2) return year1 - year2;
//     if (month1 != month2) return month1 - month2;
//     return day1 - day2;
// }

// bool SearchService::isDateInRange(const string& date, const string& fromDate, const string& toDate) {
//     return compareDates(date, fromDate) >= 0 && compareDates(date, toDate) <= 0;
// }

// string SearchService::getCurrentDate() {
//     time_t now = time(0);
//     tm* ltm = localtime(&now);
    
//     int day = ltm->tm_mday;
//     int month = ltm->tm_mon + 1;
//     int year = ltm->tm_year + 1900;
    
//     string result = "";
//     if (day < 10) result += "0";
//     result += to_string(day) + "/";
//     if (month < 10) result += "0";
//     result += to_string(month) + "/";
//     result += to_string(year);
    
//     return result;
// }

// string SearchService::getStartOfWeek() {
//     time_t now = time(0);
//     tm* ltm = localtime(&now);
    
//     int dayOfWeek = ltm->tm_wday;
//     if (dayOfWeek == 0) dayOfWeek = 7; // Sunday = 7
    
//     ltm->tm_mday -= (dayOfWeek - 1);
//     mktime(ltm);
    
//     int day = ltm->tm_mday;
//     int month = ltm->tm_mon + 1;
//     int year = ltm->tm_year + 1900;
    
//     string result = "";
//     if (day < 10) result += "0";
//     result += to_string(day) + "/";
//     if (month < 10) result += "0";
//     result += to_string(month) + "/";
//     result += to_string(year);
    
//     return result;
// }

// string SearchService::getStartOfMonth() {
//     time_t now = time(0);
//     tm* ltm = localtime(&now);
    
//     int month = ltm->tm_mon + 1;
//     int year = ltm->tm_year + 1900;
    
//     string result = "01/";
//     if (month < 10) result += "0";
//     result += to_string(month) + "/";
//     result += to_string(year);
    
//     return result;
// }

// string SearchService::getStartOfLastMonth() {
//     time_t now = time(0);
//     tm* ltm = localtime(&now);
    
//     int month = ltm->tm_mon;
//     int year = ltm->tm_year + 1900;
    
//     if (month == 0) {
//         month = 12;
//         year--;
//     }
    
//     string result = "01/";
//     if (month < 10) result += "0";
//     result += to_string(month) + "/";
//     result += to_string(year);
    
//     return result;
// }

// string SearchService::extractDayFromSchedule(const string& schedule) {
//     // VD: "T4-18:00" -> "T4"
//     size_t pos = schedule.find('-');
//     if (pos != string::npos) {
//         return schedule.substr(0, pos);
//     }
//     return schedule;
// }