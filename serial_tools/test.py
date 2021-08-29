from tkinter import *

root = Tk()
def check():
    print("python3")
menubar = Menu(root)
# openVar = IntVar()
# saveVar = IntVar()
# exitVar = IntVar()
# filemenu = Menu(menubar,tearoff=1)
# filemenu.add_checkbutton(label="打开",command=check,variable=openVar)
# filemenu.add_checkbutton(label="保存",command=check,variable=saveVar)
# #创建一个横线是退出上面的
# filemenu.add_separator()
# filemenu.add_checkbutton(label="退出",command=root.quit,variable=exitVar)
#顶级菜单名称
# menubar.add_cascade(label="文件",menu=filemenu)
editVar = IntVar()
editVar.set(2)
editmenu = Menu(menubar,tearoff=True)
editmenu.add_radiobutton(label="剪切",command=check,variable=editVar,value=1)
editmenu.add_radiobutton(label="拷贝",command=check,variable=editVar,value=2)
editmenu.add_radiobutton(label="粘贴",command=check,variable=editVar,value=3)
menubar.add_cascade(label="编辑",menu=editmenu)

root.config(menu=menubar)
mainloop()