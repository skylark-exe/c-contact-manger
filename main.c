#include <stdio.h>
#include <string.h>

// 全局数据结构定义
#define MAX_SIZE 100

struct Student {
    int number;
    char name[20];
    char telephone[12];
};

struct Student directory[MAX_SIZE]; // 通讯录数组
int count = 0;                     // 当前记录数

//  函数声明
void PrintAll();
int DeleteByName(char name[]);
int Insert(struct Student stu);
int SearchByName(char name[]);

//  主函数（菜单框架 -）
int main(void) {
    printf("==============================================================\n");
    printf("|                                                             |\n");
    printf("|          欢迎使用班级通讯录管理系统                             |\n");
    printf("|                                                             |\n");
    printf("|                作者：skylark                                 |\n");
    printf("==============================================================\n");

    // 初始化一些测试数据
    struct Student test1 = {220501, "张三", "17610011116"};
    struct Student test2 = {220502, "李四", "18533334444"};
    Insert(test1);
    Insert(test2);

    int command;
    do {
        printf("\n1.查询  2.增加  3.修改  4.删除  5.显示所有  9.退出\n");
        printf("您选择功能: ");
        scanf("%d", &command);

        switch(command) {
            // 查询
            case 1: {
                char name[20];
                printf("请输入要查询的同学姓名: ");
                scanf("%s", name);
                int index = SearchByName(name);  // 调用查询函数
                if(index != -1) {
                    printf("找到：学号:%d, 姓名:%s, 电话:%s\n",
                           directory[index].number,
                           directory[index].name,
                           directory[index].telephone);
                } else {
                    printf("未找到该同学！\n");
                }
                break;
            }

            // 增加
            case 2: {
                struct Student newStu;
                printf("请输入学号: ");
                scanf("%d", &newStu.number);
                printf("请输入姓名: ");
                scanf("%s", newStu.name);
                printf("请输入电话: ");
                scanf("%s", newStu.telephone);

                if(Insert(newStu)) {  // 调用插入函数
                    printf("操作成功！\n");
                } else {
                    printf("操作失败（通讯录可能已满）。\n");
                }
                break;
            }

            // 修改
            case 3: {
                char name[20];
                printf("请输入要修改的同学姓名: ");
                scanf("%s", name);
                int index = SearchByName(name);
                if(index != -1) {
                    printf("原信息：学号:%d, 电话:%s\n",
                           directory[index].number,
                           directory[index].telephone);
                    printf("请输入新电话: ");
                    scanf("%s", directory[index].telephone);
                    printf("修改成功！\n");
                } else {
                    printf("未找到该同学！\n");
                }
                break;
            }


            case 4: {
                char name[20];
                printf("请输入要删除的同学姓名: ");
                scanf("%s", name);
                if(DeleteByName(name)) {  // 调用删除函数
                    printf("删除成功！\n");
                } else {
                    printf("该同学不在本班！\n");
                }
                break;
            }


            case 5: {
                PrintAll();  // 调用显示函数
                break;
            }

            case 9: {
                printf("感谢您的使用，再见！\n");
                break;
            }

            default: {
                printf("无此功能，请重新选择\n");
                break;
            }
        }
    } while(command != 9);

    return 0;
}



// 显示所有记录
void PrintAll() {
    if(count == 0) {
        printf("通讯录为空。\n");
        return;
    }
    printf("\n=========== 通讯录（共%d条记录）============\n", count);
    for(int i = 0; i < count; i++) {
        printf("%d. 学号:%d  姓名:%-10s  电话:%s\n",
               i+1,
               directory[i].number,
               directory[i].name,
               directory[i].telephone);
    }
    printf("============================================\n");
}

// 按姓名查询
int SearchByName(char name[]) {
    for(int i = 0; i < count; i++) {
        if(strcmp(directory[i].name, name) == 0) {
            return i;  // 返回找到的索引
        }
    }
    return -1;  // 未找到
}

// 删除记录 实现删除算法
int DeleteByName(char name[]) {
    // 任务：根据姓名查找并删除记录
    // 步骤：
    // 1. 遍历数组查找姓名匹配的记录
    // 2. 如果找到，将后面所有记录前移一格
    // 3. count减1，返回1表示成功
    // 4. 如果没找到，返回0

    for(int i = 0; i < count; i++) {
        if(strcmp(directory[i].name, name) == 0) {
            // 找到要删除的记录
            for(int j = i; j < count - 1; j++) {
                directory[j] = directory[j + 1];  // 后面记录前移
            }
            count--;  // 记录数减少
            return 1; // 删除成功
        }
    }

    return 0;  // 未找到，删除失败
}

// 插入记录
int Insert(struct Student stu) {
    // 任务：按学号有序插入或更新记录
    // 步骤：
    // 1. 查找是否已存在相同学号的记录
    // 2. 如果存在，更新该记录
    // 3. 如果不存在，找到插入位置，后移其他记录，插入新记录
    // 4. 保持按学号递增有序

    // 检查数组是否已满
    if(count >= MAX_SIZE) {
        return 0;  // 失败
    }

    // 查找相同学号的记录或插入位置
    int i, pos = -1;
    for(i = 0; i < count; i++) {
        if(directory[i].number == stu.number) {
            // 找到相同学号，更新记录
            directory[i] = stu;
            return 1;  // 更新成功
        }
        if(directory[i].number > stu.number && pos == -1) {
            pos = i;  // 找到第一个比插入学号大的位置
        }
    }

    if(pos == -1) {
        pos = count;  // 如果没找到更大学号，插在最后
    }

    // 将pos位置及之后的记录后移
    for(i = count; i > pos; i--) {
        directory[i] = directory[i - 1];
    }

    // 插入新记录
    directory[pos] = stu;
    count++;  // 记录数增加
    return 1;  // 插入成功
}