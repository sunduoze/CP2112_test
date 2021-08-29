from tkinter import ttk
import tkinter.font as tkFont
from tkinter import *
import tkinter as tk

def cuotiwin():
 
    global score    #分数
    global list2     #存放错题
    var1=StringVar
    window = Tk()
    window.title("我的C语言做题系统")  # title()  定义这个窗口的标题
    window.geometry("1000x500")  # geometry()  定义窗口的大小
 
    scr = scrolledtext.ScrolledText(window, width=70, height=13,font=("隶书",18))  #滚动文本框（宽，高（这里的高应该是以行数为单位），字体样式）
    scr.place(x=50, y=50) #滚动文本框在页面的位置
 
    i = "你太优秀了，没有错题！"
 
    def panduan():
        i ="你太优秀了，没有错题！"
        if score == 100:
            scr.insert(END,i)
        else:
            for b in list2:     #遍历错题列表
                scr.insert(END,b)   #将错题显示在动文本框中
 
 
    panduan()      
def exit():
       caidanwin()    #跳转页面
       window.destroy()     #跳转后关闭当前窗口
       button2 = Button(window, text="返     回", font=("隶书", 20), command=exit)
       button2.place(x=700, y=420, width=150, height=60)

window.mainloop()