import os
import json

debug_status = False


def debug(*kwargs):
    if debug_status:
        print(*kwargs)


class config_file:
    def __init__(self):
        self.path = "./config.json"

        self.init_dict = \
            {
                "log":
                    {
                        "level": "DEBUG"
                    },
                "aslan":
                    {
                        "enable_beacon":
                            {
                                "therm_pull_high_delay": 6,
                                "therm_pull_low_delay": 1,
                                "retry_times": 5,
                                "reserve": 1
                            },
                        "disable_beacon":
                            {
                                "therm_pull_high_delay": 1,
                                "therm_pull_low_delay": 1,
                                "retry_times": 5,
                                "reserve": 1
                            },
                        "ble_scan":
                            {
                                "serial_port_name": "Prolific USB-to-Serial Comm Port",
                                "rssi_down_limit": -70,
                                "scan_times": 10,
                                "scan_cycle": 0.1,
                                "reserve": 1
                            },
                        "UI_display":
                            {
                                "retry_times": "True",
                                "scan_rssi": "True",
                                "debug": "True",
                                "reserve": "True"
                            }
                    }
            }

    def file_is_null(self):
        file_size = os.path.getsize(self.path)
        debug("file_is_null func process")
        if file_size == 0:
            debug('文件是空的')
            return True
        else:
            debug('文件不是空的')
            return False

    def read(self):
        # 加载文件到dict
        with open(self.path, 'r') as load_file:
            load_dict = json.load(load_file)
            debug(load_dict)
            debug("读取文件完成...")
        return load_dict

    # 写入dict到文件
    def add(self, dict_file):
        with open(self.path, "w") as file_temp:
            json.dump(dict_file, file_temp)
            debug("写入文件完成...")

    def erase_then_write(self):
        print("test")

    def delete_file(self):
        print("test")

    def str_to_dict(self, str_temp):
        # dumps 将数据转换成字符串
        json_str = json.dumps(str_temp)
        # print(json_str)
        # print(type(json_str))

        # loads 将字符串加载成字典
        dict_temp = json.loads(json_str)
        debug(dict_temp)
        debug(type(dict_temp))
        return dict_temp


if __name__ == '__main__':

    def func_add_file(config_dict):  # config_json_class
        config_dict.add(config_dict.str_to_dict(config_dict.init_dict))
        dict_temp = config_dict.read()

        dict_temp['lalalalall'] = \
            [8200,
             {1:
                 [
                     ['Python', 81],
                     ['shirt', 300]
                 ]}]
        print(dict_temp)

        config_dict.add(dict_temp)
        config_dict.read()


    cfg = config_file()
    status = cfg.file_is_null()
    if status is False:
        print("file is full")
        config_file = cfg.read()
        print('-----------------------')
        print(config_file['aslan']['enable_beacon']['retry_times'])
    else:
        print("file is null")
        func_add_file(cfg)
