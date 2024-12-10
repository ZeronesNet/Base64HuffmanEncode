/**
 * @file HuffmanEncode.c
 * @brief 将base64编码的文件用哈夫曼编码进行编码
 * @details 包含createNode、createHuffmantree、EncodeChar、huffmanEncode四个函数 其中huffmanEncode为主函数
 * @author ZerOnes
 * @date 2024/11/21
 * @version v1.0
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

const char Base64AlphaTablet[65]=
{
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
    struct HuffmanNode *LeftChildren;   // 指向左侧节点的指针
    struct HuffmanNode *RightChildren;  // 指向右侧节点的指针
}Node;

/**
 * @struct EncodeNode
 * @brief 储存字符及其哈夫曼编码的结构体
 * 
 * 
 * 
 * @todo 尝试用全局变量来代替Code中的数字
 * 生成哈夫曼树的时候计算最长编码长度
 */
typedef struct EncodedNode
{
    char Character;     // 字符
    int  DexCode;       // 哈夫曼编码串的十进制
    int  CodeLength;    // 哈夫曼编码的长度
    int  Code[8];       // 储存当前字符的哈夫曼编码
}EncodedNode;



Node *Node_P_List[65];       // 储存65个Node节点的指针数组
Node *Node_P_List_Copy[65];  // nodeList的复制 方便在哈夫曼树中查找节点

EncodedNode *EncodedNode_P_List[65]; // 储存65个EncodeNode节点的指针数组


/**
 * @brief 创建Node节点
 * @param fp 指向.\\temp.txt文件的指针
 * @details 打开.\\temp.txt文件并统计每个字符的数量并作为权值
 * 生成节点 将节点的地址指针储存在nodeList中
 */
void CreatNode(FILE *fp)
{

    int  CharNum = 0;
    int  CharRead;
    Node *Node_P;

    for (int x=0;x<65;x++)
    {

        // 统计字符出现的次数并储存在charNum中
        while(!feof(fp))
        {
            CharRead = fgetc(fp);
            if (CharRead==(int)Base64AlphaTablet[x])
            {
                CharNum = CharNum + 1;    
            }
        }

        Node_P = (Node *)malloc(sizeof(Node)); // 为节点申请内存空间

        // 初始化Node节点的数据
        Node_P -> Character     = Base64AlphaTablet[x];
        Node_P -> Weights       = CharNum;
        Node_P -> Num           = 0;
        Node_P -> Parent        = NULL;   
        Node_P -> LeftChildren  = NULL;
        Node_P -> RightChildren = NULL;

        Node_P_List[x]      = Node_P; // 将节点的指针储存到nodeList中
        Node_P_List_Copy[x] = Node_P; // 复制一份储存在nodeList_copy中

        fseek(fp,0,SEEK_SET); // 将文件指针回到文件开头
        CharNum = 0;
    }
    fseek(fp,0,SEEK_SET);
}

/**
 * @brief 创建哈夫曼树
 * @details 根据nodeList中的节点的权值创建哈夫曼树
 */

void CreateHuffmanTree()
{
    
    Node *Temp_P; // 作为中间变量用于交换两个节点
    Node *Node_P; // 指向由两个权值最小节点合成的新节点的指针

    for (int n=0;n<64;n++)
    {

    
        // 使用冒泡排序将权值最小的两个节点指针移到数组最前面
        for (int a=0;a<2;a++)
        { 
            for (int b=64-n;b>0;b--)
            {
                if (Node_P_List[b] -> Weights < Node_P_List[b-1] -> Weights)
                {
                    Temp_P           = Node_P_List[b];
                    Node_P_List[b]   = Node_P_List[b-1];
                    Node_P_List[b-1] = Temp_P;
                }
            }
        }

        Node_P = (Node *)malloc(sizeof(Node)); // 为节点申请内存空间

        // 初始化新节点的数据
        Node_P -> Character     = '~';
        Node_P -> Num           = 0;
        Node_P -> Weights       = Node_P_List[0] -> Weights + Node_P_List[1]->Weights;
        Node_P -> Parent        = NULL;
        Node_P -> LeftChildren  = Node_P_List[0];
        Node_P -> RightChildren = Node_P_List[1];

        // 修改新节点的左右子节点的Parent和Num的值
        Node_P -> LeftChildren  -> Parent  = Node_P;
        Node_P -> LeftChildren  -> Num     = 0;
        Node_P -> RightChildren -> Parent  = Node_P;
        Node_P -> RightChildren -> Num     = 1;

        // 用新节点指针覆盖数组第二位后将数组所有指针向前移一位
        Node_P_List[1] = Node_P;
        for (int c=0;c<64-n;c++){
            Node_P_List[c] = Node_P_List[c+1];
        }
    }

    // 修改哈夫曼树根的Num值为-1
    Node_P_List[0]->Num = -1;
}

/**
 * @brief 创建EncodeNode节点
 * @details 创建节点储存字符以及其哈夫曼编码
 */

