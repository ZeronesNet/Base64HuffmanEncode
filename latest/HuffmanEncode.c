/**
 * @file HuffmanEncode.c
 * @brief 将base64编码的文件用哈夫曼编码进行编码
 * @details 包含CreateHuffmanNode、CreateHuffmantree、CreateHuffmanCode、huffmanEncode四个函数 其中huffmanEncode为主函数
 * @author ZerOnes
 * @date 2024/12/31
 * @version v2.0
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

typedef struct HuffmanNode
{
    char Character;                     // 字符
    int  SignNum;                       // 数值标志 0 / 1
    unsigned int Weights;               // 字符权值
    struct HuffmanNode *Parent;         // 指向当前节点双亲的指针
    struct HuffmanNode *LeftChildren;   // 指向左侧节点的指针
    struct HuffmanNode *RightChildren;  // 指向右侧节点的指针
}HuffmanNode;



typedef struct HuffmanCode
{           
    char Character;                     // 字符
    int  Code[9];                       // 哈夫曼编码
    int  DecCode;                       // 十进制编码
    int  CodeLength;                    // 哈夫曼编码长度
}HuffmanCode;



HuffmanNode **HuffmanNodeList; // 65个HuffmanNode节点的地址的数组
HuffmanNode **CopyNodeList;    // HuffmanNodeList的复制数组
HuffmanNode * RootNode;        // 用于保存哈夫曼树的根节点

HuffmanCode **HuffmanCodeList; // 65个HuffmanCode节点的地址的数组



/**
 * @brief 创建HuffmanNode节点
 * @param TempFilePath 指向.\\temp.txt文件的指针
 * @details 打开.\\temp.txt文件并统计每个字符的数量并作为权值
 *          生成节点 将节点的地址指针储存在全局变量HuffmanNode数组中
 *          同时复制一份到全局变量CopyNodeList数组中
 */

void CreateHuffmanNode(FILE *TempFilePath)
{

    HuffmanNode *NewNode;

    // 为65个字符分别创建节点
    for (int x = 0;x<65;x++)
    {   
        NewNode = (HuffmanNode *)malloc(sizeof(HuffmanNode));

        NewNode -> Character = Base64AlphaTablet[x];
        NewNode -> SignNum   = 0;
        NewNode -> Weights   = 0;

        NewNode -> Parent        = NULL;
        NewNode -> LeftChildren  = NULL;
        NewNode -> RightChildren = NULL;

        HuffmanNodeList[x] = NewNode;
        CopyNodeList[x]    = NewNode;
    }

    int ReadCharacter = 0;;
    while (!feof(TempFilePath))
    {   

        ReadCharacter = fgetc(TempFilePath);

        // 分类字符并使对应字符的权值加1
        if      (ReadCharacter >= 65 && ReadCharacter <= 90)
        {
            HuffmanNodeList[ReadCharacter - 65] -> Weights++;
        }
        else if (ReadCharacter >= 97 && ReadCharacter <= 122)
        {
            HuffmanNodeList[ReadCharacter - 71] -> Weights++;
        }
        else if (ReadCharacter >= 48 && ReadCharacter <= 57)
        {
            HuffmanNodeList[ReadCharacter + 4 ] -> Weights++;
        }
        else if (ReadCharacter == 43)
        {
            HuffmanNodeList[62] -> Weights++;
        }
        else if (ReadCharacter == 47)
        {
            HuffmanNodeList[63] -> Weights++;
        }
        else if (ReadCharacter == 61)
        {
            HuffmanNodeList[64] -> Weights++;
        }
    }
    fseek(TempFilePath,0,SEEK_SET);
}


/**
 * @brief 创建哈夫曼树
 * @details 根据nodeList中的节点的权值创建哈夫曼树
 *          将根节点的地址赋值给全局变量RootNode
 *          释放全局变量HuffmanNodeList数组的内存
 */

