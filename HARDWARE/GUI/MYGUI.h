/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-16 14:19:11
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-20 14:14:54
 */
#ifndef __MYGUI_H  
#define __MYGUI_H   
#include "sys.h"
#define ON 1
#define OFF 0
#define MODE 3 //1.按键切换模式 2.EC11旋转+按键切换具体任务 3.按钮切换具体任务 4.测试
extern int TIM5COUNT;
extern float ds18b20,ultrasionic;
extern short bh1750;
typedef struct
{
	u8 Cur_Index;//当前索引项
	u8 previous;//上一页
	u8 next;//下一页
	u8 enter;//确认
	u8 back;//返回
	void (*current_operation)(u8,u8);//	当前索引执行的函数(界面)
}Main_Menu;

//各界面的索引值
extern enum
{
	Task_1=0,
	Task_2,
    Task_3,
    Task_4,
    Task_5,
    Task_6,
    Task_7,
    Task_8,
    Task_9,
    Task_10,
    Task_11,
    
    
    Task_1_Option, //LED灯打开
    Task_1_Child_2,//LED灯关闭
    Task_1_Child_3,//LED灯颜色闪烁
    Task_1_Child_4,//R
    Task_1_Child_5,//G
    Task_1_Child_6,//B

    Task_2_Option,//open
    Task_2_Child_1,//close

    Task_3_Option,
    Task_3_Child_1,
    Task_3_Child_2,
    Task_3_Child_3,

    Task_4_Option,
    Task_4_Child_1,
    Task_4_Child_2,
    Task_4_Child_3,

    Task_5_Option,
    Task_5_Child_1,
    Task_5_Child_2,
    Task_5_Child_3,
    Task_5_Child_4,

    Task_6_Option,
    Task_6_Child_1,
    Task_6_Child_2,
    
    Task_7_Option,
    Task_7_Child_1,
    Task_7_Child_2,
    Task_7_Child_3,
    Task_7_Child_4,


    Task_8_Option,
    Task_8_Child_1,
    Task_8_Child_2,

    Task_81_Option,//PIN4子菜单
    Task_81_Child_1,
    Task_81_Child_2,
    Task_81_Child_3,
    Task_81_Child_4,
    Task_81_Child_5,
    Task_81_init,//过度菜单


    Task_82_Option,//PIN3子菜单
    Task_82_Child_1,
    Task_82_Child_2,
    Task_82_init,//过度菜单

    Task_9_Option,
    Task_9_Child_1,
    Task_9_Child_2,

    Task_10_Option,
    Task_10_Child_1,
    Task_10_Child_2,

    Task_11_Option,
    Task_11_Child_1,
    Task_11_Child_2,
    Task_11_Child_3,
    Task_11_Child_4,
    Task_11_Child_5,
    Task_11_Child_6,
    Task_11_Child_7,

    Task_813_Option,//RFID
    Task_813_Child_1,
    Task_813_Child_2,
    Task_813_Child_3,
    Task_813_Child_4,
}menu_id;

//按键索引值
extern enum
{
    /*
    	case 1: //KEY_S1  下
		case 2: //KEY_S2  确认
		case 3: //KEY_S3  返回
		case 4: //KEY_UP  上
    */
    KEY_NEXT=1,
	KEY_ENTER,
	KEY_BACK,
    KEY_PREVIOUS,
    KEY_S4_EC11,
}KEY_VALUE;
//void GUI_TaskInit(void);
void GUI_Refresh(void);
void Main_Menu_Func(u8 page_index, u8 key_val);
void GUI_MenuPoint(uint8_t position);
#endif



