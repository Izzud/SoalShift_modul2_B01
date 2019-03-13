# SoalShift_modul2_B01
#### Soal Shift Modul 2 Sistem Operasi 2019

## Soal 1

1. Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori `/home/[user]/modul2/gambar`.
   
    ``Catatan : Tidak boleh menggunakan crontab.``

<br>

### Penjelasan program

```
#define suffix "_grey.png"

int isNotRenamed(char* filename, int len){
  if(len<9)
    return 1;

  const char *last_nine;
  last_nine = &filename[len-9];

  if(!strcmp(last_nine,suffix))
      return 0;
  return 1;
}
```
* Pertama, buat fungsi dengan input nama file dan panjang stringnya untuk mengecek apakah file ".png" tersebut telah di rename sesuai dengan format.
* Apabila panjang string nama file kurang dari 9, akan langsung me-*return* nilai 1 atau `true`, karena pasti file tersebut belum di-*rename*
* Variabel `last_nine` berisi pointer menuju elemen ke-9 dari akhir string.
  * Misalnya string = "Red riding hood.png", maka `last nine` **adalah pointer menuju elemen ke-10**, atau `string[9]`. Jika `puts (lastnine)` dijalankan maka outputnya adalah " hood.png".
* Cek `last nine` dengan string "_grey.png" menggunakan `strcmp`
  * Jika dicek dan ternyata file tersebut telah di-*rename*, maka fungsi akan me-*return* 0 atau `false`
  * Jika tidak, fungsi akan me-*return* 1 atau `true`

```
if ((chdir("/home/izzud/modul2")) < 0) {
    exit(EXIT_FAILURE);
}
```

* Kita **menentukan dimana *working directory* dari *daemon process***, dalam kasus ini berada pada direktori `/home/[user]/modul2/gambar`. Jika direktori tidak ada atau tidak dapat diakses, program akan keluar.

```
d = opendir(".");
```

* Diambil dari **manpage opendir**: 
  ```
  The opendir() function opens a directory stream corresponding to the directory name, and returns a pointer to the directory stream. 
  The stream is positioned at the first entry in the directory.
  ```
  `opendir(".")` akan **mengembalikan pointer `directory stream` dari "." atau *working directory*** yang kemudian disimpan pada variabel `d`.

```
if (d){
  while ((dir = readdir(d)) != NULL){
      ...
  }
 closedir(d);
}
```

