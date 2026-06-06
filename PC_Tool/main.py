import matplotlib
matplotlib.use('TkAgg')
import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import re

# ==================== 解决中文乱码 ====================
plt.rcParams['font.sans-serif'] = ['SimHei', 'Microsoft YaHei', 'WenQuanYi Micro Hei']
plt.rcParams['axes.unicode_minus'] = False

# ==================== 手动指定串口 ====================
port_name = 'COM4'   # 🔴 改成你的实际串口号（如 COM3、COM5 等）
baud_rate = 9600

try:
    ser = serial.Serial(port_name, baud_rate, timeout=1)
    print(f"串口 {port_name} 已打开")
except Exception as e:
    print(f"无法打开串口 {port_name}: {e}")
    exit(1)

# ==================== 数据缓存 ====================
light_buffer = []
temp_buffer = []
humi_buffer = []
max_points = 50
win_size = 5
ALARM_THRESHOLD = 2500

# ==================== 创建图表 ====================
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))
fig.suptitle('环境监测系统', fontsize=16)

# 子图1：光照曲线
line_light, = ax1.plot([], [], 'g-', linewidth=2)
ax1.set_ylabel('光照强度 (ADC)')
ax1.set_ylim(0, 4095)
ax1.set_xlim(0, max_points)
ax1.grid(True)
ax1.set_title('光照实时曲线')

# 子图2：温湿度曲线
line_temp, = ax2.plot([], [], 'r-', linewidth=2, label='温度')
line_humi, = ax2.plot([], [], 'b-', linewidth=2, label='湿度')
ax2.set_ylabel('温度/湿度')
ax2.set_ylim(0, 100)
ax2.set_xlim(0, max_points)
ax2.grid(True)
ax2.set_title('温湿度实时曲线')
ax2.legend()

def init():
    line_light.set_data([], [])
    line_temp.set_data([], [])
    line_humi.set_data([], [])
    return line_light, line_temp, line_humi

def moving_average(buf, new_val, window=3):
    buf.append(new_val)
    if len(buf) > max_points:
        buf.pop(0)
    if len(buf) >= window:
        return sum(buf[-window:]) / window
    return new_val

def update(frame):
    global ALARM_THRESHOLD

    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if line:
            print(f"收到: {line}")

            light_match = re.search(r'Light:\s*(\d+)', line)
            temp_match = re.search(r'T:\s*(\d+)C', line)
            humi_match = re.search(r'H:\s*(\d+)%', line)
            max_match = re.search(r'MAX:\s*(\d+)', line)

            if max_match:
                ALARM_THRESHOLD = int(max_match.group(1))
                print(f"报警阈值更新: {ALARM_THRESHOLD}")

            if light_match:
                light_val = int(light_match.group(1))
                moving_average(light_buffer, light_val, win_size)

                if light_val > ALARM_THRESHOLD:
                    line_light.set_color('red')
                else:
                    line_light.set_color('green')

                line_light.set_data(range(len(light_buffer)), light_buffer)
                ax1.set_xlim(0, len(light_buffer))

            if temp_match and humi_match:
                temp_val = int(temp_match.group(1))
                humi_val = int(humi_match.group(1))
                moving_average(temp_buffer, temp_val, win_size)
                moving_average(humi_buffer, humi_val, win_size)
                line_temp.set_data(range(len(temp_buffer)), temp_buffer)
                line_humi.set_data(range(len(humi_buffer)), humi_buffer)
                ax2.set_xlim(0, len(temp_buffer))

    return line_light, line_temp, line_humi

ani = FuncAnimation(fig, update, init_func=init, interval=100, blit=False, cache_frame_data=False)
plt.tight_layout()
plt.show()

ser.close()
print("串口已关闭")