## 注意事项说明
程序仅在Windows 11 环境下对.jpg .mp4 文件进行过测试
## 包含文件说明
#### base64Encode.c  
可以将指定文件转化为base64编码并保存在运行目录下的temp.txt中
#### huffmanEncode.c  
读取运行目录下temp.txt中的base64编码，创建哈夫曼树并计算哈夫曼编码，将字符和哈夫曼值保存在运行目录下的codeText.txt中，将编码后文件保存在运行目录下的encodedText.txt中
