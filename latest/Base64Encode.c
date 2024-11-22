/** 
 * @file Base64Encode.c
 * @brief 计算文件的base64编码
 * @details 包含decTobin、binTobase64、base64Encode三个函数 其中base64Encode为主函数
 * @author ZerOnes
 * @date 2024-11-15
 * @version V1.0
 */

#include<stdio.h>
#include<math.h>

const char base64Alphatablet[64]={
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
 * @param *decArray 包含三个十进制数的数组的指针
 * @param *binArray 储存三个二进制数的数组的指针
 * @return 返回binArray数组的指针
 */


int *decTobin(unsigned char *decArray,int *binArray){
    
    int num = 0;

    for (int x=0;x<3;x++){
        num = (int)decArray[x];
        int i = 7+(x*8); 

        while (num>0){
            binArray[i] = num % 2;
            num = num / 2;
            i--;
            }
        
        // 对于不足八位的二进制数据用0补齐
        if (i>=0){
            for (i;i>=x*8;i--){
                binArray[i] = 0;
                }
            }
    }
    return binArray;
}

/**
 * @brief 二进制转换base64编码
 * @details 接受binArray中的二进制值并以六位分组并获取对应的字符储存在base64Array中
 * @param binArray 包含三个二进制数的数组的指针
 * @param base64Array 储存三个base64值的数组的指针
 * @param lackNum 三个字节缺少几个字节
 * @return 返回base64数组的指针
 */

char *binTobase64(int *binArray,char *base64Array,int lackNum){

    int num = 0;
    // 判断缺少几个字节并根据情况补0
    if (lackNum == 1){
        binArray[16] = 0;
        binArray[17] = 0;
    }
    else if (lackNum == 2){
        binArray[8]  = 0;
        binArray[9]  = 0;
        binArray[10] = 0;
        binArray[11] = 0;
    }
    for (int x=0;x<4;x++){
        int i = 0 + (6*x); // 每个数的起始位置
        int p = 0; 
        for (int y=0;y<6;y++){
            int p = pow(2,5-y);
            num += binArray[i+y] * p;
            }
        base64Array[x] = base64Alphatablet[num]; // 去base64表中查询对应的值
        num = 0;
    }

    if (lackNum == 1){
        base64Array[3] = '=';
    }
    else if (lackNum == 2){
        base64Array[2] = '=';
        base64Array[3] = '=';
    }
    return base64Array;
}

/**
 * @brief 主函数
 * @details 接受一个文件的地址并计算其base64值 在脚本目录下生成temp.txt储存base64值
 * @param path 一个文件的地址
 * @return 文件是否生成
 *      @retval 0  文件生成成功
 *      @retval -1 path中提供的文件地址不正确或者程序无文件读写权限
 */
int base64Encode(char *path){

    unsigned char decArray[3];
    int binArray[24];
    char base64Array[4];

    FILE *fileRead_p =  NULL;
    fileRead_p = fopen(path,"rb");
    FILE *fileWrite_p = NULL;
    fileWrite_p = fopen(".\\temp.txt","w");

    if (fileRead_p == NULL || fileWrite_p == NULL){
        return -1;
    }

    // 计算文件有多少字节并储存在fileSize中
    int fileSize = 0;
    fseek(fileRead_p,0,SEEK_END);
    fileSize = ftell(fileRead_p);
    fseek(fileRead_p,0,SEEK_SET);

    // 当文件字节数为3的倍数时 直接计算得到base64
    for (int x=0;x<fileSize/3;x++){

        fread(decArray,sizeof(unsigned char),3,fileRead_p);
        decTobin(decArray,binArray);
        binTobase64(binArray,base64Array,0);

        fputc(base64Array[0],fileWrite_p);
        fputc(base64Array[1],fileWrite_p);
        fputc(base64Array[2],fileWrite_p);
        fputc(base64Array[3],fileWrite_p);
    }

    // 读取剩余两个字节数并计算base64值
    if (fileSize%3 == 2){

        fread(decArray,sizeof(unsigned char),3,fileRead_p);
        decTobin(decArray,binArray);
        binTobase64(binArray,base64Array,1);

        fputc(base64Array[0],fileWrite_p);
        fputc(base64Array[1],fileWrite_p);
        fputc(base64Array[2],fileWrite_p);
        fputc(base64Array[3],fileWrite_p);
    }

    // 读取剩余一个字节数并计算base64值
    else if (fileSize%3 == 1){

        fread(decArray,sizeof(unsigned char),3,fileRead_p);
        decTobin(decArray,binArray);
        binTobase64(binArray,base64Array,2);
        
        fputc(base64Array[0],fileWrite_p);
        fputc(base64Array[1],fileWrite_p);
        fputc(base64Array[2],fileWrite_p);
        fputc(base64Array[3],fileWrite_p);
    }
    

    fclose(fileRead_p);
    fclose(fileWrite_p);
    return 0;
}

int main(){
    int back;
    back = base64Encode(".\\1.jpg");
    printf("%d",back);
}