void CreateHuffmanTree()
{
    HuffmanNode *ExchangeNode;
    HuffmanNode *NewNode;


    for (int z = 0;z<64;z++)
    {
        // 使用冒泡排序将权值最小的两个节点指针移到数组最前面
        for (int y = 0;y<2;y++)
        {
            for (int x = 64;x>z;x--)
            {
                if (HuffmanNodeList[x] -> Weights < HuffmanNodeList[x - 1] -> Weights)
                {
                    ExchangeNode           = HuffmanNodeList[x];
                    HuffmanNodeList[x]     = HuffmanNodeList[x - 1];
                    HuffmanNodeList[x - 1] = ExchangeNode;
                }
            }
        }

        NewNode = (HuffmanNode *)malloc(sizeof(HuffmanNode));

        // 初始化新节点的数据
        NewNode -> Character     = '-';
        NewNode -> SignNum       = 0;
        NewNode -> Weights       = HuffmanNodeList[z] -> Weights + HuffmanNodeList[z + 1] -> Weights;
        NewNode -> Parent        = NULL;
        NewNode -> LeftChildren  = HuffmanNodeList[z];
        NewNode -> RightChildren = HuffmanNodeList[z + 1];

        // 修改新节点的左右子节点的Parent和SignNum的值
        NewNode -> LeftChildren  -> Parent  = NewNode;
        NewNode -> RightChildren -> Parent  = NewNode;
        NewNode -> LeftChildren  -> SignNum = 0;
        NewNode -> RightChildren -> SignNum = 1;

        HuffmanNodeList[z + 1] = NewNode;
    }
    HuffmanNodeList[64] -> SignNum = -1;

    // 获取整棵哈夫曼树的根节点
    RootNode = HuffmanNodeList[64];

    // 释放HuffmanNodeList数组的内存
    free(HuffmanNodeList);
    HuffmanNodeList = NULL;


}   




/**
 * @brief 创建HuffmanCode节点
 * @details 创建节点储存字符以及其哈夫曼编码
 *          将每个节点的地址储存在全局变量HuffmanCodeList数组中
 *          释放CopyNodeList数组的内存
 */         

void CreateHuffmanCode()
{

    HuffmanCode *NewNode;

    int Code[9]         = {0};
    char Character      = 0;
    int  DecCode        = 0;
    int  CodeLength     = 0;
    int  CodeLengthCopy = 0;

    for (int x=0;x<65;x++)
    {   
        NewNode   = (HuffmanCode *)malloc(sizeof(HuffmanCode));
        Character = CopyNodeList[x] -> Character;

        // 将反序哈夫曼编码写入Code中
        while (CopyNodeList[x] -> SignNum != -1)
        {   
            Code[CodeLength] = CopyNodeList[x] -> SignNum;
            CopyNodeList[x]  = CopyNodeList[x] -> Parent;
            CodeLength       = CodeLength + 1;
        }

        // 根据Code中的值计算DexCode
        for (int y=0;y<CodeLength;y++)
        {
            DecCode = DecCode + pow(2,y) * Code[y];
        }

        // 将Code中的值逆序写入NewNode -> Code中
        CodeLengthCopy = CodeLength;
        for (int z = 0;z<CodeLengthCopy;z++)
        {
            NewNode -> Code[z] = Code[CodeLength - 1];
            CodeLength--;
        }
        NewNode -> Code[CodeLengthCopy] = -1;

        // 写入向节点中写入DexCode CodeLength CodeLength
        NewNode -> Character  = Character;
        NewNode -> DecCode    = DecCode;
        NewNode -> CodeLength = CodeLengthCopy;

        // 将节点地址保存在HuffmanCodeList中
        HuffmanCodeList[x] = NewNode;

        // 重置函数中 Character DexCode CodeLength CodeLengthCopy的值
        Character      = 0;
        DecCode        = 0;
        CodeLength     = 0;
        CodeLengthCopy = 0;
    }

    // 释放CopyNodeList数组的内存
    free(CopyNodeList);
    CopyNodeList = NULL;
}


/**
 * @brief 释放HuffmanNode节点内存
 * @details 接受哈夫曼树的根节点的地址
 *          后序遍历所有节点 
 *          释放所有节点的内存
 * @param RootNode 哈夫曼树的根节点的地址
 * @return 节点是否释放成功
 *          @retval 0 释放成功
 */

int FreeHuffmanNode(HuffmanNode *Node)
{
    if (Node == NULL)
    {
        return 0;
    }

    FreeHuffmanNode(Node ->LeftChildren);
    FreeHuffmanNode(Node ->RightChildren);

    free(Node);
    Node = NULL;

}


/**
 * @brief 程序主函数
 * @details 用哈夫曼编码脚本目录下.\\temp.txt文件 生成编码后文件EncodedText.hfm
 * @return 文件是否生成
 *      @retval 0  文件生成成功
 *      @retval -1 文件读取失败或者程序无文件读写权限
 *      @retval -2 申请内存空间失败
 */

