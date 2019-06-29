import struct
import math
import time, random
from collections import deque
from matplotlib import pyplot as plt
import matplotlib.animation as animation
from rocket_timer_usb import RocketTimerUsb, channels

start = time.time()

class RealtimePlot:
    def __init__(self, rtu, max_entries = 1000):
        self.rtu = rtu
        self.axis_time = deque(maxlen=max_entries)
        self.data = []
        for c in channels:
            dq = deque(maxlen=max_entries)
            self.data.append(dq)

        self.max_entries = max_entries

        self.fig, self.axes = plt.subplots(4, sharex=True, figsize=(4,4))

        self.data_ind = 0

    def animate(self, figure, interval = 100):
        def init_func():
            self.p_line, = self.axes[0].plot([], [], "b-")
            self.t_line, = self.axes[1].plot([], [], "r-")
            self.xl_lines = self.axes[2].plot([], [], 'r-',
                                              [], [], 'b-',
                                              [], [], 'g-',
                                              [], [], 'm-',
                                              [], [], 'c-',
                                              [], [], 'k-')
            self.mag_lines = self.axes[3].plot([], [], 'r-',
                                              [], [], 'b-',
                                              [], [], 'g-')
            self.axes[0].set_autoscaley_on(True)
            self.axes[1].set_autoscaley_on(True)
            self.axes[2].set_autoscaley_on(True)
            self.axes[3].set_autoscaley_on(True)
            return [self.p_line, self.t_line] + self.xl_lines + self.mag_lines

        def wrapper(frame_index):
            ttp = self.rtu.get_packets()
            if ttp != None:
                for x in ttp:
                    for i, v in enumerate(x):
                        self.data[i].append(float(v) * channels[i][3])
                    self.data_ind += 1

            self.p_line.set_data(self.data[0], self.data[2])
            self.t_line.set_data(self.data[0], self.data[1])
            for i in range(6):
                self.xl_lines[i].set_data(self.data[0], self.data[i+3])
            for i in range(3):
                self.mag_lines[i].set_data(self.data[0], self.data[i+9])
            self.axes[0].relim(); self.axes[0].autoscale_view() # rescale the y-axis
            self.axes[1].relim(); self.axes[1].autoscale_view() # rescale the y-axis
            self.axes[2].relim(); self.axes[2].autoscale_view() # rescale the y-axis
            self.axes[3].relim(); self.axes[3].autoscale_view() # rescale the y-axis
            return [self.p_line, self.t_line] + self.xl_lines + self.mag_lines
        return animation.FuncAnimation(figure, wrapper,
                                       init_func=init_func, interval=interval)


def main():
    import matplotlib as mpl

    mpl.rc('text', usetex=False)
    rtu = RocketTimerUsb()
    #rtu.set_led(1)

    display = RealtimePlot(rtu)

    ani = display.animate(display.fig)
    plt.show()

    rtu.set_led(0)

if __name__ == "__main__": main()