/*
'''

OLED多级显示 菜单索引表生成

'''
front='Task_'#前缀
after_Child='_Child_'#子级菜单后缀
after_Func='_Func'#函数后缀

#单个菜单
one_MENU="{Task_%d_Option,Task_%d_Option,Task_%d_Option,Task_%d_Option,Task_%d,Task_%d_Func},"
#有单个子菜单与父菜单
two_MENU1="{" \
       "Task_%d_Option," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d_Option," \
       "Task_%d," \
       "Task_%d_Func},"
two_MENU2="{" \
       "Task_%d_Child_%d," \
       "Task_%d_Option," \
       "Task_%d_Option," \
       "Task_%d_Child_%d," \
       "Task_%d," \
       "Task_%d_Func},"

#有两个子菜单以上的父菜单
MENU_1="{Task_%d_Option,Task_%d_Child_%d,Task_%d_Child_%d,Task_%d_Option,Task_%d,Task_%d_Func},"
#有两个子菜单以上的首个子菜单
MENU_2="{" \
       "Task_%d_Child_%d," \
       "Task_%d_Option," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d," \
       "Task_%d_Func},"
#有两个子菜单以上的中间子菜单
MENU_3="{" \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d," \
       "Task_%d_Func},"
#有两个子菜单以上的尾部子菜单
MENU_4="{" \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d_Option," \
       "Task_%d_Child_%d," \
       "Task_%d," \
       "Task_%d_Func},"
FUNC_1="void Task_%d_Func(u8 page_index, u8 key_val)\n{\n" \
       "    switch (page_index)\n    {\n    case Task_%d_Option:\n\n        break;"
FUNC_2="    case Task_%d_Child_%d:\n\n        break;"
FUNC_3="    default:\n        break;\n    }\n}"


#生成子级菜单
#@parm MenuX 任务x
#@parm n 子级菜单个数+1
#@parm Enum 是否生成枚举
def Menu_add(MenuX=None, n=None,Enum=False,Func=False):
    if n<=0:
        print("error")
        return
    if Enum == True:
        print("*********************Enum***********************" )

        if n==1:
            print("Task_%d_Option,"%MenuX)
        elif n==2:
            print("Task_%d_Option," % MenuX)
            print("Task_%d_Child_%d," % MenuX)
        elif n >= 3:
            print("Task_%d_Option," % MenuX)
            for i in range(1,n):
                print("Task_%d_Child_%d," % (MenuX,i))

        print("*********************Enum***********************" )
    print("---------------------n=%d-----------------------" % n)
    if n == 1:
        print( one_MENU%
        (MenuX, MenuX, MenuX, MenuX, MenuX, MenuX))
    elif n == 2:
        print(two_MENU1 %
        (MenuX, MenuX, 1, MenuX, 1, MenuX, MenuX, MenuX))
        print(two_MENU2 %
        (MenuX, 1, MenuX, MenuX, MenuX, 1, MenuX, MenuX))
    elif n == 3:
        print(MENU_1 %
              (MenuX, MenuX, 2, MenuX, 1, MenuX, MenuX, MenuX))
        print(MENU_2 %
              (MenuX, 1, MenuX, MenuX, 2, MenuX, 1, MenuX,MenuX))
        print(MENU_4 %
              (MenuX, 2, MenuX, 1, MenuX, MenuX, 2, MenuX, MenuX))
    elif n >=4:
        print(MENU_1 %
              (MenuX, MenuX, n-1, MenuX, 1, MenuX, MenuX, MenuX))
        print(MENU_2 %
              (MenuX, 1, MenuX, MenuX, 2, MenuX, 1, MenuX, MenuX))
        for i in range(2,n-1):
            print(MENU_3 %
                  (MenuX, i, MenuX, i-1, MenuX, i+1, MenuX, i, MenuX, MenuX))
        print(MENU_4 %
              (MenuX, n-1, MenuX, n-2, MenuX, MenuX,  n-1, MenuX, MenuX))

    print("---------------------n=%d-----------------------" % n)
    if Func == True:
        if n == 1:
            print("void Task_%d_Func(u8 page_index, u8 key_val)\n{\n    if (Task_%d_Option == page_index)\n    {\n\n    }\n}" % (MenuX,MenuX))
        if n >= 2:
            print("*********************Func***********************")
            print(FUNC_1%(MenuX,MenuX))
            for i in range(1, n):
                print(FUNC_2 % (MenuX, i))
            print(FUNC_3)
            print("*********************Func***********************")

Menu_add(1,5,True,True)

*/