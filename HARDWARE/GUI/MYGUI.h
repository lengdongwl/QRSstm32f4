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
#define MODE 3 //1.�����л�ģʽ 2.EC11��ת+�����л��������� 3.��ť�л��������� 4.����
extern int TIM5COUNT;
extern float ds18b20,ultrasionic;
extern short bh1750;
typedef struct
{
	u8 Cur_Index;//��ǰ������
	u8 previous;//��һҳ
	u8 next;//��һҳ
	u8 enter;//ȷ��
	u8 back;//����
	void (*current_operation)(u8,u8);//	��ǰ����ִ�еĺ���(����)
}Main_Menu;

//�����������ֵ
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
    
    
    Task_1_Option, //LED�ƴ�
    Task_1_Child_2,//LED�ƹر�
    Task_1_Child_3,//LED����ɫ��˸
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

    Task_81_Option,//PIN4�Ӳ˵�
    Task_81_Child_1,
    Task_81_Child_2,
    Task_81_Child_3,
    Task_81_Child_4,
    Task_81_Child_5,
    Task_81_init,//���Ȳ˵�


    Task_82_Option,//PIN3�Ӳ˵�
    Task_82_Child_1,
    Task_82_Child_2,
    Task_82_init,//���Ȳ˵�

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

//��������ֵ
extern enum
{
    /*
    	case 1: //KEY_S1  ��
		case 2: //KEY_S2  ȷ��
		case 3: //KEY_S3  ����
		case 4: //KEY_UP  ��
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

OLED�༶��ʾ �˵�����������

'''
front='Task_'#ǰ׺
after_Child='_Child_'#�Ӽ��˵���׺
after_Func='_Func'#������׺

#�����˵�
one_MENU="{Task_%d_Option,Task_%d_Option,Task_%d_Option,Task_%d_Option,Task_%d,Task_%d_Func},"
#�е����Ӳ˵��븸�˵�
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

#�������Ӳ˵����ϵĸ��˵�
MENU_1="{Task_%d_Option,Task_%d_Child_%d,Task_%d_Child_%d,Task_%d_Option,Task_%d,Task_%d_Func},"
#�������Ӳ˵����ϵ��׸��Ӳ˵�
MENU_2="{" \
       "Task_%d_Child_%d," \
       "Task_%d_Option," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d," \
       "Task_%d_Func},"
#�������Ӳ˵����ϵ��м��Ӳ˵�
MENU_3="{" \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d_Child_%d," \
       "Task_%d," \
       "Task_%d_Func},"
#�������Ӳ˵����ϵ�β���Ӳ˵�
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


#�����Ӽ��˵�
#@parm MenuX ����x
#@parm n �Ӽ��˵�����+1
#@parm Enum �Ƿ�����ö��
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