* Perlu dicatat, bahwa `opendir()` akan me-*return* pointer **NULL** apabila terjadi error pada saat membuka direktori tersebut. Maka, blok while hanya akan di eksekusi apabila folder `/gambar` dapat dibuka.
* `readdir()` akan mengembalikan pointer dari *struct* bertipe *dirent* yang berupa *directory entry* selanjutnya dalam *directory stream*  yang ditunjuk oleh variabel d. Jika sudah mencapai akhir dari `directory stream` atau terjadi erro, pointer NULL akan di-*return*. 
* *Struct* `dir` ini selanjutnya akan berisi informasi `directory entry` yang salah satunya berupa *filename* atau *directory name* yang disimpan dalam variabel `d_name`.
* *Directory stream* lalu ditutup setelah selesai.
  
    [further read](http://pubs.opengroup.org/onlinepubs/009695399/functions/readdir_r.html)

```
memset(newname,0,200);
```
* Mengosongkan variabel **newname** yang digunakan untuk menampung nama file yang baru.

```
filename = dir->d_name;
len = strlen(filename);
```
* **Copy** isi variabel `d_name` yang **berisi string nama file/direktori** ke variabel `filename`. Lalu dapatkan panjang dari string tersebut.

```
ext = strrchr(filename, '.');
```
* `strrchr()` akan **me-*return* pointer dari *last occurence* sebuah karakter dalam string**. 
  * Misalnya string = "Mary had a little lamb.jpg_orig.jpg" maka yang di-*return* adalah pointer __index ke 32 dari array string__ atau `string[31]`, sehingga ketika `printf("%s", ext)` dijalankan akan menghasilkan output `.jpg`.

```
if(ext && strlen(ext)==4 && !strcmp(ext,".png") && isNotRenamed(filename, len)){
    ...
}
```
* *Condition* pertama: `ext` tidaklah **NULL**
* *Condition* kedua: panjang `ext` harus 4 untuk menghindari error saat `strcmp` dijalankan
* *Condition* ketiga: harus ekstensinya harus `.png`

```
if(isNotRenamed(filename, len)){
  char fname[200] = {0};
  strncpy(fname,filename, len-4);
  snprintf(newname, 200, "gambar/%s_grey.png", fname);
}
else{
  snprintf(newname, 200, "gambar/%s", filename);
}
```
* Cek terlebih dahulu **apakah file sudah pernah di*rename* sesuai format** dengan memanggil fungsi `isNotRenamed()`
  * Jika file belum di-*rename*:
    * Membuat variabel `fname` untuk menampung nama file **tanpa ekstensi**.
    * `snprintf` akan membuat string sesuai dengan format yang dispesifikasikan, mirip seperti `printf` tetapi output string tersebut akan disimpan ke dalam array dengan panjang maksimal 200.
  * Jika file sudah di-*rename*, maka cukup menyimpan string `gambar/[filename]` ke array `newname` menggunakan `snprintf`
* `rename` tidak hanya digunakan untuk **mengganti nama file**, namun dapat digunakan juga untuk **memindahkan file**, dengan format `[path to directory]/filename` 

#### [Source code program](https://github.com/Izzud/SoalShift_modul2_B01/blob/master/soal1.c)

<br>

## Soal 2

1. Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama `elen.ku` pada direktori `hatiku`. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C **yang bisa mendeteksi owner dan group dan menghapus file `elen.ku` setiap 3 detik** dengan syarat **ketika owner dan grupnya menjadi `www-data`**. Ternyata kamu memiliki kendala karena permission pada file `elen.ku`. Jadi, **ubahlah permissionnya menjadi 777**. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on. 
    
    `Catatan: Tidak boleh menggunakan crontab`

<br>

### Penjelasan program

```
if ((chdir("/home/izzud/Documents/SoalShift_modul2_B01/hatiku")) < 0) {
    exit(EXIT_FAILURE);
}
```

* Kita **men-spesifikasikan dimana *working directory*** dari *daemon process*, dalam kasus ini berada pada direktori `/home/izzud/Documents/SoalShift_modul2_B01/hatiku`. Jika *directory* tidak ada atau tidak dapat diakses, program akan keluar.

```
char filename[] = {"elen.ku"};
struct stat sb;

if(stat(filename, &sb) == 0){
    ...
}
```
* Diambil dari **manpage stat**: 
  ```
  These functions return information about a file, in the buffer pointed to by statbuf. 
  No permissions are required on the file itself, but—in the case of stat(), fstatat(), and lstat()—execute (search) permission is required on all of the directories in pathname that lead to the file.

  stat() and fstatat() retrieve information about the file pointed to by pathname
  ```
  `stat(filename, &sb)` akan **menyimpan infromasi dari file** (yang ada di dalam pada variabel `filename`) ke dalam *struct* `sb` dengan tipe *stat*, dan me-*return* nilai 0 apabila proses berhasil.

```
mode_t perm = sb.st_mode | 0777;
chmod(filename, perm);
```
* Variabel `st_mode` berisi tipe dan *permission mode* dari file tersebut.
* Untuk mengubah *permission mode*nya menjadi 777, pertama kita perlu untuk melakukan `bitwise OR` operation dengan bilangan **octal** `0777`. Karena kita tidak ingin mengubah bit yang lain, hanya **9-bit terakhir** menjadi 777.
* Update *permission mode* file dengan command `chmod`

```
struct passwd *pw = getpwuid(sb.st_uid);
struct group  *gr = getgrgid(sb.st_gid);

owner = pw->pw_name;
group = gr->gr_name;
```
* Selain variabel `st_mode`, di dalam *struct* `sb` juga terdapat variabel `st_uid` yang **menyimpan user ID dari *owner* file** dan `st_gid` yang **menyimpan group ID dari *group owner* file**.
* `getpwuid` akan **mencocokkan ID dari file owner** dengan *records* pada database password dan **mengembalikan pointer ke *struct* yang berisi informasi mengenai user dengan *uid* tersebut**. 
* Sedangkan `getgrgid` akan **mencocokkan ID dari group owner** dengan *records* pada database group dan **mengembalikan pointer ke *struct* yang berisi informasi mengenai group dengan *gid*** tersebut.
* Lalu kita simpan isi dari variabel `pw_name` yang berisi nama owner ke variabel `owner` dan `gr_name` yang berisi nama group owner ke variabel `group`

```
if(!strcmp(owner, keyword) && !strcmp(group, keyword)){
    remove(filename);
}
```
* *Condition* pertama: `group` berisi *keyword*: `www-data`.
* *Condition* kedua: `owner` berisi *keyword*: `www-data`.
* Jika semua kondisi terpenuhi, maka file `elen.ku` akan dihapus

```
sleep(3);
```
* Program akan berhenti sebentar selama 3 detik, lalu memulai loop kembali.
  
#### [Source code program](https://github.com/Izzud/SoalShift_modul2_B01/blob/master/soal2.c)

<br>

## Soal 5

5. Kerjakan poin a dan b di bawah:
Buatlah program c untuk mencatat log setiap menit dari file log pada `syslog` ke `/home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log` Ket:
   1. Per 30 menit membuat folder `/[dd:MM:yyyy-hh:mm]` Per menit memasukkan `log#.log` ke dalam folder tersebut ‘#’ : increment per menit. Mulai dari 1
   2. Buatlah program c untuk menghentikan program di atas.
   
    NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

<br>

### Penjelasan program

#### 5a.
```
char* getDate(){
    time_t raw;
    struct tm *timeinfo;
    
    char *re = (char *)calloc(20, sizeof(char));

    time(&raw);                                     
    timeinfo = localtime(&raw);                     

    strftime(re, 20, "%d:%m:%Y-%R", timeinfo);    

    return re;
}
```
* Pertama, membuat function getDate() yang akan **me-*return* string tanggal dengan format `dd:MM:yyyy-hh:mm`**
* **Alokasikan array `re` secara dinamis** sehingga nilainya dapat dikembalikan ke fungsi `main()`
* **Dapatkan jam pada saat fungsi dipanggil menggunakan `time`**, hasilnya yang bertipe `time_t` akan disimpan ke variabel `raw`
* Variabel `raw` kemudian akan dipass ke dalam fungsi `localtime` yang akan mengembalikan *struct* yang berisi representasi *timer* (dalam hal ini variabel `raw`) dalam *local time*.
* Lalu menggunakan `strftime` kita dapat mengambil data tanggal, waktu, bulan dan tahun dalam format `dd:MM:yyyy-hh:mm`. Untuk formatnya, kita dapat menggunakan *format specifier*: 
  * `%d sebagai pengganti tanggal`
  * `%m sebagai pengganti bulan`
  * `%Y sebagai pengganti tahun` 
  * `%R yang ekuivalen dengan waktu dengan format hh:mm (24 jam)`

```
void copyToDirectory(char* directory){
    FILE *source, *out;
    int i = 1;
    char    ch,
            newname[28],
            src[]={"/var/log/syslog"};

    while(i<=30){
      ...
    }
}
```
* Lalu kita membuat fungsi untuk menyalin isi file `/var/log/syslog` ke dalam file `log#.log` (dengan `#` adalah *increment*) di direktori yang telah dibuat. File log tersebut akan dibuat 1 kali setiap menitnya dan diulang sebanyak 30 kali.
* Pertama, inisiasi variabel yang dibutuhkan:
  * Pointer ke file log di `/var/log/syslog` dan file output
  * `i` sebagai counter
  * `ch` sebagai menyimpan karakter yang akan disalin
  * Array `newname` sebagai nama file output

```
 while(i<=30){
    source = fopen(src, "r");
    snprintf(newname, 28, "%s/log%d.log", directory, i);

    out = fopen(newname, "w");

    if(source != NULL && out != NULL){
        while ((ch = fgetc(source)) != EOF)
              fputc(ch, out);

         fclose(out);
    }

    i++;

    fclose(source);
    sleep(60);  
}
```
* Buka file log di `/var/log/syslog` dengan mode "r" atau *read-only*, simpan pointernya di variabel `source`
* Masukkan nama file yang tujuan dengan format `[directory]/log#.log` (dengan `#` adalah nilai variabel `i`) menggunakan `snprintf` ke dalam array `newname`
* Buka file baru dengan format yang ada di dalam `newname` dengan mode "w", sehingga terbuat file kosong baru. Simpan pointernya ke dalam variabel `out`
* Apabila telah berhasil membuka kedua file, maka block `while` akan dijalankan:
  * `fgetc` akan **mengambil setiap karakter dari file *source* sampai EOF**
  * Karakter yang telah diambil kemudian akan **disalin ke file output oleh `fputc`**
* **Close** *file stream* untuk output dan input.
*  Nilai variabel `i` akan ditambah 1;
*  Program lalu **diberhentikan untuk sementara selama 60 detik** menggunakan `sleep(60)`, dan akan memulai loop kembali sebanyak 30 kali.

Kembali ke fungsi main():

```
if ((chdir("/home/izzud/Documents/SoalShift_modul2_B01")) < 0) {
    exit(EXIT_FAILURE);
}
```
* Seperti biasa, kita tentukan *working directory* dari proses ini, dalam kasus ini berada pada direktori `/home/izzud/Documents/SoalShift_modul2_B01`. Jika *directory* tidak ada atau tidak dapat diakses, program akan keluar.

```
char *date = NULL;
date = getDate();
```
* Kita panggil fungsi `getDate()` untuk mendapatkan nama direktori yang harus dibuat

```
if(!mkdir(date, S_IRWXU | S_IRWXO | S_IRWXG))
    copyToDirectory(date);
```
* Lalu buat direktori sesuai nama yang telah didapat dengan *permission mode* 777 dengan cara menggunakan `bitwise OR` operation terhadap:
  * `S_IRWXU` untuk **read, write dan execute permission bagi *owner***
  * `S_IRWXG` untuk **read, write dan execute permission bagi *group***
  * `S_IRWXO` untuk **read, write dan execute permission bagi *other* atau user lain**
* Jika berhasil membuat direktori, jalankan fungsi `copyToDirectory` untuk membuat file-file log yang diambil dari `/var/log/syslog` ke dalam folder tersebut
```
free(date);
```
* Karena date adalah pointer yang mengarah ke *dynamic array*, maka harus di-`free`
  
#### [Source code program](https://github.com/Izzud/SoalShift_modul2_B01/blob/master/soal5a.c)


#### 5b.

```
FILE *cmd = popen("pidof /home/izzud/Documents/SoalShift_modul2_B01/soal5a", "r");
fgets(out, 7, cmd);
```
* Untuk mendapatkan pid dari sebuah proses, perlu command `pidof`. Tetapi `pidof` adalah `shell command`, alternatifnya bisa menggunakan `popen` dari library C untuk menjalankan *command* di `shell` dan mendapatkan hasil dari command tersebut. Diambil dari **manpage popen**:
  ```
  The popen() function opens a process by creating a pipe, forking, and invoking the shell. 
  Since a pipe is by definition unidirectional, the type argument may specify only reading or writing, not both; the resulting stream is correspondingly read-only or write-only. 
  ```
  * Shell command `pidof` akan mencari **pid** dari input string nama proses yang berjalan. Dalam kasus ini, adalah **absolute path** dari file soal5a
  
* Output dari command `pidof` akan disalin ke array `out`
```
pid_t pid = strtoul(out, NULL, 10);
pclose(cmd);

if(!kill(pid, SIGKILL))
    printf("Process with pid of %d is successfully killed\n", pid);
```
* Isi dari array `out` yang merupakan string kemudian dikonversi menjadi ***unsigned long integer*** dan disimpan di variabel `pid` dengan tipe `pid_t`
* Close *stream* menggunakan `pclose`
* Kirim sinyal `SIGKILL` menggunakan command `kill` ke pid dari proses soal5a.c

#### [Source code program](https://github.com/Izzud/SoalShift_modul2_B01/blob/master/soal5b.c)

<br>

## References:

* http://man7.org/linux/man-pages/man3/opendir.3.html
* http://man7.org/linux/man-pages/man3/readdir.3.html
* http://pubs.opengroup.org/onlinepubs/009695399/functions/readdir_r.html
* http://www.cplusplus.com/reference/cstring/strrchr/
* http://www.cplusplus.com/reference/cstdio/snprintf/
* http://man7.org/linux/man-pages/man2/stat.2.html
* http://man7.org/linux/man-pages/man2/chmod.2.html
* https://linux.die.net/man/3/getpwuid
* https://linux.die.net/man/3/getgrgid
* http://www.cplusplus.com/reference/ctime/localtime/
* http://www.cplusplus.com/reference/ctime/strftime/
* https://linux.die.net/man/3/popen