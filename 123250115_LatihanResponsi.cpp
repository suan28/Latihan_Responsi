#include <iostream>
using namespace std;

struct Menu{
    string nama, kategori, status;
    int harga;
    Menu *left, *right;
};

struct Waiting{
    string pelanggan, menu;
    Waiting *next;
};

struct Undo{
    string aksi, menu;
    Undo *next;
};

Menu *root = nullptr;
Waiting *frontQ = nullptr, *rearQ = nullptr;
Undo *top = nullptr;

Menu* insert(Menu* r,string n,string k,int h){
    if (!r){
        Menu* b=new Menu{n , k, "Tersedia", h, nullptr, nullptr};
        return b;
    }

    if(n<r->nama) r->left=insert(r->left,n,k,h);
    else if(n>r->nama) r->right=insert(r->right,n,k,h);

    return r;
}

Menu* cari(Menu* r,string n){
    if(!r||r->nama==n) return r;
    if(n<r->nama) return cari(r->left,n);
    return cari(r->right,n);
}

void tampil(Menu* r){
    if(!r) return;

    tampil(r->left);

    cout<<"\n"<<r->nama
        <<" | "<<r->kategori
        <<" | "<<r->harga
        <<" | "<<r->status;

    tampil(r->right);
}

Menu* minNode(Menu* r){
    while(r->left) r=r->left;
    return r;
}

Menu* hapus(Menu* r,string n){
    if(!r) return r;

    if(n<r->nama) r->left=hapus(r->left,n);
    else if(n>r->nama) r->right=hapus(r->right,n);
    else{
        if(!r->left){
            Menu* t=r->right;
            delete r;
            return t;
        }
        if(!r->right){
            Menu* t=r->left;
            delete r;
            return t;
        }

        Menu* t=minNode(r->right);

        r->nama=t->nama;
        r->kategori=t->kategori;
        r->harga=t->harga;
        r->status=t->status;

        r->right=hapus(r->right,t->nama);
    }
    return r;
}

// Queue
void enqueue(string p,string m){
    Waiting *b=new Waiting{p,m,nullptr};

    if(!frontQ)
        frontQ=rearQ=b;
    else{
        rearQ->next=b;
        rearQ=b;
    }
}

bool dequeueMenu(string menu,string &pelanggan){
    Waiting *cur=frontQ,*prev=nullptr;

    while(cur){
        if(cur->menu==menu){

            pelanggan=cur->pelanggan;

            if(!prev) frontQ=cur->next;
            else prev->next=cur->next;

            if(cur==rearQ) rearQ=prev;

            delete cur;
            return true;
        }

        prev=cur;
        cur=cur->next;
    }
    return false;
}

void tampilAntrian(){
    if(!frontQ){
        cout<<"\nAntrian kosong\n";
        return;
    }

    Waiting* p=frontQ;

    while(p){
        cout<<p->pelanggan
            <<" menunggu menu "
            <<p->menu<<endl;
        p=p->next;
    }
}

// Stack
void push(string aksi,string menu){
    Undo *b=new Undo{aksi,menu,top};
    top=b;
}

void undo(){
    if(!top){
        cout<<"\nTidak ada aksi untuk di-undo\n";
        return;
    }

    Undo* t=top;
    top=top->next;

    if(t->aksi=="PESAN")
        cout<<"\nUndo: Pesanan menu "
            <<t->menu<<" dibatalkan\n";
    else{
        Menu* m=cari(root,t->menu);
        if(m) m->status="Habis";

        cout<<"\nUndo: Menu "
            <<t->menu
            <<" kembali habis\n";
    }

    delete t;
}

void tambahMenu(){
    string n,k;
    int h;

    cin.ignore();

    cout<<"Nama Menu : ";
    getline(cin,n);

    cout<<"Kategori : ";
    getline(cin,k);

    cout<<"Harga : ";
    cin>>h;

    root=insert(root,n,k,h);

    cout<<"Menu berhasil ditambahkan\n";
}

void pesan(){
    string p,m;

    cin.ignore();

    cout<<"Nama Pelanggan : ";
    getline(cin,p);

    cout<<"Nama Menu : ";
    getline(cin,m);

    Menu* menu=cari(root,m);

    if(!menu){
        cout<<"Menu tidak ditemukan\n";
        return;
    }

    if(menu->status=="Tersedia"){
        push("PESAN",m);
        cout<<"Pesanan "<<p<<" berhasil dibuat\n";
    }else{
        enqueue(p,m);
        cout<<"Menu sedang habis, "
            <<p
            <<" masuk ke waiting list\n";
    }
}

void menuTersedia(){
    string n;

    cin.ignore();

    cout<<"Nama Menu : ";
    getline(cin,n);

    Menu* m=cari(root,n);

    if(!m){
        cout<<"Menu tidak ditemukan\n";
        return;
    }

    m->status="Tersedia";
    push("UPDATE",n);

    cout<<"Menu tersedia kembali\n";

    string p;

    if(dequeueMenu(n,p))
        cout<<"Pesanan langsung diberikan kepada "
            <<p<<" dari waiting list\n";
}

void menuHabis(){
    string n;

    cin.ignore();

    cout<<"Nama Menu : ";
    getline(cin,n);

    Menu* m=cari(root,n);

    if(m){
        m->status="Habis";
        cout<<"Status menu menjadi Habis\n";
    }
}

void hapusMenu(){
    string n;

    cin.ignore();

    cout<<"Nama Menu : ";
    getline(cin,n);

    if(!cari(root,n)){
        cout<<"Menu tidak ditemukan\n";
        return;
    }

    root=hapus(root,n);

    cout<<"Menu berhasil dihapus\n";
}

int main(){
    int pilih;

    do{
        cout<<"\n=== SISTEM RESTORAN ===";
        cout<<"\n1. Tambah Menu";
        cout<<"\n2. Lihat Menu";
        cout<<"\n3. Catat Pesanan";
        cout<<"\n4. Menu Tersedia Kembali";
        cout<<"\n5. Hapus Menu";
        cout<<"\n6. Undo";
        cout<<"\n7. Waiting List";
        cout<<"\n8. Menu Habis";
        cout<<"\n9. Keluar";
        cout<<"\nPilihan : ";
        cin>>pilih;

        switch(pilih){
            case 1: tambahMenu(); break;
            case 2: tampil(root); break;
            case 3: pesan(); break;
            case 4: menuTersedia(); break;
            case 5: hapusMenu(); break;
            case 6: undo(); break;
            case 7: tampilAntrian(); break;
            case 8: menuHabis(); break;
        }

    }while(pilih!=9);

    return 0;
}