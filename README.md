xmlReaderの解説
====

## 最初の注意点
  今回書いたプログラムは「もどき」で完全な動作はできません。具体的には、
* 「<ElementName Attributename="value"">～</Element">」の書き方しか許してません
* <ElementName Attributename="value"">～</Element">の間にテキストを埋め込んでも意味はありません

以上の注意点に留意すること

## Tag
1. タグは「<>」の全体のことを言う
2. 今回のプログラムではタグの中身の解析はこのクラスが行う
3. 「>」が来るまで解析を行って、Elementの名前とAttributeを回収する
4. rootエレメントの生成をコンストラクタで行なう

## Element
1. その名の通り要素　（子のElementとAttributeを所有する）
2. Elementはxmlの「<>～</>」での中身のこと
3. 今回のプログラムでは、解析はTagクラスに押し付けているぞ！
4. ElementのコンストラクタでTagの解析処理を行い、回収した要素はそのElemenntの物となる
5. Elementの中身にElementがあれば子エレメントとして保管する
6. 子を発見したらその時点で子のElementの解析を行う
7. 子が大量に展開されるとTagとElementが大量に発生して、普通にメモリがやばくなるはず。多分大丈夫だけど注意する