int HuffmanEncode()
{
    FILE *TempFilePath_P    = NULL;
    FILE *TempEncodedText_P = NULL;
    FILE *EncodedText_P     = NULL;

    TempFilePath_P    = fopen(".\\temp.txt","r");
    TempEncodedText_P = fopen(".\\TempEncodedText.hfm","wb+");
    EncodedText_P     = fopen(".\\EncodedText.hfm","wb");

    if (TempFilePath_P == NULL || TempEncodedText_P == NULL || EncodedText_P == NULL)
    {
        return -1;
    }


    // 申请内存
    HuffmanNodeList = NULL; // 65个HuffmanNode节点的地址的数组
    CopyNodeList    = NULL; // HuffmanNodeList的复制数组
    RootNode        = NULL; // 用于保存哈夫曼树的根节点

    HuffmanNodeList = (HuffmanNode **)malloc(sizeof(HuffmanNode *)*65);
    CopyNodeList    = (HuffmanNode **)malloc(sizeof(HuffmanNode *)*65);
    RootNode        = (HuffmanNode * )malloc(sizeof(HuffmanNode *));

    HuffmanCodeList = (HuffmanCode **)malloc(sizeof(HuffmanCode *)*65);

    if (HuffmanNodeList == NULL || CopyNodeList == NULL || RootNode == NULL)
    {
        return -2;
    }


    CreateHuffmanNode(TempFilePath_P); // 创建65个哈夫曼节点
    CreateHuffmanTree();               // 根据已创建的哈夫曼节点生成哈夫曼树
    CreateHuffmanCode();               // 根据哈夫曼树生成每个字符的哈夫曼编码

    FreeHuffmanNode(RootNode);         // 释放65个哈夫曼节点的内存空间


    // 分区查找 将temp.txt中的字符转换为哈夫曼编码
    // 将二进制值暂时写入TempEncodedText.hfm中
    int ReadCharacter = 0;
    while (!feof(TempFilePath_P))
    {
        ReadCharacter = fgetc(TempFilePath_P);
        int TempNum = 0;

        if      (ReadCharacter >= 65 && ReadCharacter <= 90)
        {   
            
            for (int x = 65;x<=90;x++)
            {
                if (ReadCharacter == HuffmanCodeList[x - 65] -> Character)
                {
                    TempNum = x - 65;
                }
            }
        }
        else if (ReadCharacter >= 97 && ReadCharacter <= 122)
        {
            for (int x = 97;x<=122;x++)
            {
                if (ReadCharacter == HuffmanCodeList[x - 71] -> Character)
                {
                    TempNum = x -71;
                }
            }
        }
        else if (ReadCharacter >= 48 && ReadCharacter <= 57)
        {
           
            for (int x = 48;x<=57;x++)
            {
                if (ReadCharacter == HuffmanCodeList[x + 4] -> Character)
                {
                    TempNum = x + 4;
                }
            } 
        }
        else if (ReadCharacter == 43)
        {
            TempNum = 62;
        }
        else if (ReadCharacter == 47)
        {
            TempNum = 63;
        }
        else if (ReadCharacter == 61)
        {
            TempNum = 64;
        }

        int y = 0;
        while (HuffmanCodeList[TempNum] -> Code[y] != -1)
        {
            fputc(HuffmanCodeList[TempNum] -> Code[y],TempEncodedText_P);
            y++;
        }
    }

    // 向EncodedText.hfm中写入编码本
    for (int x = 0;x<65;x++)
    {
        fputc(HuffmanCodeList[x] -> Character ,EncodedText_P);
        fputc(HuffmanCodeList[x] -> CodeLength,EncodedText_P);
        fputc(HuffmanCodeList[x] -> DecCode   ,EncodedText_P);
    }

    
    fseek(TempEncodedText_P,0,SEEK_SET); // 复位TempEncodedText.hfm文件指针开始读取二进制值


    // 以TempEncodedText.hfm文件中8位转换为1位十进制并写入EncodedText.hfm中
    while (!feof(TempEncodedText_P))
    {
        int TempNum = 0;
        for (int x = 7;x>=0;x--)
        {
            int ReadNum = fgetc(TempEncodedText_P);

            if (ReadNum == -1)
            {
                break;
            }
            TempNum += ReadNum * pow(2,x);
        }
        fputc(TempNum,EncodedText_P);
    }


    fclose(TempFilePath_P);
    fclose(TempEncodedText_P);
    fclose(EncodedText_P);
    
    remove(".\\temp.txt");
    remove(".\\TempEncodedText.hfm");

    return 0;
}



void main()
{
    HuffmanEncode();
}