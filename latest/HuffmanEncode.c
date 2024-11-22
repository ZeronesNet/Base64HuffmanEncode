/**
 * @file HuffmanEncode.c
 * @brief 将base64编码的文件用哈夫曼编码进行编码
 * @details 包含createNode、createHuffmantree、encodeChar、huffmanEncode四个函数 其中huffmanEncode为主函数
 * @author ZerOnes
 * @date 2024/11/21
 * @version v1.0
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const char base64Alphatablet[65]={
    'A', 'B', 'C', 'D', 'E', 'F', 'G',
    'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '+', '/','='
};

/** 
 * @struct HuffmanNode
 * @brief 哈夫曼树节点的结构体
 */
typedef struct HuffmanNode
{
    char Character;                     // 字符
    unsigned int  Weights;              // 字符的权值
    int  Num;                           // 为左子节点时为0 右子节点时为0
    struct HuffmanNode *Parent;         // 指向当前节点双亲的指针
    struct HuffmanNode *leftChildren;   // 指向左侧节点的指针
    struct HuffmanNode *rightChildren;  // 指向右侧节点的指针
}Node;

/**
 * @struct EncodeNode
 * @brief 储存字符及其哈夫曼编码的结构体
 */
typedef struct EncodeNode
{
    char Character;                     // 字符
    int  Code[33];                      // 储存当前字符的哈夫曼编码
}EncodeNode;



Node *nodeList[65];       // 储存65个节点的指针数组
Node *nodeList_copy[65];  // nodeList的复制 方便在哈夫曼树中查找节点

EncodeNode *codeList[65]; // 储存65个节点的指针数组


/**
 * @brief 创建Node节点
 * @details 打开.\\temp.txt文件并统计每个字符的数量并作为权值
 * 生成节点 将节点的地址指针储存在nodeList中
 */
void creatNode(){

    int  charNum = 0;
    int  charRead;
    Node *node_p;
    FILE *fp = fopen(".\\temp.txt","r");

    for (int x=0;x<65;x++){

        // 统计字符出现的次数并储存在charNum中
        while(!feof(fp)){
            charRead = fgetc(fp);
            if (charRead==(int)base64Alphatablet[x]){
                charNum = charNum + 1;    
            }
        }

        node_p = (Node *)malloc(sizeof(Node)); // 为节点申请内存空间

        // 初始化Node节点的数据
        node_p->Character     = base64Alphatablet[x];
        node_p->Weights       = charNum;
        node_p->Num           = 0;
        node_p->Parent        = NULL;   
        node_p->leftChildren  = NULL;
        node_p->rightChildren = NULL;

        nodeList[x]      = node_p; // 将节点的指针储存到nodeList中
        nodeList_copy[x] = node_p; // 复制一份储存在nodeList_copy中

        fseek(fp,0,SEEK_SET); // 将文件指针回到文件开头
        charNum = 0;
    }
    fclose(fp);
}

/**
 * @brief 创建哈夫曼树
 * @details 根据nodeList中的节点的权值创建哈夫曼树
 */

void createHuffmantree(){
    
    Node *temp_p; // 作为中间变量用于交换两个节点
    Node *node_p; // 指向由两个权值最小节点合成的新节点的指针

    for (int n=0;n<64;n++){

    
        // 使用冒泡排序将权值最小的两个节点指针移到数组最前面
        for (int a=0;a<2;a++){ 
            for (int b=64-n;b>0;b--){
                if (nodeList[b]->Weights < nodeList[b-1]->Weights){
                    temp_p        = nodeList[b];
                    nodeList[b]   = nodeList[b-1];
                    nodeList[b-1] = temp_p;
                }
            }
        }

        node_p = (Node *)malloc(sizeof(Node)); // 为节点申请内存空间

        // 初始化新节点的数据
        node_p->Character     = '~';
        node_p->Num           = 0;
        node_p->Weights       = nodeList[0]->Weights + nodeList[1]->Weights;
        node_p->Parent        = NULL;
        node_p->leftChildren  = nodeList[0];
        node_p->rightChildren = nodeList[1];

        // 修改新节点的左右子节点的Parent和Num的值
        node_p->leftChildren->Parent  = node_p;
        node_p->leftChildren->Num     = 0;
        node_p->rightChildren->Parent = node_p;
        node_p->rightChildren->Num    = 1;

        // 用新节点指针覆盖数组第二位后将数组所有指针向前移一位
        nodeList[1] = node_p;
        for (int c=0;c<64-n;c++){
            nodeList[c] = nodeList[c+1];
        }
    }

    // 修改哈夫曼树根的Num值为-1
    nodeList[0]->Num = -1;
}

