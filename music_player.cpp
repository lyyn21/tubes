#include "music_player.h" 
#include <iostream>
#include <string>
#include <limits> 
#include <iomanip>

using namespace std;


// 1. Constructor
SongList::SongList() {
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// 2. Cek apakah list kosong
bool SongList::isEmpty() {
    return head == nullptr;
}

// 3. Cari Node lagu berdasarkan ID
Node* SongList::findSongById(int id) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// 4. Tambah Lagu
void SongList::addSong(Song newSong) {
    if (findSongById(newSong.id) != nullptr) {
        cout << "[ERROR] Lagu dengan ID " << newSong.id << " sudah ada!\n";
        return;
    }

    Node* newNode = new Node;
    newNode->data = newSong;
    newNode->next = nullptr;
    newNode->prev = tail;

    if (isEmpty()) {
        head = newNode;
    } else {
        tail->next = newNode;
    }
    tail = newNode;
    size++;
}

// 5. Hapus Lagu
void SongList::deleteSong(int id) {
    Node* target = findSongById(id);
    if (target == nullptr) {
        cout << "[INFO] Lagu dengan ID " << id << " tidak ditemukan.\n";
        return;
    }

    if (target == head && target == tail) { 
        head = nullptr;
        tail = nullptr;
    } else if (target == head) { 
        head = head->next;
        head->prev = nullptr;
    } else if (target == tail) { 
        tail = tail->prev;
        tail->next = nullptr;
    } else { 
        target->prev->next = target->next;
        target->next->prev = target->prev;
    }

    delete target;
    size--;
    cout << "[SUKSES] Lagu berhasil dihapus.\n";
}

// 6. Tampilkan Semua Lagu (SUDAH DIRAPIKAN)
void SongList::displayAll() {
    if (isEmpty()) {
        cout << "(Library/Playlist Kosong)\n";
        return;
    }
    Node* current = head;

    // Header Tabel
    cout << "--------------------------------------------------------------------------------------\n";
    cout << left 
         << setw(5) << "ID" 
         << "| " << setw(25) << "Judul Lagu" 
         << "| " << setw(20) << "Artis" 
         << "| " << setw(15) << "Genre" 
         << "| " << setw(6) << "Tahun" 
         << endl;
    cout << "--------------------------------------------------------------------------------------\n";
    
    // Isi Tabel
    while (current != nullptr) {
        cout << left 
             << setw(5) << current->data.id 
             << "| " << setw(25) << current->data.title.substr(0, 24) 
             << "| " << setw(20) << current->data.artist.substr(0, 19)
             << "| " << setw(15) << current->data.genre.substr(0, 14)
             << "| " << setw(6) << current->data.year 
             << endl;
        current = current->next;
    }
    cout << "--------------------------------------------------------------------------------------\n";
}

// 7. Ambil Head
Node* SongList::getHead() { 
    return head; 
}

// 8. Update Data Lagu
void SongList::updateSong(int id) {
    Node* target = findSongById(id);
    if (target == nullptr) {
        cout << "Lagu tidak ditemukan.\n";
        return;
    }
    
    cout << "--- UPDATE DATA (Tekan Enter jika tidak ingin mengubah data) ---\n";
    string input;

    cout << "Judul Baru (" << target->data.title << "): "; 
    getline(cin, input);
    if (!input.empty()) target->data.title = input;

    cout << "Artis Baru (" << target->data.artist << "): "; 
    getline(cin, input);
    if (!input.empty()) target->data.artist = input;

    cout << "Genre Baru (" << target->data.genre << "): "; 
    getline(cin, input);
    if (!input.empty()) target->data.genre = input;
    
    cout << "Album Baru (" << target->data.album << "): "; 
    getline(cin, input);
    if (!input.empty()) target->data.album = input;

    cout << "Tahun Baru (" << target->data.year << "): "; 
    string yearInput;
    getline(cin, yearInput);
    if (!yearInput.empty()) {
        try {
            target->data.year = stoi(yearInput);
        } catch (...) {
            cout << "Input tahun tidak valid.\n";
        }
    }
    cout << "[SUKSES] Data berhasil diperbarui.\n";
}

// 9. Cari Lagu (SUDAH DIRAPIKAN)
void SongList::searchSong(string keyword) {
    Node* current = head;
    bool found = false;
    cout << "\n--- HASIL PENCARIAN '" << keyword << "' ---\n";
    
    while (current != nullptr) {
        if (current->data.title.find(keyword) != string::npos || 
            current->data.artist.find(keyword) != string::npos) {
            
            if (!found) { 
                cout << "------------------------------------------------------------------\n";
                cout << left << setw(25) << "Judul" << "| " << setw(20) << "Artis" << " | ID" << endl;
                cout << "------------------------------------------------------------------\n";
            }

            cout << left 
                 << setw(25) << current->data.title.substr(0, 24)
                 << "| " << setw(20) << current->data.artist.substr(0, 19)
                 << " | " << current->data.id << endl;
            found = true;
        }
        current = current->next;
    }
    
    if (found) cout << "------------------------------------------------------------------\n";
    else cout << "Tidak ada lagu yang cocok.\n";
}

// 10. Rekomendasi (LOGIKA DIUBAH: GENRE DULU, BARU ARTIS)
Node* SongList::findSimilar(Node* currentSong) {
    if (!currentSong) return nullptr;
    
    Node* temp = head; 
    
    // PRIORITAS 1: Cari GENRE yang sama dulu
    while (temp != nullptr) {
        if (temp != currentSong) { 
            if (temp->data.genre == currentSong->data.genre) return temp;
        }
        temp = temp->next;
    }

    // PRIORITAS 2: Jika tidak ada Genre sama, cari ARTIS yang sama
    temp = head;
    while (temp != nullptr) {
        if (temp != currentSong) {
            if (temp->data.artist == currentSong->data.artist) return temp;
        }
        temp = temp->next;
    }

    return nullptr;
}