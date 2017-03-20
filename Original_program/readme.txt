sph_sample.c : SPH法のソースプログラム。
sph_sample.h : sph_sample.cで参照するヘッダファイル。
glview2d.c   : SPHの計算結果をopenGLで出力するプログラム。
GLbitmap.h   : openGLでの出力結果をイメージングするときに使うヘッダ


$ make sph_sample              // sph_sample.cのコンパイル       
$ ./sph_sample.out             // SPH計算 
$ make glview2d                // glview2d.cのコンパイル
$ ./glview2d.out               // openGLで出力




◎make glview2d に失敗したとき

    １.synapticでopenGLをインストール

    ２.誰かのパソコン(narukoとか)の/usr/X11R6/lib/から
       libglut.so, libGLU.so, libGL.soをコピーしてきて、
       自分の/usr/X11R6/lib/に入れる

    ３.それでもダメだったら誰かに聞いてください 