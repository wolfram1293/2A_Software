追加コマンド

課題１
rectangle
rectangle x0 y0 x1 y1 の形でx0 y0を起点とし、幅x1、高さy1の長方形を描画

circle
circle x0 y0 r の形でx0 y0を中心とし、半径rの円を描画

課題2
load
load filename の形でログファイルをロードする
filenameの指定がない場合はhistory.txtをデフォルトで読み込む

課題3
pen
pen 記号 の形でペン先を記号に変換
選択中のペン先は画面左上に表示される


課題4
redo
redo コマンドでundoを一回分もとに戻す
回数に制限はないが、途中で新たな描画コマンドが入力されると、それより先のundo履歴は消去される

eraser
eraser コマンドでペンに消しゴムをセットする

fill
fill x0 y0 の形でx0 y0を含む閉区間内を選択されているペンで塗りつぶす