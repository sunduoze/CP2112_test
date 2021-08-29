# coding:utf-8
from tkinter import *
from tkinter import ttk  # 导入ttk模块，因为下拉菜单控件在ttk中


class drop_down_box:
    def __init__(self):
        self.win = Tk()
        self.win.title("下拉框")
        self.win.geometry("600x200")
        '''下拉框样式'''
        # 创建下拉列表，设置下拉列表中的值
        self.normal_ddl = Label(self.win, text='下拉框选项：')
        self.ddl = ttk.Combobox(self.win)
        self.ddl['value'] = ('下拉选项1', '下拉选项2', '下拉选项3', '下拉选项4')
        # 设置默认值，即默认下拉框中的内容,索引从0开始
        self.ddb_default_L = Label(self.win, text='下拉框默认值：')
        self.ddb_default = ttk.Combobox(self.win)
        self.ddb_default['value'] = ('下拉选项1', '下拉选项2', '下拉选项3', '下拉选项4')
        self.ddb_default.current(2)
        # get()获取选中的值
        self.ddb_acquire_L = Label(self.win, text='获取下拉框的值：')
        self.ddb_acquire = ttk.Combobox(self.win)
        self.ddb_acquire['value'] = ('下拉选项1', '下拉选项2', '下拉选项3', '下拉选项4')
        self.ddb_acquire.current(3)
        ddl_get = self.ddb_acquire.get()
        self.ddb_acquire_echo = Label(self.win, text=str(ddl_get))
        # 下拉框中插入文本insert(插入位置，插入内容)
        self.ddb_insert_L = Label(self.win, text='下拉框中插入值：')
        self.ddb_insert = ttk.Combobox(self.win)
        self.ddb_insert.insert(0, 'data')
        # 下拉框删除文本，delete(删除开始位置，删除结束位置)
        self.ddb_delete_L = Label(self.win, text='下拉框中插入值：')
        self.ddb_delete = ttk.Combobox(self.win)
        self.ddb_delete['value'] = ('下拉选项1', '下拉选项2', '下拉选项3', '下拉选项4')
        self.ddb_delete.current(0)
        self.ddb_delete.delete(1, 4)
        # 下拉框状态
        self.ddb_status = Label(self.win, text='下拉框状态：')
        self.ddb_disabled = ttk.Combobox(self.win)
        self.ddb_disabled['value'] = ('禁用状态')
        self.ddb_disabled.current(0)
        self.ddb_disabled.config(state=DISABLED)
        self.ddb_normal = ttk.Combobox(self.win)
        self.ddb_normal['value'] = ('普通状态')
        self.ddb_normal.current(0)
        self.ddb_normal.config(state=NORMAL)
        self.ddb_active = ttk.Combobox(self.win)
        self.ddb_active['value'] = ('活跃状态')
        self.ddb_active.current(0)
        self.ddb_active.config(state=ACTIVE)
        # 下拉框权限
        self.ddb_permission = Label(self.win, text='下拉框权限：')
        self.dropdown_permission = ttk.Combobox(self.win)
        self.dropdown_permission['value'] = ('只读权限')
        self.dropdown_permission.current(0)
        self.dropdown_permission['state'] = 'readonly'
        self.dropdown_read_write = ttk.Combobox(self.win)
        self.dropdown_read_write['value'] = ('读写权限')
        self.dropdown_read_write.current(0)
        self.dropdown_read_write['state'] = 'read-write'
        # 下拉框宽度
        self.ddb_width_label = Label(self.win, text='下拉框宽度：')
        self.ddb_width = ttk.Combobox(self.win, width=9)
        # 下拉框颜色
        combostyle = ttk.Style()
        combostyle.theme_create('combostyle', parent='alt',
                                settings={'TCombobox':
                                    {'configure':
                                        {
                                            'foreground': 'blue',  # 前景色
                                            'selectbackground': 'black',  # 选择后的背景颜色
                                            'fieldbackground': 'white',  # 下拉框颜色
                                            'background': 'red',  # 下拉按钮颜色
                                        }}}
                                )
        combostyle.theme_use('combostyle')

        '''grid布局'''
        self.normal_ddl.grid(row=1, column=1, sticky='E')
        self.ddl.grid(row=1, column=2, sticky='NW')
        self.ddb_default_L.grid(row=2, column=1, sticky='E')
        self.ddb_default.grid(row=2, column=2, sticky='NW')
        self.ddb_acquire_L.grid(row=3, column=1, sticky='E')
        self.ddb_acquire.grid(row=3, column=2, sticky='NW')
        self.ddb_acquire_echo.grid(row=3, column=3, sticky='W')
        self.ddb_insert_L.grid(row=4, column=1, sticky='E')
        self.ddb_insert.grid(row=4, column=2, sticky='NW')
        self.ddb_delete_L.grid(row=5, column=1, sticky='E')
        self.ddb_delete.grid(row=5, column=2, sticky='NW')
        self.ddb_status.grid(row=6, column=1, sticky='E')
        self.ddb_disabled.grid(row=6, column=2, sticky='NW')
        self.ddb_normal.grid(row=6, column=3, sticky='NW')
        self.ddb_active.grid(row=6, column=4, sticky='NW')
        self.ddb_permission.grid(row=7, column=1, sticky='E')
        self.dropdown_permission.grid(row=7, column=2, sticky='NW')
        self.dropdown_read_write.grid(row=7, column=3, sticky='NW')
        self.ddb_width_label.grid(row=8, column=1, sticky='E')
        self.ddb_width.grid(row=8, column=2, sticky='NW')
        self.win.mainloop()

    def select(self):
        print(self.ddl.get())


if __name__ == '__main__':
    drop_down_box()