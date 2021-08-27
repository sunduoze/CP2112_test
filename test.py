import math

def bsp_set_adc_gain(gain_l):
    n1 = gain_l * 2.0 + 0.5
    if (n1 < 0):
        n1 = 0
    if n1 > 95:
        n1 = 95
    temp_0 = n1 / 2.0
    temp_2 = pow(10, temp_0 / 20.0)
    print("Gain:%f" % gain_l + " REG:0x%x" % int(temp_2))
    # print("0x%x" %temp_3)


def bsp_set_adc_gain2(gain_l):
    # n1 = gain_l * 2.0 + 0.5
    # if (n1 < 0):
    #     n1 = 0
    # if n1 > 95:
    #     n1 = 95
    # temp_0 = n1 / 2.0
    # temp_2 = pow(10, temp_0 / 20.0)

    dB = 20 * math.log10(gain_l)
    reg = dB * 2
    print("Gain:%3.f" % gain_l + "dB:%2.2fdB" % dB + " REG:0x%2x" % int(reg))
    # print("0x%x" %temp_3)

bsp_set_adc_gain2(1)    # 0dB
bsp_set_adc_gain2(10)   # 20dB
bsp_set_adc_gain2(100)  # 40dB
bsp_set_adc_gain2(237.13737057)    # 47.5dB