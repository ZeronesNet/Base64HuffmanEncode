/** 
 * @file Base64Encode.c
 * @brief 计算文件的base64编码
 * @details 通过进制转换将二进制文件转换成base64编码文件
 *          并储存在同目录下的temp.txt文件中
 * @author ZerOnes
 * @date 2024-12-27
 * @version V1.0
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// Base64编码表
const char Base64AlphaTablet[64] =
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
    '+', '/'
};



/** 
 * @brief 十进制转换二进制函数
 * @details 接受DecArray中的三个十进制值并逐个计算二进制值储存在BinArray中
 * @param DecArray 包含3个十进制数的数组的指针
 * @param BinArray 储存3 * 8位二进制数的数组的指针
 * @note 因为在文件中1字节的十进制值不超过255
 *       所以函数中未对传入参数进行校验
 */


void DexToBin(int *DexArray,int *BinArray)
{

    int TempNum  = 0; // 临时储存DexArray中的一个十进制数
    int ArrayNum = 0; // BinArray数组的下标

    /**
     * 将TempNum转换成二进制数
     * 并按照 0-7 8-15 16-23 的分组储存
    */
    for (int x = 0;x<3;x++)
    {
        TempNum  = DexArray[x];
        ArrayNum = 7 + (x * 8);
        
        // 十进制转换二进制
        while (TempNum > 0)
        {
            BinArray[ArrayNum] = TempNum % 2;
            TempNum            = TempNum / 2;
            ArrayNum--;
        }

        // 二进制数不足8位时 用0补齐
        if (ArrayNum > (8 * x) - 1 )
        {
            for (ArrayNum;ArrayNum >= (8 * x);ArrayNum--)
            {
                BinArray[ArrayNum] = 0;
            }
        }

        // 当TempNum的值为-1时 修改BinArray中指定位置的值为-1作为标识
        if (TempNum == -1)
        {
            BinArray[(x + 1) * 6] = -1;
        }
    }

}

/**
 * @brief 二进制转换base64编码
 * @details 接受BinArray中的二进制值并以六位分组并获取对应的字符储存在Base64Array中
 * @param BinArray    包含3个二进制数的数组的指针
 * @param Base64Array 储存4个base64值的数组的指针
 */

void BinToBase64(int *BinArray,char *Base64Array)
{

    int TabletNum = 0;
    int StartNum  = 0;

    for (int x = 0;x<4;x++)
    {
        StartNum = 6 * x; // 每个数的起始位置
        
        // 6位二进制数首位为-1时 代表原数据不足3字节 用'='补齐Base64Array中四位字符
        if (BinArray[StartNum] == -1)
        {
            Base64Array[x] = '=';
        }
        else
        {   
            for (int y = 0;y<6;y++)
            {
                // 将BinArray中的6位二进制值转换成十进制
                TabletNum += BinArray[StartNum + y] * pow(2,5-y);
            }
            Base64Array[x] = Base64AlphaTablet[TabletNum]; // Base64AlphaTablet去表中查询对应的值
            TabletNum = 0;
        }
    }
}

/**
 * @brief 主函数
 * @details 接受一个文件的地址并计算其base64值 在脚本目录下生成temp.txt储存base64值
 * @param FilePath 一个文件的地址
 * @return 文件是否生成
 *      @retval 0  文件生成成功
 *      @retval -1 FilePath中提供的文件地址不正确或者程序无文件读写权限
 */

int Base64Encode(char *FilePath)
{
    FILE *ReadFile_P  = NULL;
    FILE *WriteFile_P = NULL;

    ReadFile_P  = fopen(FilePath,"rb");     // 待编码文件的地址
    WriteFile_P = fopen(".\\temp.txt","w"); // temp.txt文件的地址

    if (ReadFile_P == NULL || WriteFile_P == NULL)
    {
        return -1;
    }

    int  DexArray[3]    = {0}; // 储存3个十进制数
    int  BinArray[24]   = {0}; // 储存3 * 8 位二进制数
    char Base64Array[4] = {0}; // 储存4个Base64字符

    // 循环读取文件并以3个字节为1组开始编码
    while (!feof(ReadFile_P))
    {
        DexArray[0] = fgetc(ReadFile_P);
        DexArray[1] = fgetc(ReadFile_P);
        DexArray[2] = fgetc(ReadFile_P);

        // 当DexArray[0]的值为-1 文件读到末尾
        if (DexArray[0] == -1)
        {
            break;
        }
        DexToBin(DexArray,BinArray);
        BinToBase64(BinArray,Base64Array);

        fputc(Base64Array[0],WriteFile_P);
        fputc(Base64Array[1],WriteFile_P);
        fputc(Base64Array[2],WriteFile_P);
        fputc(Base64Array[3],WriteFile_P);
    }


    fclose(ReadFile_P);
    fclose(WriteFile_P);

    return 0;


}


void main()
{
    Base64Encode(".\\1.jpg");

}