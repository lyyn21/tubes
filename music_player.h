#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <string>
#include <iostream>

using namespace std;

// --- STRUKTUR DATA (RECORD) ---
struct Song {
    int id;
    string title;
    string artist;
    string genre;
    string album;
    int year;
};

// --- NODE UNTUK DOUBLY LINKED LIST ---
struct Node {
    Song data;
    Node* next;
    Node* prev;
};

// --- CLASS: DOUBLY LINKED LIST ---
class SongList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    SongList(); 
    
    bool isEmpty();
    Node* findSongById(int id);
    void addSong(Song newSong);
    void deleteSong(int id);
    void displayAll();
    Node* getHead();
    void updateSong(int id);
    void searchSong(string keyword);
    Node* findSimilar(Node* currentSong);
};

#endif