void EncodeChar()
{

    EncodedNode *Encoded_Node_P; 

    for (int a=0;a<65;a++)
    {

        Encoded_Node_P = (EncodedNode *)malloc(sizeof(EncodedNode));    // 为节点申请内存空间
        Encoded_Node_P -> Character = Node_P_List_Copy[a] -> Character;

        // 向Node_P_List_Copy[a]的父节点溯源
        // 记录每个节点的Num值得到相反哈夫曼编码
        int CodeLength = 0;     // 字符对应的哈夫曼编码的长度
        int TempCodeList[8];    // 临时储存字符的相反哈夫曼编码

        while (Node_P_List_Copy[a] -> Num != -1)
        {
            TempCodeList[CodeLength] = Node_P_List_Copy[a] -> Num;
            Node_P_List_Copy[a]      = Node_P_List_Copy[a] -> Parent;
            CodeLength               = CodeLength + 1;
            // 当CodeLength的值大于8时 哈夫曼编码失去意义
        }

        Encoded_Node_P ->CodeLength = CodeLength;
        
        // 根据TempCodeList[8]中的值计算整个二进制串的十进制值
        int DexCode = 0;
        for (int n=0;n<CodeLength;n++)
        {
            DexCode = DexCode + pow(2,n) * TempCodeList[n];
        }

        Encoded_Node_P -> DexCode = DexCode;
        
        // 将TempCodeList[8]中的值逆序存入Encoded_Node_P -> Code[8]中
        int CodeLength_Copy = CodeLength;
        for (int TempNum=0;TempNum<CodeLength_Copy;TempNum++)
        {
            Encoded_Node_P -> Code[TempNum] = TempCodeList[CodeLength-1];
            CodeLength--;
        }

        Encoded_Node_P -> Code[CodeLength_Copy] = -1; // 将正序哈夫曼编码的后一位数字修改为-1 记录为中断点
        EncodedNode_P_List[a] = Encoded_Node_P;       // 将EncodeNode节点的地址指针储存在EncodedNode_P_List中
    }
}


/**
 * @brief 程序主函数
 * @details 用哈夫曼编码脚本目录下.\\temp.txt文件 生成编码后文件EncodedText.hfm 编码本codeText.txt
 * @return 文件是否生成
 *      @retval 0  文件生成成功
 *      @retval -1 文件读取失败或者程序无文件读写权限
 * 
 * 
 * @todo 取消codeText.txt的读取，将数据直接写入EncodedText.hfm
 * 取消直接写入二进制数据 化成十六进制数写入
 */

int HuffmanEncode()
{

    FILE *ReadText         = NULL; // 临时文件
    FILE *EncodedText_Temp = NULL; // 临时生成的文件用于保存base64码用哈夫曼编码后的二进制值
    FILE *EncodedText      = NULL; // 保存待编码文件的哈夫曼编码
    
    ReadText         = fopen(".\\temp.txt","r");
    EncodedText_Temp = fopen(".\\EncodedText_temp.hfm","wb+"); 
    EncodedText      = fopen(".\\EncodedText.hfm","w");

    if (ReadText == NULL || EncodedText_Temp == NULL)
    {
        return -1;
    }

    CreatNode(ReadText);    // 生成字符节点
    CreateHuffmanTree();    // 根据字符节点创建哈夫曼树
    EncodeChar();           // 根据哈夫曼树生成每个字符的编码

    // 向CodeText.etxt中写入字符以及其对应的哈夫曼编码
    for (int a=0;a<65;a++)
    {

        fputc(EncodedNode_P_List[a] -> Character,EncodedText);
        fputc(EncodedNode_P_List[a] -> CodeLength,EncodedText);
        fputc(EncodedNode_P_List[a] -> DexCode,EncodedText);
    }
    fputc(9,EncodedText); // 密码本与密文之间的间隔值

    // 将base64编码用哈夫曼编码后的二进制数据保存在EncodeText_temp.hfm
    char ReadChar;
    while (!feof(ReadText))
    {
        
        ReadChar = fgetc(ReadText);

        for (int x=0;x<65;x++)
        {
            
            if (ReadChar == EncodedNode_P_List[x] -> Character)
            {

                int n = 0;
                while (EncodedNode_P_List[x] -> Code[n] != -1)
                {
                    fputc(EncodedNode_P_List[x] -> Code[n],EncodedText_Temp);
                    n = n + 1;
                }
            }
        }

    }



    int FileLength = ftell(EncodedText_Temp); // EncodedText_Temp.hfm文件大小
    int LackBit    = 8 - (FileLength % 8);    // EncodedText_Temp.hfm文件末尾差几个比特满足一字节
    int DexBuff    = 0;

    fputc(LackBit,EncodedText); // 向encodedText.hfm文件的第一个字节写入lackBit

    fseek(EncodedText_Temp,0,SEEK_SET); // 复位文件指针
    
    // 读取encodedText_temp.hfm中8位换算成一个字节写入encodedText.hfm
    for (int x=0;x<(FileLength / 8)+1;x++)
    {
        
        int TempNum = 0;
        int ReadNum = 0;

        // 处理文件最后几位
        if (x == FileLength / 8){
            for (int y=FileLength%8;y>0;y--)
            {
                ReadNum = fgetc(EncodedText_Temp);
                TempNum = pow(2,y-1) * ReadNum;
                DexBuff = TempNum + DexBuff;
            }
        }   
        // 处理8位
        else{
    
            for(int z=7;z>=0;z--)
            {
                ReadNum = fgetc(EncodedText_Temp);
                TempNum = pow(2,z) * ReadNum;
                DexBuff = DexBuff + TempNum;
            }

        }
        fputc(DexBuff,EncodedText);
        DexBuff = 0;
    }

    fclose(ReadText);
    fclose(EncodedText_Temp);
    fclose(EncodedText);
        
    //remove(".\\temp.txt");
    remove(".\\EncodedText_temp.hfm");

    return 0;

}

void main()
{
    HuffmanEncode();
}