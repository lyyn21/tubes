#include <iostream>
#include <string>
#include <limits>
#include <cstdlib> 
#include "music_player.h" 

using namespace std;

// --- VARIABEL GLOBAL ---
SongList library;       
SongList myPlaylist;    
Node* currentPlaying = nullptr; 
bool isPlayingFromPlaylist = false; 

// --- AKUN LOGIN (Hardcoded) ---
const string ADMIN_USER = "admin";
const string ADMIN_PASS = "admin123";
const string USER_USER = "user";
const string USER_PASS = "user123";

// Membersihkan layar terminal (Cross-platform)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Membersihkan buffer input (sisa enter/karakter sampah)
void cleanBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Menunggu user menekan enter
void pressEnter() {
    cout << "\nTekan Enter untuk kembali...";
    cleanBuffer(); 
    cin.get();
}

// --- LOGIKA PEMUTAR MUSIK ---

void playSong(Node* songNode, bool fromPlaylist) {
    if (songNode == nullptr) return;
    
    currentPlaying = songNode;
    isPlayingFromPlaylist = fromPlaylist;
    
    cout << "\n[SEDANG MEMUTAR] " << (fromPlaylist ? "(Playlist)" : "(Library)") << "\n";
    cout << "Judul  : " << currentPlaying->data.title << "\n";
    cout << "Artis  : " << currentPlaying->data.artist << "\n";
    cout << "Album  : " << currentPlaying->data.album << "\n";
    cout << "Tahun  : " << currentPlaying->data.year << "\n";
    cout << "-----------------------------------\n";
}

void stopSong() {
    if (currentPlaying) {
        cout << "\n[BERHENTI] Lagu " << currentPlaying->data.title << " dihentikan.\n";
        currentPlaying = nullptr;
    } else {
        cout << "Tidak ada lagu yang sedang diputar.\n";
    }
}

void playNext() {
    if (currentPlaying == nullptr) {
        cout << "Putar lagu terlebih dahulu.\n";
        return;
    }

    Node* nextSong = nullptr;

    if (isPlayingFromPlaylist) {
        // Logika Playlist: Lanjut ke node berikutnya di linked list playlist
        if (currentPlaying->next != nullptr) {
            nextSong = currentPlaying->next;
            playSong(nextSong, true);
        } else {
            cout << "Akhir dari playlist. Memutar ulang dari awal playlist...\n";
            playSong(myPlaylist.getHead(), true); 
        }
    } else {
        // Logika Library: Cari lagu yang MIRIP (Rekomendasi)
        cout << "Mencari rekomendasi lagu serupa (Artis/Genre sama)...\n";
        nextSong = library.findSimilar(currentPlaying);
        
        if (nextSong) {
            cout << "Rekomendasi ditemukan!\n";
            playSong(nextSong, false); 
        } else {
            cout << "Tidak ditemukan lagu mirip. Memutar lagu selanjutnya di library.\n";
            // Fallback: putar lagu selanjutnya di list, atau balik ke awal jika habis
            if (currentPlaying->next) playSong(currentPlaying->next, false);
            else playSong(library.getHead(), false);
        }
    }
}

void playPrev() {
     if (currentPlaying == nullptr) return;
     
     if (currentPlaying->prev != nullptr) {
         playSong(currentPlaying->prev, isPlayingFromPlaylist);
     } else {
         cout << "Ini adalah lagu pertama.\n";
     }
}

// --- MENU ADMIN ---
void adminMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== MENU ADMIN ===\n";
        cout << "1. Tambah Lagu Baru\n";
        cout << "2. Lihat Semua Lagu (Library)\n";
        cout << "3. Edit Data Lagu\n";
        cout << "4. Hapus Lagu\n";
        cout << "0. Logout\n";
        cout << "Pilihan: "; 
        
        // Validasi input angka
        if (!(cin >> choice)) {
            cin.clear(); 
            cleanBuffer(); 
            choice = -1; 
        } else {
            cleanBuffer(); // Bersihkan sisa enter setelah input angka
        }

        if (choice == 1) {
            Song s;
            cout << "ID Lagu (Angka Unik): "; cin >> s.id; cleanBuffer();
            cout << "Judul: "; getline(cin, s.title);
            cout << "Artis: "; getline(cin, s.artist);
            cout << "Genre: "; getline(cin, s.genre);
            cout << "Album: "; getline(cin, s.album);
            cout << "Tahun: "; cin >> s.year; cleanBuffer();
            
            library.addSong(s);
            cout << "Lagu berhasil ditambahkan!\n";
            cout << "\nTekan Enter..."; cin.get(); 
        } else if (choice == 2) {
            library.displayAll();
            cout << "\nTekan Enter..."; cin.get();
        } else if (choice == 3) {
            int id;
            library.displayAll();
            cout << "Masukkan ID Lagu yang ingin diedit: "; cin >> id; cleanBuffer();
            library.updateSong(id);
            cout << "\nTekan Enter..."; cin.get();
        } else if (choice == 4) {
            int id;
            library.displayAll();
            cout << "Masukkan ID Lagu yang ingin dihapus: "; cin >> id; cleanBuffer();
            library.deleteSong(id);
            myPlaylist.deleteSong(id);
            cout << "\nTekan Enter..."; cin.get();
        }
    } while (choice != 0);
}

