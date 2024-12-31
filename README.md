## 注意事项说明
程序仅在64位Windows 11 环境下对.jpg .mp4 文件进行过测试
## 最新更新说明
UpdateTime:2024/12/31
上传了新的Base64Encode.c和HuffmanEncode.c
Base64Encode.c  修正了不正确的函数类型 使用了更少的代码实现逻辑 提升了性能
HuffmanEncode.c 修正了对字符数量的错误统计 引入动态内存和分区查找 提升了性能
## EncodedText.hfm文件说明
#### 编码本(1-200字节?)
包含字符 对应的哈夫曼编码长度 哈夫曼编码的十六进制表示
其分别占据1个字节
整个编码本的实际长度视情况而定
#### 密文(201-?)
密文首个字节为EncodedText_Temp.txt中差几个比特满足一字节
其余为原文经哈夫曼编码后的密文

## 包含文件说明
#### Base64Encode.c  
可以将指定文件转化为base64编码并保存在运行目录下的temp.txt中
#### HuffmanEncode.c  
读取运行目录下temp.txt中的base64编码，创建哈夫曼树并计算哈夫曼编码，将字符和哈夫曼值保存在运行目录下的codeText.txt中，将编码后文件保存在运行目录下的encodedText.txt中
