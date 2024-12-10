/** 
 * @file Base64Encode.c
 * @brief 计算文件的base64编码
 * @details 包含decTobin、BinToBase64、base64Encode三个函数 其中base64Encode为主函数
 * @author ZerOnes
 * @date 2024-11-15
 * @version V1.0
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


const char Base64AlphaTablet[64]=
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
 * @brief 二进制转换十进制函数
 * @details 接受decArray中的三个十进制值并逐个计算二进制值储存在binArray中
 * @param *DecArray 包含三个十进制数的数组的指针
 * @param *BinArray 储存三个二进制数的数组的指针
 * @return 返回binArray数组的指针
 */


int *DecTobin(unsigned char *DecArray,int *BinArray)
{
    
    int num = 0;

    for (int x=0;x<3;x++)
    {
        num = (int)DecArray[x];
        int i = 7 + (x * 8); 

        while (num > 0)
        {
            BinArray[i] = num % 2;
            num = num / 2;
            i--;
        }
        
        // 对于不足八位的二进制数据用0补齐
        if (i>=0)
        {
            for (i;i>=x*8;i--)
            {
                BinArray[i] = 0;
            }
        }
    }
    return BinArray;
}

/**
 * @brief 二进制转换base64编码
 * @details 接受binArray中的二进制值并以六位分组并获取对应的字符储存在base64Array中
 * @param BinArray 包含三个二进制数的数组的指针
 * @param Base64Array 储存三个base64值的数组的指针
 * @param LackNum 三个字节缺少几个字节
 * @return 返回base64数组的指针
 */

char *BinToBase64(int *BinArray,char *Base64Array,int LackNum)
{

    int num = 0;
    // 判断缺少几个字节并根据情况补0
    if (LackNum == 1)
    {
        BinArray[16] = 0;
        BinArray[17] = 0;
    }
    else if (LackNum == 2)
    {
        BinArray[8]  = 0;
        BinArray[9]  = 0;
        BinArray[10] = 0;
        BinArray[11] = 0;
    }
    for (int x=0;x<4;x++)
    {
        int i = 0 + (6*x); // 每个数的起始位置
        int p = 0; 
        for (int y=0;y<6;y++){
            int p = pow(2,5-y);
            num += BinArray[i+y] * p;
            }
        Base64Array[x] = Base64AlphaTablet[num]; // 去base64表中查询对应的值
        num = 0;
    }

    if (LackNum == 1)
    {
        Base64Array[3] = '=';
    }
    else if (LackNum == 2)
    {
        Base64Array[2] = '=';
        Base64Array[3] = '=';
    }
    return Base64Array;
}

/**
 * @brief 主函数
 * @details 接受一个文件的地址并计算其base64值 在脚本目录下生成temp.txt储存base64值
 * @param FilePath 一个文件的地址
 * @return 文件是否生成
 *      @retval 0  文件生成成功
 *      @retval -1 path中提供的文件地址不正确或者程序无文件读写权限
 */
int Base64Encode(char *FilePath)
{

    unsigned char DecArray[3];
    int BinArray[24];
    char Base64Array[4];
    
    FILE *FileRead_P  = NULL;
    FILE *FileWrite_P = NULL;

    FileRead_P  = fopen(FilePath,"rb");
    FileWrite_P = fopen(".\\temp.txt","w");

    if (FileRead_P == NULL || FileWrite_P == NULL)
    {
        return -1;
    }


    // 计算文件有多少字节并储存在fileSize中
    int FileSize = 0;
    fseek(FileRead_P,0,SEEK_END);
    FileSize = ftell(FileRead_P);
    fseek(FileRead_P,0,SEEK_SET);

    // 当文件字节数为3的倍数时 直接计算得到base64
    for (int x=0;x<FileSize/3;x++)
    {

        fread(DecArray,sizeof(unsigned char),3,FileRead_P);
        DecTobin(DecArray,BinArray);
        BinToBase64(BinArray,Base64Array,0);

        fputc(Base64Array[0],FileWrite_P);
        fputc(Base64Array[1],FileWrite_P);
        fputc(Base64Array[2],FileWrite_P);
        fputc(Base64Array[3],FileWrite_P);
    }

    // 读取剩余两个字节数并计算base64值
    if (FileSize%3 == 2)
    {

        fread(DecArray,sizeof(unsigned char),3,FileRead_P);
        DecTobin(DecArray,BinArray);
        BinToBase64(BinArray,Base64Array,1);

        fputc(Base64Array[0],FileWrite_P);
        fputc(Base64Array[1],FileWrite_P);
        fputc(Base64Array[2],FileWrite_P);
        fputc(Base64Array[3],FileWrite_P);
    }

    // 读取剩余一个字节数并计算base64值
    else if (FileSize%3 == 1)
    {

        fread(DecArray,sizeof(unsigned char),3,FileRead_P);
        DecTobin(DecArray,BinArray);
        BinToBase64(BinArray,Base64Array,2);
        
        fputc(Base64Array[0],FileWrite_P);
        fputc(Base64Array[1],FileWrite_P);
        fputc(Base64Array[2],FileWrite_P);
        fputc(Base64Array[3],FileWrite_P);
    }
    

    fclose(FileRead_P);
    fclose(FileWrite_P);
    return 0;
}

int main()
{
    int back;
    back = Base64Encode(".\\1.jpg");
    printf("%d",back);
}