// --- MENU USER ---
void userMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== MENU USER ===\n";
        if (currentPlaying) {
            cout << "[SEDANG MEMUTAR]: " << currentPlaying->data.title << " - " << currentPlaying->data.artist << "\n";
            cout << "   (11. Stop | 12. Next | 13. Prev)\n\n";
        }
        
        cout << "1. Lihat Library Lagu\n";
        cout << "2. Cari Lagu\n";
        cout << "3. Kelola Playlist\n";
        cout << "4. Putar Lagu dari ID\n";
        cout << "0. Logout\n";
        cout << "Pilihan: "; 
        
        if (!(cin >> choice)) {
            cin.clear();
            cleanBuffer();
            choice = -1;
        } else {
            cleanBuffer();
        }

        if (choice == 1) {
            library.displayAll();
            cout << "\nTekan Enter..."; cin.get();
        } else if (choice == 2) {
            string key;
            cout << "Masukkan Kata Kunci: "; getline(cin, key);
            library.searchSong(key);
            cout << "\nTekan Enter..."; cin.get();
        } else if (choice == 3) {
            int pChoice;
            cout << "\n-- Playlist Manager --\n";
            cout << "1. Lihat Playlist\n";
            cout << "2. Tambah Lagu ke Playlist\n";
            cout << "3. Hapus Lagu dari Playlist\n";
            cout << "4. Putar Playlist\n";
            cout << "Pilihan: "; cin >> pChoice; cleanBuffer();
            
            if (pChoice == 1) myPlaylist.displayAll();
            else if (pChoice == 2) {
                library.displayAll();
                int id;
                cout << "ID Lagu: "; cin >> id; cleanBuffer();
                Node* songNode = library.findSongById(id);
                if (songNode) { 
                    myPlaylist.addSong(songNode->data); 
                    cout << "Berhasil masuk playlist!\n"; 
                }
                else cout << "ID Salah/Tidak Ditemukan.\n";
            } else if (pChoice == 3) {
                myPlaylist.displayAll();
                int id;
                cout << "ID Hapus: "; cin >> id; cleanBuffer();
                myPlaylist.deleteSong(id);
            } else if (pChoice == 4) {
                if (!myPlaylist.isEmpty()) playSong(myPlaylist.getHead(), true);
                else cout << "Playlist kosong.\n";
            }
            cout << "\nTekan Enter..."; cin.get();
        } else if (choice == 4) {
            int id;
            cout << "ID Lagu: "; cin >> id; cleanBuffer();
            Node* target = library.findSongById(id);
            if (target) playSong(target, false); 
            else cout << "Tidak ketemu.\n";
            cout << "\nTekan Enter..."; cin.get();
        } else if (choice == 11) { stopSong(); cout << "\nTekan Enter..."; cin.get(); }
        else if (choice == 12) { playNext(); cout << "\nTekan Enter..."; cin.get(); }
        else if (choice == 13) { playPrev(); cout << "\nTekan Enter..."; cin.get(); }

    } while (choice != 0);
}

// --- MAIN PROGRAM (Titik Awal Eksekusi) ---
int main() {
    // 1. Inisialisasi Data Dummy (Supaya pas run tidak kosong melompong)
    library.addSong({1, "Hati-Hati di Jalan", "Tulus", "Pop", "Manusia", 2022});
    library.addSong({2, "Gajah", "Tulus", "Pop", "Gajah", 2014});
    library.addSong({3, "Enter Sandman", "Metallica", "Metal", "Black Album", 1991});
    library.addSong({4, "Sang Dewi", "Lyodra", "Pop", "Single", 2022});

    // 2. Loop Utama Aplikasi (Login Screen)
    while (true) {
        clearScreen();
        cout << "===========================================\n";
        cout << "   APLIKASI PEMUTAR MUSIK (TUBES STRUKDAT) \n";
        cout << "===========================================\n";
        cout << "Silakan Login.\n(Ketik 'exit' di username untuk keluar aplikasi)\n\n";
        
        string username, password;
        
        cout << "Username: "; 
        cin >> username;
        
        if (username == "exit" || username == "keluar") {
            cout << "Terima kasih, sampai jumpa!\n";
            break;
        }

        cout << "Password: "; 
        cin >> password;
        cleanBuffer();

        // Logika Pengecekan Akun
        if (username == ADMIN_USER && password == ADMIN_PASS) {
            cout << "\n[SUKSES] Login sebagai ADMIN!\n";
            cout << "Tekan Enter untuk lanjut..."; cin.get();
            adminMenu(); // Masuk ke fungsi menu admin
        } 
        else if (username == USER_USER && password == USER_PASS) {
            cout << "\n[SUKSES] Login sebagai USER!\n";
            cout << "Tekan Enter untuk lanjut..."; cin.get();
            userMenu(); // Masuk ke fungsi menu user
        } 
        else {
            cout << "\n[GAGAL] Username atau Password salah!\n";
            cout << "Tekan Enter untuk coba lagi..."; cin.get();
        }
    }
    return 0;
}