/**
 * @brief 创建EncodeNode节点
 * @details 创建节点储存字符以及其哈夫曼编码
 */

void encodeChar(){

    EncodeNode *node_p; 

    for (int a=0;a<65;a++){

        node_p = (EncodeNode *)malloc(sizeof(EncodeNode)); // 为节点申请内存空间
        node_p->Character = nodeList_copy[a]->Character;

        /**
         * 从当前Node节点向上溯源 直到到达根节点 同时将经过的每一节点
         * 的Num值记录在EncodeNode节点的Code数组中*/
        int num = 0;
        while (nodeList_copy[a]->Num != -1){
            node_p->Code[num] = nodeList_copy[a]->Num;
            num = num + 1;
            nodeList_copy[a] = nodeList_copy[a]->Parent;
        }

        node_p->Code[num] = -1; // 将哈夫曼编码的后一位数字修改为-1 记录为中断点
        codeList[a] = node_p;   // 将EncodeNode节点的地址指针储存在codeList中
    }
}

/**
 * @brief 程序主函数
 * @details 用哈夫曼编码脚本目录下.\\temp.txt文件 生成编码后文件EncodedText.hfm 编码本codeText.txt
 * @return 文件是否生成
 *      @retval 0  文件生成成功
 *      @retval -1 文件读取失败或者程序无文件读写权限
 */

int huffmanEncode(){

    FILE *readText         = NULL; // 待编码文件
    FILE *encodedText_temp = NULL; // 临时生成的文件用于保存base64码用哈夫曼编码后的二进制值
    FILE *encodedText      = NULL; // 保存待编码文件的哈夫曼编码
    FILE *codeText         = NULL; // 字符以及其哈夫曼编码

    readText         = fopen(".\\temp.txt","r");
    encodedText_temp = fopen(".\\EncodedText_temp.hfm","wb+"); 
    encodedText      = fopen(".\\EncodedText.hfm","w");
    codeText         = fopen(".\\codeText.txt","w");

    if (readText == NULL || encodedText_temp == NULL || codeText == NULL){
        return -1;
    }

    creatNode();
    createHuffmantree();
    encodeChar();

    // 向codeText.txt中写入字符以及其对应的哈夫曼编码
    for (int a=0;a<65;a++){

        fputc(codeList[a]->Character,codeText);
        fputc(' ',codeText);
        
        int n = 0;
        while (codeList[a]->Code[n] != -1){
            fputc((char)codeList[a]->Code[n]+48,codeText);
            n = n + 1;
        }

        fputc('\n',codeText);
        
    }

    // 将base64编码用哈夫曼编码后的二进制数据保存在EncodeText_temp.hfm
    char readChar;
    while (!feof(readText)){
        
        readChar = fgetc(readText);

        for (int x=0;x<65;x++){
            
            if (readChar == codeList[x]->Character){

                int n = 0;
                while (codeList[x]->Code[n] != -1){
                    fputc(codeList[x]->Code[n],encodedText_temp);
                    n = n + 1;
                }
            }
        }

    }



    int fileLength = ftell(encodedText_temp); // encodedText_temp.hfm文件大小
    int lackBit    = 8 - (fileLength % 8);    // encodedText_temp.hfm文件末尾差几个比特满足一字节
    int dexBuff    = 0;

    fputc(lackBit,encodedText); // 向encodedText.hfm文件的第一个字节写入lackBit

    fseek(encodedText_temp,0,SEEK_SET); // 复位文件指针
    
    // 读取encodedText_temp.hfm中8位换算成一个字节写入encodedText.hfm
    for (int x=0;x<(fileLength / 8)+1;x++){
        
        int temp = 0;
        int readNum = 0;

        // 处理文件最后几位
        if (x == fileLength / 8){
            for (int y=fileLength%8;y>0;y--){
                readNum = fgetc(encodedText_temp);
                temp = pow(2,y-1) * readNum;
                dexBuff = temp + dexBuff;
            }
        }   
        // 处理8位
        else{
    
            for(int z=7;z>=0;z--){
                readNum = fgetc(encodedText_temp);
                temp = pow(2,z) * readNum;
                dexBuff = dexBuff + temp;
            }

        }
        fputc(dexBuff,encodedText);
        dexBuff = 0;
    }

    fclose(readText);
    fclose(encodedText_temp);
    fclose(encodedText);
    fclose(codeText);
        
    remove(".\\temp.txt");
    remove(".\\EncodedText_temp.hfm");

    return 0;

}

void main(){
    huffmanEncode();
}